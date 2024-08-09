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
#pragma hdrstop

#include "PP_CWinControl.h"

/////////////////////////////////////////////////////////////////////////////
//PUBLIC SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: return true if the className is a parent of this object
 parameters : className - className wanted
---------------------------------------------------------------------------*/
bool CPIWinControl::IsDerivedFrom(CPString className)
{
   return ((className == ClassName()) || (CPIControl::IsDerivedFrom(className)));
}

/*---------------------------------------------------------------------------
 description: paints the control and then paints its children controls
---------------------------------------------------------------------------*/
void CPIWinControl::Update()
{
   if (!GetEnabled())
   {
     return;
   }

   CPIControl::Update();
   for(int i=0; i<GetControls()->Count(); i++)
      //(*GetControls())[i]->Update();
      (GetControls()->GetControl(i))->Update();
}

/*---------------------------------------------------------------------------
 description: paints the control and then paints its children controls
---------------------------------------------------------------------------*/
void CPIWinControl::Paint()
{
    if (!GetVisible())
    {
        return;
    }

   CPIControl::Paint();

   //if (!GetEnabled())
   //{
   //  SetClipRegion();
   //  GetGraphicInstance()->FillRectangle(NULL, GetAbsoluteBoundsRect(), 0x7f7f7f);
     //GetGraphicInstance();
   //}

   for(int i=GetControls()->Count() - 1; i>=0; i--)
   /*
      if ((*GetControls())[i]->GetVisible() == true)
         (*GetControls())[i]->Paint();
   */
      if ((GetControls()->GetControl(i))->GetVisible() == true)
         (GetControls()->GetControl(i))->Paint();

}

/*---------------------------------------------------------------------------
 description: adds a control in the controls list
 parameters : Control - the one it's all about
---------------------------------------------------------------------------*/
void CPIWinControl::AddControl (CPIControl *Control)
{
   if (ContainControl(Control) == false)
   {
      GetControls()->Add(Control);
   }
}

/*---------------------------------------------------------------------------
 description: removes the control from list, but doesnt free its memory
 parameters : Control - the one it's all about
---------------------------------------------------------------------------*/
void CPIWinControl::RemoveControl (CPIControl *Control)
{
   if (ContainControl(Control))
   {
      for(int i = 0; i < GetControls()->Count(); i++)
      {
        if (GetControls()->GetControl(i) == Control)
        {
          GetControls()->Remove(i);
          return;
        }
      }
      //GetControls()->Delete(Control);
   }
}

/*---------------------------------------------------------------------------
 description: removes all controls from list, but not frees their memory
---------------------------------------------------------------------------*/
void CPIWinControl::RemoveAllControls ()
{
   for(int i = 0; i < GetControls()->Count(); i++)
   {
     GetControls()->Remove(i);
   }
}

/*---------------------------------------------------------------------------
 description: removes the control from list and frees its memory
 parameters : Control - the one it's all about
---------------------------------------------------------------------------*/
void CPIWinControl::FreeControl (CPIControl *Control)
{
   if (ContainControl(Control))
   {
      for(int i = 0; i < GetControls()->Count(); i++)
      {
        if (GetControls()->GetControl(i) == Control)
        {
          GetControls()->Delete(i);
          return;
        }
      }
      //GetControls()->Delete(Control);
      //GetControls()->Capacity = GetControls()->Count;
   }
}

/*---------------------------------------------------------------------------
 description: removes all controls from list and frees their memory
---------------------------------------------------------------------------*/
void CPIWinControl::FreeAllControls ()
{
   //not implemented
   for(int i = 0; i < GetControls()->Count(); i++)
   {
     GetControls()->Delete(i);
   }
}

/*---------------------------------------------------------------------------
 description: returns true if the control is contained by this
 parameters : Control - the one it's all about
---------------------------------------------------------------------------*/
bool CPIWinControl::ContainControl (CPIControl *Control)
{
   return GetControls()->Contain(Control);
}

/*---------------------------------------------------------------------------
 description: sends a control in the first position (that means it will be painted last)
 parameters : Control - the one it's all about
---------------------------------------------------------------------------*/
void CPIWinControl::BringControlToFront (CPIControl *Control)
{
   if (ContainControl(Control))
   {
      for(int i = 0; i < GetControls()->Count(); i++)
      {
        if (GetControls()->GetControl(i) == Control)
        {
          GetControls()->Remove(i);
          GetControls()->Insert(Control, 0);
          return;
        }
      }
      //GetControls()->Remove(Control);
      //GetControls()->Insert(0, Control);
   }
}
/*---------------------------------------------------------------------------
 description: sends a control in the first position (that means it will be painted first)
 parameters : Control - the one it's all about
---------------------------------------------------------------------------*/
void CPIWinControl::SendControlToBack (CPIControl *Control)
{
   if (ContainControl(Control))
   {
      for(int i = 0; i < GetControls()->Count(); i++)
      {
        if (GetControls()->GetControl(i) == Control)
        {
          GetControls()->Remove(i);
          GetControls()->Add(Control);
          return;
        }
      }
   }
}

/*---------------------------------------------------------------------------
 description: gets the last enabled control that contains X, Y
 parameters : X, Y - the point in screen coord
---------------------------------------------------------------------------*/
CPIControl *CPIWinControl::GetControlInPoint(int X, int Y)
{
   if (IsPointInside(CPPoint(X, Y)))
   {
      for(int i=0; i < GetControls()->Count(); i++)
      {
         if ((GetControls()->GetControl(i))->IsPointInside(CPPoint(X, Y)) == true)
//            if ((*GetControls())[i]->IsDerivedFrom(PICN_WINCONTROL))
//               return (((CPIWinControl *)GetControls()->GetControl(i))->GetControlInPoint(X, Y));
//            else
//               return GetControls()->GetControl(i);
            //return GetControls()->GetControl(i)->GetControlInPoint(X, Y);
            return (GetControls()->GetControl(i))->GetControlInPoint(X, Y);
      }
      return this;
   }
   else
      return NULL;
}

/*---------------------------------------------------------------------------
  description: on hide event default handler
---------------------------------------------------------------------------*/
void CPIWinControl::OnShow()
{
   CPIControl::OnShow();
   if (GetVisible() == true)
   {//only if the control was shown
      for(int i=0; i<GetControls()->Count(); i++)
         (GetControls()->GetControl(i))->OnShow();
   }
}

/*---------------------------------------------------------------------------
  description: on hide event default handler
---------------------------------------------------------------------------*/
void CPIWinControl::OnHide()
{
   CPIControl::OnHide();
   if (GetVisible() == false)
   {//only if the control was shown
      for(int i=0; i<GetControls()->Count(); i++)
         (GetControls()->GetControl(i))->OnHide();
   }
}

/////////////////////////////////////////////////////////////////////////////
//PROTECTED SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: constructor
---------------------------------------------------------------------------*/
CPIWinControl::CPIWinControl(CPIWinControl *Parent) : CPIControl(Parent)
{
   mControls = new CPIControlList();
   //mControls = new CPList<CPIControl>(1024);
}

/////////////////////////////////////////////////////////////////////////////
//PUBLIC SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: destructor
---------------------------------------------------------------------------*/
CPIWinControl::~CPIWinControl()
{
   FreeAllControls();
   delete mControls;
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

