/*
 * Glyph.cs
 * 
 * XYZ glyph format (.gyp file and memory image).
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
    /// Glyph generation. Can be Tiny (small "vector" format for the
    /// Delta Partner machine) or Raster (standard raster format).
    /// </summary>
    [Flags]
    public enum Generation : byte { Tiny = 0x01, Raster = 0x02};

    /// <summary>
    /// Glyph header. All glyphs (fons, and sprites) share the same
    /// header. By examining the glyph generation you can find out
    /// the glyph type.
    /// 
    /// Glyph header is followed by data structures, that depend on
    /// generation and can include direct raster data, glyph index table, 
    /// single glyph (for sprite), multiple glyphs (for fonts), etc.
    /// </summary>
    [StructLayout(LayoutKind.Explicit)]
    public struct GlyphHeader
    {
        [FieldOffset(0)]
        public Generation Generation;
        
        /// <summary>
        /// Width in pixels for fixed glyphs. Max width for proportional glyphs.
        /// </summary>
        [FieldOffset(1)]
        public byte Width; 
        
        /// <summary>
        /// Height in pixels.
        /// </summary>
        [FieldOffset(2)]
        public byte Height;

        /// <summary>
        /// Width in bytes for raster glyphs. This field is not used for vector glyphs.
        /// </summary>
        [FieldOffset(3)]
        public byte WidthInBytes;
    }

    /// <summary>
    /// Each individual glyph starts with this header.
    /// </summary>
    [StructLayout(LayoutKind.Explicit)]
    public struct GlyphDataHeader
    {
        /// <summary>
        /// First glyph point. This only makes sense for vector fonts 
        /// (first point of first stroke). For raster fonts it is not there.
        /// </summary>
        [FieldOffset(0)]
        public byte OriginX;

        [FieldOffset(1)]
        public byte OriginY;
    }
}