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

 description: Class Piron Interface - Screen
              it should allow only one instance (Singleton Pattern)
 last modify: 19 04 00
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#ifndef PP_CScreenH
#define PP_CScreenH

#include "PP_CWinControl.h"
#include "PP_CActionList.h"
#include "PP_CLabel.h"

class CPIForm;

class CPIScreen
{
  protected:
      CPIScreen();   //default constructor
      virtual ~CPIScreen();  //destructor

  public:
      virtual void Update();

  public:
      virtual void ReleaseMouseCapture ();
      virtual void ReleaseKeyboardCapture ();

      virtual void OnHint(CPIObject *Sender);
   //get set operations
  public:
      virtual CPIWinControl * GetDesktop ();
      virtual void SetDesktop (CPIWinControl * value);

      virtual CPIControl * GetMouseCaptureControl ();
      virtual void SetMouseCaptureControl (CPIControl* Control);

      virtual CPIControl * GetKeyboardCaptureControl ();
      virtual void SetKeyboardCaptureControl (CPIControl* Control);

      virtual CPIActionList *GetActions();
      virtual void SetActions(CPIActionList *value);

      virtual bool GetShowHint();
      virtual void SetShowHint(bool value);

      virtual bool GetControlShowHint();
      virtual void SetControlShowHint(bool value);

      virtual CPILabel *GetHintControl();
      virtual void SetHintControl(CPILabel *value);

      virtual void SetModalDialog(CPIForm* value) { mModalDialog = value; }
      CPIForm* GetModalDialog() { return mModalDialog; }

  private: //## implementation
      CPIWinControl *mDesktop;                  //pointer to the desktop
      CPIControl *mMouseCaptureControl;         //the control that captured the mouse events
      CPIControl *mKeyboardCaptureControl;      //the control that captured the keyb events
      CPIControl *mLastControlLeftDown;         //the last control in which the left mouse was down
      CPIControl *mLastControlRightDown;        //the last control in which the right mouse was down

      bool        mShowHint;
      bool        mControlShowHint;
      CPILabel*   mHintControl;
      CPIForm*    mModalDialog;

      CPIActionList *mActions;

      //void (__closure *mHandleOnHint)(CPIObject *Sender);
      StandardGUICallback mHandleOnHint;

      void UpdateEvents();
};

/////////////////////////////////////////////////////////////////////////////
//SET-GET (INLINE) SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: Desktop
---------------------------------------------------------------------------*/
inline CPIWinControl* CPIScreen::GetDesktop ()
{
  return mDesktop;
}

inline void CPIScreen::SetDesktop (CPIWinControl * value)
{
  mDesktop = value;

  mHintControl->SetParent(value);
}

/*---------------------------------------------------------------------------
 description: MouseCaptureControl
---------------------------------------------------------------------------*/
inline CPIControl * CPIScreen::GetMouseCaptureControl ()
{
  return mMouseCaptureControl;
}

inline CPIControl * CPIScreen::GetKeyboardCaptureControl ()
{
  return mKeyboardCaptureControl;
}

/*---------------------------------------------------------------------------
 description: Actions
---------------------------------------------------------------------------*/
inline CPIActionList *CPIScreen::GetActions()
{
   return mActions;
}

inline void CPIScreen::SetActions(CPIActionList *value)
{
   mActions = value;
}

/*---------------------------------------------------------------------------
 description: ShowHint
---------------------------------------------------------------------------*/
inline bool CPIScreen::GetShowHint()
{
   return mShowHint;
}

inline void CPIScreen::SetShowHint(bool value)
{
   mShowHint = value;
}

/*---------------------------------------------------------------------------
 description: ControlShowHint
---------------------------------------------------------------------------*/
inline bool CPIScreen::GetControlShowHint()
{
   return mControlShowHint;
}

inline void CPIScreen::SetControlShowHint(bool value)
{
   mControlShowHint = value;
}

/*---------------------------------------------------------------------------
 description: HintControl
---------------------------------------------------------------------------*/
inline CPILabel *CPIScreen::GetHintControl()
{
   return mHintControl;
}
inline void CPIScreen::SetHintControl(CPILabel *value)
{
   mHintControl = value;
   mHintControl->SetParent(mDesktop);
}
//---------------------------------------------------------------------------

#endif

