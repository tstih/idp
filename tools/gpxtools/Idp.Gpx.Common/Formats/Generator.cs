namespace Idp.Gpx.Common.Formats
{
    public class Generator
    {
        protected string Spaces(int n) { return new string(' ', n); }
        protected string FixedWidth(string s, int n) { if (s.Length < n) return s + new string(' ',n-s.Length); else return s; }
    }
}
