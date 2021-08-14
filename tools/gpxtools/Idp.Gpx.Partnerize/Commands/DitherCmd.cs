/*
 * DitherCmd.cs
 * 
 * Dither an image and save in raw format.
 *  
 * MIT License (see: LICENSE)
 * Copyright (c) 2021 Tomaz Stih
 * 
 * 31.05.2021   tstih
 * 
 */
using System.Text;
using System.Drawing;
using System.Drawing.Imaging;

using Idp.Gpx.Common.CmdLine;
using System.IO;
using Idp.Gpx.Common.Generators;
using System;
using Idp.Gpx.Common.Utils;

namespace Idp.Gpx.Partnerize.Commands
{

    public class DitherCmd : Cmd
    {

        #region Ctor
        public DitherCmd()
        {
            Width = Height = 1024;
        }
        #endregion // Ctor

        #region Command Line Arguments
        [Argument(Aliases = "f,file", Required = true, Description = "Source image.")]
        public string Filename { get; set; }

        [Argument(Aliases = "o,save", Required = false, Description = "Output filename without extension (default=ani, extension depends on format)")]
        public string Output { get; set; }

        [Argument(Aliases = "w", Required = false, Description = "Fit to this width. Only for reducing size.")]
        public int Width { get; set; }

        [Argument(Aliases = "h", Required = false, Description = "Fit to this height. Only for reducing size.")]
        public int Height { get; set; }

        [Argument(Aliases = "inv", Required = false, Description = "Inverse bitmap.")]
        public bool Inverse { get; set; }

        #endregion // Command Line Arguments

        #region Properties
        public StringBuilder Std { get; set; }
        public StringBuilder Err { get; set; }
        public Bitmap SourceBitmap { get; set; }
        #endregion // Properties

        #region Override(s)
        public override string Name { get { return "dither"; } }
        public override string Desc { get { return "Convert an image to Iskra Delta Partner raw binary format."; } }
        public override int Execute(StringBuilder std, StringBuilder err)
        {
            // Store output streams.
            Std = std;
            Err = err;

            // Dither.
            Bitmap dithered=Dither(Filename, Inverse);

            // And export as asm.
            Export(dithered);

            // And go home.
            return (int)RetCode.SUCCESS;
        }
        #endregion Override(s)

        #region Helper(s)
        private void Export(Bitmap dithered)
        {
            // Now generate assembly.
            StringBuilder assembly = new StringBuilder();
            string id = string.Format("{0}_raster", Output);
            AsmCodeGenerator gen = new AsmCodeGenerator(assembly, 2, 8);

            // Generate standard header.
            int stride = 0;
            if (dithered.Width % 8 != 0)
                stride = dithered.Width / 8 + 1;
            else
                stride = dithered.Width / 8;

            gen.AddHeader(Output, id, ".s", "see gpx.h for format details")
                .AddDirective("module", id).NextLine()
                .AddDirective("globl", "_" + id).NextLine()
                .AddDirective("area", "_CODE")
                .AddLabel("_" + id, true)
                .AddComment("raster header")
                .AddDirective("db", (byte)(stride - 1), "class(top nibble) + stride(bot nibble)")
                .AddDirective("db", (dithered.Width-1).ToString(), "width")
                .AddDirective("db", (dithered.Height-1).ToString(), "height")
                .AddDirective("db", 0, "reserved")
                .AddComment("bitmap raw data");

            GlyphProcessor gproc = new GlyphProcessor(dithered);
            byte[] bytes = gproc.ToBytes();
            gen.AddByteTable(bytes, 16);

            // And save as text.
            File.WriteAllText(Output+".s", assembly.ToString());

            // Now convert back from byes and save image as a test.
            Bitmap bmp = new Bitmap(dithered.Width, dithered.Height);

            for(int y=0;y<dithered.Height;y++)
                for(int x=0;x<dithered.Width;x++)
                {
                    int offset = y * stride + x / 8;
                    int bit = 7 - (x % 8);
                    int b = bytes[offset];
                    b = b >> bit;
                    b = b & 0x01;
                    if (b != 0)
                        bmp.SetPixel(x, y, Color.Black);
                    else
                        bmp.SetPixel(x, y, Color.White);
                }

            bmp.Save(Output + ".png", ImageFormat.Png);

            using (FileStream fileStream = new FileStream(Output + ".gph", FileMode.Create))
            {
                fileStream.WriteByte((byte)(stride-1));
                fileStream.WriteByte((byte)(dithered.Width - 1));
                fileStream.WriteByte((byte)(dithered.Height - 1));
                fileStream.WriteByte(0);
                fileStream.Write(bytes);
            }
        }

        Bitmap Dither(string fname, bool inverse=false)
        {
            // Generate...
            Color[] monochrome = new Color[]
            {
                Color.FromArgb( 0x00, 0x00, 0x00 ),
                Color.FromArgb( 0xff, 0xff, 0xff )
            };

            // Load...
            GlyphProcessor gproc=new GlyphProcessor(fname);
            gproc.SizeToFit(Width, Height);
            Bitmap dithered=gproc.QuantDither(monochrome);
            if (inverse)
            {
                for (int y = 0; y < dithered.Height; y++)
                    for (int x = 0; x < dithered.Width; x++)
                        if (dithered.GetPixel(x, y) == monochrome[0])
                            dithered.SetPixel(x, y, monochrome[1]);
                        else
                            dithered.SetPixel(x, y, monochrome[0]);
            }
            return dithered;
        }
        #endregion // Helper(s)
    }
}
