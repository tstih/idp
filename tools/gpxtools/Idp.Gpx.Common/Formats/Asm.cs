using System;
using System.Collections;
using System.Text;

namespace Idp.Gpx.Common.Formats
{
    public class Asm : Generator
    {
        private StringBuilder _sb;
        public Asm(StringBuilder sb) { _sb = sb; Tab = 8; }
        public void AddFontHeader(string name, FontType generation, byte width, byte height, byte widthInBytes, byte firstAscii, byte lastAscii)
        {
            // File name.
            _sb.AppendFormat("{0};;{1}{2}.s{3}", Spaces(2*Tab), Spaces(6), name, Environment.NewLine);
            // File description.
            _sb.AppendFormat("{0};;{1}{2} font{3}", Spaces(2 * Tab), Spaces(6), name, Environment.NewLine);
            // Empty line and notes.
            _sb.AppendFormat("{0};;{1}", Spaces(2 * Tab), Environment.NewLine);
            _sb.AppendFormat("{0};;{1}notes: see font.h for format details{2}", Spaces(2 * Tab), Spaces(6), Environment.NewLine);

            // Module, globals, label...
            _sb.AppendFormat("{0}.module {1}_font{2}{2}{0}.globl {1}_font{2}{2}{0}.area _CODE{2}{1}_font::{2}{0};; font header{2}", Spaces(2 * Tab), name, Environment.NewLine);

            // All the numbers.
            AddCommentedByte((byte)generation, "font generation");
            AddCommentedByte(width, "font (max) width");
            AddCommentedByte(height, "font height");
            AddCommentedByte(widthInBytes, "width in bytes");

            // First and last ascii.
            AddCommentedByte(firstAscii, "first ascii");
            AddCommentedByte(lastAscii, "last ascii");

            // Font defs.
            if (generation.HasFlag(FontType.Typo))
            { // Typographic font?
                // TODO: Not done yet.
                AddCommentedByte(0, "cap");
                AddCommentedByte(0, "ascent");
                AddCommentedByte(0, "descent");
                AddCommentedByte(0, "median");
                AddCommentedByte(0, "baseline");
            }

            // End of header.
            _sb.AppendLine();
        }
        
        public void AddGlyph(int ascii, BitArray bits, byte widthInBytes, byte height, byte originx=0, byte originy=0)
        {
            string glyph;
            if (ascii < 32) glyph = "<control>";
            else if (ascii > 126) glyph = "<non standard>";
            else glyph = ((char)ascii).ToString();
            _sb.AppendFormat("{0};; ascii {1}: '{2}'{3}", Spaces(2 * Tab), ascii, glyph, Environment.NewLine);

            for (int line = 0; line < height; line++)
            {
                _sb.Append(Spaces(2 * Tab)); // Ident.
                bool bFirstInRow = true;
                for (int byt = 0; byt < widthInBytes; byt++)
                {
                    if (!bFirstInRow) _sb.Append(", "); else { _sb.Append(".db ");  bFirstInRow = false; }
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

        private void AddCommentedByte(byte b, string comment)
        {
            string db = string.Format("{0}.db {1}", Spaces(2*Tab), b);
            db = FixedWidth(db, 5 * Tab);
            _sb.AppendFormat("{0};; {1}{2}", db, comment, Environment.NewLine);
        }

        public int Tab { get; set; }
    }
}
