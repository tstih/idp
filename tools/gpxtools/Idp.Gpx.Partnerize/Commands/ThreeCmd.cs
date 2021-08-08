/*
 * ThreeCmd.cs
 * 
 * Convert image to three colors (quant), size to fit resolution,
 * and dither using only black, white and dotted line.
 *  
 * MIT License (see: LICENSE)
 * Copyright (c) 2021 Tomaz Stih
 * 
 * 08.08.2021   tstih
 * 
 */
using System;
using System.IO;
using System.Text;
using System.Linq;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Imaging;

using AForge.Imaging.Filters;
using AForge.Imaging.ColorReduction;

using Idp.Gpx.Common.CmdLine;
using Idp.Gpx.Common.Generators;
using Idp.Gpx.Common.Utils;

namespace Idp.Gpx.Partnerize.Commands
{

    public class ThreeCmd : Cmd
    {

        #region Ctor
        public ThreeCmd()
        {
        }
        #endregion // Ctor

        #region Command Line Arguments
        [Argument(Aliases = "f,file", Required = true, Description = "Source image.")]
        public string Filename { get; set; }

        [Argument(Aliases = "o,save", Required = false, Description = "Output filename without extension (default=ani, extension depends on format)")]
        public string Output { get; set; }

        [Argument(Aliases = "w", Required = false, Description = "Fit to this width.")]
        public int Width { get; set; }

        [Argument(Aliases = "h", Required = false, Description = "Fit to this height.")]
        public int Height { get; set; }

        [Argument(Aliases = "de,dark", Required = false, Description = "Draw dark edges.")]
        public bool DarkEdge { get; set; }

        [Argument(Aliases = "le,light", Required = false, Description = "Draw light edges.")]
        public bool LightEdge { get; set; }

        #endregion // Command Line Arguments

        #region Properties
        public StringBuilder Std { get; set; }
        public StringBuilder Err { get; set; }
        public Bitmap SourceBitmap { get; set; }
        #endregion // Properties

        #region Override(s)
        public override string Name { get { return "three"; } }
        public override string Desc { get { return "Convert image to three colors (quant), size to fit resolution, and dither using only black, white and dotted line."; } }
        public override int Execute(StringBuilder std, StringBuilder err)
        {
            // Store output streams.
            Std = std;
            Err = err;

            // Go.
            Bitmap normalizedBmp = ChangeFormat(Filename, PixelFormat.Format24bppRgb);
            Bitmap sizedBmp = SizeToFit(normalizedBmp);
            Bitmap grayBmp = Gray(sizedBmp);
            // Detect edges?
            if (DarkEdge) grayBmp = DetectEdges(grayBmp, Black, Green);
            else if (LightEdge) grayBmp = DetectEdges(grayBmp, Green, Black);
            normalizedBmp = ChangeFormat(grayBmp, PixelFormat.Format24bppRgb);
            Color[] palette;
            Bitmap threeBmp = Quant(normalizedBmp, 3, out palette);
            normalizedBmp = ChangeFormat(threeBmp, PixelFormat.Format24bppRgb);
            Bitmap hatchedBmp = Hatch(normalizedBmp, palette, Black, Green);
            // And export as asm.
            Export(hatchedBmp);
    
            // And go home.
            return (int)RetCode.SUCCESS;
        }
        #endregion Override(s)

        #region Helper(s)
        private Color Black { get { return Color.Black; } }
        private Color Green {  get { return Color.FromArgb(0, 255, 0); } }

        private void Export(Bitmap processedBmp)
        {
            // Now generate assembly.
            StringBuilder assembly = new StringBuilder();
            string id = string.Format("{0}_bitmap", Output);
            AsmCodeGenerator gen = new AsmCodeGenerator(assembly, 2, 8);

            // Generate standard header.
            gen.AddHeader(Output, id, ".s", "see glyph.h for format details")
                .AddDirective("module", id).NextLine()
                .AddDirective("globl", "_" + id).NextLine()
                .AddDirective("area", "_CODE")
                .AddLabel("_" + id, true)
                .AddComment("bitmap header")
                .AddDirective("db", (byte)0, "bitmap generation")
                .AddDirective("dw", processedBmp.Width.ToString(), "bitmap width")
                .AddDirective("dw", processedBmp.Height.ToString(), "bitmap height")
                .AddDirective("db", (byte)Math.Floor((float)processedBmp.Width / 8.0f), "bitmap stride")
                .AddComment("bitmap raw data");

            GlyphProcessor gproc = new GlyphProcessor(processedBmp);
            byte[] bytes = gproc.ToBytes();
            gen.AddByteTable(bytes, 16);

            // And save as text.
            File.WriteAllText(Output + ".s", assembly.ToString());

            // Now convert back from byes and save image as a test.
            Bitmap bmp = new Bitmap(processedBmp.Width, processedBmp.Height);
            int stride;
            if (processedBmp.Width % 8 != 0)
                stride = processedBmp.Width / 8 + 1;
            else
                stride = processedBmp.Width / 8;

            for (int y = 0; y < processedBmp.Height; y++)
                for (int x = 0; x < processedBmp.Width; x++)
                {
                    int offset = y * stride + x / 8;
                    int bit = x % 8;
                    int b = bytes[offset];
                    b = b >> bit;
                    b = b & 0x01;
                    if (b != 0)
                        bmp.SetPixel(x, y, Color.Black);
                    else
                        bmp.SetPixel(x, y, Color.White);
                }

            processedBmp.Save(Output + ".png", ImageFormat.Png);

            File.WriteAllBytes(Output + ".bin", bytes);

        }

        Bitmap SizeToFit(Bitmap bmp)
        {
            float scaleFactor = ScaleFactor(
                new Rectangle(0, 0, Width, Height), // Desired size.
                new Rectangle(0,0,bmp.Width,bmp.Height) // Actual size
                );
            if (scaleFactor < 1) // Only reduce size. Don't increase!
            {
                int newWidth = (int)Math.Floor((float)bmp.Width * scaleFactor);
                int newHeight = (int)Math.Floor((float)bmp.Height * scaleFactor);
                // Resize filter.
                ResizeBicubic filter = new ResizeBicubic(newWidth, newHeight);
                // apply the filter
                Bitmap newImage = filter.Apply(bmp);
                return newImage;
            }
            else
                return bmp;
        }

        Bitmap DetectEdges(Bitmap bmp, Color black, Color white)
        {
            // create filter
            CannyEdgeDetector filter = new CannyEdgeDetector();
            // apply the filter
            Bitmap edges = filter.Apply(bmp);
            edges.Save("edges.png");
            Bitmap normalEdges=ChangeFormat(edges, PixelFormat.Format24bppRgb);
            // New bitmap.
            Bitmap finalBmp = new Bitmap(bmp.Width, bmp.Height);
            // Now copy only first and last color.
            for (int y = 0; y < bmp.Height; y++)
                for (int x = 0; x < bmp.Width; x++)
                {
                    Color c = normalEdges.GetPixel(x, y);
                    if (c.R < filter.LowThreshold && c.G < filter.LowThreshold & c.B < filter.LowThreshold)
                        // For black pixels, copy source.
                        finalBmp.SetPixel(x, y, bmp.GetPixel(x, y));
                    else
                        finalBmp.SetPixel(x, y, black);
                }
            return finalBmp;
        }

        Bitmap ChangeFormat(string filename, PixelFormat pf=PixelFormat.Format32bppArgb)
        {
            return AForge.Imaging.Image.Clone(
                Bitmap.FromFile(filename) as Bitmap,
                pf);
        }

        Bitmap ChangeFormat(Bitmap bmp, PixelFormat pf = PixelFormat.Format32bppArgb)
        {
            return AForge.Imaging.Image.Clone(
                bmp,
                pf);
        }

        Bitmap Quant(Bitmap bmp, int colors, out Color[] palette)
        {
            // Instantiate the images' color quantization class.
            ColorImageQuantizer ciq = new ColorImageQuantizer(new MedianCutQuantizer());
            palette = ciq.CalculatePalette(bmp, colors).OrderBy(c => c.R).ToArray();
            // Reduce colors in the specified image,
            return ciq.ReduceColors(bmp, colors);
        }

        Bitmap Gray(Bitmap bmp)
        {
            // Create grayscale filter.
            Grayscale filter = new Grayscale(0.2125, 0.7154, 0.0721);
            // Apply the filter,
            Bitmap grayImage = filter.Apply(bmp);
            // And return.
            return grayImage;
        }

        Bitmap Hatch(Bitmap inputBmp, Color[] palette, Color black, Color white)
        {
            // Sort the palette.
            palette.OrderBy(c => c.R);
            // New image.
            Bitmap finalBmp = new Bitmap(inputBmp.Width, inputBmp.Height);
            // Hatch entire bitmap. 
            using (Graphics g = Graphics.FromImage(finalBmp))
            using (HatchBrush hbrush = new HatchBrush(HatchStyle.Percent50, black, white))
                g.FillRectangle(hbrush, 0, 0, finalBmp.Width, finalBmp.Height);
            // Now copy only first and last color.
            for (int y = 0; y < inputBmp.Height; y++)
                for (int x = 0; x < inputBmp.Width; x++)
                {
                    Color c = inputBmp.GetPixel(x, y);
                    if (c.R == palette[0].R)
                        finalBmp.SetPixel(x, y, black);
                    else if (c.R == palette[2].R)
                        finalBmp.SetPixel(x, y, white);
                }
            // And return it.
            return finalBmp;
        }

        private float ScaleFactor(Rectangle outer, Rectangle inner)
        {
            float factor = (float)outer.Height / (float)inner.Height;
            if ((float)inner.Width * factor > outer.Width) // Switch!  
                factor = (float)outer.Width / (float)inner.Width;
            return factor;
        }

        #endregion // Helper(s)
    }
}
