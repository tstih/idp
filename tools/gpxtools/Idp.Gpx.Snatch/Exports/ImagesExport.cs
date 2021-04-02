/*
 * ImagesExport.cs
 * 
 * Extension functions for decomposing glyph to multiple
 * images and exporting each of them as PNG file.
 *  
 * MIT License (see: LICENSE)
 * Copyright (c) 2021 Tomaz Stih
 * 
 * 02.03.2021   tstih
 * 
 */
using System.Drawing;
using System.Drawing.Imaging;

using Idp.Gpx.Snatch.Commands;

namespace Idp.Gpx.Snatch.Exports
{
    public class ImagesExport : Export
    {
        #region Export Implementation
        public override RetCode OnGlyph(ArrayCmd cmd)
        {
            Bitmap gbmp = new Bitmap(cmd.GlyphWidth, cmd.GlyphHeight);
            using (Graphics gg = Graphics.FromImage(gbmp))
            {
                gg.DrawImage(
                    cmd.SourceBitmap,
                    new Rectangle(0, 0, cmd.CurrentGlyphRect.Width, cmd.CurrentGlyphRect.Height),
                    cmd.CurrentGlyphRect,
                    GraphicsUnit.Pixel);
            }
            string ofname = string.Format("{0}_{1}.png", cmd.Output, cmd.CurrentGlyphAscii);
            gbmp.Save(ofname, ImageFormat.Png);

            return RetCode.SUCCESS;
        }
        #endregion // Export Implementation
    }
}
