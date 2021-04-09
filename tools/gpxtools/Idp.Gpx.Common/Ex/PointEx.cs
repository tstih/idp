/*
 * PointEx.cs
 * 
 * Useful point extensions.
 * 
 * MIT License (see: LICENSE)
 * Copyright (c) 2021 Tomaz Stih
 * 
 * 09.04.2021   tstih
 * 
 */
using System.Drawing;

namespace Idp.Gpx.Common.Ex
{

    public static class PointEx
    {
        /// <summary>
        /// Add two points.
        /// </summary>
        public static Point Add(this Point p1, Point p2)
        {
            return new Point(p1.X + p2.X, p1.Y + p2.Y);
        }

        /// <summary>
        /// Subtract two points.
        /// </summary>
        public static Point Sub(this Point p1, Point p2)
        {
            return new Point(p1.X - p2.X, p1.Y - p2.Y);
        }

    }
}
