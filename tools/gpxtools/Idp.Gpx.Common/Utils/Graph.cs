/*
 * Graph.cs
 * 
 * Graph theory helpers.
 *  
 * MIT License (see: LICENSE)
 * Copyright (c) 2021 Tomaz Stih
 * 
 * 20.04.2021   tstih
 * 
 */
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;

namespace Idp.Gpx.Common.Utils
{
    public class Graph
    {
        #region Private(s)
        private Pixel[] _route;
        private byte _threshold;
        #endregion // Private(s)

        #region Ctor
        public Graph(Pixel[] pixels, int w, int h, byte threshold = 32)
        {
            _route = pixels;
            _threshold = threshold;
        }
        #endregion // Ctor

        #region Method(s)
        public Pixel[] TSP2Opt()
        {
            int cost = TotalCost(_route);
            int swappable = _route.Length - 1;
        restart:
            for (int i = 0; i < swappable - 1; i++) {
                for (int k = i + 1; k < swappable; k++) {
                    Pixel[] new_route = TwoOptSwap(_route, i, k);
                    int new_cost = TotalCost(new_route);
                    if (new_cost < cost)
                    {
                        _route = new_route;
                        cost = new_cost;
                        goto restart;
                    }
                }
            }
            return _route;
        }
        #endregion // Method(s)

        #region Helper(s)
        private Pixel[] TwoOptSwap(Pixel[] route, int i, int k)
        {
            List<Pixel> result = new List<Pixel>();
            for (int n = 0; n <= i - 1; n++)
                result.Add(route[n]);
            int dec = 0;
            for (int n = i; n <= k; ++n)
            {
                result.Add(route[k - dec]);
                dec++;
            }
            for (int n = k + 1; n < route.Length; n++)
                result.Add(route[n]);

            return result.ToArray();
        }
        private bool SameColor(Pixel p1, Pixel p2)
        { 
            double distance = Math.Sqrt(Math.Pow(p1.C.R - p2.C.R, 2) + Math.Pow(p1.C.G - p2.C.G, 2) + Math.Pow(p1.C.B - p2.C.B, 2));
            return distance < _threshold;
        }
        private byte Cost(Pixel p1, Pixel p2, ref int dx, ref int dy)
        {
            dx = p1.X - p2.X;
            dy = p1.Y - p2.Y;
            int dist= Math.Max(Math.Abs(dx), Math.Abs(dy));
            if (dist > 1)
                dist += 2; // Pen up and pen down operation cost.
            else if (!SameColor(p1, p2))
                dist += 1; // Change color operation cost.
            return (byte)dist;
        }
        private int TotalCost(Pixel[] route)
        {
            int sum = 0, dx=0, dy=0, prevdx=0, prevdy=0;
            int linelen = 0;
            for (int i = 1; i < route.Length; i++)
            {
                // Get the cost.
                byte cost=(Cost(route[i - 1], route[i], ref dx, ref dy));
                if (cost == 1 && dx == prevdx && dy == prevdy && linelen < 4)
                {
                    // Special case, dx=prev dx and dy=prev dy (we've got a line!)
                    linelen++;
                    cost = 0; // It's a freebie!
                }
                else linelen = 0;
                sum += cost;
                prevdx = dx; prevdy = dy;
            }
            return sum;
        }
        #endregion // Helper(s)
    }
}
