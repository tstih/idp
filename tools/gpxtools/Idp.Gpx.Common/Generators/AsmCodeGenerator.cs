/*
 * AsmCodeGenerator.cs
 * 
 * Functions to generate assembly code.
 *  
 * MIT License (see: LICENSE)
 * Copyright (c) 2021 Tomaz Stih
 * 
 * 02.03.2021   tstih
 * 
 */
using System;
using System.Collections;
using System.Text;

namespace Idp.Gpx.Common.Generators
{
    public class AsmCodeGenerator : CodeGenerator
    {
        #region Protected(s)
        protected StringBuilder _sb;
        protected int _codeIdentTabs;
        protected int _commentIdentTabs;
        #endregion // Protected(s)

        #region Ctor
        public AsmCodeGenerator(StringBuilder sb, int codeIdentTabs = 2, int commentIdentTabs=10) { 
            _sb = sb;
            _codeIdentTabs = codeIdentTabs;
            _commentIdentTabs = commentIdentTabs;
            TabSize = 4;
        }
        #endregion // Ctor

        #region Method(s)
        public AsmCodeGenerator AddHeader(string name, string desc=null, string ext=".s", string notes=null)
        {
            // Standard header prefix.
            string hdrPrefix = PadTabs(string.Format("{0};;", Tabs(_codeIdentTabs)), 3);

            // First the file name.
            _sb.AppendFormat("{0}{1}{2}{3}{0}{3}", hdrPrefix, name, ext, Environment.NewLine);

            // File description.
            _sb.AppendFormat("{0}{1}{2}", hdrPrefix, string.IsNullOrEmpty(desc)? name : desc, Environment.NewLine);

            // Do we need to add notes?
            if (!string.IsNullOrEmpty(notes))
            {
                _sb.AppendFormat("{0};; {1}", Tabs(_codeIdentTabs), Environment.NewLine); // Empty ;;
                _sb.AppendFormat("{0}notes: {1}{2}{0}{2}", hdrPrefix, notes, Environment.NewLine);
            }

            _sb.AppendFormat("{0}MIT License (see: LICENSE){1}{0}copyright(c) 2021 tomaz stih{1}{0}{1}", hdrPrefix, Environment.NewLine);
            _sb.AppendFormat("{0}{1}   tstih{2}",hdrPrefix,DateTime.Today.ToString("dd.MM.yyyy"),Environment.NewLine);

            // Fluent insert.
            return this;
        }

        public AsmCodeGenerator AddDirective(string directive, string val = null, string comment = null)
        {
            string dirVal = string.Format("{0}.{1}{2}", Tabs(_codeIdentTabs), directive, string.IsNullOrEmpty(val)?string.Empty:" "+val); // .directive
            if (!string.IsNullOrEmpty(comment))
                _sb.AppendFormat("{0}; {1}{2}", PadTabs(dirVal,_commentIdentTabs), comment, Environment.NewLine);
            else
                _sb.AppendFormat("{0}{1}", dirVal, Environment.NewLine);

            // Fluent insert.
            return this;
        }

        public AsmCodeGenerator AddDirective(string directive, byte val, string comment = null)
        { AddDirective(directive, val.ToString(), comment); return this; }

        public AsmCodeGenerator AddInstruction(string inst, string comment = null)
        {
            string instVal = string.Format("{0}{1}", Tabs(_codeIdentTabs), inst); // instruction.
            if (!string.IsNullOrEmpty(comment))
                _sb.AppendFormat("{0}; {1}{2}", PadTabs(instVal, _commentIdentTabs), comment, Environment.NewLine);
            else
                _sb.AppendFormat("{0}{1}", instVal, Environment.NewLine);

            // Fluent insert.
            return this;
        }

        public AsmCodeGenerator AddLabel(string label, bool global=false)
        {
            if (!global)
                _sb.AppendFormat("{0}:{1}", label, Environment.NewLine);
            else
                _sb.AppendFormat("{0}::{1}", label, Environment.NewLine);

            // Fluent insert.
            return this;
        }

        public AsmCodeGenerator AddComment(string comment)
        {
            _sb.AppendFormat("{0};; {1}{2}", Tabs(_codeIdentTabs), comment, Environment.NewLine);

            // Fluent insert.
            return this;
        }

        public AsmCodeGenerator AddWordTable(ushort[] tbl, int columns)
        {
            int index = 0;
            StringBuilder sb = new StringBuilder();
            while (index < tbl.Length)
            {
                if (index % columns == 0)
                { // Row start.

                    if (index!=0) // We have full string builder.
                        AddDirective("dw", sb.ToString());

                    // And reset string builder.
                    sb = new StringBuilder();
                    sb.AppendFormat("0x{0:X4}", tbl[index]);

                }
                else
                    sb.AppendFormat(", 0x{0:X4}", tbl[index]);
                index++;
            }
            return this;
        }

        public AsmCodeGenerator AddByteTable(byte[] tbl, int columns)
        {
            int index = 0;
            StringBuilder sb = new StringBuilder();
            while (index < tbl.Length)
            {
                if (index % columns == 0)
                { // Row start.

                    if (index != 0) // We have full string builder.
                        AddDirective("db", sb.ToString());

                    // And reset string builder.
                    sb = new StringBuilder();
                    sb.AppendFormat("0x{0:X2}", tbl[index]);

                }
                else
                    sb.AppendFormat(", 0x{0:X2}", tbl[index]);
                index++;
            }
            return this;
        }


        public AsmCodeGenerator NextLine()
        {
            _sb.AppendFormat("{0}", Environment.NewLine);

            // Fluent insert.
            return this;
        }
        #endregion // Method(s)
    }
}
