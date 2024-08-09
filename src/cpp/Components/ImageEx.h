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
#ifndef ImageExH
#define ImageExH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
//#include <DsgnIntf.hpp>

#include <StdCtrls.hpp>
#include <Graphics.hpp>
#include <Forms.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <System.hpp>

class TBitmapEx : public TBitmap
{
  private:

  protected:

  public:
    __fastcall TBitmapEx();
    //virtual void __fastcall Assign(Classes::TPersistent* Source);
    virtual void __fastcall LoadFromFile(const AnsiString FileName);
    virtual void __fastcall SaveToFile(const AnsiString FileName);
    __fastcall virtual ~TBitmapEx();

  __published:
};

// componenta care extinde TImage
class PACKAGE TImageEx : public TGraphicControl
{
private:
  bool               FCenter;
  bool               FAutoSize;
  bool               FStretch;
  //bool               FTransparent;
  bool               FFit;
  TBitmapEx*         FBitmap;

  void __fastcall    SetFit(bool value);
  void __fastcall    SetTransparent(bool value);
  void __fastcall    SetStretch(bool value);
  void __fastcall    SetAutoSize(bool value);
  void __fastcall    SetCenter(bool value);
  void __fastcall    SetBitmap(TBitmapEx *newBitmap);

  TBitmapEx* __fastcall GetBitmap();
  TCanvas*   __fastcall GetCanvas();
  bool __fastcall    GetTransparent();

protected:

public:
  __fastcall TImageEx(TComponent* Owner);
  virtual void __fastcall Paint(void);
  void __fastcall Clear();
  __fastcall ~TImageEx();

__published:
  __property bool Center       = {read = FCenter,      write = SetCenter, default = false};
  __property bool AutoSize     = {read = FAutoSize,    write = SetAutoSize};
  __property bool Stretch      = {read = FStretch,     write = SetStretch};
  __property bool FitInWindow  = {read = FFit,         write = SetFit};
  __property bool Transparent  = {read = GetTransparent, write = SetTransparent};
  __property TBitmapEx *Bitmap = {read = GetBitmap, write = SetBitmap};
  //__property TCanvas   *Canvas = {read = GetCanvas};

  __property Align;
  __property Tag;
  __property DragCursor;
  __property DragKind;
  __property DragMode;
  __property Enabled;
  __property Visible;
  __property Anchors;
  __property ShowHint;

  __property OnClick;
  __property OnDblClick;
  __property OnDragDrop;
  __property OnDragOver;
  __property OnEndDock;
  __property OnEndDrag;
  __property OnMouseDown;
  __property OnMouseUp;
  __property OnMouseMove;
  __property OnStartDock;
  __property OnStartDrag;
};
//---------------------------------------------------------------------------

// componenta de dialog pentru open file, cu preview al fisierului grafic
class PACKAGE TOpenImageEx : public TOpenDialog
{
private:
  TBitmapEx* FBitmapEx;
  TPaintBox* FPaintBox;
  TPanel*    FPanel, *FOutLine;
  UnicodeString TemplateConst;

protected:
  DYNAMIC void __fastcall DoShow            (void);
  DYNAMIC void __fastcall DoClose           (void);
  DYNAMIC void __fastcall DoSelectionChange (void);
  void __fastcall DoThePaint                (void);
  void __fastcall DoClear                   (void);

public:
  __fastcall TOpenImageEx         (TComponent* Owner);
  virtual bool __fastcall Execute ();
  __fastcall ~TOpenImageEx        ();

__published:
};
//---------------------------------------------------------------------------

//typedef void __fastcall(* TDesignTimeHook)(void);

//extern PACKAGE TDesignTimeHook DesignTimeHook;

#endif
