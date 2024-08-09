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

#ifndef ImageExDesignH
#define ImageExDesignH
//---------------------------------------------------------------------------

#include <DesignIntf.hpp>
#include <DesignEditors.hpp>
#include <ToolsApi.hpp>
//---------------------------------------------------------------------------

// gunoi forwarding
// class TImgExEditorForm;

// clasa de editare a proprietatii
class PACKAGE TBitmapProperty : public TClassProperty
{
private:
protected:
public:
  //__fastcall TBitmapProperty(const _di_IFormDesigner ADesigner, int APropCount) : TClassProperty(ADesigner, APropCount) {};
  __fastcall TBitmapProperty(const _di_IDesigner ADesigner, int APropCount) : TClassProperty(ADesigner, APropCount) {};
  virtual void __fastcall Edit();
  virtual TPropertyAttributes __fastcall GetAttributes(void);
  __fastcall virtual ~TBitmapProperty(void);
__published :
};
//---------------------------------------------------------------------------

// clasa de editare a componentei
class PACKAGE TBitmapEditor : public TDefaultEditor
{
protected:
  // BCB6 redefines this method
  virtual void __fastcall EditProperty(const _di_IProperty, bool& Continue);
  //virtual void __fastcall EditProperty(TPropertyEditor* PropertyEditor, bool& Continue, bool& FreeEditor);
public:
  virtual void __fastcall ExecuteVerb(int Index);
  virtual AnsiString __fastcall GetVerb(int Index);
  virtual int __fastcall GetVerbCount(void);
  //__fastcall virtual TBitmapEditor(TComponent* AComponent, _di_IFormDesigner ADesigner);
  __fastcall virtual TBitmapEditor(TComponent* AComponent, _di_IDesigner ADesigner);
  __fastcall virtual ~TBitmapEditor(void);
};
//---------------------------------------------------------------------------

#endif
