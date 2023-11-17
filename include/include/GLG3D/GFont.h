/**
 @file GFont.h
 
 @maintainer Morgan McGuire, morgan@graphics3d.com

 @created 2002-11-02
 @edited  2006-02-10
 */

#ifndef G3D_GFONT_H
#define G3D_GFONT_H

#include "GLG3D/Texture.h"
#include "G3D/BinaryInput.h"
#include "G3D/CoordinateFrame.h"
#include <string>

namespace G3D {

typedef ReferenceCountedPointer<class GFont> GFontRef;

/**
 Font class for use with RenderDevice.  Renders variable size and color 
 fonts from high-resolution bitmaps.

 Font rendering is (inherently) slow-- you can achieve better performance
 for static text by creating bitmap textures with whole words and 
 sentences on them.
 <P>

 The following fonts are provided with G3D in the
 <CODE>data/font</CODE> directory.  See the <CODE>copyright.txt</CODE>
 file in that directory for information about the source of these
 files and rules for distribution.  <P> <IMG SRC="font.png">
 */
class GFont : public ReferenceCountedObject {
public:
    /** Constant for draw2D.  Specifies the horizontal alignment
	of an entire string relative to the supplied x,y position */
    enum XAlign {XALIGN_RIGHT, XALIGN_LEFT, XALIGN_CENTER};

    /** Constant for draw2D.  Specifies the vertical alignment
	of the characters relative to the supplied x,y position.
      */
    enum YAlign {YALIGN_TOP, YALIGN_BASELINE, YALIGN_CENTER, YALIGN_BOTTOM};

    /** Constant for draw2D.  Proportional width (default)
        spaces characters based on their size.  Fixed spacing gives
        uniform spacing regardless of character width. */
    enum Spacing {PROPORTIONAL_SPACING, FIXED_SPACING};

private:

    /** The actual width of the character. */ 
    int subWidth[128];

    /** The width of the box, in texels, around the character. */
    int charWidth;
    int charHeight;

    /** Y distance from top of the bounding box to the font baseline. */
    int baseline;

    TextureRef texture;

    /** Assumes you are already inside of beginPrimitive(QUADS) */
    Vector2 drawString(
        class RenderDevice*     renderDevice,
        const std::string&      s,
        double                  x,
        double                  y,
        double                  w,
        double                  h,
        Spacing                 spacing) const;

    /** Packs vertices for rendering the string
        into the array as tex/vertex, tex/vertex, ...
     */
    Vector2 computePackedArray(
        const std::string&  s,
        double              x,
        double              y,
        double              w,
        double              h,
        Spacing             spacing,
        Vector2*            array) const;

    class RenderDevice*             renderDevice;

    GFont(class RenderDevice* renderDevice, const std::string& filename, BinaryInput& b);

public:

    /** @deprecated */
    static GFontRef G3D_DEPRECATED
        fromFile(class RenderDevice* renderDevice, const std::string& filename);

    /** The filename must be a FNT (proportional width font) file.
        <P> If a font file is not found, an assertion will fail, an
        exception will be thrown, and texelSize() will return (0, 0).
        <P> Several fonts in this format at varying resolutions are
        available in the data/font directory.  The large fonts require
        500k of memory when loaded and look good when rendering
        characters up to about 64 pixels high. The small fonts require
        130k and look good up to about 32 pixels.

        See GFont::convertTGAtoPWF for creating new fonts in the FNT format:
    
      <P>
       This file is compressed by BinaryOutput::compress().  The contents 
       after decompression  have the following format (little endian): 
          <pre>
           int32                       Version number (must be 1)
           128 * int16                 Character widths, in texels
           uint16                      Baseline from top of box, in texels
           uint16                      Texture width (texture height is always 1/2 texture width)
           (pow(width, 2) / 2) * int8  Texture data
          </pre>
        The width of a character's bounding box is always width / 16.  The height is always width / 8.
    */
    static GFontRef fromFile(const std::string& filename);

    /** see GFont::fromFile */
    static GFontRef fromMemory(const std::string& name, const uint8* bytes, const int size);

    /**
     Converts an 8-bit RAW font texture and INI file as produced by
     the Bitmap Font Builder program to a graphics3d PWF font.  inFile
     should have no extension-- .raw and .ini will be appended to it.
     outfile should end with ".FNT" or be "" for the default.  <P>

      The Bitmap Font Builder program can be downloaded from http://www.lmnopc.com/bitmapfontbuilder/

      Use the full ASCII character set; the conversion will strip
	  infrequently used characters automatically. Write out RAW files
	  with characters CENTER aligned and right side up using this
	  program.  Then, also write out an INI file; this contains the
	  width of each character in the font.  Example: 

      <PRE>
	  GFont::convertRAWINItoPWF("c:/tmp/g3dfont/news",
	                            "d:/graphics3d/book/cpp/data/font/news.fnt"); 
      </PRE> 

      @param infileBase The name of the raw/ini files @param outfile Defaults
	  to infileBase + ".fnt"
     */
    static void convertRAWINItoPWF
    (const std::string& infileBase, 
     std::string outfile = "");


    /** Returns the natural character width and height of this font. */
    Vector2 texelSize() const;

    /**
     Draws a proportional width font string.  Assumes device->push2D()
     has been called.  Leaves all rendering state as it was, except for the
     texture coordinate on unit 0.

     @param size The distance between successive lines of text.  Specify
     texelSize().y / 1.5 to get 1:1 texel to pixel

     @param outline If this color has a non-zero alpha, a 1 pixel border of
     this color is drawn about the text.

     @param spacing Fixed width fonts are spaced based on the width of the 'M' character.

     @return Returns the x and y bounds (ala get2DStringBounds) of the printed string.

     You can draw rotated text by setting the RenderDevice object to world matrix
     manually.  The following example renders the word "ANGLE" on a 45-degree angle
     at (100, 100).
     <PRE>
    app->renderDevice->push2D();
        CoordinateFrame cframe(
            Matrix3::fromAxisAngle(Vector3::unitZ(), toRadians(45)),
            Vector3(100, 100, 0));
        app->renderDevice->setObjectToWorldMatrix(cframe);
        app->debugFont->draw2D("ANGLE", Vector2(0, 0), 20);
    app->renderDevice->pop2D();
     </PRE>
     */
    Vector2 draw2D(
        RenderDevice*       renderDevice,
        const std::string&  s,
        const Vector2&      pos2D,
        double              size    = 12,
        const Color4&       color   = Color3::black(),
        const Color4&       outline = Color4::clear(),
        XAlign              xalign  = XALIGN_LEFT,
        YAlign              yalign  = YALIGN_TOP,
        Spacing             spacing = PROPORTIONAL_SPACING) const;

    /** @deprecated  Use the version that accepts a RenderDevice as the 1st argument.
     */
    Vector2 G3D_DEPRECATED draw2D(
        const std::string&  s,
        const Vector2&      pos2D,
        double              size    = 12,
        const Color4&       color   = Color3::black(),
        const Color4&       outline = Color4::clear(),
        XAlign              xalign  = XALIGN_LEFT,
        YAlign              yalign  = YALIGN_TOP,
        Spacing             spacing = PROPORTIONAL_SPACING) const {
        return draw2D(renderDevice, s, pos2D, size, color, outline, xalign, yalign, spacing);
    }

    /**
     Text is visible from behind.  The text is oriented so that it
     reads "forward" when the pos3D z-axis points towards the viewer.

     Note that text, like all transparent objects, should be rendered
     in back to front sorted order to achieve proper alpha blending.

     @param size In meters.
     */
    Vector2 draw3D(
        RenderDevice*               renderDevice,
        const std::string&          s,
        const CoordinateFrame&      pos3D,
        double              size    = .1,
        const Color4&       color   = Color3::black(),
        const Color4&       outline = Color4::clear(),
        XAlign              xalign  = XALIGN_LEFT,
        YAlign              yalign  = YALIGN_TOP,
        Spacing             spacing = PROPORTIONAL_SPACING) const;

    /** @deprecated Use the version that accepts a RenderDevice as the 1st argument.*/
    Vector2 G3D_DEPRECATED draw3D(
        const std::string&          s,
        const CoordinateFrame&      pos3D,
        double              size    = .1,
        const Color4&       color   = Color3::black(),
        const Color4&       outline = Color4::clear(),
        XAlign              xalign  = XALIGN_LEFT,
        YAlign              yalign  = YALIGN_TOP,
        Spacing             spacing = PROPORTIONAL_SPACING) const {
        return draw3D(renderDevice, s, pos3D, size, color, outline, xalign, yalign, spacing);
    }

    /**
     Useful for drawing centered text and boxes around text.
     */
    Vector2 get2DStringBounds(
        const std::string&  s,
        double              size = 12,
        Spacing             spacing = PROPORTIONAL_SPACING) const;
};

}
#endif
