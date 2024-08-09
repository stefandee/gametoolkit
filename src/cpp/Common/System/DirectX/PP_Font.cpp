//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    // 
// Copyright (C) 1998, 1999, 2000, 2001 Stefan Dicu & Tudor Girba            //
//                                                                           //
// PPTactical Engine is free software; you can redistribute it               //
// and/or modify it under the terms of the GNU Lesser General Public License as     // 
// published by the Free Software Foundation; either version 2.1 of the        //
// License, or (at your option) any later version.                           //
//                                                                           //  
// PPTactical Engine is distributed in the hope that it will be useful, but  //
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY// 
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License  //
// for more details.                                                         //
//                                                                           // 
// You should have received a copy of the GNU Lesser General Public License         //
// along with PPTactical Engine; if not, write to the Free Software          //
// Foundation Inc. 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   // 
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------
 application: Pure Power

 description: Class Piron Font
 last modify: 06 01 01
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
//#include <vcl.h>
#pragma hdrstop

#include "PP_Font.h"

/*---------------------------------------------------------------------------
 description: constructor
 parameters : fontName - the name of the font
---------------------------------------------------------------------------*/
CPFont::CPFont(CPString fontName)
:  mName(fontName),
   mSize(7),
   mColor(0xffffff),
   mItalic(false),
   mStrikeout(false),
   mUnderline(false),
   mBold(false),
   mFont(0)
{
   CreateFontHandle();
}

/*---------------------------------------------------------------------------
 description: destructor
---------------------------------------------------------------------------*/
CPFont::~CPFont()
{
   DeleteObject(mFont);
}

/*---------------------------------------------------------------------------
 description: paint the text
 parameters : X, Y - the coord on the screen
              text - the text to be painted
---------------------------------------------------------------------------*/
void CPFont::PaintText(int X, int Y, CPString text)
{
}

/*---------------------------------------------------------------------------
 description: paints the text and clips it if it's necesary
 parameters : clippingRect - the clipping rectangle in screen
              text - the text to be painted

---------------------------------------------------------------------------*/
void CPFont::PaintTextClipped(CPRect clippingRect, CPString text)
{
}

/*---------------------------------------------------------------------------
 PROTECTED SECTION
---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
 description: creates the font with the attributes specified in the fields
---------------------------------------------------------------------------*/
void CPFont::CreateFontHandle()
{
   if (mFont)
   {
      DeleteObject(mFont);
   }

   mFont = CreateFont(
                    -mSize,                // logical height of font
                    0,                   // logical average character width
                    0,                       // angle of escapement
                    0,                       // base-line orientation angle
                    mBold*700,                 // font weight
                    mItalic,                 // italic attribute flag
                    mUnderline,              // underline attribute flag
                    mStrikeout,              // strikeout attribute flag
                    DEFAULT_CHARSET,         // character set identifier
                    OUT_DEFAULT_PRECIS,      // output precision
                    CLIP_DEFAULT_PRECIS,     // clipping precision
                    DEFAULT_QUALITY,         // output quality
                    DEFAULT_PITCH,           // pitch and family
                    mName                // pointer to typeface name string
                    );
}

/*---------------------------------------------------------------------------
 description: this should be a temporary method and returns a HFONT
---------------------------------------------------------------------------*/
HFONT CPFont::GetFontHandle()
{
   return mFont;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: converts the internal alignment flags into system font alignment
              flags
---------------------------------------------------------------------------*/
int CPFont::TransformTextFlags(int _flags)
{
  int lFlags = 0;

  if (_flags & PPDT_RIGHT)
  {
    lFlags |= DT_RIGHT;
  }

  if (_flags & PPDT_LEFT)
  {
    lFlags |= DT_LEFT;
  }
  
  if (_flags & PPDT_VCENTER)
  {
    lFlags |= DT_VCENTER;
  }

  if (_flags & PPDT_TOP)
  {
    lFlags |= DT_TOP;
  }

  if (_flags & PPDT_SINGLELINE)
  {
    lFlags |= DT_SINGLELINE;
  }

  if (_flags & PPDT_CENTER)
  {
    lFlags |= DT_CENTER;
  }

  return lFlags;
}
//---------------------------------------------------------------------------

int CPFont::GetTextWidth(CPString _text)
{
  SIZE lSize;

  HDC lHdc;

  SelectObject(lHdc, mFont);

  //GetTextExtentPoint32(mFont, _text.c_str(), _text.Length(), &lSize);
  GetTextExtentPoint32(lHdc, _text.c_str(), _text.Length(), &lSize);

  return lSize.cx;
}
//---------------------------------------------------------------------------

int CPFont::GetTextHeight(CPString _text)
{
  SIZE lSize;

  HDC lHdc;

  SelectObject(lHdc, mFont);

  //GetTextExtentPoint32(mFont, _text.c_str(), _text.Length(), &lSize);
  GetTextExtentPoint32(lHdc, _text.c_str(), _text.Length(), &lSize);

  return lSize.cy;
}
//---------------------------------------------------------------------------


#ifdef __BORLANDC__
#pragma package(smart_init)
#endif
