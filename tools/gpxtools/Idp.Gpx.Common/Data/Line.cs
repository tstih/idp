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

namespace Idp.Gpx.Common.Data
{
    [StructLayout(LayoutKind.Explicit, CharSet = CharSet.Ansi)]
    public struct Line
    {
        [FieldOffset(0)] public ushort x1;
        [FieldOffset(2)] public ushort y1;
        [FieldOffset(4)] public ushort x2;
        [FieldOffset(6)] public ushort y2;
    }
}