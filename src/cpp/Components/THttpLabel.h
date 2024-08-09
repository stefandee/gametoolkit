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

//-----------------------------------------------------------------------------
//  Unit              : Http Label
//
//  Version           : 0.8
//
//  Description       :
//    * on click, opens a web page into the default browser
//    * the lack of a such component in bcb6 is disturbing ;)
//    * needed for the tools Help->About forms :D
//    * it does not verify the link (URL); you may write there a file and
//      it will open it with the default application (e.g., a jpg or other)
//    * still experimental
//
//  Hystory           :
//    [11.09.2004] - [Karg] - created
//-----------------------------------------------------------------------------

#ifndef THttpLabelH
#define THttpLabelH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------

enum TLinkStyle { TLinkSingleClick, TLinkDoubleClick };

static const TFont* const DEFAULT_FONT = new TFont();

class PACKAGE THttpLabel : public TCustomLabel
{
private:
  AnsiString       FLink;
  TLinkStyle       FLinkStyle;
  Graphics::TFont* FFontMouseOver, *mTempFont;

  void StartNavigator(AnsiString _link);
  
  void __fastcall SetFontMouseOver(TFont* _v);
  void __fastcall MyOnClick(TObject* Sender);
  void __fastcall MyOnDblClick(TObject* Sender);
  void __fastcall MyOnMouseEnter(TObject* Sender);
  void __fastcall MyOnMouseLeave(TObject* Sender);

protected:

public:
        __fastcall THttpLabel(TComponent* Owner);
        __fastcall ~THttpLabel();
__published:
  // editable - this component
  __property TLinkStyle LinkStyle = {read = FLinkStyle, write = FLinkStyle, default = TLinkSingleClick};
  __property AnsiString Link      = {read = FLink, write=FLink, stored=true};
  __property Graphics::TFont* FontMouseOver = {read=FFontMouseOver, write=SetFontMouseOver, stored = true};


  // editable - predefined
  __property Caption;
  __property Font;

  // events
  __property OnClick;
  __property OnDblClick;
};
//---------------------------------------------------------------------------
#endif
