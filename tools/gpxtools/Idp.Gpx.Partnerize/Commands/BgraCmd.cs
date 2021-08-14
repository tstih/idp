/*
 * BgraCmd.cs
 * 
 * Convert an image (of any type) into raw rgb format.
 *  
 * MIT License (see: LICENSE)
 * Copyright (c) 2021 Tomaz Stih
 * 
 * 08.06.2021   tstih
 * 
 */
using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;
using System.Drawing;
using System.Drawing.Imaging;

using Idp.Gpx.Common.CmdLine;

using Idp.Gpx.Common.Generators;

using Idp.Gpx.Common.Utils;

namespace Idp.Gpx.Partnerize.Commands
{

    public class BgraCmd : Cmd
    {

        #region Ctor
        public BgraCmd()
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
        public override string Name { get { return "bgra"; } }
        public override string Desc { get { return "Convert an image to raw 3bpp RGB format."; } }
        public override int Execute(StringBuilder std, StringBuilder err)
        {
            // Store output streams.
            Std = std;
            Err = err;

            // And export as asm.
            Export(Filename, Output);

            // And go home.
            return (int)RetCode.SUCCESS;
        }
        #endregion Override(s)

        #region Helper(s)
        private void Export(string fin, string fout)
        {
            // Input bitmap.
            Bitmap bmp = Image.FromFile(fin) as Bitmap;

            // each pixel takes 3 bytes, 24bpp
            int stride = 4 * bmp.Width;

            // Make space for it.
            byte[] rawb = new byte[bmp.Height * stride];

            // Populate it.
            for (int y = 0; y < bmp.Height; y++)
                for (int x = 0; x < bmp.Width; x++)
                {
                    int offset = stride * y + 4 * x;
                    Color c = bmp.GetPixel(x, y);
                    rawb[offset] = c.B;
                    rawb[offset + 1] = c.G;
                    rawb[offset + 2] = c.R;
                    rawb[offset + 3] = 0; // A
                }

            // Now generate C source code.
            StringBuilder sourceCode = new StringBuilder();
            string id = string.Format("{0}", Output);
            CCodeGenerator gen = new CCodeGenerator(sourceCode);
            string mainComment = string.Format("resolution is {0}x{1}, format is 32bpp BGRA, stride is {2}, size (in bytes) is {3}", bmp.Width, bmp.Height, stride, stride * bmp.Height);
            string arrayHead = string.Format("uint8_t {0}[] = {{", id), arrayTail = "};";
            gen
                .AddHeader(id, "Raster image (raw BGRA) bytes.", ".cpp", "tstih", "Tomaz Stih", cpp:true)
                .LineOfCode("#include <cstdint>")
                .NextLine()
                .CommentOnly(mainComment, 0, true)
                .LineOfCode(arrayHead)
                .ArrayAsBytes(rawb, 16, null, true)
                .LineOfCode(arrayTail);


            // And save as text.
            File.WriteAllText(Output + ".cpp", sourceCode.ToString());
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
            GlyphProcessor gproc = new GlyphProcessor(Image.FromFile(fname) as Bitmap);
            return gproc.QuantDither(monochrome);
        }
        #endregion // Helper(s)
    }
}
