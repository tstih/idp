/*
 * ArrayCmd.cs
 * 
 * Create an image array from single image, and output 
 * individual images as .C, .S, .PNG or .GYP.
 * 
 * NOTES:
 *  .GYP is a xyz os glyph format, easy to render on
 *  older (8/16 bit) environments.
 *  
 * TODO:
 *  Refactor private classes to the Idp.Gpx.Common library.
 *  
 * MIT License (see: LICENSE)
 * Copyright (c) 2021 Tomaz Stih
 * 
 * 02.03.2021   tstih
 * 
 */
using System;
using System.Collections;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;

using XYZ.Formats;
using Idp.Gpx.Common.CmdLine;
using Idp.Gpx.Snatch.Generators;
using Idp.Gpx.Snatch.Exports;

namespace Idp.Gpx.Snatch.Commands
{

    public class ArrayCmd : Cmd {

        #region Const(s)
        const int SUCCESS = 0;
        const int INVALID_FORMAT = 5;
        #endregion // Const(s)

        #region Private Class(es)
        /*
        private class AsmExport : Export
        {
            private StringBuilder _internal;
            private FontAsmCodeGenerator _asm;

            public AsmExport()
            {
                _internal = new StringBuilder();
            }

            public override int Begin(ArrayCmd cmd)
            {
                _asm = new FontAsmCodeGenerator(_internal); 
                byte bytesPerGlyphLine = (byte)((cmd.GlyphWidth - 1) / 8 + 1);
                FontType targetType = 0;
                if (!cmd.Proportional) targetType |= FontType.Fixed;
                if (cmd.Tiny) targetType |= FontType.Tiny;
                _asm.AddFontHeader(
                    cmd.Output, 
                    targetType, 
                    (byte)cmd.GlyphWidth,
                    (byte)cmd.GlyphHeight,
                    bytesPerGlyphLine, 
                    (byte)cmd.First, 
                    (byte)cmd.Last);
                return SUCCESS;
            }

            public override int OnGlyph(
                ArrayCmd cmd, 
                Bitmap bmp, 
                Rectangle rect, 
                int ascii, 
                StringBuilder std, 
                StringBuilder err)
            {
                BitArray bits=null;
                byte bytesPerGlyphLine = 0;

                if (cmd.Tiny)
                {
                    // Let's vectorize the shit ouf of it!
                    byte[] moves = Vectorize(bmp,rect, cmd.Tiny);
                }
                else
                {
                    // Char to bitfield.
                    bytesPerGlyphLine = (byte)((cmd.GlyphWidth - 1) / 8 + 1);
                    bits = new BitArray(8 * bytesPerGlyphLine * cmd.GlyphHeight, false);
                    for (int y = 0; y < rect.Height; y++)
                        for (int x = 0; x < rect.Width; x++)
                            bits[y * 8 * bytesPerGlyphLine + x] = 
                                !IsWhite((bmp.GetPixel(rect.Left + x, rect.Top + y)));
                }

                // And generate as 0b const.
                _asm.AddRasterFontGlyph(
                    ascii, 
                    bits, 
                    bytesPerGlyphLine, 
                    (byte)rect.Height, 0, 0);

                return SUCCESS;
            }

            public override int End(ArrayCmd cmd, Bitmap bmp, StringBuilder std, StringBuilder err)
            {
                // Write result to text file.
                File.WriteAllText(cmd.Output + ".s", _internal.ToString());

                // And write result to std. output.
                std.Append(_internal.ToString());

                return SUCCESS;
            }

            private class Pixel
            {
                public int X;
                public int Y;
                public bool Match(int x, int y) { return x == X && y == Y; }
            }

            private byte[] Vectorize(Bitmap bmp, Rectangle grect, bool tiny)
            {

                List<Pixel> pixels = new List<Pixel>();
                // First enumerate all pixels.
                for (int y = 0; y < grect.Height; y++)
                    for (int x = 0; x < grect.Width; x++)
                        if (!IsWhite(bmp.GetPixel(
                            x + grect.Left, 
                            y + grect.Top)))
                        {
                            pixels.Add(new Pixel() { X=x, Y=y });
                        }

                // Now create a graph of all "connected" pixels.
                HashSet<Tuple<int,int>> edges = new HashSet<Tuple<int,int>>();
                int i;
                for(i=0;i<pixels.Count;i++)
                {
                    for (int cy=pixels[i].Y-1;cy<=pixels[i].Y+1;cy++)
                        for(int cx=pixels[i].X-1;cx<=pixels[i].X+1;cx++)
                            // If we are inside the glyph and we are not the same glyph.
                            if ( cx>0 && cx<bmp.Width 
                                && cy>0 && cy<bmp.Height
                                && !(pixels[i].Match(cx, cy))) // Not the same.
                            {
                                int j;
                                for (j = 0; j < pixels.Count; j++)
                                    if (pixels[j].Match(cx, cy))
                                        break; // We found the right pixel. It's in j.

                                if (j<pixels.Count) // Found it?
                                {
                                    if (i < j) edges.Add(new Tuple<int, int>(i,j));
                                    else edges.Add(new Tuple<int, int>(j, i));
                                }
                            }
                }

                if (pixels.Count == 0) return null; // No pixels for this one.

                List<byte> result = new List<byte>();

                List<int[]> strokes = new List<int[]>();
                while (pixels.Count > 0)
                {
                    // Get longest stroke.
                    var stroke = Route(
                        pixels.Count,
                        edges.ToArray()
                    );

                    // Add stroke to strokes.
                    strokes.Add(stroke);

                    // Remove edges from or to pixels.
                    edges.RemoveWhere(e => stroke.Contains(e.Item1) || stroke.Contains(e.Item2));
                    // And pixels.
                    foreach (int sidx in stroke.OrderByDescending(v => v))
                        pixels.RemoveAt(sidx);
                }
                
                // TODO: Optimize stroke connections.

                // Great, we have our strokes. Now generate byte array for them.
                // First byte is the origin i.e. the first point of first stroke.
                var origin = pixels[strokes[0][0]];
                result.Add((byte)origin.X);
                result.Add((byte)origin.Y);

                // Now add pixel by pixel from strokes.
                int[] prev=null;
                foreach(var stroke in strokes)
                {
                    if (prev != null)
                    // Connect with prev. stroke.
                        result.AddRange(FindPath(pixels[prev.Last()], pixels[stroke.First()]));
                    else prev = stroke; // Make this stroke previous stroke.

                    // Iterate through pixels. First put pen.
                    Pixel pixFrom = pixels[stroke[0]];
                    for(int ip=1; ip<stroke.Length; ip++)
                    {
                        Pixel pixTo = pixels[stroke[i]];

                        // Decode direction and add pen down, paint pixel.
                        byte direction = (byte)(DecodeDirection(pixFrom, pixTo)|0x18);
                        result.Add(direction); // Add direction!

                        // Next.
                        pixFrom = pixTo; // Iterate...
                    }
                }

                return null;
            }

            private byte[] FindPath(Pixel p1, Pixel p2)
            {
                List<byte> result = new List<byte>();
                Pixel pixel = new Pixel() { X=p1.X, Y=p1.Y };
                while (!pixel.Match(p2.X,p2.Y))
                {
                    // Move to the right direction.
                    int dx=0, dy=0;
                    if (pixel.X < p2.X) dx = 1; else if (pixel.X>p2.X) dx = -1;
                    if (pixel.Y < p2.Y) dy = 1; else if (pixel.Y > p2.Y) dy = -1;
                    pixel = new Pixel() { X = p1.X + dx, Y = p1.Y + dy };
                    result.Add(DecodeDirection(pixel, p2)); ;
                }
                return result.ToArray();
            }

            private byte DecodeDirection(Pixel p1, Pixel p2)
            {
                int dx = p1.X - p2.X, dy = p1.Y - p2.Y;
                if (dx == 1 && dy == 0) return 0; // right
                else if (dx == 1 && dy == -1) return 1; // right, up.
                else if (dx == 0 && dy == -1) return 2; // up.
                else if (dx == -1 && dy == -1) return 3; // left, up.
                else if (dx == 0 && dy == 1) return 4; // down.
                else if (dx == 1 && dy == 1) return 5; // right, down.
                else if (dx == -1 && dy == 0) return 6; // left.
                else if (dx == -1 && dy == 1) return 7; // down, left.
                else return 0xff; // Error!
            }

            private int[] Route(
                int n,
                Tuple<int, int>[] edges,
                HashSet<int> visited = null,
                int index=-1
                )
            {
                if (index < 0) // Level 0.
                {
                    HashSet<int> longest = null;
                    // Initialize visited.
                    for (index = 0; index < n; index++)
                    {
                        visited = new HashSet<int>(); // New path.
                        Route(n, edges, visited, index);
                        if (longest == null) longest = new HashSet<int>(visited);
                        else if (longest.Count < visited.Count) longest = new HashSet<int>(visited);
                    }
                    return longest.ToArray();
                } else
                {
                    // We've been here before.
                    if (!visited.Contains(index))
                    {
                        visited.Add(index);
                        // Visit all edges.
                        for (int i = 0; i < edges.Length; i++)
                            if (edges[i].Item1 == index)
                                Route(n, edges, visited, edges[i].Item2);
                            else if (edges[i].Item2 == index)
                                Route(n, edges, visited, edges[i].Item1);
                        // Now build path to all pixels not in path.

                    }
                    return null;
                }
            }

            private bool IsWhite(Color c, byte threshold=192)
            {
                byte avg = (byte)((c.R + c.G + c.B) / 3);
                return avg > threshold;
            }
        }
        */
        
        #endregion // Private Class(es)

        #region Ctor
        public ArrayCmd()
        {
            // Default arguments.
            Output = "glyph";
            Left = Right = Top = Bottom = 0;
            HorizontalPadding = VerticalPadding = 0;
            First = 32;
            Last = 127;
            Format = "glyphs";
        }
        #endregion // Ctor

        #region Command Line Arguments
        [Argument(Aliases = "f,file", Required = true, Description = "Source image.")]
        public string Filename { get; set; }

        [Argument(Aliases = "o,save", Required = false, Description = "Output filename without extension (default=glyph, extension depends on format)")]
        public string Output { get; set; }

        [Argument(Aliases = "l", Required = false, Description = "Left margin.")]
        public int Left { get; set; }

        [Argument(Aliases = "t", Required = false, Description = "Top margin.")]
        public int Top { get; set; }

        [Argument(Aliases = "r", Required = false, Description = "Right margin.")]
        public int Right { get; set; }

        [Argument(Aliases = "b", Required = false, Description = "Bottom margin.")]
        public int Bottom { get; set; }

        [Argument(Aliases = "gw", Required = true, Description = "Glyph width.")]
        public int GlyphWidth { get; set; }

        [Argument(Aliases = "gh", Required = true, Description = "Glyph height.")]
        public int GlyphHeight { get; set; }

        [Argument(Aliases = "fmt", Description = "Export format: grid, c, asm, glyphs.")]
        public string Format { get; set; }

        [Argument(Aliases = "hp,ghp", Required = false, Description = "Glyph horizontal padding.")]
        public int HorizontalPadding { get; set; }

        [Argument(Aliases = "vp,gvp", Required = false, Description = "Glyph vertical padding.")]
        public int VerticalPadding { get; set; }

        [Argument(Aliases = "start,s", Required = false, Description = "First ascii.")]
        public int First { get; set; }

        [Argument(Aliases = "end,e", Required = false, Description = "Last ascii.")]
        public int Last { get; set; }

        [Argument(Aliases = "p", Required = false, Description = "Make font proportional.")]
        public bool Proportional { get; set; }

        [Argument(Aliases = "c", Required = false, Description = "Point color in (r,g,b) format, no spaces!")]
        public string PointColor { get; set; }

        [Argument(Aliases = "m,mc", Required = false, Description = "Mask color in (r,g,b) format, no spaces!")]
        public string MackColorColor { get; set; }

        [Argument(Aliases = "tc", Required = false, Description = "Transparent color in (r,g,b) format, no spaces!")]
        public string TransparentColor { get; set; }

        [Argument(Aliases = "th", Required = false, Description = "Point color threshold (0-255) for soft recognition of points.")]
        public int Threshold { get; set; }

        [Argument(Required = false, Description = "Create tiny vector output.")]
        public bool Tiny { get; set; }
        #endregion // Command Line Arguments

        #region Properties
        public StringBuilder Std { get; set; }
        public StringBuilder Err { get; set; }
        public Bitmap SourceBitmap { get; set; }
        public int CurrentGlyphAscii { get; set; }
        public Rectangle CurrentGlyphRect { get; set; }
        #endregion // Properties

        #region Override(s)
        public override string Name { get {return "Array";} }
        public override string Desc { get {return "Create an array from image and export it.";} }
        public override int Execute(StringBuilder std, StringBuilder err)
        {
            // Store output streams.
            Std = std;
            Err = err;

            // Available exports.
            var ie = new ImagesExport();
            Dictionary<string, Export> exports = new Dictionary<string, Export>(StringComparer.InvariantCultureIgnoreCase)
            {
                { "grid", new GridExport() },
                /*{ "c", new CExport() },
                { "asm", new AsmExport() },*/
                { "glyphs", ie },
                { "images", ie },
            };

            // Format must be valid.
            Export export;
            if (exports.Keys.FirstOrDefault(f => f.Equals(Format, StringComparison.InvariantCultureIgnoreCase)) == null)
            {
                Err.AppendFormat("Invalid format {0}.{1}", Format, Environment.NewLine);
                return INVALID_FORMAT;
            }
            else export = exports[Format];

            // Get the bitmap.
            Bitmap loadBmp = Bitmap.FromFile(Filename) as Bitmap;

            // Just in case it is indexed pixel format...
            SourceBitmap = new Bitmap(loadBmp.Width, loadBmp.Height);
            using (Graphics g = Graphics.FromImage(SourceBitmap)) g.DrawImage(loadBmp, 0, 0, loadBmp.Width, loadBmp.Height);

            // Initialize basic members.
            CurrentGlyphAscii = First;
            int x = Left, y = Top; // Respect margins.
            int maxx = SourceBitmap.Width - Left - Right, maxy = SourceBitmap.Height - Top - Bottom;

            // Initialize export.
            int exitCode = (int)export.Begin(this);
            if (exitCode != SUCCESS) return exitCode;


            int advancex = (GlyphWidth + HorizontalPadding), advancey = (GlyphHeight + VerticalPadding);
            for (int yc = y; yc + GlyphHeight - 1 <= maxy; yc += advancey)
                for (int xc = x; xc + GlyphWidth - 1 <= maxx; xc += advancex)
                {
                    // Are we too far?
                    if (CurrentGlyphAscii > Last) goto done;

                    // Get current glyph rect.
                    CurrentGlyphRect = new Rectangle(xc, yc, GlyphWidth, GlyphHeight);

                    // glyph found?
                    exitCode = (int)export.OnGlyph(this);
                    if (exitCode != SUCCESS) return exitCode;

                    // Next ascii.
                    CurrentGlyphAscii++;
                }

            done:
            return (int)export.End(this);
        }
        #endregion Override(s)
    }
}