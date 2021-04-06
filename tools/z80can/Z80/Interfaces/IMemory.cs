using Z80.Emulation;

namespace Z80.Interfaces
{
	/// <summary>
	/// Interface for memory access of the Z80 CPU
	/// </summary>
	public interface IMemory
	{
		byte Read(
			ushort in_address
		);

		// if in_address=inst_address then in M1 state.
		byte Fetch(
			ushort in_address,
			ushort inst_address);

		void Write(ushort in_address, byte in_value);
	}
}
