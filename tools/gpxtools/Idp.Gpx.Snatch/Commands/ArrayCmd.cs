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

        [Argument(Aliases = "ew", Required = false, Description = "Empty glyph width?")]
        public int EmptyWidth { get; set; }
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
                /*{ "c", new CExport() },*/
                { "asm", new AsmExport() },
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

            // Empty glyph width.
            if (EmptyWidth == 0) EmptyWidth = GlyphWidth;

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