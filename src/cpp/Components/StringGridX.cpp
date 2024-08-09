//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop

#include "StringGridX.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TStringGridX *)
{
    new TStringGridX(NULL);
}
//---------------------------------------------------------------------------

__fastcall TStringGridX::TStringGridX(TComponent* Owner)
    : TStringGrid(Owner)
{
  // setup the string grid
  FixedCols        = 0;
  DefaultRowHeight = 16;
  RowCount         = 2;

  Options << goFixedVertLine << goFixedHorzLine << goVertLine << goHorzLine << goRangeSelect << goColSizing << goRowSelect;

  mPopMenu = new TPopupMenu(this);

  // setup the popup menu configuration
  mPopMenu->Items->Clear();

  TMenuItem* lItem;

  // add new
  lItem = new TMenuItem(this);
  lItem->Caption = "New";
  lItem->Tag = 0;
  lItem->OnClick = PopupMenuNewClick;
  mPopMenu->Items->Add(lItem);

  // insert
  lItem = new TMenuItem(this);
  lItem->Caption = "Insert";
  lItem->Tag = 2;
  lItem->OnClick = PopupMenuInsertClick;
  mPopMenu->Items->Add(lItem);

  // clone
  lItem = new TMenuItem(this);
  lItem->Caption = "Clone";
  lItem->Tag = 3;
  lItem->OnClick = PopupMenuCloneClick;
  mPopMenu->Items->Add(lItem);

  // delete
  lItem = new TMenuItem(this);
  lItem->Caption = "Delete";
  lItem->Tag = 4;
  lItem->OnClick = PopupMenuDeleteClick;
  mPopMenu->Items->Add(lItem);

  AddSep();

  // cut
  lItem = new TMenuItem(this);
  lItem->Caption = "Cut";
  lItem->Tag = 5;
  lItem->OnClick = PopupMenuCutClick;
  mPopMenu->Items->Add(lItem);

  // copy
  lItem = new TMenuItem(this);
  lItem->Caption = "Copy";
  lItem->Tag = 6;
  lItem->OnClick = PopupMenuCopyClick;
  mPopMenu->Items->Add(lItem);

  // paste
  lItem = new TMenuItem(this);
  lItem->Caption = "Paste";
  lItem->Tag = 7;
  lItem->OnClick = PopupMenuPasteClick;
  mPopMenu->Items->Add(lItem);

  AddSep();

  // move up
  lItem = new TMenuItem(this);
  lItem->Caption = "Move Up";
  lItem->Tag = 8;
  lItem->OnClick = PopupMenuMoveUpClick;
  mPopMenu->Items->Add(lItem);

  // move down
  lItem = new TMenuItem(this);
  lItem->Caption = "Move Down";
  lItem->Tag = 9;
  lItem->OnClick = PopupMenuMoveDownClick;
  mPopMenu->Items->Add(lItem);

  PopupMenu = mPopMenu;
}
//---------------------------------------------------------------------------

void TStringGridX::AddSep()
{
  TMenuItem* lBar;

  lBar = new TMenuItem(this);
  lBar->Caption = "-";
  mPopMenu->Items->Add(lBar);
}
//---------------------------------------------------------------------------

void __fastcall TStringGridX::PopupMenuNewClick(TObject* Sender)
{
  if (FOnPopNew)
  {
    OnPopNew(Sender);
  }
}
//---------------------------------------------------------------------------

void __fastcall TStringGridX::PopupMenuInsertClick(TObject* Sender)
{
  if (FOnPopInsert)
  {
    OnPopInsert(Sender);
  } 
}
//---------------------------------------------------------------------------

void __fastcall TStringGridX::PopupMenuCloneClick(TObject* Sender)
{
  if (FOnPopClone)
  {
    OnPopClone(Sender);
  }    
}
//---------------------------------------------------------------------------

void __fastcall TStringGridX::PopupMenuDeleteClick(TObject* Sender)
{
  if (FOnPopDelete)
  {
    OnPopDelete(Sender);
  }  
}
//---------------------------------------------------------------------------

void __fastcall TStringGridX::PopupMenuCutClick(TObject* Sender)
{
  if (FOnPopCut)
  {
    OnPopCut(Sender);
  }  
}
//---------------------------------------------------------------------------

void __fastcall TStringGridX::PopupMenuCopyClick(TObject* Sender)
{
  if (FOnPopCopy)
  {
    OnPopCopy(Sender);
  }
}
//---------------------------------------------------------------------------

void __fastcall TStringGridX::PopupMenuPasteClick(TObject* Sender)
{
  if (FOnPopPaste)
  {
    OnPopPaste(Sender);
  }
}
//---------------------------------------------------------------------------

void __fastcall TStringGridX::PopupMenuMoveUpClick(TObject* Sender)
{
  if (FOnPopMoveUp)
  {
    OnPopMoveUp(Sender);
  }
}
//---------------------------------------------------------------------------

void __fastcall TStringGridX::PopupMenuMoveDownClick(TObject* Sender)
{
  if (FOnPopMoveDown)
  {
    OnPopMoveDown(Sender);
  }
}
//---------------------------------------------------------------------------

namespace Stringgridx
{
    void __fastcall PACKAGE Register()
    {
         TComponentClass classes[1] = {__classid(TStringGridX)};
         RegisterComponents("Piron", classes, 0);
    }
}
//---------------------------------------------------------------------------
 