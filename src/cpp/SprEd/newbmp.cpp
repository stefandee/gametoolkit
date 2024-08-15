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

#include "newbmp.h"
#include "ExprEval.h"
#include "PP_Stdlib.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TNewForm *NewForm;
//---------------------------------------------------------------------------
__fastcall TNewForm::TNewForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TNewForm::ed_xbmpExit(TObject *Sender)
{
  int tempint;

  try
  {
	tempint = CPExprEval().Evaluate(WStringToString(ed_xbmp->Text.c_str()));
  }
  catch(...)
  {
	MessageBox(0, L"Cannot evaluate", L"Error", MB_OK);
    tempint = 128;
  }

  if (tempint <= 0)
  {
    tempint = 128;
  }

  ed_xbmp->Text = AnsiString(tempint);
}
//---------------------------------------------------------------------------
void __fastcall TNewForm::ed_ybmpExit(TObject *Sender)
{
  int tempint;

  try
  {
	tempint = CPExprEval().Evaluate(WStringToString(ed_ybmp->Text.c_str()));
  }
  catch(...)
  {
	MessageBox(0, L"Cannot evaluate", L"Error", MB_OK);
    tempint = 128;
  }

  if (tempint <= 0)
  {
    tempint = 128;
  }

  ed_ybmp->Text = AnsiString(tempint);
}
//---------------------------------------------------------------------------
void __fastcall TNewForm::ed_xsizeExit(TObject *Sender)
{
  int tempint;

  try
  {
	tempint = CPExprEval().Evaluate(WStringToString(ed_xsize->Text.c_str()));
  }
  catch(...)
  {
	MessageBox(0, L"Cannot evaluate", L"Error", MB_OK);
    tempint = 128;
  }

  if (tempint <= 0)
  {
    tempint = 128;
  }

  ed_xsize->Text = UnicodeString(tempint);
}
//---------------------------------------------------------------------------
void __fastcall TNewForm::ed_ysizeExit(TObject *Sender)
{
  int tempint;

  try
  {
	tempint = CPExprEval().Evaluate(WStringToString(ed_ysize->Text.c_str()));
  }
  catch(...)
  {
	MessageBox(0, L"Cannot evaluate", L"Error", MB_OK);
	tempint = 128;
  }

  if (tempint <= 0)
  {
	tempint = 128;
  }

  ed_ysize->Text = UnicodeString(tempint);
}
//---------------------------------------------------------------------------