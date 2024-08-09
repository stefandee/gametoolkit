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

 description: Class Piron Component Interface - Check Button
 last modify: 21 04 00
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#ifndef PP_CCheckButtonH
#define PP_CCheckButtonH

#include "PP_CButton.h"
#include "PP_GuiDef.h"
#include "PP_Sprite.h"

class CPICheckButton: public CPIButtonControl
{
   public:
      CPICheckButton(CPIWinControl *Parent = NULL);
      virtual ~CPICheckButton();
      virtual CPString ClassName() {return PICN_CHECKBUTTON;}
      virtual bool IsDerivedFrom(CPString className);
      virtual void OnLeftMouseClick();
      virtual void Paint();

   public:
      virtual bool GetChecked();
      virtual void SetChecked(bool value);

      virtual int GetGroupIndex();
      virtual void SetGroupIndex(int value);

      virtual int  GetCheckedAnim();
      virtual void SetCheckedAnim(int value);

      virtual CPGISprite* GetIconSprite ();
      virtual void SetIconSprite (CPGISprite* value);

      virtual void SetTextAllignment(int value);
      virtual int GetTextAllignment();

      CPFont *GetFont();
      void  SetFont(CPFont *value);

   private:
      bool        mChecked;
      int         mGroupIndex;
      int         mCheckedAnim;

      CPGISprite *mIconSprite;
      int         mTextAllignment;

      CPFont   *mFont;
};

/////////////////////////////////////////////////////////////////////////////
//GET-SET (INLINE) SECTION
/////////////////////////////////////////////////////////////////////////////
inline bool CPICheckButton::GetChecked()
{
   return mChecked;
}

inline void CPICheckButton::SetChecked(bool value)
{
   mChecked = value;
   if (mChecked == true)
      SetAnimType(GetCheckedAnim());   //checked animation
   else
      SetAnimType(0);   //unchecked animation
   CPIButtonControl::OnLeftMouseClick();
}

/*---------------------------------------------------------------------------
 description: group index
---------------------------------------------------------------------------*/
inline int CPICheckButton::GetGroupIndex()
{
   return mGroupIndex;
}
inline void CPICheckButton::SetGroupIndex(int value)
{
   mGroupIndex = value;
}

/*---------------------------------------------------------------------------
 description: checked anim
---------------------------------------------------------------------------*/
inline int  CPICheckButton::GetCheckedAnim()
{
   return mCheckedAnim;
}
inline void CPICheckButton::SetCheckedAnim(int value)
{
   mCheckedAnim = value;
}

/*---------------------------------------------------------------------------
 description: icon Sprite
---------------------------------------------------------------------------*/
inline CPGISprite* CPICheckButton::GetIconSprite ()
{
   return mIconSprite;
}

inline void CPICheckButton::SetIconSprite (CPGISprite* value)
{
   mIconSprite = value;
}

/*---------------------------------------------------------------------------
 description: text allignment
---------------------------------------------------------------------------*/
inline void CPICheckButton::SetTextAllignment(int value)
{
   mTextAllignment = value;
}

inline int CPICheckButton::GetTextAllignment()
{
   return mTextAllignment;
}

/*---------------------------------------------------------------------------
 description: Font
---------------------------------------------------------------------------*/
inline CPFont* CPICheckButton::GetFont()
{
   return mFont;
}
inline void  CPICheckButton::SetFont(CPFont* value)
{
//   delete mFont;
   mFont = value;
}
//---------------------------------------------------------------------------

#endif

