/*
 * Font.cs
 * 
 * XYZ font format (.gyp file and memory image).
 *  
 * MIT License (see: LICENSE)
 * Copyright (c) 2021 Tomaz Stih
 * 
 * 02.03.2021   tstih
 * 
 */
using System;
using System.Runtime.InteropServices;

namespace XYZ.Formats
{
    /// <summary>
    /// Extending glyph type to add font specific data to upper nibble of generation flag.
    /// </summary>
    public enum FontType : byte { Fixed = 0x40 };

    /// <summary>
    /// Font header is the same for each font.
    /// </summary>
    [StructLayout(LayoutKind.Explicit)]
    public struct FontHeader 
    {
        /// <summary>
        /// Starts with standard glyph header.
        /// </summary>
        [FieldOffset(0)]
        public GlyphHeader GlyphHeader;

        /// <summary>
        /// First ascii char. Usually 32.
        /// </summary>
        [FieldOffset(6)]
        public byte FirstAscii;

        /// <summary>
        /// Last ascii char. Usually 126.
        /// </summary>
        [FieldOffset(7)]
        public byte LastAscii;
    };
}