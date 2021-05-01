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
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Drawing.Imaging;

using XYZ.Ex;
using XYZ.Formats;
using Idp.Gpx.Common.CmdLine;
using System.IO;
using Idp.Gpx.Common.Generators;
using System;
using Idp.Gpx.Common.Utils;

namespace Idp.Gpx.Silhouette.Commands {

    public class AnimateCmd : Cmd {

        #region Private Classes
        private class AnimationFrame
        {
            public int Index;
            public int Delay;
            public List<Line> Lines;
        }
        #endregion // Private Classes

        #region Ctor
        public AnimateCmd()
        {
            // Default arguments.
            Output = "ani";
            Outline = true;
            Threshold = 1;
            Format = "C";
        }
        #endregion // Ctor

        #region Command Line Arguments
        [Argument(Aliases = "f,file", Required = true, Description = "Source image.")]
        public string Filename { get; set; }

        [Argument(Aliases = "o,save", Required = false, Description = "Output filename without extension (default=ani, extension depends on format)")]
        public string Output { get; set; }

        [Argument(Aliases = "th", Required = false, Description = "Douglas Peucker threshold for polgygon point reduction (default=1).")]
        public float Threshold { get; set; }

        [Argument(Required = false, Description = "Generate outline instead of fill.")]
        public bool Outline { get; set; }

        [Argument(Aliases = "fmt", Description = "Export format: c.")]
        public string Format { get; set; }
        #endregion // Command Line Arguments

        #region Properties
        public StringBuilder Std { get; set; }
        public StringBuilder Err { get; set; }
        public Bitmap SourceBitmap { get; set; }
        #endregion // Properties

        #region Override(s)
        public override string Name { get { return "Animate"; } }
        public override string Desc { get { return "Create an line animation out of animated gif."; } }
        public override int Execute(StringBuilder std, StringBuilder err)
        {
            // Store output streams.
            Std = std;
            Err = err;

            AnimationHeader a = new AnimationHeader()
            {
                GlyphHeader = new GlyphHeader()
                {
                    Generation= (byte)((int)GenerationGlyphType.Animation|((int)GenerationDrawMode.Lines<<3)),
                    LineWidthInBytes=0
                }
            };

            // For each bitmap in animated gif...
            int frameCount = 0;
            List<AnimationFrame> frames = new List<AnimationFrame>();

            int w=0, h=0;
            foreach (Bitmap bmp in SeparateFrames(Filename))
            {
                // We'll need this for the header.
                w = bmp.Width; h = bmp.Height;

                // Extract the frame.
                AnimationFrame frame = GetNextFrame(bmp, frameCount);

                // Add it to frames.
                frames.Add(frame);

                // Next image.
                frameCount++;
            }
            a.GlyphHeader.Width = (ushort)w; a.GlyphHeader.Height = (ushort)h;

            // Finally, export full animation.
            // And, finally, write C code.
            StringBuilder sb = new StringBuilder();
            CCodeGenerator cgen = new CCodeGenerator(sb);

            if (Outline)
            {
                cgen
                    .AddHeader(Output, "Outline animmation.", ".c", "tstih", "Tomaz Stih")
                    .Include("yos.h", true)
                    .LineOfCode(string.Format("byte_t {0}[] = {{", Output.ToLower()));

                // Export glyph header first.
                cgen.CommentedByte((byte)a.GlyphHeader.Generation, "Lines animation format.", true);
                cgen.CommentedShort((ushort)a.GlyphHeader.Width, string.Format("Glyph width={0}.", a.GlyphHeader.Width));
                cgen.CommentedShort((ushort)a.GlyphHeader.Height, string.Format("Glyph height={0}.", a.GlyphHeader.Height));
                cgen.CommentedByte((byte)0, "Reserved byte. Only used for raster graphics.");
                cgen.CommentedByte((byte)frames.Count, string.Format("# animation frames={0}.", frames.Count));

                // And now frames.
                foreach (AnimationFrame f in frames)
                {
                    if (f.Lines.Count > 0)
                    {
                        // Write animation header.
                        cgen.NextLine();
                        cgen.CommentOnly(string.Format("Frame #{0}.", f.Index));
                        cgen.CommentedByte((byte)f.Index, string.Format("Frame index={0}.", f.Index));
                        cgen.CommentedByte((byte)f.Delay, "Delay before playing.");
                        cgen.CommentedShort((ushort)f.Lines.Count, string.Format("Number of lines following={0}.", f.Lines.Count));
                        cgen.LinesAsBytes(f.Lines.ToArray());
                    }
                }

                // Add frames here.

                // Conclude byte array.
                cgen.LineOfCode("};");

            }

            // And save!
            File.WriteAllText(Output + ".c", sb.ToString());

            // And go home.
            return (int)RetCode.SUCCESS;
        }
        #endregion Override(s)

        #region Helper(s)
        private AnimationFrame GetNextFrame(Bitmap bmp, int index)
        {
            AnimationFrame af = new AnimationFrame()
            {
                Index = index,
                Delay = 0, // TODO: Get this data from animated gif.
            };

            if (Outline)
            {
                // Get outline.
                af.Lines = GetOutline(bmp);

                
            }

            return af;
        }

        private List<Line> GetOutline(Bitmap bmp)
        {
            // Scan bmp.
            var gp = new GlyphProcessor(bmp);
            var bounds = gp.OutlineBitmap();

            // Result here.
            List<Line> lines = new List<Line>();

            // Iterate through strokes.
            foreach (var stroke in bounds)
            {
                // Minimize number of points by interpolation.
                var min = gp.DouglasPeuckerReduction(stroke, Threshold);
                if (min.Count > 1)
                    lines.AddRange(gp.Polygon2Lines(min));
            }

            // And return the outline.
            return lines;
        }

        // Animated bitmap to multiple bitmaps, convert to 1bpp.
        private Bitmap[] SeparateFrames(string filename)
        {
            List<Bitmap> frames = new List<Bitmap>();

            Image animation = Image.FromFile(filename);
            FrameDimension dimension = new FrameDimension(animation.FrameDimensionsList[0]);
            // Number of frames
            int frameCount = animation.GetFrameCount(dimension);
            for (int index = 0; index < frameCount; index++)
            {
                // Return an Image at a certain index
                animation.SelectActiveFrame(dimension, index);
                Image frameImg = new Bitmap(animation.Width, animation.Height);
                using (Graphics g = Graphics.FromImage(frameImg))
                {
                    // Separate single frame from animated gif.
                    g.DrawImageUnscaled(animation, Point.Empty);

                    // Make 1bpp.
                    BitmapData bmpData = (frameImg as Bitmap).LockBits(
                        new Rectangle(0, 0, frameImg.Width, frameImg.Height),
                        ImageLockMode.ReadOnly,
                        PixelFormat.Format1bppIndexed);

                    Bitmap bmp1bpp = new Bitmap(frameImg.Width, frameImg.Height,
                        bmpData.Stride, PixelFormat.Format1bppIndexed, bmpData.Scan0);
                    frames.Add(bmp1bpp);
                }
            }
            return frames.ToArray();
        }
        #endregion // Helper(s)
    }
}
