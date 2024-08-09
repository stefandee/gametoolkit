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
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------

#include "ImageExDesign.h"
#include "ImageEx.h"
#include "TImgExEditorForm.h"
#include <memory>

#pragma resource "extdlgs.res"

namespace Imageexdesign
{
 void __fastcall PACKAGE Register()
 {
   RegisterComponentEditor(__classid(TImageEx), __classid(TBitmapEditor));
   RegisterPropertyEditor(__typeinfo(Graphics::TBitmap), NULL,
                         "",
                         __classid(TBitmapProperty));
 }
}
//---------------------------------------------------------------------------

//------------------------------------------------------------------------//
//                                                                        //
//                                                                        //
// clasa de editare a proprietatii                                        //
//                                                                        //
//                                                                        //
//------------------------------------------------------------------------//
void __fastcall TBitmapProperty::Edit()
{
  try
  {
    std::auto_ptr<TImgExEditorForm> ImgExEditorForm(new TImgExEditorForm(Application));

    ImgExEditorForm->ImageEx->Bitmap = ((TImageEx*)GetComponent(0))->Bitmap;
    
    if (ImgExEditorForm->ShowModal() == mrOk)
    {
      ((TImageEx*)GetComponent(0))->Clear();
      ((TImageEx*)GetComponent(0))->Bitmap = ImgExEditorForm->ImageEx->Bitmap;
      Modified();
    }  
  }
  catch(...)
  {
  }
}                                         
//---------------------------------------------------------------------------

TPropertyAttributes __fastcall TBitmapProperty::GetAttributes(void)
{
  return (TPropertyAttributes() << paDialog);
}
//---------------------------------------------------------------------------

__fastcall TBitmapProperty::~TBitmapProperty(void)
{
}
//---------------------------------------------------------------------------


//------------------------------------------------------------------------//
//                                                                        //
//                                                                        //
// clasa de editare a componentei                                         //
//                                                                        //
//                                                                        //
//------------------------------------------------------------------------//
//__fastcall TBitmapEditor::TBitmapEditor(TComponent* AComponent, _di_IFormDesigner ADesigner)
__fastcall TBitmapEditor::TBitmapEditor(TComponent* AComponent, _di_IDesigner ADesigner)
                   : TDefaultEditor(AComponent, ADesigner)
{
}
//---------------------------------------------------------------------------

// BCB6 redefines this method
void __fastcall TBitmapEditor::EditProperty(_di_IProperty PropertyEditor, bool& Continue)
//void __fastcall TBitmapEditor::EditProperty(TPropertyEditor* PropertyEditor, bool& Continue, bool& FreeEditor)
{
  String PropName(PropertyEditor->GetName());
  if (strcmpi(PropName.c_str(), "BITMAP") == 0)            
  {
    PropertyEditor->Edit();
    Continue = false;
  }

  //FreeEditor = true;
}
//---------------------------------------------------------------------------

void __fastcall TBitmapEditor::ExecuteVerb(int Index)
{
  if (Index == 0) Edit();
}
//---------------------------------------------------------------------------

AnsiString __fastcall TBitmapEditor::GetVerb(int Index)
{
  return "Bitmap Editor...";
}
//---------------------------------------------------------------------------

int __fastcall TBitmapEditor::GetVerbCount(void)
{
  return 1;
}
//---------------------------------------------------------------------------

__fastcall TBitmapEditor::~TBitmapEditor(void)
{
}
//---------------------------------------------------------------------------

#pragma package(smart_init)
