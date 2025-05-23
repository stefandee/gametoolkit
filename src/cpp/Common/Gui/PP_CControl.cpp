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
               is an interface and cannot be instantiated
 last modify: 20 05 00
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#pragma hdrstop

#include "PP_CControl.h"
#include "PP_CWinControl.h"
#include "PP_Cursor.h"
#include "PPIG_CScreen.h" //only temporar

/////////////////////////////////////////////////////////////////////////////
//PROTECTED SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: default constructor (protected)
---------------------------------------------------------------------------*/
CPIControl::CPIControl(CPIWinControl *Parent) : CPIObject()
{
   mWidth            = 100;
   mHeight           = 100;
   mLeft             = 0;
   mTop              = 0;
   mAbsoluteLeft     = 0;
   mAbsoluteTop      = 0;

   mVisible          = true;
   mEnabled          = true;
   mUpdateEnabled    = true;
   mUpdateCounter    = 0;
   mUpdateMaxCount   = 1;

   mLeftMouseCaptureEnabled  = false;
   mRightMouseCaptureEnabled = false;
   mWasLeftMouseDown         = false;
   mWasRightMouseDown        = false;

   mAnimType         = 0;
   mAnimCounter      = 0;
   mAnimMaxCount     = 1;
   mPreviousAnimType = 0;

   mParent           = Parent;
   mText             = "";
   mSprite           = NULL;
   mSurface          = NULL;
   mColor            = 0; //that's black
   mAutoSizeToSprite = false;
   mShowHint         = true;
   mHint             = "";
   mMouseType        = 0; //default mouse
   mAlpha            = 255; // opaque

   mParentClip       = false;

   //events handlers
   /*
   mHandleOnPaint                = NULL;
   mHandleOnMouseOn              = NULL;
   mHandleOnMouseMove            = NULL;
   mHandleOnLeftMouseDown        = NULL;
   mHandleOnLeftMouseUp          = NULL;
   mHandleOnLeftMouseClick       = NULL;
   mHandleOnLeftMouseDblClick    = NULL;
   mHandleOnRightMouseDown       = NULL;
   mHandleOnRightMouseUp         = NULL;
   mHandleOnRightMouseClick      = NULL;
   mHandleOnRightMouseDblClick   = NULL;
   mHandleOnKeyPressed           = NULL;
   mHandleOnKeyDown              = NULL;
   mHandleOnKeyUp                = NULL;
   mHandleOnShow                 = NULL;
   mHandleOnHide                 = NULL;
   mHandleOnMove                 = NULL;
   mHandleOnResize               = NULL;
   */
}
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//PUBLIC SECTION
/////////////////////////////////////////////////////////////////////////////
/*--------------------------------- ------------------------------------------
 description: destructor (protected)
---------------------------------------------------------------------------*/
CPIControl::~CPIControl()
{
}
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//PUBLIC SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: return true if the className is a parent of this object
 parameters : className - className wanted
---------------------------------------------------------------------------*/
bool CPIControl::IsDerivedFrom(CPString className)
{
   return ((className == ClassName()) || (CPIObject::IsDerivedFrom(className)));
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: updates the control
 others     : overwrite this method to update your custom control
              this method offer the animation mecanism
---------------------------------------------------------------------------*/
void CPIControl::Update ()
{
   if (GetUpdateEnabled() == true)
   {
      SetUpdateCounter((GetUpdateCounter() + 1) % GetUpdateMaxCount());
      if (GetUpdateCounter() == 0)
      {
         SetAnimCounter((GetAnimCounter() + 1) % GetAnimMaxCount());
      }
   }
   CPPoint lPt;
   lPt = CoordToScreen(CPPoint(0, 0));
   SetAbsoluteLeft(lPt.x);
   SetAbsoluteTop(lPt.y);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: paints the control
 others     : overwrite this method to paint your custom control
---------------------------------------------------------------------------*/
void CPIControl::Paint ()
{
   OnPaint();
}
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//PROTECTED SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: sets the clip region for this control
 others     : overwrite this method to clip your control in your own way
              this only implements the rectangle clip region and should be called before the paint
---------------------------------------------------------------------------*/
void CPIControl::SetClipRegion()
{
   CPGIGraphicSystem *lGSI;
   try
   {
      lGSI = GetGraphicInstance();
   }
   catch(...)
   {
      throw;
   }

   /*
   CPPoint lPoint;
   lPoint = CoordToScreen(CPPoint(0, 0));

   int lL, lT, lB, lR;
   lL = lPoint.x;
   lT = lPoint.y;
   lR = lL + GetWidth();
   lB = lT + GetHeight();
   */
   CPRect clipRect = GetClipRegion();

   CPRect lParentRect;

   if (GetParent() != NULL)
   {
      lParentRect = GetParent()->GetClipRegion();

      if (mParentClip)
      {
        clipRect.left = lParentRect.left;
        clipRect.top = lParentRect.top;
        clipRect.right = lParentRect.right;
        clipRect.bottom = lParentRect.bottom;
      }
      else
      {
          if (clipRect.left < lParentRect.left)
             clipRect.left = lParentRect.left;
          if (clipRect.top < lParentRect.top)
             clipRect.top = lParentRect.top;
          if (clipRect.right > lParentRect.right)
             clipRect.right = lParentRect.right;
          if (clipRect.bottom > lParentRect.bottom)
             clipRect.bottom = lParentRect.bottom;
      }
   }
   /*
   else
   {
     // the hint control is the only one that does not have a parent...
     lL = GetAbsoluteLeft();
     lT = GetAbsoluteTop();
     lR = lL + GetWidth();
     lB = lT + GetHeight();
     logWriteLn("this control has no parent");
   }
   */

#if PPT_USE_SDL
   lGSI->SetClipRegion(clipRect);
#else
   lGSI->SetClipRegion(NULL, clipRect);
#endif // PPT_USE_SDL
}
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//PUBLIC SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: gets the clip region for this control
 others     : overwrite this method to clip your control in your own way
              this only implements the rectangle clip region and should be called before the paint
---------------------------------------------------------------------------*/
CPRect CPIControl::GetClipRegion()
{
   CPPoint lPoint;
   lPoint = CoordToScreen(CPPoint(0, 0));
   return CPRect(lPoint.x, lPoint.y, lPoint.x + GetWidth(), lPoint.y + GetHeight());
}
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//PUBLIC SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: makes the control hidden  and generates OnHide event
---------------------------------------------------------------------------*/
void CPIControl::Hide ()
{
   SetVisible(false);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: makes the control visible and generates OnShow event
---------------------------------------------------------------------------*/
void CPIControl::Show ()
{
   SetVisible(true);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: resizes the control
 parameters : Width, Height - new dimensions, in absolute units
 others     : doesn't call SetHeight & SetWidth because this would generate 2 OnResize events
---------------------------------------------------------------------------*/
void CPIControl::Resize(int Width, int Height)
{
   if (mWidth != Width || mHeight != Height)
   {
      int lOldWidth = mWidth, lOldHeight = mHeight;
      mWidth = Width;
      mHeight = Height;
      OnResize(lOldWidth, lOldHeight);
   }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: moves the control
 parameters : Left, Top - new coord, in absolute units
 others     : doesn't call SetLeft & SetTop because this would generate 2 OnMove events
---------------------------------------------------------------------------*/
void CPIControl::Move(int Left, int Top)
{
   if (mLeft != Left || mTop != Top)
   {
      int lOldLeft = Left, lOldTop = Top;
      mLeft = Left;
      mTop  = Top;
      OnMove(lOldLeft, lOldTop);
   }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: moves the control
 parameters : dx, dy - deltas to add to current coords
 others     : doesn't call SetLeft & SetTop because this would generate 2 OnMove events
---------------------------------------------------------------------------*/
void CPIControl::MoveBy(int dx, int dy)
{
   if (dx != 0 || dy != 0)
   {
      int lOldLeft = mLeft, lOldTop = mTop;
      mLeft += dx;
      mTop  += dy;
      OnMove(lOldLeft, lOldTop);
   }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: transforms the control coord into scren coord
 parameters : point - in control coord
---------------------------------------------------------------------------*/
CPPoint CPIControl::CoordToScreen (CPPoint Point)
{
   if (GetParent() != NULL)
   {
      CPPoint lParentCoord = GetParent()->CoordToScreen(CPPoint(mLeft, mTop));
      return CPPoint(lParentCoord.x + Point.x, lParentCoord.y + Point.y);
   }
   else // this means it's the desktop or other control that does not have a parent
      return CPPoint(GetLeft() + Point.x, GetTop() + Point.y); //eventualy + left, top ?
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: transforms the scren coord into control coord
 parameters : point - in screen coord
---------------------------------------------------------------------------*/
CPPoint CPIControl::CoordFromScreen (CPPoint Point)
{
   if (GetParent() != NULL)
   {
      CPPoint lParentCoord = GetParent()->CoordToScreen(CPPoint(mLeft, mTop));
      //return CPPoint(lParentCoord.x - Point.x, lParentCoord.y - Point.y);
      return CPPoint(-lParentCoord.x + Point.x, -lParentCoord.y + Point.y);
   }
   else // adica ii desktop
      return CPPoint(Point.x + GetLeft(), Point.y + GetTop());
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: gets the recangle of the control in it's parent relative coord
---------------------------------------------------------------------------*/
CPRect CPIControl::GetBoundsRect ()
{
   return CPRect(mLeft, mTop, mLeft + mWidth, mTop + mHeight);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: gets the recangle of the control in it's absolute coord
---------------------------------------------------------------------------*/
CPRect CPIControl::GetAbsoluteBoundsRect ()
{
   return CPRect(mAbsoluteLeft, mAbsoluteTop, mAbsoluteLeft + mWidth, mAbsoluteTop + mHeight);
}
//---------------------------------------------------------------------------


/*---------------------------------------------------------------------------
 description: returns true if the point (in screen coord) is inside the control and the control is enabled
 parameters : point - in screen coord
---------------------------------------------------------------------------*/
bool CPIControl::IsPointInside (CPPoint point)
{
   return ((point.x >= GetAbsoluteLeft()) && (point.x <= GetAbsoluteLeft() + GetWidth()) &&
           (point.y >= GetAbsoluteTop()) && (point.y <= GetAbsoluteTop() + GetHeight())&&
           GetEnabled() && GetVisible());
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: puts the control in front of all other controls in its parent control
---------------------------------------------------------------------------*/
void CPIControl::BringToFront ()
{
   GetParent()->BringControlToFront(this);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: puts a control behind all other control
---------------------------------------------------------------------------*/
void CPIControl::SendToBack ()
{
   GetParent()->SendControlToBack(this);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: gets the last enabled control that contains X, Y
 parameters : X, Y - the point in screen coord
 others     : this is a compromise (i did not know how else)
---------------------------------------------------------------------------*/
CPIControl *CPIControl::GetControlInPoint(int X, int Y)
{
   return this;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: on paint event handler
 others     : overwrite this method to handle this event
---------------------------------------------------------------------------*/
void CPIControl::OnPaint()
{
	if (mHandleOnPaint) {
		mHandleOnPaint(this);
	}
}

/*---------------------------------------------------------------------------
 description: on mouse on event handler
 parameters : X, Y - the point in screen coord
 others     : overwrite this method to handle this event
---------------------------------------------------------------------------*/
void CPIControl::OnMouseOn (int X, int Y)
{
//   CPIICursor *lCI;  //cursor instance
//   lCI = GetCursorInstance();
//   lCI->SetType(GetMouseType());
   if (mHandleOnMouseOn) {
      mHandleOnMouseOn(this, X, Y);
   }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: on mouse move event handler
 parameters : X, Y - the point in screen coord
 others     : overwrite this method to handle this event
---------------------------------------------------------------------------*/
void CPIControl::OnMouseMove (int X, int Y)
{
   CPIICursor *lCI;  //cursor instance
   lCI = GetCursorInstance();
   lCI->SetType(GetMouseType());
   if (mHandleOnMouseMove)
      mHandleOnMouseMove(this, X, Y);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: on mouse down event handler
 parameters : X, Y - the point in screen coord
 others     : overwrite this method to handle this event
---------------------------------------------------------------------------*/
void CPIControl::OnLeftMouseDown (int X, int Y)
{
   if (GetLeftMouseCaptureEnabled())
   {
      SetWasLeftMouseDown(true);
      CPIScreen *lMSI;
      try
      {
         lMSI = GetMainScreenInstance();
      }
      catch(...)
      {
         throw;
      }
      lMSI->SetMouseCaptureControl(this);
   }

   if (mHandleOnLeftMouseDown)
      mHandleOnLeftMouseDown(this, X, Y);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: on mouse up event handler
 parameters : X, Y - the point in screen coord
 others     : overwrite this method to handle this event
---------------------------------------------------------------------------*/
void CPIControl::OnLeftMouseUp (int X, int Y)
{
   if (GetLeftMouseCaptureEnabled() && GetWasLeftMouseDown())
   {
      CPIScreen *lMSI;
      try
      {
         lMSI = GetMainScreenInstance();
      }
      catch(...)
      {
         throw;
      }
      lMSI->ReleaseMouseCapture();
      SetWasLeftMouseDown(false);
   }
   if (mHandleOnLeftMouseUp)
      mHandleOnLeftMouseUp(this, X, Y);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: on mouse click event handler
 parameters : X, Y - the point in screen coord
 others     : overwrite this method to handle this event
---------------------------------------------------------------------------*/
void CPIControl::OnLeftMouseClick ()
{
   if (mHandleOnLeftMouseClick)
      mHandleOnLeftMouseClick(this);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: on mouse double click event handler
 parameters : X, Y - the point in screen coord
 others     : overwrite this method to handle this event
---------------------------------------------------------------------------*/
void CPIControl::OnLeftMouseDblClick ()
{
   if (mHandleOnLeftMouseDblClick)
      mHandleOnLeftMouseDblClick(this);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: on mouse down event handler
 parameters : X, Y - the point in screen coord
 others     : overwrite this method to handle this event
---------------------------------------------------------------------------*/
void CPIControl::OnRightMouseDown (int X, int Y)
{
   if (GetRightMouseCaptureEnabled())
   {
      SetWasRightMouseDown(true);
      CPIScreen *lMSI;
      try
      {
         lMSI = GetMainScreenInstance();
      }
      catch(...)
      {
         throw;
      }
      lMSI->SetMouseCaptureControl(this);
   }

   if (mHandleOnRightMouseDown)
      mHandleOnRightMouseDown(this, X, Y);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: on mouse up event handler
 parameters : X, Y - the point in screen coord
 others     : overwrite this method to handle this event
---------------------------------------------------------------------------*/
void CPIControl::OnRightMouseUp (int X, int Y)
{
   if (GetRightMouseCaptureEnabled() && GetWasRightMouseDown())
   {
      CPIScreen *lMSI;
      try
      {
         lMSI = GetMainScreenInstance();
      }
      catch(...)
      {
         throw;
      }
      lMSI->ReleaseMouseCapture();
      SetWasRightMouseDown(false);
   }
   if (mHandleOnRightMouseUp)
      mHandleOnRightMouseUp(this, X, Y);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: on mouse click event handler
 others     : overwrite this method to handle this event
---------------------------------------------------------------------------*/
void CPIControl::OnRightMouseClick ()
{
   if (mHandleOnRightMouseClick)
      mHandleOnRightMouseClick(this);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: on mouse double click event handler
 others     : overwrite this method to handle this event
---------------------------------------------------------------------------*/
void CPIControl::OnRightMouseDblClick ()
{
   if (mHandleOnRightMouseDblClick)
      mHandleOnRightMouseDblClick(this);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: on key pressed event handler (called when the keys state modifies)
 others     : overwrite this method to handle this event
---------------------------------------------------------------------------*/
void CPIControl::OnKeyPressed (int key)
{
   if (mHandleOnKeyPressed)
      mHandleOnKeyPressed(this, key);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: on key down event handler (called when the keys state modifies)
 others     : overwrite this method to handle this event
---------------------------------------------------------------------------*/
void CPIControl::OnKeyDown (int key)
{
   if (mHandleOnKeyDown)
      mHandleOnKeyDown(this, key);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: on key up event handler (called when the keys state modifies)
 others     : overwrite this method to handle this event
---------------------------------------------------------------------------*/
void CPIControl::OnKeyUp (int key)
{
   if (mHandleOnKeyUp)
      mHandleOnKeyUp(this, key);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: on control shows event handler (called when the keys state modifies)
 others     : overwrite this method to handle this event
---------------------------------------------------------------------------*/
void CPIControl::OnShow()
{
   if (mHandleOnShow)
      mHandleOnShow(this);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: on control hides event handler (called when the keys state modifies)
 others     : overwrite this method to handle this event
---------------------------------------------------------------------------*/
void CPIControl::OnHide()
{
   if (mHandleOnHide)
      mHandleOnHide(this);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: on control moves event handler (called when the keys state modifies)
              occurs only when the Left or Top changes
 others     : overwrite this method to handle this event
---------------------------------------------------------------------------*/
void CPIControl::OnMove(int oldLeft, int oldTop)
{
	if (mHandleOnMove) {
      mHandleOnMove(this, oldLeft, oldTop);
	}
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: on control resizes event handler (called when the keys state modifies)
 others     : overwrite this method to handle this event
---------------------------------------------------------------------------*/
void CPIControl::OnResize(int oldWidth, int oldHeight)
{
	if (mHandleOnResize) {
		mHandleOnResize(this, oldWidth, oldHeight);
	}
}


/*
//---------------------------------------------------------------------------
// description: on paint event handler
// others     : overwrite this method to handle this event
//---------------------------------------------------------------------------
void CPIControl::OnPaint()
{
   if (mHandleOnPaint != NULL)
      mHandleOnPaint(this);
}

//---------------------------------------------------------------------------
// description: on mouse on event handler
// parameters : X, Y - the point in screen coord
// others     : overwrite this method to handle this event
//---------------------------------------------------------------------------
void CPIControl::OnMouseOn (int X, int Y)
{
//   CPIICursor *lCI;  //cursor instance
//   lCI = GetCursorInstance();
//   lCI->SetType(GetMouseType());
   if (mHandleOnMouseOn != NULL)
      mHandleOnMouseOn(this, X, Y);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// description: on mouse move event handler
// parameters : X, Y - the point in screen coord
// others     : overwrite this method to handle this event
//---------------------------------------------------------------------------
void CPIControl::OnMouseMove (int X, int Y)
{
   CPIICursor *lCI;  //cursor instance
   lCI = GetCursorInstance();
   lCI->SetType(GetMouseType());
   if (mHandleOnMouseMove != NULL)
      mHandleOnMouseMove(this, X, Y);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// description: on mouse down event handler
// parameters : X, Y - the point in screen coord
// others     : overwrite this method to handle this event
//---------------------------------------------------------------------------
void CPIControl::OnLeftMouseDown (int X, int Y)
{
   if (GetLeftMouseCaptureEnabled())
   {
      SetWasLeftMouseDown(true);
      CPIScreen *lMSI;
      try
      {
         lMSI = GetMainScreenInstance();
      }
      catch(...)
      {
         throw;
      }
      lMSI->SetMouseCaptureControl(this);
   }

   if (mHandleOnLeftMouseDown != NULL)
      mHandleOnLeftMouseDown(this, X, Y);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// description: on mouse up event handler
// parameters : X, Y - the point in screen coord
// others     : overwrite this method to handle this event
//---------------------------------------------------------------------------
void CPIControl::OnLeftMouseUp (int X, int Y)
{
   if (GetLeftMouseCaptureEnabled() && GetWasLeftMouseDown())
   {
      CPIScreen *lMSI;
      try
      {
         lMSI = GetMainScreenInstance();
      }
      catch(...)
      {
         throw;
      }
      lMSI->ReleaseMouseCapture();
      SetWasLeftMouseDown(false);
   }
   if (mHandleOnLeftMouseUp != NULL)
      mHandleOnLeftMouseUp(this, X, Y);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// description: on mouse click event handler
// parameters : X, Y - the point in screen coord
// others     : overwrite this method to handle this event
//---------------------------------------------------------------------------
void CPIControl::OnLeftMouseClick ()
{
   if (mHandleOnLeftMouseClick != NULL)
      mHandleOnLeftMouseClick(this);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// description: on mouse double click event handler
// parameters : X, Y - the point in screen coord
// others     : overwrite this method to handle this event
//---------------------------------------------------------------------------
void CPIControl::OnLeftMouseDblClick ()
{
   if (mHandleOnLeftMouseDblClick != NULL)
      mHandleOnLeftMouseDblClick(this);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// description: on mouse down event handler
// parameters : X, Y - the point in screen coord
// others     : overwrite this method to handle this event
//---------------------------------------------------------------------------
void CPIControl::OnRightMouseDown (int X, int Y)
{
   if (GetRightMouseCaptureEnabled())
   {
      SetWasRightMouseDown(true);
      CPIScreen *lMSI;
      try
      {
         lMSI = GetMainScreenInstance();
      }
      catch(...)
      {
         throw;
      }
      lMSI->SetMouseCaptureControl(this);
   }

   if (mHandleOnRightMouseDown != NULL)
      mHandleOnRightMouseDown(this, X, Y);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// description: on mouse up event handler
// parameters : X, Y - the point in screen coord
// others     : overwrite this method to handle this event
//---------------------------------------------------------------------------
void CPIControl::OnRightMouseUp (int X, int Y)
{
   if (GetRightMouseCaptureEnabled() && GetWasRightMouseDown())
   {
      CPIScreen *lMSI;
      try
      {
         lMSI = GetMainScreenInstance();
      }
      catch(...)
      {
         throw;
      }
      lMSI->ReleaseMouseCapture();
      SetWasRightMouseDown(false);
   }
   if (mHandleOnRightMouseUp != NULL)
      mHandleOnRightMouseUp(this, X, Y);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// description: on mouse click event handler
// others     : overwrite this method to handle this event
//---------------------------------------------------------------------------
void CPIControl::OnRightMouseClick ()
{
   if (mHandleOnRightMouseClick != NULL)
      mHandleOnRightMouseClick(this);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// description: on mouse double click event handler
// others     : overwrite this method to handle this event
//---------------------------------------------------------------------------
void CPIControl::OnRightMouseDblClick ()
{
   if (mHandleOnRightMouseDblClick != NULL)
      mHandleOnRightMouseDblClick(this);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// description: on key pressed event handler (called when the keys state modifies)
// others     : overwrite this method to handle this event
//---------------------------------------------------------------------------
void CPIControl::OnKeyPressed (int key)
{
   if (mHandleOnKeyPressed != NULL)
      mHandleOnKeyPressed(this, key);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// description: on key down event handler (called when the keys state modifies)
// others     : overwrite this method to handle this event
//---------------------------------------------------------------------------
void CPIControl::OnKeyDown (int key)
{
   if (mHandleOnKeyDown != NULL)
      mHandleOnKeyDown(this, key);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// description: on key up event handler (called when the keys state modifies)
// others     : overwrite this method to handle this event
//---------------------------------------------------------------------------
void CPIControl::OnKeyUp (int key)
{
   if (mHandleOnKeyUp != NULL)
      mHandleOnKeyUp(this, key);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// description: on control shows event handler (called when the keys state modifies)
// others     : overwrite this method to handle this event
//---------------------------------------------------------------------------
void CPIControl::OnShow()
{
   if (mHandleOnShow != NULL)
      mHandleOnShow(this);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// description: on control hides event handler (called when the keys state modifies)
// others     : overwrite this method to handle this event
//---------------------------------------------------------------------------
void CPIControl::OnHide()
{
   if (mHandleOnHide != NULL)
      mHandleOnHide(this);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// description: on control moves event handler (called when the keys state modifies)
//              occurs only when the Left or Top changes
// others     : overwrite this method to handle this event
//---------------------------------------------------------------------------
void CPIControl::OnMove(int oldLeft, int oldTop)
{
   if (mHandleOnMove != NULL)
      mHandleOnMove(this, oldLeft, oldTop);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// description: on control resizes event handler (called when the keys state modifies)
// others     : overwrite this method to handle this event
//---------------------------------------------------------------------------
void CPIControl::OnResize(int oldWidth, int oldHeight)
{
   if (mHandleOnResize != NULL)
      mHandleOnResize(this, oldWidth, oldHeight);
}
*/

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

