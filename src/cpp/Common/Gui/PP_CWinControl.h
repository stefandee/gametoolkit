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

 description: Class Piron Component Interface - Windowed Control
 last modify: 18 04 00
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#ifndef PP_CWinControlH
#define PP_CWinControlH

#include "PP_CControl.h"
#include "PP_CControlList.h"
#include "PP_GuiDef.h"
//#include "KList.h"

class CPIWinControl : public CPIControl
{
  public:
      virtual bool IsDerivedFrom(CPString className);
      virtual CPString ClassName() {return PICN_WINCONTROL;}
      virtual void Update();
      virtual void Paint();

      virtual void AddControl (CPIControl *Control);
      virtual void RemoveControl (CPIControl *Control);
      virtual void RemoveAllControls ();
      virtual void FreeControl (CPIControl *Control);
      virtual void FreeAllControls ();                        //not implemented
      virtual bool ContainControl (CPIControl *Control);
      virtual void BringControlToFront (CPIControl *Control);
      virtual void SendControlToBack (CPIControl *Control);
      virtual CPIControl *GetControlInPoint(int X, int Y);

      virtual void OnShow();     //overwritten
      virtual void OnHide();     //overwritten
  protected:
    //## Constructors
      CPIWinControl(CPIWinControl *Parent = NULL);
  public:
    //## Destructor
      virtual ~CPIWinControl();

//  protected:
   public:
    //## Get and Set Operations for Class Attributes (generated)
      virtual CPIControlList* GetControls ();
      virtual void SetControls (CPIControlList* value);
    //virtual CPList<CPIControl>* GetControls ();
    //virtual void SetControls (CPList<CPIControl>* value);

  private:
      CPIControlList *mControls;
      //CPList<CPIControl>* mControls;
};

/////////////////////////////////////////////////////////////////////////////
//GET-SET INLINE SECTION
/////////////////////////////////////////////////////////////////////////////
inline CPIControlList* CPIWinControl::GetControls ()
{
  return mControls;
}

inline void CPIWinControl::SetControls (CPIControlList* value)
{
  mControls = value;
}
//---------------------------------------------------------------------------
#endif

