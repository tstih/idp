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
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Text;

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
        private ushort[] _offs;
        private ushort _off;
        private Color foreColor, backColor;
        private List<byte> _bin;
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
            _bin = new List<byte>();
            _off = 0;
            _headers = new FontAsmCodeGenerator(_headersInternal);
            _data = new FontAsmCodeGenerator(_dataInternal);
            // Bit 8 of font flags is for proportional font
            int fontFlags = (int)cmd.HorizontalSpacingHint;
            if (cmd.Proportional)
                fontFlags |= (int)0x0080;
            _headers.AddFontHeader(
                cmd.Output,
                (byte)fontFlags,
                (byte)cmd.GlyphWidth,
                (byte)cmd.GlyphHeight,
                (byte)cmd.First,
                (byte)cmd.Last);
            _offs = new ushort[cmd.Last - cmd.First + 1];

            // Add bytes to the binary structure.
            _bin.Add((byte)fontFlags);
            _bin.Add((byte)cmd.GlyphWidth);
            _bin.Add((byte)cmd.GlyphHeight);
            _bin.Add((byte)cmd.First);
            _bin.Add((byte)cmd.Last);

            return RetCode.SUCCESS;
        }

        public override RetCode OnGlyph(ArrayCmd cmd)
        {
            // First extract glyph to a separate bitmap.
            GlyphProcessor gp = new GlyphProcessor(cmd.SourceBitmap,cmd.CurrentGlyphRect);

            // Get colors.
            foreColor = gp.ColorFromString(cmd.PointColor);
            backColor = gp.ColorFromString(cmd.TransparentColor);
            int index = cmd.CurrentGlyphAscii - cmd.First;
            int totalGlyphs = cmd.Last - cmd.First + 1;

            if (cmd.Tiny)
            {
                // Let's vectorize the shit ouf of it!
                byte maxw=0;
                byte[] moves = gp.ToTiny(ref maxw);
                _data.AddTinyGlyph(
                    cmd.CurrentGlyphAscii,
                    moves);
                // Remember glyph offset.
                _offs[index] = _off;
                int m;
                if (moves != null) m = moves.Length; else m = 1;
                _off = (ushort)(_off + m);
            }
            else
            {
                Rectangle glyphBounds;
                if (cmd.Proportional)
                {
                    gp.FindGlyphBounds(
                        out glyphBounds,
                        cmd.EmptyWidth,
                        foreColor,
                        (byte)cmd.Threshold);
                } 
                else
                    glyphBounds = new Rectangle(
                        0, 0, cmd.GlyphWidth, cmd.GlyphHeight);

                var bits = gp.ToBits(foreColor,(byte)cmd.Threshold,
                    glyphBounds);

                int stride;
                if (glyphBounds.Width % 8 != 0)
                    stride = glyphBounds.Width / 8 + 1;
                else
                    stride = glyphBounds.Width / 8;

                // And generate as 0b const.
                _data.AddRasterFontGlyph(
                    cmd.CurrentGlyphAscii,
                    bits,
                    (byte)stride,
                    (byte)glyphBounds.Width,
                    (byte)glyphBounds.Height,
                    _bin,
                    0, 
                    0);

                // Add glyph to the table of offsets.
                _offs[index] = _off;
                _off = (ushort)(_off +
                    4 + // Glyph header!
                    stride * 
                    glyphBounds.Height);
            }

            return RetCode.SUCCESS;
        }

        public override RetCode End(ArrayCmd cmd)
        {
            // Merge _data and _headers.
            StringBuilder asm = new StringBuilder();

            // Pointers to letters. Add header size (5)
            // and 2 x offset table size
            for (int i = 0; i < _offs.Length; i++)
                _offs[i] = (ushort)(_offs[i] + 5 + 2 * _offs.Length);
            _headers.GlyphOffsets(_offs);

            asm.Append(_headersInternal.ToString());
            asm.Append(_dataInternal.ToString());

            // Write result to text file.
            File.WriteAllText(cmd.Output + ".s", asm.ToString());

            // And bin array to binary file.
            File.WriteAllBytes(cmd.Output + ".f", _bin.ToArray());

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
