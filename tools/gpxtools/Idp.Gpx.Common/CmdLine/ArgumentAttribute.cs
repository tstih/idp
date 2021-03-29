using System;

namespace Idp.Gpx.Common.CmdLine {

    [AttributeUsage(AttributeTargets.Property)]
    public class ArgumentAttribute : Attribute {
        public ArgumentAttribute(
            string Aliases=null, 
            bool Mandatory=false, 
            string Help=null) {
            
            this.Aliases=Aliases;
            this.Mandatory=Mandatory;
            this.Help=Help;

        }

        public string Aliases {get;set;}
        public bool Mandatory {get;set;}
        public string Help {get;set;}

    }

}