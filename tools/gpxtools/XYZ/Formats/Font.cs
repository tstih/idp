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
    [Flags]
    public enum FontType : byte { Tiny = Generation.Tiny, Raster = Generation.Raster, Fixed = 0x10, Typo = 0x20  };

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


    /// <summary>
    /// Typography font header.
    /// </summary>
    [StructLayout(LayoutKind.Explicit)]
    public struct TypoFontHeader
    {
        /// <summary>
        /// Start with standard font header.
        /// </summary>
        [FieldOffset(0)]
        public FontHeader FontHeader;

        // Typo font specific, for explanation see: https://en.wikipedia.org/wiki/Typeface
        [FieldOffset(8)]
        public byte Cap;
        [FieldOffset(9)]
        public byte Ascent;
        [FieldOffset(10)] 
        public byte Descent;
        [FieldOffset(11)]
        public byte Median;
        [FieldOffset(12)]
        public byte Baseline;
    };
}