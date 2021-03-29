using System.Text;
using Idp.Gpx.Common.CmdLine; 

namespace Idp.Gpx.Snatch.Commands {
    public class CropCmd : Cmd {

        public override string Name { get {return "Crop";} }
        public override string Desc { get {return "Crop an image.";} }

        [Argument(Aliases="f,file", Mandatory=true, Help="Image to crop.")]
        public string Filename {get;set;}

        [Argument(Aliases="o,save", Mandatory=false, Help="Output filename (default=cropped.png)")]
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

        [Argument(Aliases="hp,ghp", Mandatory=false, Help="Glyph horizontal padding.")]
        public int HorizontalPadding {get;set;}

        [Argument(Aliases="vp,gvp", Mandatory=false, Help="Glyph vertical padding.")]
        public int VerticalPadding {get;set;}

        public CropCmd() {
            // Default arguments.
            Output="cropped.png";
            Left=Right=Top=Bottom=0;
            HorizontalPadding=VerticalPadding=0;
        }

        public override int Execute(StringBuilder std, StringBuilder err) {
        } 
    }
}