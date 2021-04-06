using Z80.Emulation;

namespace Z80.Interfaces
{
	public interface IPort
	{
		byte ReadIO(ushort addr);
		void WriteIO(ushort addr, byte value);
	}
}