//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    // 
// Copyright (C) 1998, 1999, 2000, 2001 Stefan Dicu & Tudor Girba            //
//                                                                           //
// PPTactical Engine is free software; you can redistribute it               //
// and/or modify it under the terms of the GNU General Public License as     // 
// published by the Free Software Foundation; either version 2 of the        //
// License, or (at your option) any later version.                           //
//                                                                           //  
// PPTactical Engine is distributed in the hope that it will be useful, but  //
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY// 
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License  //
// for more details.                                                         //
//                                                                           // 
// You should have received a copy of the GNU General Public License         //
// along with PPTactical Engine; if not, write to the Free Software          //
// Foundation Inc. 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   // 
//-----------------------------------------------------------------------------
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "cell.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TCellForm *CellForm;
//---------------------------------------------------------------------------
__fastcall TCellForm::TCellForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TCellForm::ed_xsizeExit(TObject *Sender)
{
  try
  {
    int tempint = ed_xsize->Text.ToInt();
    if (tempint == 0)
    {
      ed_xsize->Text = "1";
    }
  }
  catch(...)
  {
	MessageBox(0, L"Error", L"Invalid value (>0 req)", MB_OK);
	ed_xsize->Text = "1";
  }
}
//---------------------------------------------------------------------------

void __fastcall TCellForm::ed_ysizeExit(TObject *Sender)
{
  try
  {
	int tempint = ed_ysize->Text.ToInt();
	if (tempint == 0)
	{
	  ed_ysize->Text = "1";
	}
  }
  catch(...)
  {
    MessageBox(0, L"Error", L"Invalid value (>0 req)", MB_OK);
    ed_ysize->Text = "1";
  }
}
//---------------------------------------------------------------------------

