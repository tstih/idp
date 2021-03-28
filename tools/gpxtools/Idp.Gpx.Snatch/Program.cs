using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace EXFONT
{
    class Program
    {
        const int width = 6;
        const int height = 6;
        const int rows = 3;
        const int cols = 32;
        const int firstascii = 32;
        const string filename = @"w:\Charset_6x6_192x18.png";
        const string ofilename = @"w:\font6x6.bin";
        const string asmfilename = @"w:\font6x6.s";

        static void Main(string[] args)
        {
            int bit = 0;
            byte[] font = new byte[((width * height)+4) * rows * cols / 8];
            for (int i = 0; i < font.Length; i++) font[i] = 0;

            Bitmap bmp = Image.FromFile(filename) as Bitmap;
            for (int row = 0; row < rows; row++)
                for (int col = 0; col < cols; col++)
                {
                    // Where are we?
                    int ascii = 32 + row * cols + col;
                    for (int y = row * height; y < row * height + height; y++)
                    {
                        for (int x = col * width; x < col * width + width; x++)
                        {
                            Color c = bmp.GetPixel(x, y);
                            int avg = (c.R + c.G + c.B) / 3;
                            if (avg < 128) // It is black pixel.
                            {
                                int B = bit / 8;
                                byte pattern = (byte)((byte)0x80 >> (bit % 8));
                                font[B] = (byte)(font[B] | pattern);
                            }
                            bit++;
                        }
                    }
                    bit += 4; // Align to a byte boundary.
                }
            File.WriteAllBytes(ofilename, font);
            WriteAsm(font);
        }

        static void WriteAsm(byte[] font)
        {
            string header=string.Join(Environment.NewLine,
                "\t\t;; font6x6.s",
		        "\t\t;; system font 6x6",
		        "\t\t;;",
		        "\t\t;; tomaz stih, thu aug 27 2015",
		        "\t\t.module\tfont6x6",
                "",
                "\t\t.globl\t_font6x6",
                "",
                "\t\t.area\t_CODE",
                "_font6x6::"
                );

            StringBuilder sb=new StringBuilder();
            sb.Append(header);

            for (int B = 0; B < font.Length; B++)
            {
                if (B % 8 == 0)
                {
                    sb.AppendLine();
                    sb.AppendFormat("\t\t.byte {0}", font[B]);
                } 
                else
                    sb.AppendFormat(", {0}", font[B]);
            }

            File.WriteAllText(asmfilename, sb.ToString());
        }
    }              
}
