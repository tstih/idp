/*
 * Animation.cs
 * 
 * XYZ animation format (.ani file).
 *  
 * MIT License (see: LICENSE)
 * Copyright (c) 2021 Tomaz Stih
 * 
 * 11.04.2021   tstih
 * 
 */
using System.Runtime.InteropServices;

namespace XYZ.Formats
{
    [StructLayout(LayoutKind.Explicit, CharSet = CharSet.Ansi)]
    public struct AnimationHeader
    {
        [FieldOffset(0)]
        public GlyphHeader GlyphHeader;

        /// <summary>
        /// Number of animation frames.
        /// </summary>
        [FieldOffset(6)]
        public byte Frames;
    }

    [StructLayout(LayoutKind.Explicit, CharSet = CharSet.Ansi)]
    public struct AnimationDataHeader
    {
        [FieldOffset(0)]
        public byte Index;

        [FieldOffset(1)]
        public byte Delay;

        [FieldOffset(2)]
        public ushort Lines;
    }
}
