using System;
using System.Runtime.InteropServices;

namespace Idp.Gpx.Common.Formats
{
    [Flags]
    public enum Generation : byte { Tiny = 0x01, Raster = 0x02};

    [StructLayout(LayoutKind.Explicit)]
    public struct Glyph
    {
        [FieldOffset(0)]
        public Generation Generation;
        [FieldOffset(1)]
        public byte Width;
        [FieldOffset(2)]
        public byte Height;
        [FieldOffset(3)]
        public byte WidthInBytes;
        [FieldOffset(3)]
        public byte Reserved;
    }

    [StructLayout(LayoutKind.Explicit)]
    public struct GlyphData
    {
        [FieldOffset(0)]
        public byte OriginX;
        [FieldOffset(1)]
        public byte OriginY;
        [FieldOffset(2)]
        public byte Width;
    }
}