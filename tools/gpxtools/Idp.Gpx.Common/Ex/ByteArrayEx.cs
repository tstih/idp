using System;
using System.Runtime.InteropServices;

namespace Idp.Gpx.Common.Ex {
    public static class ByteArrayEx {
        public static S ToStruct<S>(this byte[] bytes)
        {
            Type t = typeof(S);

            byte[] buffer = new byte[Marshal.SizeOf(t)];

            S result;
            GCHandle handle = GCHandle.Alloc(bytes, GCHandleType.Pinned);
            result = (S)Marshal.PtrToStructure(handle.AddrOfPinnedObject(),t);
            handle.Free();
            return result;
        }

        static byte[] ToByteArray<S>(this S s) where S: struct
        {
            // This function copies the structure data into a byte[] 

            //Set the buffer to the correct size 
            byte[] buffer = new byte[Marshal.SizeOf(s)];

            //Allocate the buffer to memory and pin it so that GC cannot use the 
            //space (Disable GC) 
            GCHandle h = GCHandle.Alloc(buffer, GCHandleType.Pinned);

            // copy the struct into int byte[] mem alloc 
            Marshal.StructureToPtr(s, h.AddrOfPinnedObject(), false);

            h.Free(); //Allow GC to do its job 

            return buffer; // return the byte[]. After all that's why we are here 
                            // right. 
        }
    }
}
