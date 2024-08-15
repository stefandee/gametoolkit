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

#include "m_spred.h"
#include "newbmp.h"
#include "cell.h"
#include "SpecReplace.h"
#include "FAbout.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ImageEx"
#pragma resource "*.dfm"
TMainForm *MainForm;

//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
        : TForm(Owner)
{
  StatusBar1->Panels->Items[0]->Text = "";
  fileName                           = "";
  modified                           = false;
  newFile                            = false;
  historyItems                       = 0;
  historyCrtItems                    = 0;

  // incarc configuratia
  if (!cfgFile.Open(CFG_FILE_NAME))
  {
    if (!cfgFile.Create(CFG_FILE_NAME))
    {
	  Application->MessageBox(L"Cannot create cfg file", L"Error", MB_OK);
	  return;
	}
	else
	{
	  if (!cfgFile.SetInfo("Sprite Editor cfg"))
	  {
        Application->MessageBox(L"Cannot write cfg header", L"Error", MB_OK);
        return;
	  }

	  UnicodeString empty = L"";

      // adaug resursa pentru directorul curent
	  cfgFile.AddResource("CurrentDir", RES_STRING256, empty.c_str());

	  // adaug inca 5 resurse pentru ultimele fisiere (history)
	  cfgFile.AddResource("File1", RES_STRING256, empty.c_str());
	  cfgFile.AddResource("File2", RES_STRING256, empty.c_str());
	  cfgFile.AddResource("File3", RES_STRING256, empty.c_str());
	  cfgFile.AddResource("File4", RES_STRING256, empty.c_str());
      cfgFile.AddResource("File5", RES_STRING256, empty.c_str());
    }
  }
  else
  {
    // citesc resursele
    char buffer[CFG_BUFFER_SIZE];

    // directorul curent
    if (cfgFile.GetResource("CurrentDir", buffer))
    {
      SetCurrentDir(AnsiString((char*)buffer));
    }

    // cele 5 fisiere din history
    TMenuItem* menuItem;
    AnsiString historyPath;

    for(int i = 1; i < MAX_HISTORY_LINKS; i++)
    {
      if (cfgFile.GetResource(("File" + AnsiString(i)).c_str(), buffer))
	  {
		historyPath = AnsiString((char*)buffer);

        if (!historyPath.IsEmpty())
        {
          menuItem            = new TMenuItem(MainForm->MainMenu);
          menuItem->Caption   = "&" + AnsiString(historyItems + 1) + " " + historyPath;
          menuItem->Enabled   = true;
          // menuItem->MenuIndex = HISTORY_MENU_INDEX;

          // aleg metoda de tratare a click'ului
          switch(historyItems)
          {
            case 0 :
              menuItem->OnClick   = History1Click;
              break;
            case 1 :
              menuItem->OnClick   = History2Click;
              break;
            case 2 :
              menuItem->OnClick   = History3Click;
              break;
            case 3 :
              menuItem->OnClick   = History4Click;
              break;
            case 4 :
              menuItem->OnClick   = History5Click;
              break;
          }

          // menuItem->Command   = historyItems + 1;
          MainForm->MainMenu->Items->Items[0]->Insert(HISTORY_MENU_INDEX + historyItems, menuItem);

          // numarul de itemuri din history + 1
          historyItems++;
          historyCrtItems++;
        }
      }
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::New1Click(TObject *Sender)
{
  if (NewForm->ShowModal() == mrOk)
  {
    Sprite->Picture->Bitmap->Width = NewForm->ed_xbmp->Text.ToInt();
    Sprite->Picture->Bitmap->Height = NewForm->ed_ybmp->Text.ToInt();

    //if (!Sprite->Picture->Bitmap->Empty)
    {
      // clear this bitmap
      TRect dest;
      dest.Left = 0; dest.Right = Sprite->Picture->Bitmap->Width + 1;
      dest.Top = 0; dest.Bottom = Sprite->Picture->Bitmap->Height + 1;
      Sprite->Picture->Bitmap->Canvas->Brush->Style = bsSolid;
      Sprite->Picture->Bitmap->Canvas->Brush->Color = clWhite;
      Sprite->Picture->Bitmap->Canvas->FillRect(dest);
    }

    xCellSize = NewForm->ed_xsize->Text.ToInt();
    yCellSize = NewForm->ed_ysize->Text.ToInt();
    Replace1->Enabled      = true;
    Replace->Enabled       = true;
    Clear1->Enabled        = true;
    Savebitmap1->Enabled   = true;
    Savebitmapas1->Enabled = true;
    Save1->Enabled         = true;
    Saveas1->Enabled       = true;
    Resize1->Enabled       = true;
    //Bitmapinfo1->Enabled = true;
    //Insert1->Enabled     = true;

    fileName         = "noname.bmp";
    modified         = true;
    newFile          = true;
    StatusBar1->Panels->Items[0]->Text = "Modified";
    StatusBar1->Panels->Items[1]->Text = fileName;
  }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::Replace1Click(TObject *Sender)
{
  TBitmapEx* tempic;

  if (OpenPictureDialog->Execute())
  {
    try
    {
      tempic = new TBitmapEx;
    }
    catch(...)
    {
	  MessageBox(0, L"Error", L"Cannot allocate TBitmapEx*", MB_OK);
	  return;
	}

	tempic->Width = xCellSize;
	tempic->Height = yCellSize;

	try
	{
	  tempic->LoadFromFile(OpenPictureDialog->FileName);
	}
	catch(...)
	{
	  MessageBox(0, L"Error", L"Cannot load bitmap", MB_OK);
      delete tempic;
      return;
    }

    if (xCellSel > Sprite->Picture->Width) xCellSel = Sprite->Picture->Width - 1;
    if (yCellSel > Sprite->Picture->Height) yCellSel = Sprite->Picture->Height - 1;

    int pos_x = (int)(xCellSel/xCellSize) * xCellSize;
    int pos_y = (int)(yCellSel/yCellSize) * yCellSize;

    TRect dest, src;
    dest.Left = pos_x; dest.Right = pos_x + xCellSize;
    dest.Top = pos_y; dest.Bottom = pos_y + yCellSize;
    src.Left = 0; src.Right = xCellSize;
    src.Top = 0; src.Bottom = yCellSize;

    Sprite->Picture->Bitmap->Canvas->CopyRect(dest, tempic->Canvas, src);
    modified = true;
    Save1->Enabled = true;
    Saveas1->Enabled = true;
    StatusBar1->Panels->Items[0]->Text = "Modified";
    
    delete tempic;
  }

}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SpriteMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if (Button == mbRight)
  {
    xCellSel = X;
    yCellSel = Y;
  }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::Load1Click(TObject *Sender)
{
  if (OpenPictureDialog->Execute())
  {
    if (LoadSpriteBmp(OpenPictureDialog->FileName))
    {
      //fileName = OpenPictureDialog->FileName;

      // adaug fileName la history
      if (historyCrtItems < MAX_HISTORY_LINKS)
      {
        TMenuItem* menuItem;

        menuItem            = new TMenuItem(MainForm->MainMenu);
        menuItem->Caption   = "&" + AnsiString(historyItems + 1) + " " + AnsiString(fileName);
        menuItem->Enabled   = true;
        // menuItem->MenuIndex = HISTORY_MENU_INDEX;

        // aleg metoda de tratare a click'ului
        switch(historyItems)
        {
          case 0 :
            menuItem->OnClick   = History1Click;
            break;
          case 1 :
            menuItem->OnClick   = History2Click;
            break;
          case 2 :
            menuItem->OnClick   = History3Click;
            break;
          case 3 :
            menuItem->OnClick   = History4Click;
            break;
          case 4 :
            menuItem->OnClick   = History5Click;            
            break;
        }

        // menuItem->Command   = historyItems + 1;
        MainForm->MainMenu->Items->Items[0]->Insert(HISTORY_MENU_INDEX + historyItems, menuItem);

        // numarul de itemuri din history + 1
        historyItems++;
        historyItems %= (MAX_HISTORY_LINKS - 1);
        historyCrtItems++;
      }
      else
      {
        File1->Items[HISTORY_MENU_INDEX + historyItems]->Caption   = "&" + AnsiString(historyItems + 1) + " " + AnsiString(fileName);
        File1->Items[HISTORY_MENU_INDEX + historyItems]->Enabled   = true;
        // menuItem->MenuIndex = HISTORY_MENU_INDEX;
        // File1->Items[HISTORY_MENU_INDEX + historyItems]->OnClick   = HistoryClick;

        historyItems++;
        historyItems %= (MAX_HISTORY_LINKS - 1);
      }
    }
  }
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::Savebitmap1Click(TObject *Sender)
{
  // daca fisierul abia a fost creat (new), atunci procedura de salvare
  // este echivalenta cu Save As
  if (newFile)
  {
    newFile = false;
    Savebitmapas1Click(this);
  }
  else
  {
    // do the saving business
    try
    {
      Sprite->Picture->Bitmap->SaveToFile(fileName);
    }
    catch(...)
    {
	  MessageBox(0, L"Save error", L"Error", MB_OK);
      return;
    }
    // end of saving
  }

  StatusBar1->Panels->Items[0]->Text = "";
  modified = false;
  Save1->Enabled = false;
  Saveas1->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Resize1Click(TObject *Sender)
{
  TRect dest, src;
  Graphics::TBitmap* tempic = new Graphics::TBitmap();

  if (NewForm->ShowModal() == mrOk)
  {
    // new cell size
    xCellSize = NewForm->ed_xsize->Text.ToInt();
    yCellSize = NewForm->ed_ysize->Text.ToInt();

    tempic->Assign(Sprite->Picture->Bitmap);

    // resize & clear the Sprite
    Sprite->Picture->Bitmap->Width = NewForm->ed_xbmp->Text.ToInt();
    Sprite->Picture->Bitmap->Height = NewForm->ed_ybmp->Text.ToInt();
    dest.Left = 0; dest.Right = Sprite->Picture->Bitmap->Width + 1;
    dest.Top = 0; dest.Bottom = Sprite->Picture->Bitmap->Height + 1;
    Sprite->Picture->Bitmap->Canvas->Brush->Style = bsSolid;
    Sprite->Picture->Bitmap->Canvas->Brush->Color = clWhite;
    Sprite->Picture->Bitmap->Canvas->FillRect(dest);

    // copy back the image
    dest.Left = 0; dest.Right = tempic->Width + 1;
    dest.Top = 0; dest.Bottom = tempic->Height + 1;
    src.Left = 0; src.Right = tempic->Width;
    src.Top = 0; src.Bottom = tempic->Height;
    Sprite->Picture->Bitmap->Canvas->CopyRect(dest, tempic->Canvas, src);

    StatusBar1->Panels->Items[0]->Text = "Modified";
    modified = true;
    Save1->Enabled = true;
    Saveas1->Enabled = true;
  }

  delete tempic;
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::Clear1Click(TObject *Sender)
{
  int pos_x, pos_y;

  if (xCellSel > Sprite->Picture->Width) xCellSel = Sprite->Picture->Width - 1;
  if (yCellSel > Sprite->Picture->Height) yCellSel = Sprite->Picture->Height - 1;

  pos_x = (int)(xCellSel/xCellSize) * xCellSize;
  pos_y = (int)(yCellSel/yCellSize) * yCellSize;

  TRect dest, src;
  dest.Left = pos_x; dest.Right = pos_x + xCellSize;
  dest.Top = pos_y; dest.Bottom = pos_y + yCellSize;

  Sprite->Picture->Bitmap->Canvas->Brush->Style = bsSolid;
  Sprite->Picture->Bitmap->Canvas->Brush->Color = clWhite;
  Sprite->Picture->Bitmap->Canvas->FillRect(dest);

  StatusBar1->Panels->Items[0]->Text = "Modified";
  modified = true;
  Save1->Enabled = true;
  Saveas1->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Savebitmapas1Click(TObject *Sender)
{
  if (SavePictureDialog->Execute())
  {
    // do the saving business
    try
    {
      Sprite->Picture->Bitmap->SaveToFile(SavePictureDialog->FileName);
    }
    catch(...)
    {
      MessageBox(0, L"Error", L"Save error", MB_OK);
      return;
    }
    // end of saving
    modified = false;
    fileName = SavePictureDialog->FileName;
    StatusBar1->Panels->Items[1]->Text = fileName;
    StatusBar1->Panels->Items[0]->Text = "";
  }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ReplaceClick(TObject *Sender)
{
  if (xCellSel > Sprite->Picture->Width) xCellSel = Sprite->Picture->Width - 1;
  if (yCellSel > Sprite->Picture->Height) yCellSel = Sprite->Picture->Height - 1;

  if (FormReplace->ShowModal() != mrOk)
  {
    return;
  }

  if (FormReplace->rgStyle->ItemIndex == 0)
  {
    FillByRows();
  }
  else
  {
    FillByCols();
  }

  modified = true;
  Save1->Enabled = true;
  Saveas1->Enabled = true;
}
//---------------------------------------------------------------------------

void TMainForm::FillByRows()
{
  int i, j, k, fileIndexRow, fileIndexCol, zeros, originalXCell, pos_x, pos_y;
  UnicodeString stringFile, fileName;
  TBitmapEx* tempic;

  // special replace
  originalXCell = xCellSel;
  fileIndexRow = FormReplace->startFile;
  switch(FormReplace->format)
  {
    case _000 :
      zeros = 3;
      if (fileIndexRow > 999)
      {
		Application->MessageBox(L"First file number is out of range", L"Error", MB_OK);
		return;
	  }
	  break;
	case _0000 :
	  zeros = 4;
	  if (fileIndexRow > 9999)
	  {
		Application->MessageBox(L"First file number is out of range", L"Error", MB_OK);
        return;
      }
      break;
    default :
      zeros = 3;
      if (fileIndexRow > 999)
      {
		Application->MessageBox(L"First file number is out of range", L"Error", MB_OK);
        return;
      }
  }

  try
  {
    tempic = new TBitmapEx;
  }
  catch(...)
  {
    Application->MessageBox(L"Cannot alloc TBitmapEx*", L"Error", MB_OK);
    return;
  }

  tempic->Width  = xCellSize;
  tempic->Height = yCellSize;

  // bucla principala
  for(i = 0; i < FormReplace->rows; i++)
  {
    fileIndexCol = fileIndexRow;

    for(j = 0; j < FormReplace->cols; j++)
    {
      fileName = FormReplace->baseFile;

      for(k = 0; k < zeros - AnsiString(fileIndexCol).Length(); k++)
      {
        fileName = fileName + "0";
      }

      fileName = fileName + UnicodeString(fileIndexCol) + L".bmp";

      // citesc fisierul bmp si il pun in sprite
      try
      {
        tempic->LoadFromFile(FormReplace->pathToFiles + fileName);
	  }
      catch(...)
      {
		Application->MessageBox((L"Cannot open " + fileName).c_str(), L"Error", MB_OK);
        delete tempic;
        return;
      }

      pos_x = (int)(xCellSel/xCellSize) * xCellSize; // sau %
      pos_y = (int)(yCellSel/yCellSize) * yCellSize; // sau %

      TRect dest, src;
      dest.Left = pos_x; dest.Right = pos_x + xCellSize;
      dest.Top = pos_y; dest.Bottom = pos_y + yCellSize;
      src.Left = 0; src.Right = xCellSize;
      src.Top = 0; src.Bottom = yCellSize;

      Sprite->Picture->Bitmap->Canvas->CopyRect(dest, tempic->Canvas, src);
      modified = true;
      Save1->Enabled = true;
      Saveas1->Enabled = true;
      StatusBar1->Panels->Items[0]->Text = "Modified";

      fileIndexCol += FormReplace->rows;
      if (fileIndexCol > FormReplace->lastFile) fileIndexCol = FormReplace->firstFile + fileIndexCol - FormReplace->lastFile - 1;
      xCellSel += xCellSize;
    } // end for j

    fileIndexRow++;
    yCellSel += yCellSize;
    xCellSel = originalXCell;
  } // end for i

  delete tempic;
}
//---------------------------------------------------------------------------

void TMainForm::FillByCols()
{
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::History1Click(TObject *Sender)
{
  AnsiString temp = File1->Items[HISTORY_MENU_INDEX]->Caption;
  LoadSpriteBmp(temp.SubString(4, temp.Length() - 3));
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::History2Click(TObject *Sender)
{
  AnsiString temp = File1->Items[HISTORY_MENU_INDEX + 1]->Caption;
  LoadSpriteBmp(temp.SubString(4, temp.Length() - 3));
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::History3Click(TObject *Sender)
{
  AnsiString temp = File1->Items[HISTORY_MENU_INDEX + 2]->Caption;
  LoadSpriteBmp(temp.SubString(4, temp.Length() - 3));
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::History4Click(TObject *Sender)
{
  AnsiString temp = File1->Items[HISTORY_MENU_INDEX + 3]->Caption;
  LoadSpriteBmp(temp.SubString(4, temp.Length() - 3));
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::History5Click(TObject *Sender)
{
  AnsiString temp = File1->Items[HISTORY_MENU_INDEX + 4]->Caption;
  LoadSpriteBmp(temp.SubString(4, temp.Length() - 3));
}
//---------------------------------------------------------------------------

bool TMainForm::LoadSpriteBmp(AnsiString spriteFileName)
{
  TBitmapEx* tempic;

  if (CellForm->ShowModal() == mrOk)
  {
    xCellSize = CellForm->ed_xsize->Text.ToInt();
    yCellSize = CellForm->ed_ysize->Text.ToInt();

    // erase old contents
    if (!Sprite->Picture->Bitmap->Empty)
    {
      // clear this bitmap
      TRect dest;
      dest.Left = 0; dest.Right = Sprite->Picture->Bitmap->Width + 1;
      dest.Top = 0; dest.Bottom = Sprite->Picture->Bitmap->Height + 1;
      Sprite->Picture->Bitmap->Canvas->Brush->Style = bsSolid;
      Sprite->Picture->Bitmap->Canvas->Brush->Color = clWhite;
      Sprite->Picture->Bitmap->Canvas->FillRect(dest);
    }

    try
    {
      tempic = new TBitmapEx;
    }
    catch(...)
    {
	  MessageBox(0, L"Error", L"Cannot alloc TBitmapEx*", MB_OK);
	  return false;
	}

	// incarca imaginea
	try
	{
	  //Sprite->Picture->Bitmap->LoadFromFile(spriteFileName);
	  tempic->LoadFromFile(spriteFileName);
	}
	catch(...)
	{
      MessageBox(0, L"Error", L"Cannot load image file", MB_OK);
      return false;
    }

    Sprite->Picture->Bitmap->Width  = tempic->Width;
    Sprite->Picture->Bitmap->Height = tempic->Height;

    // copy image to destination
    Sprite->Picture->Bitmap->Canvas->Draw(0, 0, tempic);

    // enable the items in popup menu
    Clear1->Enabled        = true;
    Replace1->Enabled      = true;
    Replace->Enabled       = true;
    Clear1->Enabled        = true;
    Savebitmap1->Enabled   = true;
    Savebitmapas1->Enabled = true;
    Saveas1->Enabled       = true;
    Resize1->Enabled       = true;
    //Save1->Enabled       = true;
    //Bitmapinfo1->Enabled = true;
    //Insert1->Enabled = true;

    modified = false;
    StatusBar1->Panels->Items[0]->Text = "";
    fileName = spriteFileName;
    StatusBar1->Panels->Items[1]->Text = fileName;

    delete tempic;

    return true;
  }

  return false;
}
//---------------------------------------------------------------------------




void __fastcall TMainForm::FormClose(TObject *Sender, TCloseAction &Action)
{
  char buffer[CFG_BUFFER_SIZE];
  AnsiString temp;

  // salvez calea curenta, data de ultimul fisier deschis (fileName)
  if (!fileName.IsEmpty())
  {
    // salvez si calea curenta
	cfgFile.SetResource(AnsiString("CurrentDir").c_str(), ExtractFilePath(fileName).c_str());
  }

  for(int i = 0; i < historyCrtItems; i++)
  {
    // iau din meniu sirul care reprezinta calea de fisier

    // pt manipulare usoara
    temp = File1->Items[HISTORY_MENU_INDEX + i]->Caption;
    temp = temp.SubString(4, temp.Length() - 3);

    strcpy(buffer, temp.c_str());
    cfgFile.SetResource((AnsiString("File") + AnsiString(i + 1)).c_str(), buffer);
  }

  cfgFile.Close();

  Action = caFree;
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::About1Click(TObject *Sender)
{
  FormAbout->ShowModal();
}
//---------------------------------------------------------------------------

