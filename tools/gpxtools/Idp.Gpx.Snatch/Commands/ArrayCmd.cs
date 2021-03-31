using System;
using System.Collections;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Linq;
using System.Text;

using Idp.Gpx.Common.CmdLine;
using Idp.Gpx.Common.Formats;

namespace Idp.Gpx.Snatch.Commands {
    public class ArrayCmd : Cmd {

        const int SUCCESS = 0;
        const int INVALID_FORMAT = 5;

        private class Export
        {
            public virtual int Begin(ArrayCmd cmd, Bitmap bmp, StringBuilder std, StringBuilder err) { return 0; }
            public virtual int OnGlyph(ArrayCmd cmd, Bitmap bmp, Rectangle rect, int ascii, StringBuilder std, StringBuilder err) { return 0; }
            public virtual int End(ArrayCmd cmd, Bitmap bmp, StringBuilder std, StringBuilder err) { return 0; }
        }

        private class AsmExport : Export
        {
            private StringBuilder _internal;
            private Asm _asm;

            public AsmExport()
            {
                _internal = new StringBuilder();
            }

            public override int Begin(ArrayCmd cmd, Bitmap bmp, StringBuilder std, StringBuilder err)
            {
                _asm = new Asm(_internal); 
                byte bytesPerGlyph = (byte)((cmd.GlyphWidth - 1) / 8 + 1);
                _asm.AddFontHeader(cmd.Output, FontType.Tiny|FontType.Fixed,(byte)cmd.GlyphWidth,(byte)cmd.GlyphHeight,bytesPerGlyph, (byte)cmd.First, (byte)cmd.Last);
                return SUCCESS;
            }

            public override int OnGlyph(ArrayCmd cmd, Bitmap bmp, Rectangle rect, int ascii, StringBuilder std, StringBuilder err)
            {
                // Char to bitfield.
                byte bytesPerGlyph = (byte)((cmd.GlyphWidth - 1) / 8 + 1);   
                int bit = 0;
                BitArray bits = new BitArray(8 * bytesPerGlyph * cmd.GlyphHeight,false);
                for (int y = 0; y < rect.Height; y++)
                    for (int x = 0; x < rect.Width; x++)
                        bits[y * 8 * bytesPerGlyph + x] = !IsWhite((bmp.GetPixel(rect.Left+x, rect.Top+y)));

                // And generate as 0b const.
                _asm.AddGlyph(ascii, bits, bytesPerGlyph, (byte)rect.Height, 0, 0);

                return SUCCESS;
            }

            public override int End(ArrayCmd cmd, Bitmap bmp, StringBuilder std, StringBuilder err)
            {
                // Write result to text file.
                File.WriteAllText(cmd.Output + ".s", _internal.ToString());

                // And write result to std. output.
                std.Append(_internal.ToString());

                return SUCCESS;
            }

            private bool IsWhite(Color c, byte threshold=192)
            {
                byte avg = (byte)((c.R + c.G + c.B) / 3);
                return avg > threshold;
            }
        }

        private class CExport : Export { }

        private class GridExport : Export {
            private Pen _pen;
            private Graphics _graphics;
            public override int Begin(ArrayCmd cmd, Bitmap bmp, StringBuilder std, StringBuilder err)
            {
                _pen = new Pen(Color.FromArgb(224, 224, 224, 224));
                _graphics = Graphics.FromImage(bmp);
                return SUCCESS;
            }
            public override int OnGlyph(ArrayCmd cmd, Bitmap bmp, Rectangle rect, int ascii, StringBuilder std, StringBuilder err)
            {
                _graphics.DrawRectangle(_pen, rect.Left, rect.Top, rect.Width - 1, rect.Height- 1);
                return SUCCESS;
            }
            public override int End(ArrayCmd cmd, Bitmap bmp, StringBuilder std, StringBuilder err)
            {
                // Release pen and graphics.
                _pen.Dispose();
                _graphics.Dispose();
                // Save image.
                bmp.Save(cmd.Output + ".png", ImageFormat.Png);
                // And return success.
                return SUCCESS;
            }
        }

        private class ImagesExport : Export {
            public override int OnGlyph(ArrayCmd cmd, Bitmap bmp, Rectangle rect, int ascii, StringBuilder std, StringBuilder err)
            {
                Bitmap gbmp = new Bitmap(cmd.GlyphWidth, cmd.GlyphHeight);
                using (Graphics gg = Graphics.FromImage(gbmp))
                {
                    gg.DrawImage(
                        bmp,
                        new Rectangle(0, 0, rect.Width, rect.Height),
                        rect,
                        GraphicsUnit.Pixel);
                }
                string ofname = string.Format("{0}_{1}.png", cmd.Output, ascii);
                gbmp.Save(ofname, ImageFormat.Png);

                return SUCCESS;
            }
        }

        public override string Name { get {return "Array";} }
        public override string Desc { get {return "Create an array from image and export it.";} }

        [Argument(Aliases="f,file", Mandatory=true, Help="Source image.")]
        public string Filename {get;set;}

        [Argument(Aliases="o,save", Mandatory=false, Help="Output filename without extension (default=glyph, extension depends on format)")]
        public string Output {get;set;}

        [Argument(Aliases="l", Mandatory=false, Help="Left margin.")]
        public int Left {get;set;}

        [Argument(Aliases="t", Mandatory=false, Help="Top margin.")]
        public int Top {get;set;}

        [Argument(Aliases="r", Mandatory=false, Help="Right margin.")]
        public int Right {get;set;}

        [Argument(Aliases="b", Mandatory=false, Help="Bottom margin.")]
        public int Bottom {get;set;}

        [Argument(Aliases="gw", Mandatory=true, Help="Glyph width.")]
        public int GlyphWidth {get;set;}

        [Argument(Aliases="gh", Mandatory=true, Help="Glyph height.")]
        public int GlyphHeight {get;set;}

        [Argument(Aliases="fmt", Help = "Export format: grid, c, asm, glyphs.")]
        public string Format { get; set; }

        [Argument(Aliases="hp,ghp", Mandatory=false, Help="Glyph horizontal padding.")]
        public int HorizontalPadding {get;set;}

        [Argument(Aliases="vp,gvp", Mandatory=false, Help="Glyph vertical padding.")]
        public int VerticalPadding {get;set;}

        [Argument(Aliases = "start,s", Mandatory = false, Help = "First ascii.")]
        public int First { get; set; }

        [Argument(Aliases = "end,e", Mandatory = false, Help = "Last ascii.")]
        public int Last { get; set; }

        [Argument(Aliases = "p", Mandatory = false, Help = "Make font proportional.")]
        public bool Proportional { get; set; }

        [Argument(Aliases = "c", Mandatory = false, Help = "Point color in (r,g,b) format, no spaces!")]
        public string Color { get; set; }

        [Argument(Aliases = "th", Mandatory = false, Help = "Point color threshold (0-255) for soft recognition of points.")]
        public int Threshold { get; set; }

        [Argument(Aliases = "i", Mandatory = false, Help = "Use color and threshold to recognize background instead of point.")]
        public bool Inverse { get; set; }

        public ArrayCmd() {
            // Default arguments.
            Output="glyph";
            Left=Right=Top=Bottom=0;
            HorizontalPadding=VerticalPadding=0;
            First = 32;
            Last = 127;
            Format = "glyphs";
        }

        public override int Execute(StringBuilder std, StringBuilder err) {

            // Available exports.
            var ie = new ImagesExport();
            Dictionary<string, Export> exports = new Dictionary<string, Export>(StringComparer.InvariantCultureIgnoreCase)
            {
                { "grid", new GridExport() },
                { "c", new CExport() },
                { "asm", new AsmExport() },
                { "glyphs", ie },
                { "images", ie },
            };

            // Format must be valid.
            Export export;
            if (exports.Keys.FirstOrDefault(f => f.Equals(Format, StringComparison.InvariantCultureIgnoreCase)) == null)
            {
                err.AppendFormat("Invalid format {0}.{1}", Format, Environment.NewLine);
                return INVALID_FORMAT;
            } else export = exports[Format];

            // Get the bitmap.
            Bitmap loadBmp = Bitmap.FromFile(Filename) as Bitmap;

            // Just in case it is indexed pixel format...
            Bitmap bmp = new Bitmap(loadBmp.Width, loadBmp.Height);
            using (Graphics g = Graphics.FromImage(bmp)) g.DrawImage(loadBmp, 0, 0, loadBmp.Width, loadBmp.Height);

            // Initialize basic members.
            int ascii = First;
            int x = Left, y = Top; // Respect margins.
            int maxx = bmp.Width - Left - Right, maxy = bmp.Height - Top - Bottom;

            // Initialize export.
            int exitCode = export.Begin(this, bmp, std, err);
            if (exitCode != SUCCESS) return exitCode;

            
            int advancex = (GlyphWidth + HorizontalPadding), advancey = (GlyphHeight + VerticalPadding);
            for (int yc = y; yc + GlyphHeight - 1 <= maxy; yc += advancey)
                for (int xc = x; xc + GlyphWidth - 1 <= maxx; xc += advancex)
                {
                    // Are we too far?
                    if (ascii > Last) goto done;

                    // glyph found?
                    exitCode=export.OnGlyph(this, bmp, new Rectangle(xc, yc, GlyphWidth, GlyphHeight), ascii, std, err);
                    if (exitCode != SUCCESS) return exitCode;

                    // Next ascii.
                    ascii++;
                }

            done:
            return export.End(this, bmp, std, err);
        } 
    }
}