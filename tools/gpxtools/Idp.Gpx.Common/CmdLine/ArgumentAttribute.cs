/*
 * ArgumentAttribute.cs
 * 
 * An attribute to convert property to command line argument.
 *  
 * MIT License (see: LICENSE)
 * Copyright (c) 2021 Tomaz Stih
 * 
 * 07.09.2015   tstih
 * 
 */
using System;

namespace Idp.Gpx.Common.CmdLine {

    [AttributeUsage(AttributeTargets.Property)]
    public class ArgumentAttribute : Attribute
    {
        #region Ctor
        public ArgumentAttribute(
            string Aliases = null,
            bool Required = false,
            string Description = null)
        {

            this.Aliases = Aliases;
            this.Required = Required;
            this.Description = Description;
        }
        #endregion // Ctor

        #region Properties
        /// <summary>
        /// When converting property to command line argument, property name will be used
        /// as argument name i.e. if you convert property Filename to command line argument
        /// it will be detected if the user uses -filename or /filename. If you want to have
        /// multiple names for it, add them to aliases. For example adding "f,file" for 
        /// property Filename will allow -f, /f, -file, and /file syntax too.
        /// </summary>
        public string Aliases { get; set; }

        /// <summary>
        /// Is parameter required.
        /// </summary>
        public bool Required { get; set; }

        /// <summary>
        /// Used to generate autoatic help page.
        /// </summary>
        public string Description { get; set; }
        #endregion // Properties
    }
}