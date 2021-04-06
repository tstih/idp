namespace Z80.Interfaces
{
    public enum Mnemonic
    {
        NOP
    } 

    /// <summary>
    /// For dissasembler.
    /// </summary>
    public interface IListing
    {
        void Disassemble(Mnemonic mnemonic);
    }
}
