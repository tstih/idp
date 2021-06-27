/*
 * BinCmd.cs
 * 
 * Convert a file to bin C source code.
 *  
 * MIT License (see: LICENSE)
 * Copyright (c) 2021 Tomaz Stih
 * 
 * 13.06.2021   tstih
 * 
 */
using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;
using System.Drawing;
using System.Drawing.Imaging;

using XYZ.Ex;
using XYZ.Formats;
using Idp.Gpx.Common.CmdLine;

using Idp.Gpx.Common.Generators;

using Idp.Gpx.Common.Utils;

namespace Idp.Gpx.Partnerize.Commands
{

    public class BinCmd : Cmd
    {

        #region Ctor
        public BinCmd()
        {
        }
        #endregion // Ctor

        #region Command Line Arguments
        [Argument(Aliases = "f,file", Required = true, Description = "Source file.")]
        public string Filename { get; set; }

        [Argument(Aliases = "o,save", Required = false, Description = "Output filename without extension (default=bin)")]
        public string Output { get; set; }
        #endregion // Command Line Arguments

        #region Properties
        public StringBuilder Std { get; set; }
        public StringBuilder Err { get; set; }
        public Bitmap SourceBitmap { get; set; }
        #endregion // Properties

        #region Override(s)
        public override string Name { get { return "bin"; } }
        public override string Desc { get { return "Convert an file to C binary array."; } }
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
            // Load it.
            byte[] rawb = File.ReadAllBytes(fin);

            // Now generate C source code.
            StringBuilder sourceCode = new StringBuilder();
            string id = string.Format("{0}", Output);
            CCodeGenerator gen = new CCodeGenerator(sourceCode);
            string mainComment = string.Format("Format is {0}, size (in bytes) is {1}.", Path.GetExtension(fin), rawb.Length);
            string arrayHead = string.Format("uint8_t {0}[] = {{", id), arrayTail = "};";
            gen
                .AddHeader(id, string.Format("{0} raw binary.", Path.GetExtension(fin)), ".cpp", "tstih", "Tomaz Stih", cpp:true)
                .LineOfCode("#include <cstdint>")
                .NextLine()
                .CommentOnly(mainComment, 0, true)
                .LineOfCode(arrayHead)
                .ArrayAsBytes(rawb, 16, null, true)
                .LineOfCode(arrayTail);


            // And save as text.
            File.WriteAllText(Output + ".cpp", sourceCode.ToString());
        }
        #endregion // Helper(s)
    }
}
