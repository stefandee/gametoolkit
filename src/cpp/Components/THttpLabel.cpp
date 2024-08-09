//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    //
// Copyright (C) 1998-2004 Stefan Dicu                                       //
//                                                                           //
// PPTactical Engine is free software; you can redistribute it               //
// and/or modify it under the terms of the GNU Lesser General Public License //
// as published by the Free Software Foundation; either version 2.1 of the   //
// License, or (at your option) any later version.                           //
//                                                                           //
// PPTactical Engine is distributed in the hope that it will be useful, but  //
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY//
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public   //
// License for more details.                                                 //
//                                                                           //
// You should have received a copy of the GNU Lesser General Public License  //
// along with PPTactical Engine; if not, write to the Free Software          //
// Foundation Inc. 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   //
//-----------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop
#pragma resource "*.res"

#include "THttpLabel.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

//const TFont* DEFAULT_FONT = new TFont();

static inline void ValidCtrCheck(THttpLabel *)
{
        new THttpLabel(NULL);
}
//---------------------------------------------------------------------------

__fastcall THttpLabel::THttpLabel(TComponent* Owner)
        : TCustomLabel(Owner)
{
  OnClick    = MyOnClick;
  OnDblClick = MyOnDblClick;
  OnMouseEnter = MyOnMouseEnter;
  OnMouseLeave = MyOnMouseLeave;
  LinkStyle  = TLinkSingleClick;
  FLink      = "www.something.com";
  
  FFontMouseOver = new Graphics::TFont();

  // will be allocated when needed - when the mouse enters this wonderful label
  mTempFont = 0;
}
//---------------------------------------------------------------------------

__fastcall THttpLabel::~THttpLabel()
{
  delete mTempFont;
}
//---------------------------------------------------------------------------

void __fastcall THttpLabel::SetFontMouseOver(TFont* _v)
{
  if (_v)
  {
    FFontMouseOver = _v;
  }
}
//---------------------------------------------------------------------------

void __fastcall THttpLabel::MyOnClick(TObject* Sender)
{
  if (LinkStyle == TLinkSingleClick)
  {
    StartNavigator(Caption);
  }
}
//---------------------------------------------------------------------------

void __fastcall THttpLabel::MyOnDblClick(TObject* Sender)
{
  if (LinkStyle == TLinkDoubleClick)
  {
    StartNavigator(Caption);
  }
}
//---------------------------------------------------------------------------

void __fastcall THttpLabel::MyOnMouseEnter(TObject* Sender)
{
  // workaround - i cannot set this in the constructor
  // probably it can be done with a hidden property? 
  if (!mTempFont)
  {
    mTempFont = new TFont;
    mTempFont->Assign(Font);
  }

  Font->Assign(FFontMouseOver);
}
//---------------------------------------------------------------------------

void __fastcall THttpLabel::MyOnMouseLeave(TObject* Sender)
{
  Font->Assign(mTempFont);
}
//---------------------------------------------------------------------------

void THttpLabel::StartNavigator(AnsiString _link)
{
  // see what is the default html navigator
  //Application->MessageBox(_link.c_str(), "Test");

  //char lUha[4096];

  //FindExecutable("http://www.bla.com", "", lUha);

  ShellExecute(0, "open", FLink.c_str(), "", "", SW_SHOW);
}
//---------------------------------------------------------------------------

namespace Thttplabel
{
        void __fastcall PACKAGE Register()
        {
                 TComponentClass classes[1] = {__classid(THttpLabel)};
                 RegisterComponents("PPTactical", classes, 0);
        }
}
//---------------------------------------------------------------------------
 