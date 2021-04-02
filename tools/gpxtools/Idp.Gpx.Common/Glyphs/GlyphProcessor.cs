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

namespace Idp.Gpx.Common.Glyphs
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
        #endregion // Ctor

        #region Method(s)
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
                    if (!IsPixel(x,y,color))
                        pixels.Add(new Pixel() { X = x, Y = y, C=color });

            // And return it.
            return pixels.ToArray();
        }

        /// <summary>
        /// Return all connections between pixels in the array.
        /// A connection exists if pixels touch each other (the junction
        /// can be vertical, horizontal or diagonal).
        /// </summary>
        public Tuple<int,int>[] Edges(Pixel[] pixels)
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
        public bool IsPixel(int x, int y, Color c) { return _bmp.GetPixel(x, y) == c; }

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
        #endregion // Method(s)

        #region Helper(s)
        private int[] Route(
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
                    Route(n, edges, visited, index);
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
                            Route(n, edges, visited, edges[i].Item2);
                        else if (edges[i].Item2 == index)
                            Route(n, edges, visited, edges[i].Item1);

                    // Now build path to all pixels not in path.
                    for(int i=0;i<n;i++)
                        if (!visited.Contains(i))
                        { // Try this pixel.

                        }
                    // Pixel[] shadowPixels = AddPath(p1,;
                    // HashSet<int> shadowVisited = new HashSet<int>(visited);
                }
                return null;
            }
        }

        private Pixel[] AddPath(Pixel p1, Pixel p2)
        {
            List<Pixel> result = new List<Pixel>();
            Pixel pixel = new Pixel() { X = p1.X, Y = p1.Y };
            while (!pixel.Match(p2.X, p2.Y)) ;
            {
                // Move to the right direction.
                int dx = 0, dy = 0;
                if (pixel.X < p2.X) dx = 1; else if (pixel.X > p2.X) dx = -1;
                if (pixel.Y < p2.Y) dy = 1; else if (pixel.Y > p2.Y) dy = -1;
                pixel = new Pixel() { X = p1.X + dx, Y = p1.Y + dy, Move = true };
                if (!pixel.Match(p2.X,p2.Y)) result.Add(pixel);
            } 
            return result.ToArray();
        }
        #endregion // Helper(s)
    }
}
