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
#include <SDL_ttf.h>
#include <SDL.h>
#include "Stream_PhysFS.h"
#pragma hdrstop

#include "basepath.h"
#include "PP_Font.h"
#include "PP_Texture.h"
#include "PP_Graphic.h"
#include "easylogging++.h"
#include "PP_FontCache.h"
#include "ColorConv.h"

/*---------------------------------------------------------------------------
 description: constructor
 parameters : fontName - the name of the font
---------------------------------------------------------------------------*/
CPFont::CPFont(CPString fontName)
    :  mName(fontName),
       mSize(7),
       mHighQuality(false),
       mColor(0xffffff),
       mItalic(false),
       mStrikeout(false),
       mUnderline(false),
       mBold(false),
       mFont(0),
       mInvalidate(true),
       mPrevText("")
{
    CreateFontHandle();
    mRenderTexture = new CPGITexture();
}

CPFont::CPFont(CPString fontName, int fontSize, bool highQuality)
    : mName(fontName),
      mSize(fontSize),
      mHighQuality(highQuality),
      mColor(0xffffff),
      mItalic(false),
      mStrikeout(false),
      mUnderline(false),
      mBold(false),
      mFont(0),
      mInvalidate(true),
      mPrevText("")
{
    CreateFontHandle();
    mRenderTexture = new CPGITexture();
}
/*---------------------------------------------------------------------------
 description: destructor
---------------------------------------------------------------------------*/
CPFont::~CPFont()
{
    delete mRenderTexture;

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
    int lW, lH;

    TTF_SizeText(mFont, _text.c_str(), &lW, &lH);

    return lW;
}

int CPFont::GetTextHeight(CPString _text)
{
    int lW, lH;

    TTF_SizeText(mFont, _text.c_str(), &lW, &lH);

    return lH;
}

/*---------------------------------------------------------------------------
 description: paint the text
 parameters : X, Y - the coord on the screen
              text - the text to be painted
---------------------------------------------------------------------------*/
void CPFont::PaintText(int X, int Y, CPString text)
{
    int lRenderStyle = TTF_STYLE_NORMAL;

    if (mItalic)
    {
        lRenderStyle |= TTF_STYLE_ITALIC;
    }

    if (mBold)
    {
        lRenderStyle |= TTF_STYLE_BOLD;
    }

    if (mUnderline)
    {
        lRenderStyle |= TTF_STYLE_UNDERLINE;
    }

    // since mFont is shared between CPFont instances and TTF_SetFontStyle apparently resets SDL_ttf internal glyph cache,
    // we should also add the render style to the cache keys for further optimization
    // but font styles are not yet used in the engine, so until then...
    if (TTF_GetFontStyle(mFont) != lRenderStyle)
    {
        TTF_SetFontStyle(mFont, lRenderStyle);
    }

    if (mInvalidate || mPrevText != text)
    {
        // this code has the biggest impact on performance: creating and deleting surfaces/textures
        if (mRenderTexture->impl)
        {
            SDL_DestroyTexture(mRenderTexture->impl);
        }

        SDL_Color lColor = {static_cast<Uint8>(gfGetRValue(mColor)), static_cast<Uint8>(gfGetGValue(mColor)), static_cast<Uint8>(gfGetBValue(mColor))};

        SDL_Surface* surface = mHighQuality ? TTF_RenderText_Blended(mFont, text.c_str(), lColor) : TTF_RenderText_Solid(mFont, text.c_str(), lColor);

        mRenderTexture->impl = SDL_CreateTextureFromSurface(GetGraphicInstance()->mRenderer, surface);

        mInvalidate = false;
        mPrevText = text;
    }

    if (mRenderTexture->impl)
    {
        GetGraphicInstance()->PaintTexture(X, Y, mRenderTexture);
    }
}

/*---------------------------------------------------------------------------
 description: paints the text and clips it if it's necesary
 parameters : clippingRect - the clipping rectangle in screen
              text - the text to be painted

---------------------------------------------------------------------------*/
void CPFont::PaintTextWrapped(int X, int Y, CPString text, int width)
{
    int lRenderStyle = TTF_STYLE_NORMAL;

    if (mItalic)
    {
        lRenderStyle |= TTF_STYLE_ITALIC;
    }

    if (mBold)
    {
        lRenderStyle |= TTF_STYLE_BOLD;
    }

    if (mUnderline)
    {
        lRenderStyle |= TTF_STYLE_UNDERLINE;
    }

    // since mFont is shared between CPFont instances and TTF_SetFontStyle apparently resets SDL_ttf internal glyph cache,
    // we should also add the render style to the cache keys for further optimization
    // but font styles are not yet used in the engine, so until then...
    if (TTF_GetFontStyle(mFont) != lRenderStyle)
    {
        TTF_SetFontStyle(mFont, lRenderStyle);
    }

    if (mInvalidate || mPrevText != text)
    {
        // this code has the biggest impact on performance: creating and deleting surfaces/textures
        if (mRenderTexture->impl)
        {
            SDL_DestroyTexture(mRenderTexture->impl);
        }

        SDL_Color lColor = {static_cast<Uint8>(gfGetRValue(mColor)), static_cast<Uint8>(gfGetGValue(mColor)), static_cast<Uint8>(gfGetBValue(mColor))};

        SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(mFont, text.c_str(), lColor, width);

        mRenderTexture->impl = SDL_CreateTextureFromSurface(GetGraphicInstance()->mRenderer, surface);

        mInvalidate = false;
        mPrevText = text;
    }

    if (mRenderTexture->impl)
    {
        GetGraphicInstance()->PaintTexture(X, Y, mRenderTexture);
    }
}

/*---------------------------------------------------------------------------
 PROTECTED SECTION
---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
 description: creates the font with the attributes specified in the fields
---------------------------------------------------------------------------*/
void CPFont::CreateFontHandle()
{
    mFont = CPFontCache::Instance()->GetFont(mName.c_str(), mSize);
    mInvalidate = true;
}

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif
