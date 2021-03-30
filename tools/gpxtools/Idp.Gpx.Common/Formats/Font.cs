using System;
using System.Runtime.InteropServices;

namespace Idp.Gpx.Common.Formats
{
    [Flags]
    public enum FontType : byte { Tiny = Generation.Tiny, Raster = Generation.Raster, Fixed = 0x10, Typo = 0x20  };

    [StructLayout(LayoutKind.Explicit)]
    public struct Font
    {
        // Standard glyph header.
        [FieldOffset(0)]
        public Glyph Glyph;

        // Font specific.
        [FieldOffset(4)]
        public byte FirstAscii;
        [FieldOffset(5)]
        public byte LastAscii;
    };

    [StructLayout(LayoutKind.Explicit)]
    public struct TypoFont
    {
        // Standard font header.
        [FieldOffset(0)]
        public Font Font;

        // Typo font specific.
        [FieldOffset(6)]
        public byte Cap;
        [FieldOffset(7)]
        public byte Ascent;
        [FieldOffset(8)] 
        public byte Descent;
        [FieldOffset(9)]
        public byte Median;
        [FieldOffset(10)]
        public byte Baseline;
    };
}
