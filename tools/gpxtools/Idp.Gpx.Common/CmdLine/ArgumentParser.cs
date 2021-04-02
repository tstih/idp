/*
 * ArgumentParser.cs
 * 
 * Command line parser. Takes a class with properties, annotated with 
 * [Argument] attributes, and tries to match them with command line arguments.
 *  
 * MIT License (see: LICENSE)
 * Copyright (c) 2021 Tomaz Stih
 * 
 * 07.09.2015   tstih
 * 
 */
using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;

namespace Idp.Gpx.Common.CmdLine {

    public class ArgumentParser {

        #region Private Classes
        private class ArgProperty {
            public ArgumentAttribute Argument {get;set;}
            public PropertyInfo PropertyInfo {get;set;}
            public bool Exists { get; set; }
        }
        #endregion // Private Classes

        #region Method(s)
        public string Parse(object obj, string[] args, int startAt=0) {

            // First get all arguments.
            Dictionary<string,ArgProperty> annotatedProps=GetArguments(obj);

            // Loop through the args.
            int i=startAt;
            while(i<args.Length) {

                // Extract arg.
                string arg=args[i++].Trim();
                if (arg.StartsWith("/")||arg.StartsWith("-")) {

                    // We have an option.
                    string option = arg.Substring(1);

                    // Find option in list of options.
                    if (annotatedProps.ContainsKey(option)) {

                        PropertyInfo prop=annotatedProps[option].PropertyInfo;
                        if (prop.PropertyType!=typeof(bool)) {

                            // Expect argument after option.
                            if (i>=args.Length) return string.Format("Value expected after {0}.", arg);
                            else {
                                string val=args[i++];
                                try { annotatedProps[option].PropertyInfo.SetValue(obj,Convert.ChangeType(val,prop.PropertyType),null);}
                                catch(Exception) { return string.Format("Invalid value for {0} for option {1}.", val, arg);}
                            }

                        } else annotatedProps[option].PropertyInfo.SetValue(obj,true);
                        
                        // If we are here, it worked!
                        annotatedProps[option].Exists=true;

                    } else return string.Format("Invalid option {0}.", arg);
                } else return string.Format("Invalid argument {0}. Option expected. Options start with / or -.", arg);
            }

            // Check if all required properties are present.
            StringBuilder missing = new StringBuilder();
            bool first = true;
            HashSet<ArgProperty> consumed = new HashSet<ArgProperty>();
            foreach (var p in annotatedProps)
                if (p.Value.Argument.Required && !p.Value.Exists)
                {
                    if (first)
                    {
                        first = false;
                        missing.AppendFormat("Missing mandatory argument {0}.", p.Key);
                        consumed.Add(p.Value); 
                    }
                    else
                    {
                        if (!consumed.Contains(p.Value))
                        {
                            missing.AppendFormat("{0}Missing mandatory argument {1}.", Environment.NewLine, p.Key);
                            consumed.Add(p.Value);                        
                        }
                    }
                }
            if (!first)
                return missing.ToString();
            
            // We're good.
            return null; // Success.
        }


        public string[] GenerateManual(object obj) {
            // First get all arguments.
            Dictionary<string,ArgProperty> args=GetArguments(obj);

            // Return description.
            List<string> argSpec=new List<string>();
            foreach(var arg in args.Select(a=>a.Value).Distinct())
                argSpec.Add(string.Format("-{0,-20} {1}",
                    arg.PropertyInfo.Name.ToLower(),
                    arg.Argument.Description??"<not specified>"));

            // Return list of arguments.
            return argSpec.ToArray();
        }

        #endregion // Method(s)

        #region Helper(s)
        private Dictionary<string,ArgProperty> GetArguments(object obj) {
            Dictionary<string,ArgProperty> dict=new Dictionary<string,ArgProperty>(StringComparer.InvariantCultureIgnoreCase);
            PropertyInfo[] props=obj.GetType().GetProperties(BindingFlags.Public|BindingFlags.Instance);
            foreach(PropertyInfo prop in props)
                foreach(ArgumentAttribute attr in prop.GetCustomAttributes(typeof(ArgumentAttribute))) {
                    // First create argument property.
                    ArgProperty ap=new ArgProperty() {Argument=attr,PropertyInfo=prop};
                    // Add property name.
                    dict.Add(prop.Name,ap);

                    // And all aliases too.
                    if (attr.Aliases!=null)
                        foreach(string alias in attr.Aliases.Split(new char[] {','}))
                            dict.Add(alias,ap);
                }
            return dict;
        }
        #endregion // Helper(s)
    }
}