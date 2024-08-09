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

 description: Class Piron Component Interface - Label
 last modify: 21 04 00
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#ifndef PP_CLabelH
#define PP_CLabelH

#include "PP_CControl.h"
#include "PP_GuiDef.h"
#include "PP_Font.h"

class CPILabel: public CPIControl
{
   public:
      CPILabel(CPIWinControl *Parent = NULL);
      virtual ~CPILabel();

      CPString ClassName() {return PICN_LABEL;};
      virtual bool IsDerivedFrom(CPString ClassName);

      void Update();
      void Paint();
      CPRect GetClipRegion();

   public: // get-set
      virtual void SetWidth (int value);
      virtual void SetHeight (int value);
      virtual void SetText (CPString value);

      void SetTextAllignment(int value);

   public:
      void SetAutoSize(bool value);
      bool GetAutoSize();

      void SetMultiline(bool value);
      bool GetMultiline();

      CPFont *GetFont();
      void  SetFont(CPFont *value);

      virtual void SetColor(TColor32 _color);

   private:
      bool        mAutoSize;
      bool        mMultiline;
      CPFont     *mFont;
      int         mTextAllignment;
};

/////////////////////////////////////////////////////////////////////////////
//GET-SET (INLINE) SECTION
/////////////////////////////////////////////////////////////////////////////
inline void CPILabel::SetAutoSize(bool value)
{
   mAutoSize = value;

   //here the label must be resized
   if (mAutoSize)
   {
     CPIControl::SetWidth(mFont->GetTextWidth(GetText()));
     CPIControl::SetHeight(mFont->GetTextHeight(GetText()));
   }
}

inline bool CPILabel::GetAutoSize()
{
   return mAutoSize;
}


inline void CPILabel::SetTextAllignment(int value)
{
   mTextAllignment = value;
}

inline void CPILabel::SetMultiline(bool value)
{
    mMultiline = value;
}

inline bool CPILabel::GetMultiline()
{
    return mMultiline;
}

/*---------------------------------------------------------------------------
 description: Font
---------------------------------------------------------------------------*/
inline CPFont* CPILabel::GetFont()
{
   return mFont;
}
inline void  CPILabel::SetFont(CPFont* value)
{
   delete mFont;
   mFont = value;
}
//---------------------------------------------------------------------------

#endif

