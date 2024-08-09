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
#pragma hdrstop

#include "PP_CForm.h"
#include "PPIG_InterfaceDef.h"

//TODO: temporary
#include "PPIG_CScreen.h"

/////////////////////////////////////////////////////////////////////////////
//PROTECTED SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: contructor
---------------------------------------------------------------------------*/
CPIForm::CPIForm(CPIWinControl *Parent) : CPIWinControl(Parent)
{
   mModal            = false;
   mParentControl    = 0;
   mModalResult      = PMR_CANCEL;
   //mHandleOnClose    = NULL;
   SetVisible(false);
}
/////////////////////////////////////////////////////////////////////////////
//PUBLIC SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: destructor
---------------------------------------------------------------------------*/
CPIForm::~CPIForm()
{
}

/*---------------------------------------------------------------------------
 description: return true if the className is a parent of this object
 parameters : className - className wanted
---------------------------------------------------------------------------*/
bool CPIForm::IsDerivedFrom(CPString className)
{
   return ((className == ClassName()) || (CPIWinControl::IsDerivedFrom(className)));
}


/*---------------------------------------------------------------------------
 description: paint implementation
---------------------------------------------------------------------------*/
void CPIForm::Paint()
{
   SetClipRegion();
   //GetGraphicInstance()->Rectangle(NULL, GetAbsoluteBoundsRect(), 0xffffff);
   CPIWinControl::Paint();
}

/*---------------------------------------------------------------------------
 description: closes the form
---------------------------------------------------------------------------*/
void CPIForm::Close()
{
   mModalResult = PMR_CANCEL;
   OnClose();
}

/*---------------------------------------------------------------------------
 description: shows the form modal
 parameters : the parent control
---------------------------------------------------------------------------*/
void CPIForm::ShowModal(CPIControl *ParentControl)
{
   //ParentControl->SetEnabled(false);
   //SetParentControl(ParentControl);

   GetMainScreenInstance()->SetModalDialog(this);

   SetModal(true);
   SetEnabled(true);
   SetMouseType(0);
   //ParentControl->SetMouseType(0);
   Show();
}

/*---------------------------------------------------------------------------
 description: close form event
---------------------------------------------------------------------------*/
void CPIForm::OnClose()
{
   if (GetModal() == true)
   {
      SetModal(false);
      GetParentControl()->SetEnabled(true);
      //SetParentControl(0);
   }

   Hide();

  if (mHandleOnClose)
  {
      mHandleOnClose(this);
  }

  /*
   if (mHandleOnClose != NULL)
      mHandleOnClose(this);
  */
}

void CPIForm::OnLeftMouseClick()
{
   CPIWinControl::OnLeftMouseClick();
   Close();
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: modal
---------------------------------------------------------------------------*/
void CPIForm::SetModal(bool value)
{
   if (mParentControl != NULL)
   {
      if (mModal == true &&value == false)
         mParentControl->SetEnabled(true);
      if (mModal == false &&value == true)
         mParentControl->SetEnabled(false);

      mModal = value;
   }
}
//---------------------------------------------------------------------------

#ifdef PPT_COMPILER_BORLAND
#pragma package(smart_init)
#endif
