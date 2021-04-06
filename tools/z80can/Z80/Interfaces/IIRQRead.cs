using Z80.Emulation;

namespace Z80.Interfaces {
    public interface IIRQRead {
        byte Read(Z80Emulator cpu);
    }
}
