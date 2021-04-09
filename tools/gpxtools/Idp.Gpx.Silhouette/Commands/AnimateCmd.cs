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
using Idp.Gpx.Common.Glyphs;
using System.IO;

namespace Idp.Gpx.Silhouette.Commands {

    public class AnimateCmd : Cmd {

        #region Ctor
        public AnimateCmd()
        {
            // Default arguments.
            Output = "ani";
            Outline = true;
            Threshold = 1;
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

            // For each bitmap in animated gif...
            int frameCount = 0;
            foreach (Bitmap bmp in SeparateFrames(Filename))
            {
                if (!Outline)
                    SaveLines(bmp, frameCount);
                else
                    SaveOutline(bmp, frameCount);

                // Next image.
                frameCount++;
            }

            // And go home.
            return (int)RetCode.SUCCESS;
        }
        #endregion Override(s)

        #region Helper(s)
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

        private void SaveOutline(Bitmap bmp, int index)
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
                    lines.AddRange(Poly2Lines(min));
            }

            // And save lines.
            LineHeader h = new LineHeader()
            {
                w = (ushort)bmp.Width,
                h = (ushort)bmp.Height,
                lines = (ushort)lines.Count
            };

            using (var file = File.OpenWrite(string.Format("{0}_{1}.lin", Output, index)))
            {
                // First write header.
                var ba = h.ToByteArray();
                file.Write(h.ToByteArray()); // Writes the entire list.

                // Now each line.
                foreach (Line l in lines)
                    file.Write(l.ToByteArray());
            }

            // Write test png.
            Bitmap testbmp = gp.FromLines(bmp.Width, bmp.Height, lines.ToArray());
            testbmp.Save(string.Format("{0}_{1}.png", Output, index),ImageFormat.Png);
        }

        private List<Line> Poly2Lines(List<Point> pts)
        {
            List<Line> result = new List<Line>();
             // What are you doing here?
            if (pts.Count > 2)
            {
                int prev = 0;
                for (int curr = 1; curr < pts.Count; curr++)
                {
                    result.Add(new Line()
                    {
                        x1 = (ushort)pts[prev].X,
                        x2 = (ushort)pts[curr].X,
                        y1 = (ushort)pts[prev].Y,
                        y2 = (ushort)pts[curr].Y
                    });
                    prev = curr;
                }
            }
            return result;
        }

        private void SaveLines(Bitmap bmp, int index)
        {
            // Scan bmp.
            var gp = new GlyphProcessor(bmp);
            Line[] lines = gp.AsLines(bmp);

            // And save lines.
            LineHeader h = new LineHeader()
            {
                w = (ushort)bmp.Width,
                h = (ushort)bmp.Height,
                lines = (ushort)lines.Length
            };

            using (var file = File.OpenWrite(string.Format("{0}_{1}.lin", Output, index)))
            {
                // First write header.
                var ba = h.ToByteArray();
                file.Write(h.ToByteArray()); // Writes the entire list.

                // Now each line.
                foreach (Line l in lines)
                    file.Write(l.ToByteArray());
            }
        }
        #endregion // Helper(s)
    }
}