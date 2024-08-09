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

 description: Class Piron Component Interface Control
 last modify: 20 05 00
        by grabX
 * fixed an error to CoordFromScreen
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#ifndef PP_CControlH
#define PP_CControlH

#include "PP_CObject.h"
#include "PP_Sprite.h"
#include "PP_Graphic.h"
#include "PP_GuiCallbacks.h"
#include "PP_basetypes.h"

class CPIWinControl; //forward declaration

class CPIControl : public CPIObject
{
   public:
      virtual CPString ClassName() {return PICN_CONTROL;}
      virtual bool IsDerivedFrom(CPString className);
      virtual void Update();
      virtual void Paint ();

   protected:
      virtual void SetClipRegion();   //sets the clip region for the control
   public:
      virtual CPRect GetClipRegion();    //gets the clip rectangle of the control

   public:
      virtual void Hide ();
      virtual void Show ();
      virtual void Resize(int Width, int Height);
      virtual void Move(int Left, int Top);
      virtual void MoveBy(int dx, int dy);
      virtual CPPoint CoordToScreen (CPPoint Point);
      virtual CPPoint CoordFromScreen (CPPoint Point);
      virtual CPRect GetBoundsRect ();
      virtual CPRect GetAbsoluteBoundsRect ();
      virtual bool IsPointInside (CPPoint point);
      virtual void BringToFront ();
      virtual void SendToBack ();

      virtual CPIControl *GetControlInPoint(int X, int Y);

      //Event Methos
      virtual void OnMouseOn (int X, int Y);
      virtual void OnMouseMove (int X, int Y);

      virtual void OnPaint();
      virtual void OnLeftMouseDown (int X, int Y);
      virtual void OnLeftMouseUp (int X, int Y);
      virtual void OnLeftMouseClick ();
      virtual void OnLeftMouseDblClick ();
      virtual void OnRightMouseDown (int X, int Y);
      virtual void OnRightMouseUp (int X, int Y);
      virtual void OnRightMouseClick ();
      virtual void OnRightMouseDblClick ();

      virtual void OnKeyDown(int key);
      virtual void OnKeyPressed(int key);
      virtual void OnKeyUp(int key);

      virtual void OnShow();
      virtual void OnHide();
      virtual void OnMove(int oldLeft, int oldTop);
      virtual void OnResize(int oldWidth, int oldHeight);

   protected:
    //## Constructor
      CPIControl(CPIWinControl *Parent = NULL);

   public:
    //## Virtual Destructor
      virtual ~CPIControl();

   public:  //only temporary
            //not all should be public
   //protected:
    //## Get and Set Operations for Class Attributes
      virtual int GetWidth ();
      virtual void SetWidth (int value);

      virtual int GetHeight ();
      virtual void SetHeight (int value);

      virtual int GetLeft ();
      virtual void SetLeft (int value);

      virtual int GetTop ();
      virtual void SetTop (int value);

      virtual int GetAbsoluteLeft ();
      virtual void SetAbsoluteLeft (int value);

      virtual int GetAbsoluteTop ();
      virtual void SetAbsoluteTop (int value);

      virtual bool GetVisible ();
      virtual void SetVisible (bool value);

      virtual bool GetEnabled ();
      virtual void SetEnabled (bool value);

      virtual bool GetUpdateEnabled ();
      virtual void SetUpdateEnabled (bool value);

      virtual int  GetUpdateCounter();
      virtual void SetUpdateCounter(int value);

      virtual int  GetUpdateMaxCount();
      virtual void SetUpdateMaxCount(int value);

      virtual bool GetLeftMouseCaptureEnabled();
      virtual void SetLeftMouseCaptureEnabled(bool value);

      virtual bool GetRightMouseCaptureEnabled();
      virtual void SetRightMouseCaptureEnabled(bool value);

      virtual bool GetWasLeftMouseDown();
      virtual void SetWasLeftMouseDown(bool value);

      virtual bool GetWasRightMouseDown();
      virtual void SetWasRightMouseDown(bool value);

      virtual int  GetAnimType();
      virtual void SetAnimType(int value);

      virtual int  GetAnimCounter();
      virtual void SetAnimCounter(int value);

      virtual int  GetAnimMaxCount();
      virtual void SetAnimMaxCount(int value);

      virtual int  GetPreviousAnimType();
      virtual void SetPreviousAnimType(int value);

      virtual CPIWinControl * GetParent ();
      virtual void SetParent (CPIWinControl * value);

      virtual CPString GetText ();
      virtual void SetText (CPString value);

      virtual CPGISprite* GetSprite ();
      virtual void SetSprite (CPGISprite* value);

      virtual CPGISurface* GetSurface ();
      virtual void SetSurface (CPGISurface* value);

      virtual TColor32 GetColor ();
      virtual void SetColor (TColor32 value);

      virtual bool GetAutoSizeToSprite();
      virtual void SetAutoSizeToSprite(bool value );

      virtual bool GetShowHint ();
      virtual void SetShowHint (bool value);

      virtual CPString GetHint ();
      virtual void SetHint (CPString value);

      virtual int GetMouseType ();
      virtual void SetMouseType (int value);

      virtual int  GetAlpha();
      virtual void SetAlpha(int value);

      virtual void SetParentClip(bool value);

      /*
      virtual void SetHandleOnPaint (void (__closure* value)(CPIObject *Sender));
      virtual void SetHandleOnMouseOn (void (__closure* value)(CPIObject *Sender, int X, int Y));
      virtual void SetHandleOnMouseMove (void (__closure* value)(CPIObject *Sender, int X, int Y));

      virtual void SetHandleOnLeftMouseDown (void (__closure * value)(CPIObject *Sender, int X, int Y));
      virtual void SetHandleOnLeftMouseUp (void (__closure* value)(CPIObject *Sender, int X, int Y));
      virtual void SetHandleOnLeftMouseClick (void (__closure* value)(CPIObject *Sender));
      virtual void SetHandleOnLeftMouseDblClick (void (__closure* value)(CPIObject *Sender));

      virtual void SetHandleOnRightMouseDown (void (__closure * value)(CPIObject *Sender, int X, int Y));
      virtual void SetHandleOnRightMouseUp (void (__closure* value)(CPIObject *Sender, int X, int Y));
      virtual void SetHandleOnRightMouseClick (void (__closure* value)(CPIObject *Sender));
      virtual void SetHandleOnRightMouseDblClick (void (__closure* value)(CPIObject *Sender));

      virtual void SetHandleOnKeyPressed (void ( __closure* value)(CPIObject *Sender, int key));
      virtual void SetHandleOnKeyDown (void ( __closure* value)(CPIObject *Sender, int key));
      virtual void SetHandleOnKeyUp (void ( __closure* value)(CPIObject *Sender, int key));

      virtual void SetHandleOnShow (void (__closure* value)(CPIObject *Sender));
      virtual void SetHandleOnHide (void (__closure* value)(CPIObject *Sender));
      virtual void SetHandleOnMove (void (__closure* value)(CPIObject *Sender, int oldLeft, int oldTop));
      virtual void SetHandleOnResize (void (__closure* value)(CPIObject *Sender, int oldWidth, int oldHeight));
      */

      virtual void SetHandleOnPaint(StandardGUICallback callback);
      virtual void SetHandleOnMouseOn(GUIObjectIntIntCallback callback);
      virtual void SetHandleOnMouseMove(GUIObjectIntIntCallback callback);

      virtual void SetHandleOnLeftMouseDown(GUIObjectIntIntCallback callback);
      virtual void SetHandleOnLeftMouseUp(GUIObjectIntIntCallback callback);
      virtual void SetHandleOnLeftMouseClick(StandardGUICallback callback);
      virtual void SetHandleOnLeftMouseDblClick(StandardGUICallback callback);

      virtual void SetHandleOnRightMouseDown(GUIObjectIntIntCallback callback);
      virtual void SetHandleOnRightMouseUp(GUIObjectIntIntCallback callback);
      virtual void SetHandleOnRightMouseClick(StandardGUICallback callback);
      virtual void SetHandleOnRightMouseDblClick(StandardGUICallback callback);

      virtual void SetHandleOnKeyPressed(GUIObjectIntCallback callback);
      virtual void SetHandleOnKeyDown(GUIObjectIntCallback callback);
      virtual void SetHandleOnKeyUp(GUIObjectIntCallback callback);

      virtual void SetHandleOnShow(StandardGUICallback callback);
      virtual void SetHandleOnHide(StandardGUICallback callback);
      virtual void SetHandleOnMove(GUIObjectIntIntCallback callback);
      virtual void SetHandleOnResize(GUIObjectIntIntCallback callback);

   private:
    // Data Members for Class Attributes
      int            mWidth;           //absolute
      int            mHeight;          //absolute
      int            mLeft;            //in parent coord
      int            mTop;             //in parent coord
      int            mAbsoluteLeft;    //in screen coord       --currently not used
      int            mAbsoluteTop;     //in screen coord       -- currently not used
      bool           mVisible;         //if the control is visible
      bool           mEnabled;         //if any inputs is allowed
      bool           mUpdateEnabled;   //if updating is allowed (for animation)
      int            mUpdateCounter;   //current update counter
      int            mUpdateMaxCount;  //max udate counter (period)
      bool           mLeftMouseCaptureEnabled;
                                       //if the control captures the left mouse when down
      bool           mRightMouseCaptureEnabled;
                                       //if the control captures the right mouse when down
      bool           mWasLeftMouseDown;
                                       //if the mouse was captured
      bool           mWasRightMouseDown;
                                       //if the mouse was captured
      int            mAnimType;        //current animation type
      int            mAnimCounter;     //current animation count
      int            mAnimMaxCount;    //number of animation; used when increasing mAnimCounter
      int            mPreviousAnimType;//the animation before the cuurent one
      CPIWinControl *mParent;          //parent window control
      CPString       mText;            //control's text - various usages
      CPGISprite    *mSprite;          //control's sprite - various usages
      CPGISurface   *mSurface;         //control's surface - various usages
      __int32        mColor;           //control's color - various usages
      bool           mAutoSizeToSprite;//when the sprite changes, the control size (width/height) is adjusted accordingly
      bool           mShowHint;        //if the hint is shown
      CPString       mHint;            //hint text
      int            mMouseType;       //mouse type when on control
      int            mAlpha;           //alpha-blending value (in 0..255 range, where 0 is transparent and 255 is opaque)
      bool           mParentClip;      /**< if set, use the bounds of the parent. if not set, compute own bounds */

      /*
      void (__closure* mHandleOnPaint)(CPIObject *Sender);

      void (__closure* mHandleOnMouseOn)(CPIObject *Sender, int X, int Y);
      void (__closure* mHandleOnMouseMove)(CPIObject *Sender, int X, int Y);

      void (__closure *mHandleOnLeftMouseDown)(CPIObject *Sender, int X, int Y);
      void (__closure* mHandleOnLeftMouseUp)(CPIObject *Sender, int X, int Y);
      void (__closure* mHandleOnLeftMouseClick)(CPIObject *Sender);
      void (__closure* mHandleOnLeftMouseDblClick)(CPIObject *Sender);

      void (__closure *mHandleOnRightMouseDown)(CPIObject *Sender, int X, int Y);
      void (__closure* mHandleOnRightMouseUp)(CPIObject *Sender, int X, int Y);
      void (__closure* mHandleOnRightMouseClick)(CPIObject *Sender);
      void (__closure* mHandleOnRightMouseDblClick)(CPIObject *Sender);

      void (__closure* mHandleOnKeyPressed)(CPIObject *Sender, int key);            //key event
      void (__closure* mHandleOnKeyDown)(CPIObject *Sender, int key);            //key event
      void (__closure* mHandleOnKeyUp)(CPIObject *Sender, int key);            //key event

      void (__closure* mHandleOnShow)(CPIObject *Sender);           //on show event
      void (__closure* mHandleOnHide)(CPIObject *Sender);           //on hide event
      void (__closure* mHandleOnMove)(CPIObject *Sender, int oldLeft, int oldTop);      //on control move event
      void (__closure* mHandleOnResize)(CPIObject *Sender, int olWidth, int oldHeight); //on control resize event
      */

      // paint event
      StandardGUICallback mHandleOnPaint;

      //mouse events
      GUIObjectIntIntCallback mHandleOnMouseOn;
      GUIObjectIntIntCallback mHandleOnMouseMove;

      GUIObjectIntIntCallback mHandleOnLeftMouseDown;
      GUIObjectIntIntCallback mHandleOnLeftMouseUp;
      StandardGUICallback mHandleOnLeftMouseClick;
      StandardGUICallback mHandleOnLeftMouseDblClick;

      GUIObjectIntIntCallback mHandleOnRightMouseDown;
      GUIObjectIntIntCallback mHandleOnRightMouseUp;
      StandardGUICallback mHandleOnRightMouseClick;
      StandardGUICallback mHandleOnRightMouseDblClick;

      GUIObjectIntCallback mHandleOnKeyPressed;
      GUIObjectIntCallback mHandleOnKeyDown;
      GUIObjectIntCallback mHandleOnKeyUp;

      StandardGUICallback mHandleOnShow;
      StandardGUICallback mHandleOnHide;
      GUIObjectIntIntCallback mHandleOnMove;
      GUIObjectIntIntCallback mHandleOnResize;
};

/////////////////////////////////////////////////////////////////////////////
//GET_SET (INLINE) SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: Width
---------------------------------------------------------------------------*/
inline int CPIControl::GetWidth ()
{
  return mWidth;
}

inline void CPIControl::SetWidth (int value)
{
   if (mWidth != value)
   {
      int lOldWidth = mWidth;
      mWidth = value;
      OnResize(lOldWidth, mHeight);  // event generate
   }
}

/*---------------------------------------------------------------------------
 description: Height
---------------------------------------------------------------------------*/
inline int CPIControl::GetHeight ()
{
  return mHeight;
}

inline void CPIControl::SetHeight (int value)
{
   if (mHeight != value)
   {
      int lOldHeight = mHeight;
      mHeight = value;
      OnResize(mWidth, lOldHeight);
   }
}

/*---------------------------------------------------------------------------
 description: Left
---------------------------------------------------------------------------*/
inline int CPIControl::GetLeft ()
{
  return mLeft;
}

inline void CPIControl::SetLeft (int value)
{
   if (mLeft != value)
   {
      int lOldLeft = mLeft;
      mLeft = value;
      OnMove(lOldLeft, mTop);
   }
}

/*---------------------------------------------------------------------------
 description: Top
---------------------------------------------------------------------------*/
inline int CPIControl::GetTop ()
{
  return mTop;
}

inline void CPIControl::SetTop (int value)
{
   if (mTop != value)
   {
      int lOldTop = mTop;
      mTop = value;
      OnMove(mLeft, lOldTop);
   }
}

/*---------------------------------------------------------------------------
 description: AbsoluteLeft
---------------------------------------------------------------------------*/
inline int CPIControl::GetAbsoluteLeft ()
{
  return mAbsoluteLeft;
}

inline void CPIControl::SetAbsoluteLeft (int value)
{
  mAbsoluteLeft = value;
}

/*---------------------------------------------------------------------------
 description: AbsoluteTop
---------------------------------------------------------------------------*/
inline int CPIControl::GetAbsoluteTop ()
{
  return mAbsoluteTop;
}

inline void CPIControl::SetAbsoluteTop (int value)
{
  mAbsoluteTop = value;
}

/*---------------------------------------------------------------------------
 description: Visible
---------------------------------------------------------------------------*/
inline bool CPIControl::GetVisible ()
{
  return mVisible;
}

inline void CPIControl::SetVisible (bool value)
{
   if (mVisible != value)
   {
      mVisible = value;
      if (mVisible)
         OnShow();
      else
         OnHide();
   }
}

/*---------------------------------------------------------------------------
 description: Enabled
---------------------------------------------------------------------------*/
inline bool CPIControl::GetEnabled ()
{
  return mEnabled;
}

inline void CPIControl::SetEnabled (bool value)
{
  mEnabled = value;
}

/*---------------------------------------------------------------------------
 description: UpdateEnabled
---------------------------------------------------------------------------*/
inline bool CPIControl::GetUpdateEnabled ()
{
  return mUpdateEnabled;
}

inline void CPIControl::SetUpdateEnabled (bool value)
{
  mUpdateEnabled = value;
}

/*---------------------------------------------------------------------------
 description: UpdateCounter
---------------------------------------------------------------------------*/
inline int  CPIControl::GetUpdateCounter()
{
   return mUpdateCounter;
}

inline void CPIControl::SetUpdateCounter(int value)
{
   mUpdateCounter = value;
}

/*---------------------------------------------------------------------------
 description: UpdateMAXCount
---------------------------------------------------------------------------*/
inline int  CPIControl::GetUpdateMaxCount()
{
   return mUpdateMaxCount;
}
inline void CPIControl::SetUpdateMaxCount(int value)
{
   mUpdateMaxCount = value;
}

/*---------------------------------------------------------------------------
 description: CaptureLeftMouseEnabled
---------------------------------------------------------------------------*/
inline bool CPIControl::GetLeftMouseCaptureEnabled()
{
   return mLeftMouseCaptureEnabled;
}
inline void CPIControl::SetLeftMouseCaptureEnabled(bool value)
{
   mLeftMouseCaptureEnabled = value;
   if (value == true)
      mRightMouseCaptureEnabled = false;
}

/*---------------------------------------------------------------------------
 description: CaptureRightMouseEnabled
---------------------------------------------------------------------------*/
inline bool CPIControl::GetRightMouseCaptureEnabled()
{
   return mRightMouseCaptureEnabled;
}
inline void CPIControl::SetRightMouseCaptureEnabled(bool value)
{
   mRightMouseCaptureEnabled = value;
   if (value == true)
      mLeftMouseCaptureEnabled = false;
}

/*---------------------------------------------------------------------------
 description: WasLeftMouseDown
---------------------------------------------------------------------------*/
inline bool CPIControl::GetWasLeftMouseDown()
{
   return mWasLeftMouseDown;
}

inline void CPIControl::SetWasLeftMouseDown(bool value)
{
   mWasLeftMouseDown = value;
}

/*---------------------------------------------------------------------------
 description: WasRightMouseDown
---------------------------------------------------------------------------*/
inline bool CPIControl::GetWasRightMouseDown()
{
   return mWasRightMouseDown;
}

inline void CPIControl::SetWasRightMouseDown(bool value)
{
   mWasRightMouseDown = value;
}

/*---------------------------------------------------------------------------
 description: AnimType
---------------------------------------------------------------------------*/
inline int  CPIControl::GetAnimType()
{
   return mAnimType;
}
inline void CPIControl::SetAnimType(int value)
{
   SetPreviousAnimType(mAnimType);
   mAnimType = value;
}

/*---------------------------------------------------------------------------
 description: AnimCounter
---------------------------------------------------------------------------*/
inline int  CPIControl::GetAnimCounter()
{
   return mAnimCounter;
}
inline void CPIControl::SetAnimCounter(int value)
{
   mAnimCounter = value;
}

/*---------------------------------------------------------------------------
 description: AniMaxCount
---------------------------------------------------------------------------*/
inline int  CPIControl::GetAnimMaxCount()
{
   return mAnimMaxCount;
}
inline void CPIControl::SetAnimMaxCount(int value)
{
   mAnimMaxCount = value;
}

/*---------------------------------------------------------------------------
 description: PreviousAnim
---------------------------------------------------------------------------*/
inline int CPIControl::GetPreviousAnimType()
{
   return mPreviousAnimType;
}
inline void CPIControl::SetPreviousAnimType(int value)
{
   mPreviousAnimType = value;
}

/*---------------------------------------------------------------------------
 description: Parent
---------------------------------------------------------------------------*/
inline CPIWinControl * CPIControl::GetParent ()
{
  return mParent;
}

inline void CPIControl::SetParent (CPIWinControl * value)
{
  mParent = value;
}

/*---------------------------------------------------------------------------
 description: Text
---------------------------------------------------------------------------*/
inline CPString CPIControl::GetText ()
{
  return mText;
}

inline void CPIControl::SetText (CPString value)
{
  mText = value;
}

/*---------------------------------------------------------------------------
 description: Sprite
---------------------------------------------------------------------------*/
inline CPGISprite* CPIControl::GetSprite ()
{
   return mSprite;
}

inline void CPIControl::SetSprite (CPGISprite* value)
{
   mSprite = value;
   if (GetAutoSizeToSprite() == true && mSprite != NULL)
   {
      SetWidth(mSprite->GetAnimWidth());
      SetHeight(mSprite->GetAnimHeight());
   }
}

/*---------------------------------------------------------------------------
 description: Surface
---------------------------------------------------------------------------*/
inline CPGISurface* CPIControl::GetSurface ()
{
   return mSurface;
}

inline void CPIControl::SetSurface (CPGISurface* value)
{
   mSurface = value;
}


/*---------------------------------------------------------------------------
 description: Color
---------------------------------------------------------------------------*/
inline TColor32 CPIControl::GetColor ()
{
   return mColor;
}
inline void CPIControl::SetColor (TColor32 value)
{
   mColor = value;
}

/*---------------------------------------------------------------------------
 description: AutoSizeOfSprite
---------------------------------------------------------------------------*/
inline bool CPIControl::GetAutoSizeToSprite()
{
   return mAutoSizeToSprite;
}

inline void CPIControl::SetAutoSizeToSprite(bool value )
{
   mAutoSizeToSprite = value;
   if (mAutoSizeToSprite == true && GetSprite() != NULL)
   {
      SetWidth(mSprite->GetAnimWidth());
      SetHeight(mSprite->GetAnimHeight());
   }
}

/*---------------------------------------------------------------------------
 description: ShowHint
---------------------------------------------------------------------------*/
inline bool CPIControl::GetShowHint ()
{
  return mShowHint;
}

inline void CPIControl::SetShowHint (bool value)
{
  mShowHint = value;
}

/*---------------------------------------------------------------------------
 description: Hint
---------------------------------------------------------------------------*/
inline CPString CPIControl::GetHint ()
{
  return mHint;
}

inline void CPIControl::SetHint (CPString value)
{
  mHint = value;
}

/*---------------------------------------------------------------------------
 description: MouseType
---------------------------------------------------------------------------*/
inline int CPIControl::GetMouseType ()
{
  return mMouseType;
}

inline void CPIControl::SetMouseType (int value)
{
  mMouseType = value;
}

/*---------------------------------------------------------------------------
 description: Alpha blending
---------------------------------------------------------------------------*/
inline int  CPIControl::GetAlpha()
{
  return mAlpha;
}

inline void CPIControl::SetAlpha(int value)
{
  mAlpha = value;

  // range checking
  if (mAlpha < 0)
  {
    mAlpha = 0;
  }

  if (mAlpha > 255)
  {
    mAlpha = 255;
  }
}

inline void CPIControl::SetParentClip(bool value)
{
    mParentClip = value;
}

/*---------------------------------------------------------------------------
 description: OnPaint
---------------------------------------------------------------------------*/
inline void CPIControl::SetHandleOnPaint(StandardGUICallback callback)
{
   mHandleOnPaint = callback;
}

/*---------------------------------------------------------------------------
 description: OnMouseMove
---------------------------------------------------------------------------*/
inline void CPIControl::SetHandleOnMouseOn(GUIObjectIntIntCallback callback)
{
   mHandleOnMouseOn      = callback;
}
/*---------------------------------------------------------------------------
 description: OnMouseMove
---------------------------------------------------------------------------*/
inline void CPIControl::SetHandleOnMouseMove(GUIObjectIntIntCallback callback)
{
   mHandleOnMouseMove      = callback;
}
/*---------------------------------------------------------------------------
 description: OnLeftMouseDown
---------------------------------------------------------------------------*/
inline void CPIControl::SetHandleOnLeftMouseDown(GUIObjectIntIntCallback callback)
{
   mHandleOnLeftMouseDown      = callback;
}
/*---------------------------------------------------------------------------
 description: OnLeftMouseUp
---------------------------------------------------------------------------*/
inline void CPIControl::SetHandleOnLeftMouseUp(GUIObjectIntIntCallback callback)
{
   mHandleOnLeftMouseUp        = callback;
}
/*---------------------------------------------------------------------------
 description: OnLeftMouseClick
---------------------------------------------------------------------------*/
inline void CPIControl::SetHandleOnLeftMouseClick(StandardGUICallback callback)
{
   mHandleOnLeftMouseClick     = callback;
}
/*---------------------------------------------------------------------------
 description: OnLeftMouseDblClick
---------------------------------------------------------------------------*/
inline void CPIControl::SetHandleOnLeftMouseDblClick(StandardGUICallback callback)
{
   mHandleOnLeftMouseDblClick  = callback;
}
/*---------------------------------------------------------------------------
 description: OnRightMouseDown
---------------------------------------------------------------------------*/
inline void CPIControl::SetHandleOnRightMouseDown(GUIObjectIntIntCallback callback)
{
   mHandleOnRightMouseDown      = callback;
}
/*---------------------------------------------------------------------------
 description: OnRightMouseUp
---------------------------------------------------------------------------*/
inline void CPIControl::SetHandleOnRightMouseUp(GUIObjectIntIntCallback callback)
{
   mHandleOnRightMouseUp        = callback;
}
/*---------------------------------------------------------------------------
 description: OnRightMouseClick
---------------------------------------------------------------------------*/
inline void CPIControl::SetHandleOnRightMouseClick(StandardGUICallback callback)
{
   mHandleOnRightMouseClick     = callback;
}
/*---------------------------------------------------------------------------
 description: OnRightMouseDblClick
---------------------------------------------------------------------------*/
inline void CPIControl::SetHandleOnRightMouseDblClick(StandardGUICallback callback)
{
   mHandleOnRightMouseDblClick  = callback;
}
/*---------------------------------------------------------------------------
 description: OnKeyPressed
---------------------------------------------------------------------------*/
inline void CPIControl::SetHandleOnKeyPressed(GUIObjectIntCallback callback)
{
   mHandleOnKeyPressed = callback;
}
/*---------------------------------------------------------------------------
 description: OnKeyDown
---------------------------------------------------------------------------*/
inline void CPIControl::SetHandleOnKeyDown(GUIObjectIntCallback callback)
{
   mHandleOnKeyDown            = callback;
}
/*---------------------------------------------------------------------------
 description: OnKeyUp
---------------------------------------------------------------------------*/
inline void CPIControl::SetHandleOnKeyUp(GUIObjectIntCallback callback)
{
   mHandleOnKeyUp            = callback;
}
/*---------------------------------------------------------------------------
 description: OnShow
---------------------------------------------------------------------------*/
inline void CPIControl::SetHandleOnShow(StandardGUICallback callback)
{
   mHandleOnShow           = callback;
}
/*---------------------------------------------------------------------------
 description: OnHide
---------------------------------------------------------------------------*/
inline void CPIControl::SetHandleOnHide(StandardGUICallback callback)
{
   mHandleOnHide           = callback;
}
/*---------------------------------------------------------------------------
 description: OnMove
---------------------------------------------------------------------------*/
inline void CPIControl::SetHandleOnMove(GUIObjectIntIntCallback callback)
{
   mHandleOnMove           = callback;
}
/*---------------------------------------------------------------------------
 description: OnResize
---------------------------------------------------------------------------*/
inline void CPIControl::SetHandleOnResize(GUIObjectIntIntCallback callback)
{
   mHandleOnResize         = callback;
}

/*
void CPIControl::SetHandleOnPaint (void (__closure* value)(CPIObject *Sender))
{
   mHandleOnPaint = value;
}

void CPIControl::SetHandleOnMouseOn (void (__closure* value)(CPIObject *Sender, int X, int Y))
{
   mHandleOnMouseOn      = value;
}

void CPIControl::SetHandleOnMouseMove (void (__closure* value)(CPIObject *Sender, int X, int Y))
{
   mHandleOnMouseMove      = value;
}


void CPIControl::SetHandleOnLeftMouseDown (void (__closure * value)(CPIObject *Sender, int X, int Y))
{
   mHandleOnLeftMouseDown      = value;
}

void CPIControl::SetHandleOnLeftMouseUp (void (__closure* value)(CPIObject *Sender, int X, int Y))
{
   mHandleOnLeftMouseUp        = value;
}

void CPIControl::SetHandleOnLeftMouseClick (void (__closure* value)(CPIObject *Sender))
{
   mHandleOnLeftMouseClick     = value;
}

void CPIControl::SetHandleOnLeftMouseDblClick (void (__closure* value)(CPIObject *Sender))
{
   mHandleOnLeftMouseDblClick  = value;
}

void CPIControl::SetHandleOnRightMouseDown (void (__closure * value)(CPIObject *Sender, int X, int Y))
{
   mHandleOnRightMouseDown      = value;
}

void CPIControl::SetHandleOnRightMouseUp (void (__closure* value)(CPIObject *Sender, int X, int Y))
{
   mHandleOnRightMouseUp        = value;
}

void CPIControl::SetHandleOnRightMouseClick (void (__closure* value)(CPIObject *Sender))
{
   mHandleOnRightMouseClick     = value;
}

void CPIControl::SetHandleOnRightMouseDblClick (void (__closure* value)(CPIObject *Sender))
{
   mHandleOnRightMouseDblClick  = value;
}

void CPIControl::SetHandleOnKeyPressed (void ( __closure* value)(CPIObject *Sender, int key))
{
   mHandleOnKeyPressed = value;
}

void CPIControl::SetHandleOnKeyDown (void ( __closure* value)(CPIObject *Sender, int key))
{
   mHandleOnKeyDown            = value;
}

void CPIControl::SetHandleOnKeyUp(void ( __closure* value)(CPIObject *Sender, int key))
{
   mHandleOnKeyUp            = value;
}

void CPIControl::SetHandleOnShow (void (__closure* value)(CPIObject *Sender))
{
   mHandleOnShow           = value;
}

void CPIControl::SetHandleOnHide (void (__closure* value)(CPIObject *Sender))
{
   mHandleOnHide           = value;
}

void CPIControl::SetHandleOnMove (void (__closure* value)(CPIObject *Sender, int oldLeft, int oldTop))
{
   mHandleOnMove           = value;
}

void CPIControl::SetHandleOnResize (void (__closure* value)(CPIObject *Sender, int oldWidth, int oldHeight))
{
   mHandleOnResize         = NULL;
}
*/

//---------------------------------------------------------------------------
#endif

