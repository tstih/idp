using System.Text;
using Idp.Gpx.Common.CmdLine; 

namespace Idp.Gpx.Snatch.Commands {
    public class Cmd {

        public virtual string Name {get { return string.Empty;}}
        public virtual string Desc {get { return string.Empty;}}

        public virtual int Execute(StringBuilder std, StringBuilder err) {
            err.AppendLine("Command not implemented.");   
            return 0;
        }
    }
}