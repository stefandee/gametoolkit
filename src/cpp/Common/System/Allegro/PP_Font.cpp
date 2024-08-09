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
#include <string.h>
#pragma hdrstop

#include "basepath.h"
#include "PP_Font.h"
#include "PP_Surface.h"

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
   mRenderSurface = new CPGISurface();
}

/*---------------------------------------------------------------------------
 description: destructor
---------------------------------------------------------------------------*/
CPFont::~CPFont()
{
  mFont = 0;
}

/*---------------------------------------------------------------------------
 description: font size
---------------------------------------------------------------------------*/
void CPFont:: SetSize(int value)
{
   if (value != mSize)
   {
      mSize = value;
      CreateFontHandle();
   }
}

int CPFont::GetTextWidth(CPString _text)
{
  return 0;
}

int CPFont::GetTextHeight(CPString _text)
{
  return 0;
}

/*---------------------------------------------------------------------------
 description: paint the text
 parameters : X, Y - the coord on the screen
              text - the text to be painted
---------------------------------------------------------------------------*/
void CPFont::PaintText(CPGISurface* _dest, int X, int Y, CPString text)
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
}

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif
