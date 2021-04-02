/*
 * GridExport.cs
 * 
 * Export new image with grid boundaries drawn on top of it.
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
    public class GridExport : Export
    {
        #region Private(s)
        private Pen _pen;
        private Graphics _graphics;
        #endregion // Private(s)

        #region Export Implementation
        public override RetCode Begin(ArrayCmd cmd)
        {
            _pen = new Pen(Color.FromArgb(224, 224, 224, 224));
            _graphics = Graphics.FromImage(cmd.SourceBitmap);
            return RetCode.SUCCESS;
        }

        public override RetCode OnGlyph(ArrayCmd cmd)
        {
            _graphics.DrawRectangle(
                _pen, 
                cmd.CurrentGlyphRect.Left, 
                cmd.CurrentGlyphRect.Top, 
                cmd.CurrentGlyphRect.Width - 1, 
                cmd.CurrentGlyphRect.Height - 1);
            return RetCode.SUCCESS;
        }

        public override RetCode End(ArrayCmd cmd)
        {
            // Release pen and graphics.
            _pen.Dispose();
            _graphics.Dispose();
            // Save image.
            cmd.SourceBitmap.Save(cmd.Output + ".png", ImageFormat.Png);
            // And return success.
            return RetCode.SUCCESS;
        }
        #endregion // Export Implementation
    }
}
