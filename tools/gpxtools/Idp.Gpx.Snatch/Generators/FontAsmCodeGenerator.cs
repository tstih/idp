/*
 * FontAsmCodeGenerator.cs
 * 
 * Functions to generate font source code in assembler.
 *  
 * MIT License (see: LICENSE)
 * Copyright (c) 2021 Tomaz Stih
 * 
 * 02.04.2021   tstih
 * 
 */
using System;
using System.Collections.Generic;
using System.Collections;
using System.Text;

using Idp.Gpx.Common.Generators;

namespace Idp.Gpx.Snatch.Generators
{
    public class FontAsmCodeGenerator : AsmCodeGenerator
    {
        #region Ctor
        public FontAsmCodeGenerator(StringBuilder sb, int codeIdentTabs = 2, int commentIdentTabs = 8)
            : base(sb,2,8)
        { }
        #endregion // Ctor

        #region Method(s)
        public void AddFontHeader(
            string name,
            byte fontFlags,
            byte width,
            byte height,
            byte firstAscii,
            byte lastAscii)
        {
            string id = string.Format("{0}_font", name);

            // 0 base width and height
            width--; height--;

            // Generate standard header.
            AddHeader(name, id, ".s", "see font.h for format details")
                .AddDirective("module", id).NextLine()
                .AddDirective("globl", "_"+id).NextLine()
                .AddDirective("area", "_CODE")
                .AddLabel("_"+id, true)
                .AddComment("font header")
                .AddDirective("db", (byte)fontFlags, "font flags")
                .AddDirective("db", width, "width (max width for proportional)")
                .AddDirective("db", height, "height")
                .AddDirective("db", firstAscii, "first ascii")
                .AddDirective("db", lastAscii, "last ascii");

            // And separate from glyphs.
            NextLine();
        }

        public void AddTinyGlyph(int ascii, byte[] moves)
        {
            // Move direction.
            Dictionary<int, string> dd = new Dictionary<int, string>()
            {
                { 0,"rigt" },
                { 1,"rigt up" },
                { 2,"up" },
                { 3,"left up" },
                { 4,"down" },
                { 5,"right down" },
                { 6,"left" },
                { 7,"left down" }
            };

            // Add glyph comment.
            string glyphName = GetGlyphNames(ascii).Item1;
            string glyphDesc = GetGlyphNames(ascii).Item2;
            AddComment(glyphDesc);

            if (moves != null)
            {
                AddDirective("db", moves[0], "# moves");
                AddDirective("db", moves[1], "origin x");
                AddDirective("db", moves[2], "origin y");
                // Finally, add them moves
                for (int i = 3; i < moves.Length; i++)
                {
                    int m = moves[i];
                    string comment;
                    if ((m & 0x80) != 0)
                    {
                        int dx = m >> 5 & 0x03;
                        int dy = m >> 3 & 0x03;
                        int direction = m & 0x07;
                        comment = string.Format("move dx={0}, dy={1}, direction={2}", dx,dy, dd[direction]);
                    } 
                    else
                    {
                        bool pen = (m & 0x01) != 0;
                        bool color = (m & 0x02) != 0;
                        if (!pen)
                            comment = "pen up";
                        else 
                            comment = string.Format("pen {0}, point {1}",
                                pen ? "down" : "up"
                                , color ? "set" : "reset");
                    }

                    AddDirective("db", moves[i], comment);
                }
            }
            else
                AddDirective("db", 0, "# moves");
        }

        Tuple<string,string> GetGlyphNames(int ascii)
        {
            string glyphName;
            if (ascii < 32) glyphName = "<control>";
            else if (ascii > 126) glyphName = "<non standard>";
            else glyphName = ((char)ascii).ToString();
            string glyphDesc = string.Format("ascii {0}: '{1}'", ascii, glyphName);
            return new Tuple<string, string>(glyphName, glyphDesc);
        }

        public void GlyphOffsets(ushort[] offsets, int perRow = 8)
        {
            AddComment("glpyh offsets");
            AddWordTable(offsets, perRow);
            NextLine();
        }

        public void AddRasterFontGlyph(
            int ascii,
            BitArray bits,
            byte widthInBytes,
            byte width,
            byte height,
            List<byte> bin,
            byte originx = 0,
            byte originy = 0)
        {

            string glyphName = GetGlyphNames(ascii).Item1;
            string glyphDesc = GetGlyphNames(ascii).Item2;

            // First add glyph comment.
            AddComment(glyphDesc);

            // Now add glyph header ...
            AddDirective("db", (byte)(widthInBytes - 1), "class(bits 5-7) + stride(bits 0-4)");
            AddDirective("db", (width - 1).ToString(), "width");
            AddDirective("db", (height - 1).ToString(), "height");
            AddDirective("db", 0, "reserved");
            // And binary...
            bin.Add((byte)(widthInBytes - 1));
            bin.Add((byte)(width - 1));
            bin.Add((byte)(height - 1));
            bin.Add((byte)(0));
            bin.AddRange(BitArrayToByteArray(bits));

            // ...and glyph bits.
            for (int line = 0; line < height; line++)
            {
                _sb.Append(Tabs(_codeIdentTabs)); // Ident.
                bool bFirstInRow = true;
                for (int byt = 0; byt < widthInBytes; byt++)
                {
                    if (!bFirstInRow) _sb.Append(", "); else { _sb.Append(".db "); bFirstInRow = false; }
                    _sb.Append("0b");
                    for (int bit = 0; bit < 8; bit++)
                    {
                        int index = line * 8 * widthInBytes + 8 * byt + (7-bit);
                        _sb.Append(bits[index] ? 1 : 0);
                    }
                }
                _sb.AppendLine();
            }
            _sb.AppendLine(); // Conclude glyph.
        }
        #endregion // Method(s)

        #region Private(s)
        private byte[] BitArrayToByteArray(BitArray bits)
        {
            byte[] ret = new byte[(bits.Length - 1) / 8 + 1];
            bits.CopyTo(ret, 0);
            return ret;
        }
        #endregion // Private(s)
    }
}
