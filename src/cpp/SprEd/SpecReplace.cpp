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

#include "SpecReplace.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ImageEx"
#pragma resource "*.dfm"
TFormReplace *FormReplace;
//---------------------------------------------------------------------------

__fastcall TFormReplace::TFormReplace(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TFormReplace::edBaseDblClick(TObject *Sender)
{
  if (OpenSpecial->Execute())
  {
    pathToFiles  = ExtractFilePath(OpenSpecial->FileName);

    baseFile     = ExtractFileName(OpenSpecial->FileName);
    // elimin si extensia, ultimul "." determina extensia
    int lastPos = baseFile.LastDelimiter(".");
    baseFile.SetLength(lastPos - 1);

    edBase->Text = baseFile;
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormReplace::edStartChange(TObject *Sender)
{
   //startFile = 0;
   try
   {
      startFile = edStart->Text.ToInt();
   }
   catch(const EConvertError &e)
   {
      startFile = 0;
      edStart->Text = "0";
   }
}
//---------------------------------------------------------------------------

void __fastcall TFormReplace::edRowsChange(TObject *Sender)
{
   //rows = 0;
   try
   {
      rows = edRows->Text.ToInt();
   }
   catch(const EConvertError &e)
   {
      rows = 0;
      edRows->Text = "0";
   }
}
//---------------------------------------------------------------------------

void __fastcall TFormReplace::edColChange(TObject *Sender)
{
   //cols = 0;
   try
   {
     cols = edCol->Text.ToInt();
   }
   catch(const EConvertError &e)
   {
      cols = 0;
      edCol->Text = "0";
   }
}
//---------------------------------------------------------------------------

void __fastcall TFormReplace::cbFormatChange(TObject *Sender)
{
  switch(cbFormat->ItemIndex)
  {
    case 0 :
      format = _000;
      break;

    case 1 :
      format = _0000;
      break;

    default :
      format = _000;
      break;
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormReplace::edFirstChange(TObject *Sender)
{
   //firstFile = 0;
   try
   {
      firstFile = edFirst->Text.ToInt();
   }
   catch(const EConvertError &e)
   {
      firstFile = 0;
      edFirst->Text = "0";
   }
}
//---------------------------------------------------------------------------

void __fastcall TFormReplace::edLastChange(TObject *Sender)
{
   //lastFile = 0;
   try
   {
      lastFile = edLast->Text.ToInt();
   }
   catch(const EConvertError &e)
   {
      lastFile = 0;
      edLast->Text = "0";
   }
}
//---------------------------------------------------------------------------

void __fastcall TFormReplace::FormShow(TObject *Sender)
{
  edStartChange(this);
  edRowsChange(this);
  edColChange(this);
  edFirstChange(this);
  cbFormatChange(this);
  edLastChange(this);
}
//---------------------------------------------------------------------------



