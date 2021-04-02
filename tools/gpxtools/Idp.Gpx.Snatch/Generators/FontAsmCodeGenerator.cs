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
using System.Collections;
using System.Text;

using XYZ.Formats;
using Idp.Gpx.Common.Generators;

namespace Idp.Gpx.Snatch.Generators
{
    public class FontAsmCodeGenerator : AsmCodeGenerator
    {
        #region Ctor
        public FontAsmCodeGenerator(StringBuilder sb, int codeIdentTabs = 2, int commentIdentTabs = 5)
            : base(sb,2,5)
        { }
        #endregion // Ctor

        #region Method(s)
        public void AddFontHeader(
            string name,
            FontType generation,
            byte width,
            byte height,
            byte lineWidthInBytes,
            byte firstAscii,
            byte lastAscii)
        {
            string id = string.Format("{0}_font", name);

            // Generate standard header.
            AddHeader(name, id, ".s", "see font.h for format details")
                .AddDirective("module", id).NextLine()
                .AddDirective("globl", id).NextLine()
                .AddDirective("area", "_CODE")
                .AddLabel(id, true)
                .AddComment("font header")
                .AddDirective("db", (byte)generation, "font generation")
                .AddDirective("db", width, "font width (max width for proportional)")
                .AddDirective("db", height, "font height")
                .AddDirective("db", lineWidthInBytes, "line width in bytes (unused for tiny)")
                .AddDirective("db", firstAscii, "first ascii")
                .AddDirective("db", lastAscii, "last ascii");

            // Typo font?
            if (generation.HasFlag(FontType.Typo))
            {
                // TODO: One fine day.
                AddComment("typo font (header extension)")
                    .AddDirective("db", 0, "cap")
                    .AddDirective("db", 0, "ascent")
                    .AddDirective("db", 0, "descent")
                    .AddDirective("db", 0, "median")
                    .AddDirective("db", 0, "baseline");
            }

            // And separate from glyphs.
            NextLine();
        }

        public void AddRasterFontGlyph(
            int ascii,
            BitArray bits,
            byte widthInBytes,
            byte height,
            byte originx = 0,
            byte originy = 0)
        {

            string glyphName;
            if (ascii < 32) glyphName = "<control>";
            else if (ascii > 126) glyphName = "<non standard>";
            else glyphName = ((char)ascii).ToString();
            string glyphDesc = string.Format("ascii {0}: '{1}'", ascii, glyphName);

            // First add glyph comment.
            AddComment(glyphDesc);

            // Now add glyph bytes.
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
                        int index = line * 8 * widthInBytes + 8 * byt + bit;
                        _sb.Append(bits[index] ? 1 : 0);
                    }
                }
                _sb.AppendLine();
            }
            _sb.AppendLine(); // Conclude glyph.
        }
        #endregion // Method(s)
    }
}
