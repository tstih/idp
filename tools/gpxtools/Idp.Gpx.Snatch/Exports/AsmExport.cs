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
using Idp.Gpx.Common.Utils;

namespace Idp.Gpx.Snatch.Exports
{
    public class AsmExport : Export
    {
        #region Const(s)
        private const int FONT_HEADER_SIZE = 8; // 6 bytes for font header.
        #endregion // Const(s)

        #region Private(s)
        private StringBuilder _headersInternal;
        private FontAsmCodeGenerator _headers;
        private StringBuilder _dataInternal;
        private FontAsmCodeGenerator _data;
        private ushort[] _widths;
        private ushort[] _offs;
        private ushort _off;
        #endregion // Private(s)

        #region Ctor
        public AsmExport()
        {
            _headersInternal = new StringBuilder();
            _dataInternal = new StringBuilder();
        }
        #endregion // Ctor

        #region Export Implementation
        public override RetCode Begin(ArrayCmd cmd)
        {
            _off = 0;
            _headers = new FontAsmCodeGenerator(_headersInternal);
            _data = new FontAsmCodeGenerator(_dataInternal);
            byte bytesPerGlyphLine = (byte)((cmd.GlyphWidth - 1) / 8 + 1);
            FontType targetType = 0;
            if (!cmd.Proportional) targetType |= FontType.Fixed;
            if (cmd.Tiny) targetType |= FontType.Tiny;
            _headers.AddFontHeader(
                cmd.Output,
                targetType,
                (byte)cmd.GlyphWidth,
                (byte)cmd.GlyphHeight,
                bytesPerGlyphLine,
                (byte)cmd.First,
                (byte)cmd.Last);
            _widths = new ushort[cmd.Last - cmd.First + 1];
            _offs = new ushort[cmd.Last - cmd.First + 1];

            return RetCode.SUCCESS;
        }

        public override RetCode OnGlyph(ArrayCmd cmd)
        {
            // First extract glyph to a separate bitmap.
            GlyphProcessor gp = new GlyphProcessor(cmd.SourceBitmap,cmd.CurrentGlyphRect);

            if (cmd.Tiny)
            {
                // Let's vectorize the shit ouf of it!
                byte maxw=0;
                byte[] moves = gp.ToTiny(ref maxw);
                _data.AddTinyGlyph(
                    cmd.CurrentGlyphAscii,
                    moves);
                // Remember glyph offset.
                int index = cmd.CurrentGlyphAscii - cmd.First;
                _offs[index] = _off;
                if (moves != null)
                    _widths[index] = (ushort)(maxw);
                else 
                    _widths[index] = (ushort)cmd.EmptyWidth;
                int m;
                if (moves != null) m = moves.Length; else m = 1;
                _off = (ushort)(_off + m);
            }
            else
            {
                var bytesPerGlyphLine = (byte)((cmd.GlyphWidth - 1) / 8 + 1);
                var bits = gp.ToBits();

                // And generate as 0b const.
                _data.AddRasterFontGlyph(
                    cmd.CurrentGlyphAscii,
                    bits,
                    bytesPerGlyphLine,
                    (byte)cmd.CurrentGlyphRect.Height, 0, 0);
            }

            return RetCode.SUCCESS;
        }

        public override RetCode End(ArrayCmd cmd)
        {
            // Merge _data and _headers.
            StringBuilder asm = new StringBuilder();
            
            // Table of width (if proportional).
            if (cmd.Proportional)
            {
                _headers.TableOfWidths(_widths);
            }

            // Pointers to letters if tiny.
            if (cmd.Tiny)
            {
                _headers.GlyphOffsets(_offs);
            }

            asm.Append(_headersInternal.ToString());
            asm.Append(_dataInternal.ToString());

            // Write result to text file.
            File.WriteAllText(cmd.Output + ".s", asm.ToString());

            // And write result to std. output.
            cmd.Std.Append(asm.ToString());

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
