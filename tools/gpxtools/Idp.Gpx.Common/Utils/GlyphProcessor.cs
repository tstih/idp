/*
 * GlyphProcessor.cs
 * 
 * Process glyphs (i.e. vectorize, rasterize, etc)
 *  
 * MIT License (see: LICENSE)
 * Copyright (c) 2021 Tomaz Stih
 * 
 * 02.03.2021   tstih
 * 
 */
using System;
using System.Collections.Generic;
using System.Linq;
using System.Drawing;
using System.Drawing.Imaging;

using XYZ.Formats;
using Idp.Gpx.Common.Ex;
using System.Collections;

namespace Idp.Gpx.Common.Utils
{
    public struct Pixel
    {
        public int X;
        public int Y;
        public Color C;
        public bool Move; // Default is false!
        public bool Match(int x, int y) { return x == X && y == Y; }
    }

    public class GlyphProcessor
    {
        #region Private(s)
        private Bitmap _bmp;
        #endregion // Private(s)

        #region Ctor
        public GlyphProcessor(Bitmap bmp)
        {
            _bmp = bmp;
        }

        public GlyphProcessor(Bitmap sourceBitmap, Rectangle glyphRect)
        {
            _bmp = new Bitmap(glyphRect.Width, glyphRect.Height);
            using (Graphics gg = Graphics.FromImage(_bmp))
            {
                gg.DrawImage(
                    sourceBitmap,
                    new Rectangle(0, 0, glyphRect.Width, glyphRect.Height),
                    glyphRect,
                    GraphicsUnit.Pixel);
            }
        }
        #endregion // Ctor

        #region Method(s)

        public byte[] ToTiny(ref byte maxw)
        {
            // Get all pixels from image.
            Pixel[] pixels=Pixels(Color.Black); // TODO: Configure correct color.
            if (pixels.Length == 0)
                maxw = 0;
            else
                maxw = (byte)pixels.Select(p => p.X).Max(); // For proportional font, this is max width.
            if (pixels.Length <= 1) return null; // TODO: Differentiate between an empty glyph and a 1 px.

            Graph g = new Graph(pixels, _bmp.Width, _bmp.Height, 32); // TODO Configure correct color.
            pixels=g.TSP2Opt(); // Optimize drawing.

            // And extract path as moves.
            List<byte> moves = new List<byte>();

            if (pixels == null)
            { // TODO: special case
                moves.Add(0);
            } else {

                byte[] tinyLines = ToTinyLines(pixels);

                // First byte is number of moves
                moves.Add((byte)tinyLines.Length);

                // Followed by 2 byte origin, which are added to the cursor.
                moves.Add((byte)pixels[0].X);
                moves.Add((byte)pixels[0].Y);

                // Followed by lines.
                moves.AddRange(tinyLines);
            }

            return moves.ToArray();
        }

        private byte Stroke(int prevdx, int prevdy, int line)
        {
            // Bottom 3 bytes.
            int direction;
            if (prevdx == 0 && prevdy == 0) direction = 0; else direction=Delta(prevdx, prevdy);
            int dxstep = line * Math.Abs(prevdx), dystep = line * Math.Abs(prevdy);
            int b =
                0x80 // Bit 7=1 (small vector def)
                | (dxstep << 5)
                | (dystep << 3)
                | direction;
            return (byte)b;
        }

        private byte Control(bool pen, bool down)
        {
            int b =
                0x00 // Bit 7 is not used.
                | (down ? 1 : 0) // Bit 0 is pen down
                | (pen ? 1 << 1 : 0) // Bit 1 is pen/eraser select
                | 0x04; // high speed writing
            return (byte)b;
        }

        private byte[] ToTinyLines(Pixel[] pixels)
        {
            List<byte> result=new List<byte>();
            int dx=0, dy=0, prevdx, prevdy, line;
            prevdx = prevdy = line = 0;
            bool pen;

            int curr = 0, next;
            int len = pixels.Length;
            while (curr < len)
            {
                // Get next pixel.
                next = curr + 1;
                if (next==len)
                { // There is no next pixel. Finish previous stroke, and abort!
                    result.Add(Stroke(dx, dy, line));
                } else
                { // There is next pixel.

                    // Get delta to last pixel
                    dx = pixels[next].X - pixels[curr].X;
                    dy = pixels[next].Y - pixels[curr].Y;

                    // Calc distance in absolute moves.
                    int dist = Math.Max(Math.Abs(dx), Math.Abs(dy));

                    if (dist > 1)
                    { // To far way. Finish previous stroke, and move to the new position.
                        result.Add(Stroke(prevdx, prevdy, line));

                        // A pixel or a blank?
                        pen = (SameColor(pixels[next].C, Color.Black));

                        result.Add(Control(pen, false)); // Pen up.

                        // Move to new position.
                        Pixel currPix = pixels[curr];
                        Pixel nextPix = pixels[next];
                        while (!currPix.Match(nextPix.X, nextPix.Y))
                        {
                            // Move to the right direction.
                            int dpx = 0, dpy = 0;
                            if (currPix.X < nextPix.X) dpx = 1; else if (currPix.X > nextPix.X) dpx = -1;
                            if (currPix.Y < nextPix.Y) dpy = 1; else if (currPix.Y > nextPix.Y) dpy = -1;
                            currPix = new Pixel() { X = currPix.X + dpx, Y = currPix.Y + dpy, Move = true };
                            if (!currPix.Match(nextPix.X, nextPix.Y)) // Not the last.
                                result.Add(Stroke(dpx, dpy, 1));
                        }

                        // And pen down.
                        result.Add(Control(pen, true));

                        // No line.
                        dx = dy = 0;
                        line = 1;
                    }
                    else if (!SameColor(pixels[curr].C, pixels[next - 1].C))
                    { // Finish previous stroke, move to new position and change pen.
                        result.Add(Stroke(prevdx, prevdy, line));

                        // A pixel or a blank?
                        pen = (SameColor(pixels[next].C, Color.Black));

                        // Pen up and change the color.
                        result.Add(Control(pen, false));

                        // Move to new position.
                        result.Add(Stroke(dx, dy, 1));

                        // Pen down.
                        result.Add(Control(pen, true));

                        // No line.
                        line = 1;
                    }
                    else if (line == 4)
                    { // That's long enough!
                        result.Add(Stroke(prevdx, prevdy, line));
                        line = 1;
                    } else { // We've got at least two pixels!
                        if (prevdx == dx && prevdy == dy)
                            line++;
                        else
                        {
                            if (prevdx != 0 || prevdy != 0)
                                result.Add(Stroke(prevdx, prevdy, line));
                            line = 1;
                        }
                    }
                    prevdx = dx; prevdy = dy;
                }
                // Inc current pixe.
                curr++;
            }

            return result.ToArray();
        }

        /// <summary>
        /// Convert lines to bitmap (to test)
        /// </summary>
        public Bitmap FromLines(int w, int h, Line[] lines)
        {
            Bitmap bmp = new Bitmap(w, h);
            using (Graphics g = Graphics.FromImage(bmp))
            {
                g.Clear(Color.White);
                foreach (Line l in lines)
                    g.DrawLine(Pens.Black, new Point(l.x1, l.y1), new Point(l.x2, l.y2));
            }
            return bmp;
        }

        /// <summary>
        /// Execute operation on two bitmaps (for example, XOR).
        /// </summary>
        unsafe public Bitmap Execute(int w, int h, Bitmap b1, Bitmap b2, Func<byte, byte, byte> op)
        {
            Rectangle ir = new Rectangle(0, 0, w, h);
            var locked1 = b1.LockBits(ir, ImageLockMode.ReadOnly, PixelFormat.Format1bppIndexed);
            var locked2 = b2.LockBits(ir, ImageLockMode.ReadOnly, PixelFormat.Format1bppIndexed);
            Bitmap diff = new Bitmap(w, h, PixelFormat.Format1bppIndexed);
            var difflock = diff.LockBits(ir, ImageLockMode.WriteOnly, PixelFormat.Format1bppIndexed);
            try
            {
                byte* buffer1 = (byte*)locked1.Scan0;
                byte* buffer2 = (byte*)locked2.Scan0;
                byte* diffbuff = (byte*)difflock.Scan0;
                for (ushort r = 0; r < locked1.Height; r++)
                {
                    byte* row1 = buffer1 + r * locked1.Stride;
                    byte* row2 = buffer2 + r * locked2.Stride;
                    byte* diffrow = diffbuff + r * difflock.Stride;

                    for (ushort c = 0; c < locked1.Stride; c++)
                        diffrow[c] = op(row1[c], row2[c]);
                }
            }
            finally
            {
                b1.UnlockBits(locked1);
                b2.UnlockBits(locked2);
                diff.UnlockBits(difflock);
            }
            return diff;
        }

        // This gets all boundaries in the given pixels.
        // It assumes you're looking for boundaries between non-transparent shapes on a transparent background
        // (using the isTransparent property);
        // but you could modify this, to pass in a predicate to say what background color you're looking for (e.g. White).
        public List<List<Point>> OutlineBitmap()
        {
            Size size = _bmp.Size;
            HashSet<Point> found = new HashSet<Point>();
            List<Point> list = null;
            List<List<Point>> lists = new List<List<Point>>();
            bool inside = false;

            // Defines the neighborhood offset position from current position and the neighborhood
            // position we want to check next if we find a new border at checkLocationNr.
            int width = size.Width;
            Tuple<Func<Point, Point>, int>[] neighborhood = new Tuple<Func<Point, Point>, int>[]
            {
                new Tuple<Func<Point, Point>, int>(point => new Point(point.X-1,point.Y), 7),
                new Tuple<Func<Point, Point>, int>(point => new Point(point.X-1,point.Y-1), 7),
                new Tuple<Func<Point, Point>, int>(point => new Point(point.X,point.Y-1), 1),
                new Tuple<Func<Point, Point>, int>(point => new Point(point.X+1,point.Y-1), 1),
                new Tuple<Func<Point, Point>, int>(point => new Point(point.X+1,point.Y), 3),
                new Tuple<Func<Point, Point>, int>(point => new Point(point.X+1,point.Y+1), 3),
                new Tuple<Func<Point, Point>, int>(point => new Point(point.X,point.Y+1), 5),
                new Tuple<Func<Point, Point>, int>(point => new Point(point.X-1,point.Y+1), 5)
            };

            for (int y = 1; y < size.Height-1; ++y)
            {
                for (int x = 1; x < size.Width-1; ++x)
                {
                    Point point = new Point(x, y);
                    // Scan for non-transparent pixel
                    if (found.Contains(point) && !inside)
                    {
                        // Entering an already discovered border
                        inside = true;
                        continue;
                    }
                    bool isTransparent = !IsOutlinePoint(point.X,point.Y);
                    if (!isTransparent && inside)
                    {
                        // Already discovered border point
                        continue;
                    }
                    if (isTransparent && inside)
                    {
                        // Leaving a border
                        inside = false;
                        continue;
                    }
                    if (!isTransparent && !inside)
                    {
                        lists.Add(list = new List<Point>());

                        // Undiscovered border point
                        found.Add(point); list.Add(point);   // Mark the start pixel
                        int checkLocationNr = 1;  // The neighbor number of the location we want to check for a new border point
                        Point startPos = point;      // Set start position
                        int counter = 0;       // Counter is used for the jacobi stop criterion
                        int counter2 = 0;       // Counter2 is used to determine if the point we have discovered is one single point

                        // Trace around the neighborhood
                        while (true)
                        {
                            // The corresponding absolute array address of checkLocationNr
                            Point checkPosition = neighborhood[checkLocationNr - 1].Item1(point);
                            // Variable that holds the neighborhood position we want to check if we find a new border at checkLocationNr
                            int newCheckLocationNr = neighborhood[checkLocationNr - 1].Item2;

                            // Beware that the point might be outside the bitmap.
                            // The isTransparent method contains the safety check.
                            if (IsOutlinePoint(checkPosition.X,checkPosition.Y))
                            {
                                // Next border point found
                                if (checkPosition == startPos)
                                {
                                    counter++;

                                    // Stopping criterion (jacob)
                                    if (newCheckLocationNr == 1 || counter >= 3)
                                    {
                                        // Close loop
                                        inside = true; // Since we are starting the search at were we first started we must set inside to true
                                        break;
                                    }
                                }

                                checkLocationNr = newCheckLocationNr; // Update which neighborhood position we should check next
                                point = checkPosition;
                                counter2 = 0;             // Reset the counter that keeps track of how many neighbors we have visited
                                found.Add(point); list.Add(point); // Set the border pixel
                            }
                            else
                            {
                                // Rotate clockwise in the neighborhood
                                checkLocationNr = 1 + (checkLocationNr % 8);
                                if (counter2 > 8)
                                {
                                    // If counter2 is above 8 we have traced around the neighborhood and
                                    // therefor the border is a single black pixel and we can exit
                                    counter2 = 0;
                                    list = null;
                                    break;
                                }
                                else
                                {
                                    counter2++;
                                }
                            }
                        }

                    }
                }
            }
            return lists;
        }


        /// <summary>
        /// Return all pixels of specific color in image.
        /// TODO: Threshold.
        /// </summary>
        public Pixel[] Pixels(Color color)
        {
            // Return pixels.
            List<Pixel> pixels = new List<Pixel>();

            // First enumerate all pixels.
            for (int y = 0; y < _bmp.Height; y++)
                for (int x = 0; x < _bmp.Width; x++)
                    if (IsPixel(x, y, color))
                        pixels.Add(new Pixel() { X = x, Y = y, C = color });

            // And return it.
            return pixels.ToArray();
        }

        /// <summary>
        /// Return all connections between pixels in the array.
        /// A connection exists if pixels touch each other (the junction
        /// can be vertical, horizontal or diagonal).
        /// </summary>
        public Tuple<int, int>[] Edges(Pixel[] pixels)
        {
            // Now create a graph of all "connected" pixels.
            HashSet<Tuple<int, int>> edges = new HashSet<Tuple<int, int>>();
            int i;
            for (i = 0; i < pixels.Length; i++)
            {
                for (int cy = pixels[i].Y - 1; cy <= pixels[i].Y + 1; cy++)
                    for (int cx = pixels[i].X - 1; cx <= pixels[i].X + 1; cx++)
                        // If we are inside the glyph and we are not the same glyph.
                        if (cx > 0 && cx < _bmp.Width
                            && cy > 0 && cy < _bmp.Height
                            && !(pixels[i].Match(cx, cy))) // Not the same.
                        {
                            int j;
                            for (j = 0; j < pixels.Length; j++)
                                if (pixels[j].Match(cx, cy))
                                    break; // We found the right pixel. It's in j.

                            if (j < pixels.Length) // Found it?
                            {
                                if (i < j) edges.Add(new Tuple<int, int>(i, j));
                                else edges.Add(new Tuple<int, int>(j, i));
                            }
                        }
            }

            // Return an array of all the edges.
            return edges.ToArray();
        }

        /// <summary>
        /// Is pixel of specific color?
        /// TODO: Threshold.
        /// </summary>
        public bool IsPixel(int x, int y, Color c, byte threshold=32) {
            Color p = _bmp.GetPixel(x, y);
            double distance = Math.Sqrt(Math.Pow(c.R-p.R,2)+ Math.Pow(c.G - p.G, 2)+ Math.Pow(c.B - p.B, 2));
            return distance < threshold;
        }

        public bool SameColor(Color c1, Color c2, byte threshold=32)
        {
            double distance = Math.Sqrt(Math.Pow(c1.R - c2.R, 2) + Math.Pow(c1.G - c2.G, 2) + Math.Pow(c1.B - c2.B, 2));
            return distance < threshold;
        }

        /// <summary>
        /// Calculate tiny delta between two adjacent points.
        /// </summary>
        public byte Delta(Pixel p1, Pixel p2)
        {
            int dx = p1.X - p2.X, dy = p1.Y - p2.Y;
            if (dx == 1 && dy == 0) return 0; // right
            else if (dx == 1 && dy == -1) return 1; // right, up.
            else if (dx == 0 && dy == -1) return 2; // up.
            else if (dx == -1 && dy == -1) return 3; // left, up.
            else if (dx == 0 && dy == 1) return 4; // down.
            else if (dx == 1 && dy == 1) return 5; // right, down.
            else if (dx == -1 && dy == 0) return 6; // left.
            else if (dx == -1 && dy == 1) return 7; // down, left.
            else return 0xff; // Error!
        }

        public byte Delta(int dx, int dy)
        {
            if (dx == 1 && dy == 0) return 0; // right
            else if (dx == 1 && dy == -1) return 1; // right, up.
            else if (dx == 0 && dy == -1) return 2; // up.
            else if (dx == -1 && dy == -1) return 3; // left, up.
            else if (dx == 0 && dy == 1) return 4; // down.
            else if (dx == 1 && dy == 1) return 5; // right, down.
            else if (dx == -1 && dy == 0) return 6; // left.
            else if (dx == -1 && dy == 1) return 7; // down, left.
            else return 0xff; // Error!
        }

        // Convert 1bpp bitmap to horizontal lines.
        public unsafe Line[] AsLines(Bitmap bmp)
        {
            List<Line> lines = new List<Line>();
            Rectangle ir = new Rectangle(0, 0, bmp.Width, bmp.Height);
            var locked = bmp.LockBits(ir, ImageLockMode.ReadOnly, PixelFormat.Format1bppIndexed);
            try
            {
                byte* buffer = (byte*)locked.Scan0;
                for (ushort y = 0; y < locked.Height; y++)
                {
                    byte* row = buffer + y * locked.Stride;
                    ushort x = 0; // Hires x is inside the byte.
                    Line l = new Line();
                    bool lexists = false;
                    bool linside = false;
                    for (ushort xb = 0; xb < locked.Stride; xb++)
                        for (ushort bit = 8; bit > 0; bit--)
                        {
                            // Get bit value.
                            var bval = (row[xb] >> (bit - 1)) & 0x01;

                            // We got a point? Note that black is index 0, and white 1.
                            if (bval != 1)
                            {
                                if (!linside)
                                { // We're not iside the line?
                                    linside = true; // We're in the line.
                                    if (!lexists) lexists = true;
                                    l = new Line() { x1 = x, y1 = y }; // Start the line.
                                }
                            }
                            else
                            { // No point. Are we inside (i.e. first empty point?)
                                if (linside)
                                {
                                    // Finalize the line.
                                    l.x2 = (ushort)(x - 1);
                                    l.y2 = y;
                                    linside = false; // No longer inside.
                                    lines.Add(l); // Add line.
                                }
                            }
                            // Next coordinate.
                            x++;
                        }
                }
            }
            finally
            {
                bmp.UnlockBits(locked);
            }
            return lines.ToArray();
        }

        /// <summary>
        /// Uses the Douglas Peucker algorithm to reduce the number of points.
        /// </summary>
        /// <param name="Points">The points.</param>
        /// <param name="Tolerance">The tolerance.</param>
        /// <returns></returns>
        public List<Point> DouglasPeuckerReduction
            (List<Point> Points, Double Tolerance)
        {
            if (Points == null || Points.Count < 3)
                return Points;

            Int32 firstPoint = 0;
            Int32 lastPoint = Points.Count - 1;
            List<Int32> pointIndexsToKeep = new List<Int32>();

            //Add the first and last index to the keepers
            pointIndexsToKeep.Add(firstPoint);
            pointIndexsToKeep.Add(lastPoint);

            //The first and the last point cannot be the same
            while (Points[firstPoint].Equals(Points[lastPoint]))
            {
                lastPoint--;
            }

            DouglasPeuckerReduction(Points, firstPoint, lastPoint,
            Tolerance, ref pointIndexsToKeep);

            List<Point> returnPoints = new List<Point>();
            pointIndexsToKeep.Sort();
            foreach (Int32 index in pointIndexsToKeep)
            {
                returnPoints.Add(Points[index]);
            }

            return returnPoints;
        }

        /// <summary>
        /// Creates an list of lines out of a polygon.
        /// </summary>
        public List<Line> Polygon2Lines(List<Point> pts)
        {
            List<Line> result = new List<Line>();
            // What are you doing here?
            if (pts.Count > 2)
            {
                int prev = 0;
                for (int curr = 1; curr < pts.Count; curr++)
                {
                    result.Add(new Line()
                    {
                        x1 = (ushort)pts[prev].X,
                        x2 = (ushort)pts[curr].X,
                        y1 = (ushort)pts[prev].Y,
                        y2 = (ushort)pts[curr].Y
                    });
                    prev = curr;
                }
            }
            return result;
        }

        public BitArray ToBits()
        {
            // Char to bitfield.
            BitArray bits;
            int bytesPerGlyphLine = (byte)((_bmp.Width - 1) / 8 + 1);
            bits = new BitArray(8 * bytesPerGlyphLine * _bmp.Height, false);
            for (int y = 0; y < _bmp.Height; y++)
                for (int x = 0; x < _bmp.Width; x++)
                    bits[y * 8 * bytesPerGlyphLine + x] =
                        IsPixel(x,y, Color.Black); // TODO: configure pixel color.
            return bits;
        }

        #endregion // Method(s)

        #region Helper(s)

        /// <summary>
        /// Douglases the peucker reduction.
        /// </summary>
        /// <param name="points">The points.</param>
        /// <param name="firstPoint">The first point.</param>
        /// <param name="lastPoint">The last point.</param>
        /// <param name="tolerance">The tolerance.</param>
        /// <param name="pointIndexsToKeep">The point index to keep.</param>
        private void DouglasPeuckerReduction(List<Point>
            points, Int32 firstPoint, Int32 lastPoint, Double tolerance,
            ref List<Int32> pointIndexsToKeep)
        {
            Double maxDistance = 0;
            Int32 indexFarthest = 0;

            for (Int32 index = firstPoint; index < lastPoint; index++)
            {
                Double distance = PerpendicularDistance
                    (points[firstPoint], points[lastPoint], points[index]);
                if (distance > maxDistance)
                {
                    maxDistance = distance;
                    indexFarthest = index;
                }
            }

            if (maxDistance > tolerance && indexFarthest != 0)
            {
                //Add the largest point that exceeds the tolerance
                pointIndexsToKeep.Add(indexFarthest);

                DouglasPeuckerReduction(points, firstPoint,
                indexFarthest, tolerance, ref pointIndexsToKeep);
                DouglasPeuckerReduction(points, indexFarthest,
                lastPoint, tolerance, ref pointIndexsToKeep);
            }
        }

        /// <summary>
        /// The distance of a point from a line made from point1 and point2.
        /// </summary>
        private Double PerpendicularDistance
            (Point Point1, Point Point2, Point Point)
        {
            Double area = Math.Abs(.5 * (Point1.X * Point2.Y + Point2.X *
            Point.Y + Point.X * Point1.Y - Point2.X * Point1.Y - Point.X *
            Point2.Y - Point1.X * Point.Y));
            Double bottom = Math.Sqrt(Math.Pow(Point1.X - Point2.X, 2) +
            Math.Pow(Point1.Y - Point2.Y, 2));
            Double height = area / bottom * 2;

            return height;
        }

        /// <summary>
        /// Find longest route through edges.
        /// </summary>
        private int[] LongestRoute(
            int n,
            Tuple<int, int>[] edges,
            HashSet<int> visited = null,
            int index = -1
            )
        {
            if (index < 0) // Level 0.
            {
                HashSet<int> longest = null;
                // Initialize visited.
                for (index = 0; index < n; index++)
                {
                    visited = new HashSet<int>(); // New path.
                    LongestRoute(n, edges, visited, index);
                    if (longest == null) longest = new HashSet<int>(visited);
                    else if (longest.Count < visited.Count) longest = new HashSet<int>(visited);
                }
                return longest.ToArray();
            }
            else
            {
                // We've been here before.
                if (!visited.Contains(index))
                {
                    visited.Add(index);
                    // Visit all edges.
                    for (int i = 0; i < edges.Length; i++)
                        if (edges[i].Item1 == index)
                            LongestRoute(n, edges, visited, edges[i].Item2);
                        else if (edges[i].Item2 == index)
                            LongestRoute(n, edges, visited, edges[i].Item1);

                    // Now build path to all pixels not in path.
                    for (int i = 0; i < n; i++)
                        if (!visited.Contains(i))
                        { // Try this pixel.

                        }
                }
                return null;
            }
        }

        private Pixel[] ShortestPath(Pixel p1, Pixel p2)
        {
            List<Pixel> result = new List<Pixel>();
            Pixel pixel = new Pixel() { X = p1.X, Y = p1.Y };
            while (!pixel.Match(p2.X, p2.Y))
            {
                // Move to the right direction.
                int dx = 0, dy = 0;
                if (pixel.X < p2.X) dx = 1; else if (pixel.X > p2.X) dx = -1;
                if (pixel.Y < p2.Y) dy = 1; else if (pixel.Y > p2.Y) dy = -1;
                pixel = new Pixel() { X = p1.X + dx, Y = p1.Y + dy, Move = true };
                if (!pixel.Match(p2.X, p2.Y)) result.Add(pixel);
            }
            return result.ToArray();
        }

        public HashSet<Point> GetBoundary()
        {
            HashSet<Point> points = new HashSet<Point>();
            for (int j = 0; j < _bmp.Height; j++)
                for (int i = 0; i < _bmp.Width; i++)
                    if (!IsOutlinePoint(i, j))               // Found first white pixel
                        foreach (Point p in SquareTrace(new Point(i, j)))
                            points.Add(p);
            return points;
        }

        private bool IsOutlinePoint(int x, int y)
        {
            if (x < 0 || y < 0 || x >= _bmp.Width || y >= _bmp.Height) return false; // Out of image.
            var val = _bmp.GetPixel(x, y);
            float fval = val.R + val.G + val.B;
            fval /= 3;
            if (fval < 128) return false;
            else return true;
        }

        public HashSet<Point> SquareTrace(Point start)
        {
            HashSet<Point> boundaryPoints = new HashSet<Point>();  // Use a HashSet to prevent double occurrences
                                                                   // We found at least one pixel
            boundaryPoints.Add(start);

            // The first pixel you encounter is a white one by definition, so we go left. 
            // Our initial direction was going from left to right, hence (1, 0)
            Point nextStep = GoLeft(new Point(1, 0));
            Point next = start.Add(nextStep);
            while (next != start)
            {
                // We found a black cell, so we go right and don't add this cell to our HashSet
                if (IsOutlinePoint(next.X, next.Y))
                {
                    next = next.Sub(nextStep);
                    nextStep = GoRight(nextStep);
                    next = next.Add(nextStep);
                }
                // Alternatively we found a white cell, we do add this to our HashSet
                else
                {
                    boundaryPoints.Add(next);
                    nextStep = GoLeft(nextStep);
                    next = next.Add(nextStep);
                }
            }

            return boundaryPoints;
        }

        private Point GoLeft(Point p) => new Point(p.Y, -p.X);
        
        private Point GoRight(Point p) => new Point(-p.Y, p.X);

        static IList<IList<T>> Permute<T>(T[] items)
        {
            var list = new List<IList<T>>();
            return DoPermute(items, 0, items.Length - 1, list);
        }

        static IList<IList<T>> DoPermute<T>(T[] items, int start, int end, IList<IList<T>> list)
        {
            if (start == end)
            {
                // We have one of our possible n! solutions,
                // add it to the list.
                list.Add(new List<T>(items));
            }
            else
            {
                for (var i = start; i <= end; i++)
                {
                    Swap(ref items[start], ref items[i]);
                    DoPermute(items, start + 1, end, list);
                    Swap(ref items[start], ref items[i]);
                }
            }

            return list;
        }

        static void Swap<T>(ref T a, ref T b)
        {
            var temp = a;
            a = b;
            b = temp;
        }
        #endregion // Helper(s)
    }
}
