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

namespace Idp.Gpx.Partnerize.Commands
{

    public class DitherCmd : Cmd
    {

        #region Ctor
        public DitherCmd()
        {
        }
        #endregion // Ctor

        #region Command Line Arguments
        [Argument(Aliases = "f,file", Required = true, Description = "Source image.")]
        public string Filename { get; set; }

        [Argument(Aliases = "o,save", Required = false, Description = "Output filename without extension (default=ani, extension depends on format)")]
        public string Output { get; set; }
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
            Bitmap dithered=Dither(Filename);

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
                .AddDirective("dw", dithered.Width.ToString(), "bitmap width")
                .AddDirective("dw", dithered.Height.ToString(), "bitmap height")
                .AddDirective("db", (byte)Math.Floor((float)dithered.Width/8.0f), "bitmap stride")
                .AddComment("bitmap raw data");

            GlyphProcessor gproc = new GlyphProcessor(dithered);
            byte[] bytes = gproc.ToBytes();
            gen.AddByteTable(bytes, 16);

            // And save as text.
            File.WriteAllText(Output+".s", assembly.ToString());

            // Now convert back from byes and save image as a test.
            Bitmap bmp = new Bitmap(dithered.Width, dithered.Height);
            int stride;
            if (dithered.Width % 8 != 0)
                stride = dithered.Width / 8 + 1;
            else
                stride = dithered.Width / 8;

            for(int y=0;y<dithered.Height;y++)
                for(int x=0;x<dithered.Width;x++)
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

            bmp.Save(Output + ".png", ImageFormat.Png);

            File.WriteAllBytes(Output + ".bin", bytes);

        }

        Bitmap Dither(string fname)
        {
            // Generate...
            Color[] monochrome = new Color[]
            {
                Color.FromArgb( 0x00, 0x00, 0x00 ),
                Color.FromArgb( 0xff, 0xff, 0xff )
            };

            // Load...
            GlyphProcessor gproc=new GlyphProcessor(Image.FromFile(fname) as Bitmap);
            return gproc.QuantDither(monochrome);
        }
        #endregion // Helper(s)
    }
}
