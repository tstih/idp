/*
 * Line.cs
 * 
 * XYZ line format (.lin file).
 *  
 * MIT License (see: LICENSE)
 * Copyright (c) 2021 Tomaz Stih
 * 
 * 02.03.2021   tstih
 * 
 */
using System.Runtime.InteropServices;

namespace XYZ.Formats
{
    /// <summary>
    /// lin file has line header, followed by an array of line structures.
    /// </summary>
    [StructLayout(LayoutKind.Explicit, CharSet = CharSet.Ansi)]
    public struct LineHeader
    {
        [FieldOffset(0)] public ushort w;
        [FieldOffset(2)] public ushort h;
        [FieldOffset(4)] public ushort lines;
    }

    [StructLayout(LayoutKind.Explicit, CharSet = CharSet.Ansi)]
    public struct Line
    {
        [FieldOffset(0)] public ushort x1;
        [FieldOffset(2)] public ushort y1;
        [FieldOffset(4)] public ushort x2;
        [FieldOffset(6)] public ushort y2;
    }
}
