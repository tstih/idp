/*
 * AsmExport.cs
 * 
 * Export new image to assembly.
 *  
 * MIT License (see: LICENSE)
 * Copyright (c) 2021 Tomaz Stih
 * 
 * 02.03.2021   tstih
 * 
 */
using System.Collections;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Text;

using XYZ.Formats;

using Idp.Gpx.Snatch.Commands;
using Idp.Gpx.Snatch.Generators;
using Idp.Gpx.Common.Glyphs;

namespace Idp.Gpx.Snatch.Exports
{
    public class AsmExport : Export
    {
        #region Private(s)
        private StringBuilder _internal;
        private FontAsmCodeGenerator _asm;
        #endregion // Private(s)

        #region Ctor
        public AsmExport()
        {
            _internal = new StringBuilder();
        }
        #endregion // Ctor

        #region Export Implementation
        public override RetCode Begin(ArrayCmd cmd)
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
            return RetCode.SUCCESS;
        }

        public override RetCode OnGlyph(ArrayCmd cmd)
        {
            // First extract glyph to a separate bitmap.
            GlyphProcessor gp = new GlyphProcessor(cmd.SourceBitmap,cmd.CurrentGlyphRect);

            

            if (cmd.Tiny)
            {
                // Let's vectorize the shit ouf of it!
                byte[] moves = gp.ToTiny();

            }
            else
            {
                var bytesPerGlyphLine = (byte)((cmd.GlyphWidth - 1) / 8 + 1);
                var bits = gp.ToBits();

                // And generate as 0b const.
                _asm.AddRasterFontGlyph(
                    cmd.CurrentGlyphAscii,
                    bits,
                    bytesPerGlyphLine,
                    (byte)cmd.CurrentGlyphRect.Height, 0, 0);
            }

           

            return RetCode.SUCCESS;
        }

        public override RetCode End(ArrayCmd cmd)
        {
            // Write result to text file.
            File.WriteAllText(cmd.Output + ".s", _internal.ToString());

            // And write result to std. output.
            cmd.Std.Append(_internal.ToString());

            return RetCode.SUCCESS;
        }
        #endregion // Export Implementation

        #region Helper(s)
        private bool IsWhite(Color c, byte threshold = 192)
        {
            byte avg = (byte)((c.R + c.G + c.B) / 3);
            return avg > threshold;
        }
        #endregion // Helper(s)
    }
}
