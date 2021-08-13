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
using System;
using System.Drawing;
using System.Drawing.Imaging;
using Idp.Gpx.Common.Utils;
using Idp.Gpx.Snatch.Commands;

namespace Idp.Gpx.Snatch.Exports
{
    public class GridExport : Export
    {
        #region Private(s)
        private Pen _pen;
        private Graphics _graphics;
        private int _rows;
        private Bitmap _target;
        #endregion // Private(s)

        #region Export Implementation
        public override RetCode Begin(ArrayCmd cmd)
        {
            int glyphs = cmd.Last - cmd.First + 1;
            _rows = (int)Math.Ceiling((float)glyphs / (float)cmd.GridColumns);
            int xtraPixelsX = cmd.GridColumns + 1,
                xtraPixelsY = _rows + 1;
            _target = new Bitmap(
                cmd.GridColumns * cmd.GlyphWidth + xtraPixelsX,
                _rows * cmd.GlyphHeight + xtraPixelsY
            );
            _graphics = Graphics.FromImage(_target);
            _graphics.Clear(Color.White);
            _pen = new Pen(Color.FromArgb(128, 0, 0, 128));

            // Rectangle around font.
            _graphics.DrawRectangle(
                _pen,
                new Rectangle(
                    0, 0,
                    _target.Width-1,
                    _target.Height-1
                )
            );
            for (int y = 1; y < _rows; y++)
                _graphics.DrawLine(_pen,
                    0, 
                    y*(cmd.GlyphHeight+1),
                    _target.Width,
                    y*(cmd.GlyphHeight+1)
                );

            for (int x=1;x< cmd.GridColumns; x++)
                _graphics.DrawLine(_pen,
                    x * (cmd.GlyphWidth+1),
                    0,
                    x * (cmd.GlyphWidth+1),
                    _target.Height
                );

            return RetCode.SUCCESS;
        }

        public override RetCode OnGlyph(ArrayCmd cmd)
        {
            Color foreColor, backColor;

            GlyphProcessor gp = new GlyphProcessor(
                cmd.SourceBitmap,
                new Rectangle(
                    cmd.CurrentGlyphRect.Left,
                    cmd.CurrentGlyphRect.Top,
                    cmd.CurrentGlyphRect.Width,
                    cmd.CurrentGlyphRect.Height
                )
            );
            
            foreColor = gp.ColorFromString(cmd.PointColor);
            backColor = gp.ColorFromString(cmd.TransparentColor);

            // Convert to B/W
            Bitmap converted = gp.ToMono(
                foreColor,
                backColor,
                Color.Black,
                Color.White,
                (byte)cmd.Threshold
            );

            // And draw to grid.
            int index = cmd.CurrentGlyphAscii - cmd.First;
            int row = index / cmd.GridColumns;
            int col = index % cmd.GridColumns;

            _graphics.DrawImage(
                converted,
                new Point(
                    col * (cmd.GlyphWidth + 1) + 1,
                    row * (cmd.GlyphHeight + 1) + 1
                )
            );

            return RetCode.SUCCESS;
        }

        public override RetCode End(ArrayCmd cmd)
        {
            // Save image.
            _target.Save(cmd.Output + ".png", ImageFormat.Png);
            // Dispose of everything.
            _pen.Dispose();
            _graphics.Dispose();
            _target.Dispose();
            // And return success.
            return RetCode.SUCCESS;
        }
        #endregion // Export Implementation
    }
}
