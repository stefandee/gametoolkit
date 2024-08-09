//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    //
// Copyright (C) 1998-2004 Stefan Dicu & Tudor Girba                         //
//                                                                           //
// PPTactical Engine is free software; you can redistribute it               //
// and/or modify it under the terms of the GNU Lesser General Public License //
// as published by the Free Software Foundation; either version 2.1 of the   //
// License, or (at your option) any later version.                           //
//                                                                           //
// PPTactical Engine is distributed in the hope that it will be useful, but  //
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY//
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public    //
// License for more details.                                                 //
//                                                                           //
// You should have received a copy of the GNU Lesser General Public License  //
// along with PPTactical Engine; if not, write to the Free Software          //
// Foundation Inc. 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   //
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  Unit              : Check Box Control
//
//  Version           : 1.0
//
//  Description       :
//    * obvious ;)
//
//  History           :
//    [06.10.2005] - [Karg] - unit created
//-----------------------------------------------------------------------------

#ifndef PP_CCheckBoxH
#define PP_CCheckBoxH
//---------------------------------------------------------------------------

#include "PP_GuiDef.h"
#include "PP_Sprite.h"
#include "PP_CWinControl.h"

class CPICheckBox : public CPIWinControl
{
  public:
      CPICheckBox(CPIWinControl *Parent = 0);
      virtual ~CPICheckBox();

      virtual CPString ClassName() {return PICN_CHECKBOX;}
      virtual bool IsDerivedFrom(CPString className);
      virtual void Paint();

   public:
      virtual bool GetChecked() { return mChecked; }
      virtual void SetChecked(bool value) { mChecked = value; }

      CPFont *GetFont();
      void  SetFont(CPFont *value);

      virtual void SetTextAllignment(int value);
      virtual int  GetTextAllignment();

      void SetSpacing(int v) { mSpacing = v; }
      int GetSpacing() { return mSpacing; }

      void SetColor(TColor32 _color);

      virtual void OnLeftMouseClick();

  protected:

  private:
      bool     mChecked;
      CPFont   *mFont;

      int      mTextAllignment;
      int      mSpacing;
};

#endif
