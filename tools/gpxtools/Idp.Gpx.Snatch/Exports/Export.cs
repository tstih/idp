/*
 * Export.cs
 * 
 * Base class for exporting results of the snatch operation.
 * Derive your own export from this class.
 *  
 * MIT License (see: LICENSE)
 * Copyright (c) 2021 Tomaz Stih
 * 
 * 02.04.2021   tstih
 * 
 */
using Idp.Gpx.Snatch.Commands;

namespace Idp.Gpx.Snatch.Exports
{
    public class Export
    {
        public virtual RetCode Begin(ArrayCmd cmd) { return 0; }
        public virtual RetCode OnGlyph(ArrayCmd cmd) { return 0; }
        public virtual RetCode End(ArrayCmd cmd) { return 0; }
    }
}