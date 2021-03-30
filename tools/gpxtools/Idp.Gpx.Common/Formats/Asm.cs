using System;
using System.Text;

namespace Idp.Gpx.Common.Formats
{
    public class Asm : Generator
    {
        private StringBuilder _sb;
        public Asm(StringBuilder sb) { _sb = sb; Tab = 8; }
        public void AddFontHeader(string name, FontType generation, byte width, byte height, byte widthInBytes)
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
        
        private void AddCommentedByte(byte b, string comment)
        {
            string db = string.Format("{0}.db {1}", Spaces(2*Tab), b);
            db = FixedWidth(db, 6 * Tab);
            _sb.AppendFormat("{0};; {1}{2}", db, comment, Environment.NewLine);
        }

        public int Tab { get; set; }
    }
}
