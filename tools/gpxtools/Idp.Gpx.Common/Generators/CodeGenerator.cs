/*
 * CodeGenerator.cs
 * 
 * Useful functions for code generator. Derive your generator from this class.
 *  
 * MIT License (see: LICENSE)
 * Copyright (c) 2021 Tomaz Stih
 * 
 * 02.03.2021   tstih
 * 
 */
namespace Idp.Gpx.Common.Generators
{
    public class CodeGenerator
    {
        protected CodeGenerator(int tabSize=8) { TabSize = tabSize; }
        protected string Chars(int n, char ch=' ') { return new string(ch, n); }
        protected int TabSize { get; set; }
        protected string Tabs(int n) { return Chars(n*TabSize, ' ');  }
        protected string PadChars(string s, int n, char ch = ' ') { if (s.Length < n) return s + new string(ch, n - s.Length); else return s; }
        protected string PadTabs(string s, int n) { return PadChars(s, n * TabSize); }
    }
}