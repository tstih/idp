using System;
using System.Collections.Generic;
using Z80.Components;

namespace Z80.Analysis
{
    public class Annotation
    {
        // Identifying.
        public string Symbol;

        // Profiling.
        public long ReadCount;
        public long WriteCount;
        public long ExecCount;

        // Incoming and outgoing calls.
        public HashSet<ulong> Incoming;
        public HashSet<ulong> Outgoing;

        // If opcode this is the start address.
        public ushort OpStart;
    };
    /*
    public class AnnotatedMemory : Memory
    {
        protected ushort _last_M1;
        protected Annotation[] _annotations;

        public AnnotatedMemory()
        {
            _annotations = new Annotation[Memory.MEM_SIZE];
        }

        public Annotation this[ushort in_memory]
        {
            get { return _annotations[in_memory]; }
        }

        public override byte Read(ushort in_address, bool in_m1_state, bool in_opcode)
        {
            // Remember last M1.
            if (in_m1_state) _last_M1 = in_address;

            // 
            return base.Read(in_address, in_m1_state, in_opcode);
        }

        public override void Write(ushort in_address, byte in_value)
        {
            base.Write(in_address, in_value);
        }
   
    }
    */
}