/*
 * ByteArrayEx.cs
 * 
 * Extension functions for byte array.
 * 
 * COPY'N'PASTES:
 *  Structure to and from byte array:
 *  https://www.codeproject.com/Articles/11271/Read-and-Write-Structures-to-Files-with-NET
 *  
 * MIT License (see: LICENSE)
 * Copyright (c) 2021 Tomaz Stih
 * 
 * 02.03.2021   tstih
 * 
 */
using System.Runtime.InteropServices;

namespace Idp.Gpx.Common.Ex
{

    public static class ByteArrayEx
    {

        /// <summary>
        /// Convert byte array to struct. The struct should have the
        /// [StructLayout(LayoutKind.Explicit)] attribute.
        /// </summary>
        public static S ToStruct<S>(this byte[] bytes)
        {
            S result;
            GCHandle handle = GCHandle.Alloc(bytes, GCHandleType.Pinned);
            result = (S)Marshal.PtrToStructure(handle.AddrOfPinnedObject(), typeof(S));
            handle.Free();
            return result;
        }

        /// <summary>
        /// Convert struct to byte array. The struct should have the
        /// [StructLayout(LayoutKind.Explicit)] attribute.
        /// </summary>
        public static byte[] ToByteArray<S>(this S s) where S : struct
        {
            //Set the buffer to the correct size 
            byte[] buffer = new byte[Marshal.SizeOf(s)];

            // Allocate the buffer to memory and pin it so that GC cannot use the 
            // space (Disable GC) 
            GCHandle h = GCHandle.Alloc(buffer, GCHandleType.Pinned);

            // Copy the struct into int byte[] mem alloc 
            Marshal.StructureToPtr(s, h.AddrOfPinnedObject(), false);

            // Free the memory handle.
            h.Free();

            return buffer; // Return the byte[].
        }
    }
}