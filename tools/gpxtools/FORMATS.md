# XYZ Glpyhs

01234567890123456789012345678901234567890123456789012345678901234567890123456789

## Glyph header

The XYZ glyph file starts with a header. 

### Glyph generation

? ? ? ? M M T T T

The first byte of the header is the glyph generation. Bits 2-0 specify glyph 
type as follows:
 * 000 image 
 * 001 mouse cursor
 * 010 font (multiple glyphs)
 * 011 animation (multiple glyphs)
 * 100 reserved
 * 101 reserved
 * 110 reserved
 * 111 reserved

Bits 4-3 tell the method that should be used to draw the glyph and can be
 * 00 tiny vector
 * 01 lines
 * 10 raster
 * 11 reserved

Bits 7-4 are specific to glyph type and have a different meaning for
a font and for a bitmap.

Glyph header is followed by the header extension, which depends on glyph type.