using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.Linq;
using System.Text;
using Idp.Gpx.Common.CmdLine; 

namespace Idp.Gpx.Snatch.Commands {
    public class ArrayCmd : Cmd {

        const int SUCCESS = 0;
        const int INVALID_FORMAT = 5;

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

            // Format must be valid.
            string[] validFormats = { "grid","c","asm", "glyphs", "images" };
            if (validFormats.FirstOrDefault(f=>f.Equals(Format,StringComparison.InvariantCultureIgnoreCase))==null)
            {
                err.AppendFormat("Invalid format {0}.{1}", Format, Environment.NewLine);
                return INVALID_FORMAT;
            }

            // Get the bitmap.
            Bitmap loadBmp = Bitmap.FromFile(Filename) as Bitmap;

            // Just in case it is indexed pixel format...
            Bitmap bmp = new Bitmap(loadBmp.Width, loadBmp.Height);
            using (Graphics g = Graphics.FromImage(bmp)) g.DrawImage(loadBmp, 0, 0, loadBmp.Width, loadBmp.Height);

            int ascii = First;
            int x = Left, y = Top; // Respect margins.
            int maxx = bmp.Width - Left - Right, maxy = bmp.Height - Top - Bottom;

            using (Pen p = new Pen(Color.FromArgb(224, 224, 224, 224)))
            using (Graphics g = Graphics.FromImage(bmp))
            {
                int advancex = (GlyphWidth + HorizontalPadding), advancey = (GlyphHeight + VerticalPadding);
                for (int yc = y; yc + GlyphHeight - 1 <= maxy; yc += advancey)
                    for (int xc = x; xc + GlyphWidth - 1 <= maxx; xc += advancex)
                    {
                        // Are we too far?
                        if (ascii > Last) goto done;

                        // Draw grid?
                        if ("grid".Equals(Format,StringComparison.InvariantCultureIgnoreCase))
                            g.DrawRectangle(p, xc, yc, GlyphWidth - 1, GlyphHeight - 1);
                        else if ("glyphs".Equals(Format, StringComparison.InvariantCultureIgnoreCase)
                            || ("images".Equals(Format, StringComparison.InvariantCultureIgnoreCase)))
                        {
                            Bitmap gbmp = new Bitmap(GlyphWidth, GlyphHeight);
                            using (Graphics gg=Graphics.FromImage(gbmp))
                            {
                                gg.DrawImage(
                                    bmp,
                                    new Rectangle(0, 0, GlyphWidth, GlyphHeight), 
                                    new Rectangle(xc, yc, GlyphWidth, GlyphHeight),
                                    GraphicsUnit.Pixel);
                            }
                            string ofname = string.Format("{0}_{1}.png", Output, ascii);
                            gbmp.Save(ofname,ImageFormat.Png);
                        }

                        // Next ascii.
                        ascii++;
                    }
                done:;
            }

            // Only save changed image if grid format.
            if ("grid".Equals(Format, StringComparison.InvariantCultureIgnoreCase))
                bmp.Save(Output + ".png",ImageFormat.Png);

            return 0;
        } 
    }
}