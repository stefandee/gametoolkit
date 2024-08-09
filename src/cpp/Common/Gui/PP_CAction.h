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

 description: Class Piron Component Interface - Action
 last modify: 06 05 00
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#ifndef PP_CActionH
#define PP_CActionH

#include "PP_CObject.h"
#include "PP_GuiCallbacks.h"

class CPIAction : public CPIObject  //## Inherits: <unnamed>%39140F11012C
{
  public:
    //## Constructors (generated)
      CPIAction();
      ~CPIAction();

      void Update();
      void OnActivate ();

   public:
      //## Get and Set Operations for Class Attributes (generated)
      unsigned __int8 GetKeysPressed ();
      void SetKeysPressed (unsigned __int8 value);

      unsigned __int8 GetKeysDown ();
      void SetKeysDown (unsigned __int8 value);

      unsigned __int8 GetKeysUp ();
      void SetKeysUp (unsigned __int8 value);

      void SetHandleOnActivate(StandardGUICallback callback);

      bool GetVisible();
      void SetVisible(bool value);
      bool GetEnabled();
      void SetEnabled(bool value);
      bool GetUpdateEnabled();
      void SetUpdateEnabled(bool value);

  protected:
      bool RequiredKeysArePressed();
      bool RequiredKeysAreDown();
      bool RequiredKeysAreUp();
  private: //## implementation
    // Data Members for Class Attributes
      unsigned __int8 mKeysPressed;
      unsigned __int8 mKeysDown;
      unsigned __int8 mKeysUp;
      bool mVisible;          //nefolosit
      bool mEnabled;
      bool mUpdateEnabled;    //nefolosit

      StandardGUICallback mHandleOnActivate;
};

/////////////////////////////////////////////////////////////////////////////
//GET-SET (INLINE) SECTION
/////////////////////////////////////////////////////////////////////////////

// Class CPIAction
inline unsigned __int8 CPIAction::GetKeysPressed ()
{
  return mKeysPressed;
}

inline void CPIAction::SetKeysPressed (unsigned __int8 value)
{
  mKeysPressed = value;
}

inline unsigned __int8 CPIAction::GetKeysDown ()
{
  return mKeysDown;
}

inline void CPIAction::SetKeysDown (unsigned __int8 value)
{
  mKeysDown = value;
}

inline unsigned __int8 CPIAction::GetKeysUp ()
{
  return mKeysUp;
}

inline void CPIAction::SetKeysUp (unsigned __int8 value)
{
  mKeysUp = value;
}

/*
inline void CPIAction::SetHandleOnActivate (void (__closure * value)(CPIObject *Sender))
{
  mHandleOnActivate = value;
}
*/

inline void CPIAction::SetHandleOnActivate(StandardGUICallback callback)
{
  mHandleOnActivate = callback;
}

/*---------------------------------------------------------------------------
 description: Visible
---------------------------------------------------------------------------*/
inline bool CPIAction::GetVisible()
{
   return mVisible;
}
inline void CPIAction::SetVisible(bool value)
{
   mVisible = value;
}
/*---------------------------------------------------------------------------
 description: Enabled
---------------------------------------------------------------------------*/
inline bool CPIAction::GetEnabled()
{
   return mEnabled;
}
inline void CPIAction::SetEnabled(bool value)
{
   mEnabled = value;
}
/*---------------------------------------------------------------------------
 description: Update Enabled
---------------------------------------------------------------------------*/
inline bool CPIAction::GetUpdateEnabled()
{
   return mUpdateEnabled;
}
inline void CPIAction::SetUpdateEnabled(bool value)
{
   mUpdateEnabled = value;
}

//---------------------------------------------------------------------------
#endif

