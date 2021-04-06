using System;
using System.Runtime.CompilerServices;

using Z80.Interfaces;

namespace Z80.Emulation
{
	[Flags]
	public enum Mode { Emulator, Dissasembler, Analyser }

	public sealed partial class Z80Emulator
	{
		/// <summary>
		/// Interrupt mode types
		/// </summary>
		public enum IMMode : byte
		{
			IM0 = 0,
			IM1 = 1,
			IM2 = 2
		};

		// delegates
		public delegate void RetiEvent(Z80Emulator in_sender);

		public RetiEvent RetiEventHandler;

		// opcode delegate and table size
		private delegate void OperationDelegate();

		/// <summary>
		/// Interrupr flag 1
		/// </summary>
		public byte IFF1;

		/// <summary>
		/// Interrupr flag 2
		/// </summary>
		public byte IFF2;

		/// <summary>
		/// CPU halt flag
		/// </summary>
		public bool Halted;

		/// <summary>
		/// Interrupt mode
		/// </summary>
		public IMMode IM;

		/// <summary>
		/// Total T State counter. Counts the T states (clock cycles) since the last reset
		/// </summary>
		public ulong TotalTState;

		/// <summary>
		/// Emulation mode- emulator, disassembler or analyser.
		/// </summary>
		public Mode EmulationMode;

		/// <summary>
		/// General purpose registers
		/// </summary>
		private Registers _registers;

		// Internal variables
		private uint _T_state; // t-state clock of current/last step
		private bool _noint_once;      // disable interrupts before next opcode?
		private bool _reset_PV_on_int; // reset P/V flag on interrupt? (for LD A,R / LD A,I)
		private bool _doing_opcode;    // is there an opcode currently executing?
		private byte _int_vector_req;  // opcode must be fetched from IO device? (int vector read)
		private byte _prefix;          // temprary storage for prefix instruction
		private byte _tmpbyte;
		private sbyte _tmpbyte_s;
		private ushort _lastM1;			// Last M1 fetch.

		// interfaces
		public IMemory _memory;    // memory access interface
		public IPort _ports;       // Port access interface
		public IIRQRead _int_read; // opcode reader for IM2 interrupt handling
		public IListing _listing;  // disassembly

		/// <summary>
		/// Creates Z80 CPU emulation class
		/// </summary>
		/// <param name="memory">Memory access provider class</param>
		/// <param name="inputOutputPortPorts">IO provider class</param>
		/// <param name="irqRead">Interrupt provider for IM2 mode</param>
		/// <param name="reset">True for reset the CPU after creating</param>
		public Z80Emulator(
			IMemory memory, 
			IPort inputOutputPortPorts, 
			IIRQRead irqRead,
			IListing listing,
			bool reset = true, 
			ushort start=0)
		{
			_memory = memory;

			_ports = inputOutputPortPorts;

			_int_read = irqRead;

			_listing = listing;

			_registers = new Registers() { PC = start };

			InitializeOpcodes();

			if (reset) Reset();
		}

		/// <summary>
		/// Sets/changes memory access provider
		/// </summary>
		/// <param name="in_memory"></param>
		public void SetMemory(IMemory in_memory)
		{
			_memory = in_memory;
		}

		/// <summary>
		/// Sets/ changes port acces provider
		/// </summary>
		/// <param name="in_ports"></param>
		public void SetPorts(IPort in_ports)
		{
			_ports = in_ports;
		}

		/// <summary>
		/// Returns true when instruction execution is finished after Step function. If only prefi codes are processed it returns false
		/// </summary>
		public bool InstructionDone
		{
			get { return _prefix == 0; }
		}

		/// <summary>
		/// Executes the next instruction
		/// </summary>
		/// <returns></returns>
		public uint Step()
		{
			_doing_opcode = true;
			_noint_once = false;
			_reset_PV_on_int = false;
			_T_state = 4; // set for NOP length, the actual instruction will overrite it

			byte opcode = READ_OP_M1();
			_registers.Rcnt++;

			if (_prefix == 0)
			{
				_opcodes_base[opcode]();
			}
			else
			{
				if ((_prefix | 0x20) == 0xFD && ((opcode | 0x20) == 0xFD || opcode == 0xED))
				{
					_prefix = opcode;
					_noint_once = true; // interrupts are not accepted immediately after prefix
				}
				else
				{
					OperationDelegate ofn;

					switch (_prefix)
					{
						case 0xDD:
						case 0xFD:
							if (opcode == 0xCB)
							{
								byte d = READ_OP();
								_tmpbyte_s = (sbyte)((d & 0x80) != 0 ? -(((~d) & 0x7f) + 1) : d);
								opcode = READ_OP();
								ofn = _prefix == 0xDD ? _opcodes_ddcb[opcode] : _opcodes_fdcb[opcode];
							}
							else
							{
								ofn = (_prefix == 0xDD ? _opcodes_dd[opcode] : _opcodes_fd[opcode]) ?? _opcodes_base[opcode];
							}
							break;

						case 0xED:
							ofn = _opcodes_ed[opcode] ?? _opcodes_base[0x00];
							break;

						case 0xCB:
							ofn = _opcodes_cb[opcode];
							break;

						default:
							// this mustn't happen!
							throw new SystemException(string.Format("Invalid prefix {0:X2}", _prefix));
					}
					ofn();
					_prefix = 0;
				}
			}

			_doing_opcode = false;

			TotalTState += _T_state;

			return _T_state;
		}

		/// <summary>
		/// Gets the general purpose registers of the CPU
		/// </summary>
		public Registers Registers
		{
			get { return _registers; }
		}

		/// <summary>
		/// Resets the CPU
		/// </summary>
		public void Reset()
		{
			IFF1 = IFF2 = 0;
			IM = IMMode.IM0;
			_noint_once = _reset_PV_on_int = Halted = false;
			_int_vector_req = 0;
			_doing_opcode = false;
			_T_state = 0;
			TotalTState = 0;
			_prefix = 0;

			Registers.Reset();
		}

		#region · IRQ  handlers ·

		/// <summary>
		/// NMI hanbdler
		/// </summary>
		/// <returns></returns>
		public ulong Nmi()
		{
			if (_doing_opcode || _noint_once || (_prefix != 0)) return 0;

			if (Halted) { Registers.PC++; Halted = false; } // so we met an interrupt... stop waiting

			_doing_opcode = true;

			_registers.Rcnt++; // accepting interrupt increases R by one

			// IFF2=IFF1
			// contrary to zilog z80 docs, IFF2 is not modified on NMI. proved by Slava Tretiak aka restorer
			IFF1 = 0;

			_memory.Write(--Registers.SP, Registers.PCh); // PUSH PC -- high byte

			_memory.Write(--Registers.SP, Registers.PCl); // PUSH PC -- low byte

			Registers.PC = 0x0066;
			Registers.WZ = Registers.PC; // FIXME: is that really so?

			_doing_opcode = false;

			return 11; // NMI always takes 11 t-states
		}

		/// <summary>
		/// INT handler
		/// </summary>
		/// <returns></returns>
		public ulong Int()
		{
			// If the INT line is low and iff1 is set, and there's no opcode executing just now,
			// a maskable interrupt is accepted, whether or not the
			// last INT routine has finished
			if ((IFF1 == 0) || _noint_once || _doing_opcode || (_prefix != 0)) return 0;

			_T_state = 0;

			if (Halted) { Registers.PC++; Halted = false; } // so we met an interrupt... stop waiting

			// When an INT is accepted, both iff1 and IFF2 are cleared, preventing another interrupt from
			// occurring which would end up as an infinite loop
			IFF1 = IFF2 = 0;

			// original (NMOS) zilog z80 bug:
			// If a LD A,I or LD A,R (which copy IFF2 to the P/V flag) is interrupted, then the P/V flag is reset, even if interrupts were enabled beforehand.
			// (this bug was fixed in CMOS version of z80)
			if (_reset_PV_on_int) { Registers.F = (byte)(Registers.F & ~Tables.FLAG_P); }
			_reset_PV_on_int = false;

			_int_vector_req = 1;
			_doing_opcode = true;

			switch (IM)
			{
				case IMMode.IM0:
					// note: there's no need to do R++ and WAITs here, it'll be handled by z80ex_step
					uint tt = Step();

					while (_prefix != 0)
					{ // this is not the end?
						tt += Step();
					}

					_T_state = tt;
					break;

				case IMMode.IM1:
					Registers.Rcnt++;
					_opcodes_base[0xFF](); // RST38
					break;

				case IMMode.IM2:
					Registers.Rcnt++;
					// takes 19 clock periods to complete (seven to fetch the
					// lower eight bits from the interrupting device, six to save the program
					// counter, and six to obtain the jump address)
					byte iv = READ_OP();
					ushort inttemp = (ushort)((0x100 * Registers.I) + iv);

					PUSH(Registers.PC);

					Registers.PCl = CPUReadMemory(inttemp++);
					Registers.PCh = CPUReadMemory(inttemp);
					Registers.WZ = Registers.PC;

					break;
			}

			_doing_opcode = false;
			_int_vector_req = 0;

			return _T_state;
		}

		#endregion

		#region · Memory and port read / write ·

		/// <summary>
		/// Reads opcode
		/// </summary>
		/// <returns></returns>
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		byte READ_OP_M1()
		{
			if (_int_vector_req != 0)
				return _int_read.Read(this);
			else
            {
				_lastM1 = Registers.PC;
				var ret=_memory.Fetch(Registers.PC, Registers.PC);
				Registers.PC++;
				return ret;
			}				
		}

		/// <summary>
		/// Reads opcode argument
		/// </summary>
		/// <returns></returns>
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		byte READ_OP()
		{
			return _int_vector_req != 0 ? 
				_int_read.Read(this) : 
				_memory.Fetch(Registers.PC++, _lastM1);
		}

		/// <summary>
		/// Set T state for the current instruction
		/// </summary>
		/// <param name="in_t_state"></param>
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		private void CPUSetTState(uint in_t_state)
		{
			_T_state = in_t_state;
		}

		/// <summary>
		/// Add instruction to listing.
		/// </summary>
		private void DisassembleInstruction(
			uint address,
			Mnemonic mnemonic,
			uint tstates)
        {

        }

		/**
		 * read byte from memory
		 */
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		byte CPUReadMemory(ushort in_address)
		{
			return _memory.Read(in_address);
		}

		/// <summary>
		/// Reads byte from port
		/// </summary>
		/// <param name="port"></param>
		/// <returns></returns>
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		byte CPUReadPort(ushort port)
		{
			return _ports.ReadIO(port);
		}

		/// <summary>
		/// Writes byte to memory
		/// </summary>
		/// <param name="addr"></param>
		/// <param name="val"></param>
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		void CPUWriteMemory(ushort addr, byte val)
		{
			_memory.Write(addr, val);
		}

		/// <summary>
		/// Writes byte to port
		/// </summary>
		/// <param name="port"></param>
		/// <param name="val"></param>
		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		void CPUWritePort(ushort port, byte val)
		{
			_ports.WriteIO(port, val);
		}

		public override string ToString()
		{
			string ret = string.Format("PC {0} SP {1}\n", Registers.PC, Registers.SP);
			ret += string.Format("AF {0} BC {1} DE {2} HL {3}\n", Registers.AF, Registers.BC, Registers.DE, Registers.HL);
			ret += string.Format("AF' {0} BC' {1} DE' {2} HL' {3}\n", Registers._AF_, Registers._BC_, Registers._DE_, Registers._HL_);
			ret += string.Format("IX {0} IY {1}\n", Registers.IX, Registers.IY);
			ret += string.Format("R {0:X2} I {1} IFF1 {2} IFF2 {3} {4} H {5}\n", Registers.R, Registers.I, IFF1, IFF2, IM, Halted);

			return ret;
		}

		#endregion

		#region · Private members ·  

		void InitializeOpcodes()
		{
			InitializeOpcodesBase();
			InitializeOpcodesCB();
			InitializeOpcodesDD();
			InitializeOpcodesDDCB();
			InitializeOpcodesED();
			InitializeOpcodesFD();
			InitializeOpcodesFDCB();
		}

		#endregion
	}
}
