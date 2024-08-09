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

 description: Class Piron Component Interface - Form
              this is an abstract class and cannot have instances
 last modify: 21 04 00
        by grabX
---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
#ifndef PP_CFormH
#define PP_CFormH

#include "PP_CWinControl.h"

class CPIForm : public CPIWinControl
{
  public:
      CPIForm(CPIWinControl *Parent = NULL);
      virtual ~CPIForm();

  public:
      virtual CPString ClassName() {return PICN_FORM;}
      virtual bool IsDerivedFrom(CPString ClassName);
      virtual void Paint();

      virtual void Close();   //simulates the click on the close
      virtual void ShowModal(CPIControl *ParentControl);

  public:
      virtual void OnClose();
      virtual void OnLeftMouseClick();

  public:
      virtual void SetModal(bool value);
      virtual bool GetModal();

      virtual void SetParentControl(CPIControl *value);
      virtual CPIControl* GetParentControl();

      virtual void SetModalResult(int value);
      virtual int GetModalResult();

      virtual void SetHandleOnClose (StandardGUICallback callback);

  private:
      bool           mModal;
      CPIControl     *mParentControl; //it refers at the control that calls this form to be modal
      int            mModalResult;

      StandardGUICallback mHandleOnClose;
};

inline bool CPIForm::GetModal()
{
   return mModal;
}

/*---------------------------------------------------------------------------
 description: parent control
---------------------------------------------------------------------------*/
inline void CPIForm::SetParentControl(CPIControl *value)
{
   mParentControl = value;
}
inline CPIControl* CPIForm::GetParentControl()
{
   return mParentControl;
}

/*---------------------------------------------------------------------------
 description: modal result
---------------------------------------------------------------------------*/
inline void CPIForm::SetModalResult(int value)
{
   mModalResult = value;
}
inline int CPIForm::GetModalResult()
{
   return mModalResult;
}

/*---------------------------------------------------------------------------
 description: handle on close
---------------------------------------------------------------------------*/
inline void CPIForm::SetHandleOnClose (StandardGUICallback callback)
{
   mHandleOnClose = callback;
}


//---------------------------------------------------------------------------
#endif
