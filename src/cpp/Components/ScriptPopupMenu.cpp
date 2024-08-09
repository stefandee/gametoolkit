//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ScriptPopupMenu.h"
#include <Clipbrd.hpp>
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TScriptPopupMenu *)
{
        new TScriptPopupMenu(NULL);
}
//---------------------------------------------------------------------------
__fastcall TScriptPopupMenu::TScriptPopupMenu(TComponent* Owner)
        : TPopupMenu(Owner)
{
  Items->Clear();

  TMenuItem* lBar;

  // undo
  mUndo = new TMenuItem(this);
  mUndo->Caption = "Undo";
  mUndo->Tag = 0;
  mUndo->OnClick = PopupMenuItemsClick;
  Items->Add(mUndo);

  lBar = new TMenuItem(this);
  lBar->Tag = 1;
  lBar->Caption = "-";
  Items->Add(lBar);

  // text ops
  mCut = new TMenuItem(this);
  mCut->Caption = "Cut";
  mCut->Tag = 2;
  mCut->OnClick = PopupMenuItemsClick;
  Items->Add(mCut);

  mCopy = new TMenuItem(this);
  mCopy->Caption = "Copy";
  mCopy->Tag = 3;
  mCopy->OnClick = PopupMenuItemsClick;
  Items->Add(mCopy);

  mPaste = new TMenuItem(this);
  mPaste->Caption = "Paste";
  mPaste->Tag = 4;
  mPaste->OnClick = PopupMenuItemsClick;
  Items->Add(mPaste);

  mSelect = new TMenuItem(this);
  mSelect->Caption = "Select All";
  mSelect->Tag = 5;
  mSelect->OnClick = PopupMenuItemsClick;
  Items->Add(mSelect);

  mClear = new TMenuItem(this);
  mClear->Caption = "Clear";
  mClear->Tag = 6;
  mClear->OnClick = PopupMenuItemsClick;
  Items->Add(mClear);

  lBar = new TMenuItem(this);
  lBar->Tag = 7;
  lBar->Caption = "-";
  Items->Add(lBar);

  // load/save
  mLoad = new TMenuItem(this);
  mLoad->Caption = "Load...";
  mLoad->Tag = 8;
  mLoad->OnClick = PopupMenuItemsClick;
  Items->Add(mLoad);

  mSave = new TMenuItem(this);
  mSave->Caption = "Save...";
  mSave->Tag = 9;
  mSave->OnClick = PopupMenuItemsClick;
  Items->Add(mSave);

  lBar = new TMenuItem(this);
  lBar->Tag = 10;
  lBar->Caption = "-";
  Items->Add(lBar);

  mOpenDialog = new TOpenDialog(this);
  mSaveDialog = new TSaveDialog(this);

  OnPopup = PopupMenuOnPopup;

  //C Script Language files|*.csl|All files|*.*
  //data/scripts
  //csl
}
//---------------------------------------------------------------------------

__fastcall TScriptPopupMenu::~TScriptPopupMenu()
{
  delete mOpenDialog;
  delete mSaveDialog;
}
//---------------------------------------------------------------------------

void __fastcall TScriptPopupMenu::PopupMenuItemsClick(TObject *Sender)
{
  TMenuItem *ClickedItem = dynamic_cast<TMenuItem *>(Sender);

  TComponent *pComponent = PopupComponent;

  if (!pComponent)
  {
    return;
  }

  if (!pComponent->ClassNameIs("TMemo"))
  {
    return;
  }

  if (ClickedItem)
  {
    switch (ClickedItem->Tag)
    {
      case 5:
        ((TMemo *)pComponent)->SelectAll();
        break;
        
      case 6:
        ((TMemo *)pComponent)->Clear();
        break;

      case 4:
        ((TMemo *)pComponent)->PasteFromClipboard();
        break;

      case 3:
        ((TMemo *)pComponent)->CopyToClipboard();
        break;

      case 2:
        ((TMemo *)pComponent)->CutToClipboard();
        break;

      case 0:
        ((TMemo *)pComponent)->Undo();
        break;

      case 8:
        LoadFromFile((TMemo *)pComponent);
        break;

      case 9:
        SaveToFile((TMemo *)pComponent);
        break;
    }
  }
}
//---------------------------------------------------------------------------

void TScriptPopupMenu::LoadFromFile(TMemo* _memo)
{
  if (mOpenDialog->Execute())
  {
    try
    {
      _memo->Clear();
      _memo->Lines->LoadFromFile(mOpenDialog->FileName);
    }
    catch(...)
    {
	  Application->MessageBox(L"Cannot load file", L"Error", MB_OK);
    }
  }
}
//---------------------------------------------------------------------------

void TScriptPopupMenu::SaveToFile(TMemo* _memo)
{
  if (mSaveDialog->Execute())
  {
    try
    {
      _memo->Lines->SaveToFile(mSaveDialog->FileName);
    }
    catch(...)
    {
      Application->MessageBox(L"Cannot save file", L"Error", MB_OK);
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TScriptPopupMenu::PopupMenuOnPopup(TObject *Sender)
{
  TComponent *pComponent = PopupComponent;

  if (!pComponent)
  {
    return;
  }

  if (!pComponent->ClassNameIs("TMemo"))
  {
    return;
  }

  TMemo* lMemo = ((TMemo *)pComponent);

  mPaste->Enabled = Clipboard()->HasFormat(CF_TEXT);
  mCopy->Enabled  = !(lMemo->SelText.IsEmpty()); 
  mCut->Enabled   = !(lMemo->SelText.IsEmpty()); 
}
//---------------------------------------------------------------------------

void TScriptPopupMenu::SetInitialDir(AnsiString _v)
{
  mOpenDialog->InitialDir = _v;
  mSaveDialog->InitialDir = _v;
}
//---------------------------------------------------------------------------

void TScriptPopupMenu::SetFilter(AnsiString _v)
{
  mOpenDialog->Filter = _v;
  mSaveDialog->Filter = _v; 
}
//---------------------------------------------------------------------------

void TScriptPopupMenu::SetDefaultExt(AnsiString _v)
{
  mOpenDialog->DefaultExt = _v;
  mSaveDialog->DefaultExt = _v;
}
//---------------------------------------------------------------------------

namespace Scriptpopupmenu
{
        void __fastcall PACKAGE Register()
        {
                 TComponentClass classes[1] = {__classid(TScriptPopupMenu)};
                 RegisterComponents("PPTactical", classes, 0);
        }
}
//---------------------------------------------------------------------------
 