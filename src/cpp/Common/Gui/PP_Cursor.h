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

 description: Class Cursor
              update, paint,
              mouse down, up, press
              set type, speedfactor, pos
              Singleton Pattern
 last modify: 21 04 00
        by grabX
---------------------------------------------------------------------------*/
#ifndef CursorH
#define CursorH

#include "PP_Sprite.h"
#include "PP_Input.h"
#include "PP_Counter.h"
#include "PP_GuiDef.h"
#include "PPIG_InterfaceDef.h"

class CCursor
 {
   public:
      static CCursor*   Instance();
      static void       Release();

      void Update();
      void Paint();

      void LoadSpriteFromFile(CPString fileName, int wCells, int hCells);

   public: //set-get methods
      void SetType(int value);
      int GetType();

      void SetAnim(int value);
      int GetAnim();

      void SetSprite(CPSprite *sprite);
      CPSprite* GetSprite();

      void SetHotSpot(CPPoint value);
      CPPoint GetHotSpot();

      void SetPosition(CPPoint position);
      CPPoint GetPosition();

      int  GetPosX();
      int  GetPosY();

      void SetSpeedFactor(int sf);
      int GetSpeedFactor();

      void SetRestrictedRect(CPRect value);
      CPRect GetRestrictedValue();

      void SetAnimUpdateCounter(int value);
      CPCounter& GetAnimUpdateCounter();

      void SetVisible(bool value);
      bool GetVisible();

      void SetEnabled(bool value);
      bool GetEnabled();

      bool IsLeftDown();
      bool IsLeftPressed();
      bool IsLeftUp();

      bool IsRightDown();
      bool IsRightPressed();
      bool IsRightUp();

      bool IsMoved();

   protected: //constructor-destructor
      CCursor();
      ~CCursor();

   private:
      //instanta
      static CCursor      *mInstance;
      static int           mInstanceCount;

      int                  mType; //mouse type and also the line of the animation in the sprite
      int                  mAnim; //the mouse current animation position (the column in the sprite)
      CSprite             *mSprite;

      TMouseButtons        mButtons,
                           mOldButtons; // the button current and old state

      CPPoint               mHotSpot;                 //the mouse's hot spot
      CPPoint               mPosition, mOldPosition;  //the current and old position

      int                  mSpeedFactor;                //folosit pentru a face mouse-ul sa 'mearga'
                                                        //mai repede

      CPCounter            mAnimUpdateCounter;
      CPRect                mRestrictedRect;

      bool                 mVisible;   //arata daca cursorul este vizibil
      bool                 mEnabled;   //arata daca cursorul este enabled
};

typedef CCursor CPGICursor;
typedef CCursor CPIICursor;

CPGICursor* GetCursorInstance();


/////////////////////////////////////////////////////////////////////////////
//INLINE SECTION
/////////////////////////////////////////////////////////////////////////////

/*--------------------------------------------------------------------------
 Description: cursor type
--------------------------------------------------------------------------*/
inline int CCursor::GetType()
{
   return mType;
}

/*--------------------------------------------------------------------------
 Description: cursor current animation
--------------------------------------------------------------------------*/
inline void CCursor::SetAnim(int value)
{
   mAnim = value;
}
inline int CCursor::GetAnim()
{
   return mAnim;
}

/*---------------------------------------------------------------------------
 description: cursor sprite
---------------------------------------------------------------------------*/
inline void CCursor::SetSprite(CPSprite *value)
{
   mSprite = value;
}
inline CPSprite* CCursor::GetSprite()
{
   return mSprite;
}

/*--------------------------------------------------------------------------
 Description: seteaza pozitia cursorului
 Parameters : pozitia
 Others     : se apeleaza la fiecare tick
--------------------------------------------------------------------------*/
inline void CCursor::SetPosition(CPPoint position)
{
   mPosition = position;
}

inline CPPoint CCursor::GetPosition()
{
   return mPosition;
}

/*---------------------------------------------------------------------------
 description: the cursor hot spot
---------------------------------------------------------------------------*/
inline void CCursor::SetHotSpot(CPPoint value)
{
   mHotSpot = value;
}
inline CPPoint CCursor::GetHotSpot()
{
   return mHotSpot;
}

/*--------------------------------------------------------------------------
 Description: seteaza factorul de viteza
 Others     : acesta se inmulteste cu deplasarea
--------------------------------------------------------------------------*/
inline void CCursor::SetSpeedFactor(int value)
{
   mSpeedFactor = value;
}

inline int CCursor::GetSpeedFactor()
{
   return mSpeedFactor;
}

/*---------------------------------------------------------------------------
 description: returneaza pozitia X a cursorlui
---------------------------------------------------------------------------*/
inline int CCursor::GetPosX()
{
   return mPosition.x + mHotSpot.x;
}
/*---------------------------------------------------------------------------
 description: returneaza pozitia y a cursorului
---------------------------------------------------------------------------*/
inline int CCursor::GetPosY()
{
   return mPosition.y + mHotSpot.y;
}

/*---------------------------------------------------------------------------
 description: the mouse restricted rect
---------------------------------------------------------------------------*/
inline void CCursor::SetRestrictedRect(CPRect value)
{
   mRestrictedRect = value;
}
inline CPRect CCursor::GetRestrictedValue()
{
   return mRestrictedRect;
}

/*---------------------------------------------------------------------------
 description: sets the counter mPosition
---------------------------------------------------------------------------*/
inline void CCursor::SetAnimUpdateCounter(int value)
{
   mAnimUpdateCounter = value;
}
inline CPCounter& CCursor::GetAnimUpdateCounter()
{
   return mAnimUpdateCounter;
}

/*--------------------------------------------------------------------------
 Description: sprite visible
--------------------------------------------------------------------------*/
inline void CCursor::SetVisible(bool value)
{
   mVisible = value;
}

inline bool CCursor::GetVisible()
{
   return mVisible;
}

/*--------------------------------------------------------------------------
 Description: cursor enabled
--------------------------------------------------------------------------*/
inline void CCursor::SetEnabled(bool value)
{
   mEnabled = value;
}

inline bool CCursor::GetEnabled()
{
   return mEnabled;
}


/*--------------------------------------------------------------------------
 Description: arata evenimentul de mouse left down
--------------------------------------------------------------------------*/
inline bool CCursor::IsLeftDown()
{
   return (mOldButtons.mLeft == false && mButtons.mLeft == true);
}

/*--------------------------------------------------------------------------
 Description: arata evenimentul de mouse left pressed
--------------------------------------------------------------------------*/
inline bool CCursor::IsLeftPressed()
{
   return (mOldButtons.mLeft == true && mButtons.mLeft == true);
}

/*--------------------------------------------------------------------------
 Description: arata evenimentul de mouse left up
--------------------------------------------------------------------------*/
inline bool CCursor::IsLeftUp()
{
   return (mOldButtons.mLeft == true && mButtons.mLeft == false);
}
/*--------------------------------------------------------------------------
 Description: arata evenimentul de mouse left down
--------------------------------------------------------------------------*/
inline bool CCursor::IsRightDown()
{
   return (mOldButtons.mRight == false && mButtons.mRight == true);
}

/*--------------------------------------------------------------------------
 Description: arata evenimentul de mouse left pressed
--------------------------------------------------------------------------*/
inline bool CCursor::IsRightPressed()
{
   return (mOldButtons.mRight == true && mButtons.mRight == true);
}

/*--------------------------------------------------------------------------
 Description: arata evenimentul de mouse left up
--------------------------------------------------------------------------*/
inline bool CCursor::IsRightUp()
{
   return (mOldButtons.mRight == true && mButtons.mRight == false);
}

/*---------------------------------------------------------------------------
 description: arata daca cursorul s-a mutat
---------------------------------------------------------------------------*/
inline bool CCursor::IsMoved()
{
   return (mOldPosition.x != mPosition.x || mOldPosition.y != mPosition.y);
}

//---------------------------------------------------------------------------
#endif
