/*
 * CCodeGenerator.cs
 * 
 * Functions to generate C source code.
 *  
 * MIT License (see: LICENSE)
 * Copyright (c) 2021 Tomaz Stih
 * 
 * 02.03.2021   tstih
 * 
 */
using System;
using System.Text;

using System.Collections.Generic;
using System.Runtime.InteropServices;
using Idp.Gpx.Common.Data;
using Idp.Gpx.Common.Ex;

namespace Idp.Gpx.Common.Generators
{
    public class CCodeGenerator : CodeGenerator
    {
        #region Protected(s)
        protected StringBuilder _sb;
        #endregion // Protected(s)

        #region Ctor
        public CCodeGenerator(StringBuilder sb)
        {
            _sb = sb;
            TabSize = 4;
        }
        #endregion // Ctor

        #region Method(s)
        public CCodeGenerator AddHeader(string name, string desc = null, string ext = ".c", string shortAuthor = null, string fullAuthor = null, string notes = null, bool cpp=false)
        {
            string prefix = cpp ? "//" : " *";
            NextLine(cpp?"//":"/*");
            _sb.AppendFormat(prefix+" {0}{1}{2}", name, ext, Environment.NewLine);
            NextLine(cpp?"//":" *");
            _sb.AppendFormat(prefix+" {0}{1}", string.IsNullOrEmpty(desc) ? name : desc, Environment.NewLine);
            NextLine(cpp?"//":" *");
            if (!string.IsNullOrEmpty(fullAuthor))
            {
                NextLine(prefix+" MIT License (see: LICENSE)");
                _sb.AppendFormat(prefix+" Copyright (c) 2021 {0}{1}", fullAuthor, Environment.NewLine);
            }
            NextLine(cpp?"//":" *");
            if (!string.IsNullOrEmpty(shortAuthor))
            {
                string authLine = string.Format(prefix+" {0}", DateTime.Now.ToString("dd.MM.yyyy"));
                _sb.AppendFormat("{0}{1}{2}", PadTabs(authLine, 4), shortAuthor, Environment.NewLine);
                NextLine(cpp?"//":" *");
            }
            NextLine(cpp?"//":" */");

            // Be fluent.
            return this;
        }

        public CCodeGenerator NextLine(string text = null)
        {
            _sb.AppendFormat("{0}{1}", string.IsNullOrEmpty(text) ? string.Empty : text, Environment.NewLine);

            // Fluent insert.
            return this;
        }

        public CCodeGenerator GuardStart(string name)
        {
            _sb.AppendFormat("#ifndef {0}{1}#define {0}{1}{1}", name, Environment.NewLine);
            return this;
        }

        public CCodeGenerator GuardEnd(string name)
        {
            _sb.AppendFormat("#endif /* {0} */", name); // No newline!
            return this;
        }

        public CCodeGenerator Include(string fname, bool last=false)
        {
            _sb.AppendFormat("#include \"{0}\"{1}", fname, Environment.NewLine);
            if (last)
                NextLine();
            return this;
        }

        public CCodeGenerator LineOfCode(string text)
        {
            _sb.AppendFormat("{0}{1}", text, Environment.NewLine);
            return this;
        }

        public CCodeGenerator CommentOnly(string text, int tabs=1, bool cpp=false)
        {
            if (!cpp)
                _sb.AppendFormat("{0}/* {1} */{2}", Tabs(tabs), text, Environment.NewLine);
            else
                _sb.AppendFormat("{0}// {1}{2}", Tabs(tabs), text, Environment.NewLine);
            return this;
        }

        public CCodeGenerator CommentedLineOfCode(string loc, string comment, int tabs = 1, int commentTabs = 14)
        {
            string code=string.Format("{0}{1}", Tabs(tabs), loc);
            code=PadTabs(code, commentTabs);
            code = string.Format("{0}/* {1} */{2}", code, comment, Environment.NewLine);

            _sb.Append(code);

            return this;
        }

        public CCodeGenerator CommentedShort(ushort us, string comment, bool firstLine = false, int tabs = 1, int commentTabs = 14)
        {
            var t = ShortToBytes(us);
            string code = string.Format("{0}{1}0x{2:x2}, 0x{3:x2}", Tabs(tabs), !firstLine ? ", " : "  ", t.Item1, t.Item2);
            code = PadTabs(code, commentTabs);
            code = string.Format("{0}/* {1} */{2}", code, comment, Environment.NewLine);

            _sb.Append(code);

            return this;
        }

        public CCodeGenerator CommentedByte(byte b, string comment, bool firstLine=false, int tabs = 1, int commentTabs = 14)
        {
            string code = string.Format("{0}{1}0x{2:x2}", Tabs(tabs), !firstLine?", ":"  ", b);
            code = PadTabs(code, commentTabs);
            code = string.Format("{0}/* {1} */{2}", code, comment, Environment.NewLine);

            _sb.Append(code);

            return this;
        }

        public CCodeGenerator LinesAsBytes(Line[] lines, bool firstLine=false, int tabs=1)
        {
            List<byte> bytes = new List<byte>();
            List<string> comments = new List<string>();
            for(int i=0;i<lines.Length;i++)
            {
                // Generate line comment.
                comments.Add(string.Format("line ({0},{1}) - ({2},{3})",
                    lines[i].x1, lines[i].y1,
                    lines[i].x1, lines[i].y2
                ));

                // Generate line bytes.
                bytes.AddRange(lines[i].ToByteArray<Line>());
            }

            return ArrayAsBytes(
                bytes.ToArray(), 
                Marshal.SizeOf(lines[0]), 
                comments.ToArray(), 
                firstLine);
        }

        public CCodeGenerator ArrayAsBytes(
            byte[] array, 
            int bytesPerRow, 
            string[] comments = null, 
            bool firstRow = false, 
            int tabs = 1, 
            int commentTabs=14)
        {
            int rows = (array.Length - 1) / bytesPerRow + 1;

            for (int row = 0; row < rows; row++)
            {
                StringBuilder rsb = new StringBuilder(); // row string builder.
                int cols = Math.Min(bytesPerRow, array.Length - row * bytesPerRow);
                for (int col = 0; col < cols; col++)
                {
                    int arridx = row * bytesPerRow + col; // We're here. In bytes.
                    // Do we need to prefix with a comma?
                    if (!firstRow || col > 0)
                        rsb.Append(", ");
                    else
                        rsb.Append("  ");

                    // Add byte.
                    rsb.AppendFormat("0x{0:x2}", array[arridx]);

                    // No longer.
                    firstRow = false;
                }

                /* prefix generated line with tabs */
                string line = string.Format("{0}{1}", Tabs(tabs), rsb.ToString());

                // Do we need to attach a comment to this row?
                if (comments!=null)
                    if (row < comments.Length || !string.IsNullOrEmpty(comments[row]))
                        line = string.Format("{0}/* {1} */", PadTabs(line, commentTabs),comments[row]);

                // Now appaend as line.
                NextLine(line);
            }

            return this;
        }
        #endregion // Method(s)

        #region Helper(s)
        private Tuple<byte, byte> ShortToBytes(ushort us)
        {
            Tuple<byte, byte> t = new Tuple<byte, byte>(
                 (byte)(us & 0x00ff),
                 (byte)((us >> 8) & 0x00ff)
            );
            return t;
        }
        #endregion // Helper(s)
    }
}
