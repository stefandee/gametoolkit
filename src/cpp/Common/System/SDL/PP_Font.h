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
#ifndef PP_FontH
#define PP_FontH

//#include <windows.h>
#include "PP_String.h"
//#include "PP_GuiDef.h"
#include "basic2d.h"
#include "PP_basetypes.h"
#include <SDL_ttf.h>
//#include "PP_Surface.h"

// text alignments
const int PPDT_RIGHT      = 1;
const int PPDT_VCENTER    = 2;
const int PPDT_SINGLELINE = 4;
const int PPDT_LEFT       = 8;
const int PPDT_CENTER     = 16;
const int PPDT_TOP        = 32;
const int PPDT_BOTTOM     = 64;

//class CPGIGraphicSystem;
class CPGITexture;

class CPFont
{
    //friend class CPGIGraphicSystem;

public:
    CPFont(CPString fontName);
    CPFont(CPString fontName, int fontSize, bool highQuality = true);
    virtual ~CPFont();

    virtual void PaintText(int X, int Y, CPString text);
    virtual void PaintTextWrapped(int X, int Y, CPString text, int width);

public: //get-sets
    virtual void SetName(CPString value);
    virtual CPString GetName();

    virtual void SetSize(int value);
    virtual int GetSize();

    virtual void SetHeight(int value);
    virtual int GetHeight();

    virtual void SetColor(TColor32 value);
    virtual TColor32 GetColor();

    virtual void SetItalic(bool value);
    virtual bool GetItalic();

    virtual void SetStrikeout(bool value);
    virtual bool GetStrikeout();

    virtual void SetUnderline(bool value);
    virtual bool GetUnderline();

    virtual void SetBold(bool value);
    virtual bool GetBold();

    virtual void SetHighQuality(bool value);
    virtual bool GetHighQuality();

public: // ops
    int GetTextWidth(CPString _text);
    int GetTextHeight(CPString _text);

protected:
    virtual void CreateFontHandle();
    //virtual HFONT GetFontHandle();

private:
    CPString       mName;
    int            mSize;
    int            mHeight;
    TColor32       mColor;

    bool           mItalic;
    bool           mStrikeout;
    bool           mUnderline;
    bool           mBold;
    bool           mInvalidate;
    bool           mHighQuality;

    //HFONT          mFont;
    TTF_Font*      mFont;
    CPGITexture*   mRenderTexture;
    CPString        mPrevText;
};


/*---------------------------------------------------------------------------
   GET-SET OPERATIONS
---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
 description: font name
---------------------------------------------------------------------------*/
inline void CPFont::SetName(CPString value)
{
    if (value != mName)
    {
        mName = value;
        CreateFontHandle();
    }
}

inline CPString CPFont::GetName()
{
    return mName;
}

inline int CPFont::GetSize()
{
    return mSize;
}

/*---------------------------------------------------------------------------
 description: font height TEMPORARY OUT OF ORDER
---------------------------------------------------------------------------*/
inline void CPFont::SetHeight(int value)
{
    SetSize(value);
    /*   if (value != mHeight)
       {
          mHeight = value;
          CreateFontHandle();
       }
    */
}

inline int CPFont::GetHeight()
{
    return GetSize();
//   return mHeight;
}

/*---------------------------------------------------------------------------
 description: font color
---------------------------------------------------------------------------*/
inline void CPFont::SetColor(TColor32 value)
{
    if (mColor != value)
    {
        mInvalidate = true;
    }

    mColor = value;


}

inline TColor32 CPFont::GetColor()
{
    return mColor;
}

/*---------------------------------------------------------------------------
 description: italic
---------------------------------------------------------------------------*/
inline void CPFont::SetItalic(bool value)
{
    if (value != mItalic)
    {
        mItalic = value;
        //CreateFontHandle();
    }
    mItalic = value;
}

inline bool CPFont::GetItalic()
{
    return mItalic;
}

/*---------------------------------------------------------------------------
 description: strikeout
---------------------------------------------------------------------------*/
inline void CPFont::SetStrikeout(bool value)
{
    if (value != mStrikeout)
    {
        mStrikeout = value;
        //CreateFontHandle();
    }
}

inline bool CPFont::GetStrikeout()
{
    return mStrikeout;
}

/*---------------------------------------------------------------------------
 description: underline
---------------------------------------------------------------------------*/
inline void CPFont::SetUnderline(bool value)
{
    if (value != mUnderline)
    {
        mUnderline = value;
        //CreateFontHandle();
    }
}
inline bool CPFont::GetUnderline()
{
    return mUnderline;
}

/*---------------------------------------------------------------------------
 description: bold
---------------------------------------------------------------------------*/
inline void CPFont::SetBold(bool value)
{
    if (value != mBold)
    {
        mBold = value;
        //CreateFontHandle();
    }
}

inline bool CPFont::GetBold()
{
    return mBold;
}

inline void CPFont::SetHighQuality(bool value)
{
    if (mHighQuality != value)
    {
        mHighQuality = value;
        mInvalidate = true;
    }
}

inline bool CPFont::GetHighQuality()
{
    return mHighQuality;
}

//---------------------------------------------------------------------------
#endif
