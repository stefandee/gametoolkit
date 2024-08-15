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
#include <tchar.h>
#pragma hdrstop

#include "ImageEx.h"
#include "GSgiLoad.h"
#include "GILbmLoad.h"
#include "GBmpLoad.h"
#include "GPcxLoad.h"
#include "GPImgLoad.h"
#include <fileapi.h>
#include <memory>

#pragma resource "extdlgs.res"
#pragma package(smart_init)
//---------------------------------------------------------------------------

namespace Imageex
{
 void __fastcall PACKAGE Register()
 {
   TComponentClass classes[2] = {__classid(TImageEx), __classid(TOpenImageEx)};
   RegisterComponents("PPTactical", classes, 1);
 }
}
//---------------------------------------------------------------------------

//------------------------------------------------------------------------//
//                                                                        //
//                                                                        //
// clasa care extinde TBitmap'ul (stie sa incarce mai multe formate)      //
//                                                                        //
//                                                                        //
//------------------------------------------------------------------------//
__fastcall TBitmapEx::TBitmapEx() : TBitmap()
{
}
//---------------------------------------------------------------------------

/*
void __fastcall TBitmapEx::Assign(Classes::TPersistent* Source)
{
}
//---------------------------------------------------------------------------
*/

void __fastcall TBitmapEx::LoadFromFile(const AnsiString FileName)
{
  AnsiString     fileExt;
  TGInfo         info;
  unsigned char* data;
  int            pitch;
  BITMAPINFO     bmpInfo;
  GLoad          *gLoad = NULL;

  fileExt = UpperCase(ExtractFileExt(FileName));

  if (fileExt == "")
  {
    throw EInvalidGraphic("Invalid extension");
  }

  // verific extensiile
  try
  {
    if (fileExt == ".BMP")
    {
      gLoad = (GBmpLoad*)new GBmpLoad();
    }
    else
    {
      if (fileExt == ".IFF")
      {
        gLoad = (GILbmLoad*)new GILbmLoad();
      }
      else
      {
        if (fileExt == ".PCX")
        {
          gLoad = (GPcxLoad*)new GPcxLoad();
        }
        else
        {
          if (fileExt == ".SGI")
          {
            gLoad = (GSgiLoad*)new GSgiLoad();
          }
        }
      }
    }
  }
  catch(...)
  {
    throw EInvalidGraphic("Cannot alloc temporary GLoad class");
  }

  // incarcarea efectiva
  if (!gLoad->Open(FileName.c_str()))
  {
    throw EInvalidGraphic("Cannot open");
  }

  gLoad->GetTInfo(&info);

  // aloc memorie temporara pentru incarcare
  pitch = info.width * 3 + (((info.width * 3) % 4 == 0) ? 0 : 4 - (info.width * 3) % 4);

  try
  {
    data = new unsigned char[pitch * info.height];
  }
  catch(...)
  {
    delete gLoad;
    throw EInvalidGraphic("Cannot alloc temporary memory");
  }

  // incarcarea efectiva din fisier
  if (!gLoad->LoadAligned(data, info.width, info.height, pitch, RGB_24))
  {
    delete gLoad;
    throw EInvalidGraphic("Cannot load");
  }

  Width       = info.width;
  Height      = info.height;                      

  // completez un BITMAPINFO
  bmpInfo.bmiHeader.biSize          = sizeof(BITMAPINFOHEADER);
  bmpInfo.bmiHeader.biWidth         = info.width;
  bmpInfo.bmiHeader.biHeight        = -info.height;
  bmpInfo.bmiHeader.biPlanes        = 1;
  bmpInfo.bmiHeader.biBitCount      = 24;
  bmpInfo.bmiHeader.biCompression   = BI_RGB;
  bmpInfo.bmiHeader.biSizeImage     = info.width * info.height * 3;
  bmpInfo.bmiHeader.biClrUsed       = 0;
  bmpInfo.bmiHeader.biClrImportant  = 0;

  // ... si fac un SetDIB
  // if (SetDIBits(Canvas->Handle, Handle, 0, info.height, data, &bmpInfo, DIB_RGB_COLORS) == 0)
  if (StretchDIBits(Canvas->Handle,
		0, 0, info.width, info.height,
		0, 0, info.width, info.height,
		data,
		&bmpInfo,
		DIB_RGB_COLORS,SRCCOPY) == 0)
  {
   TCHAR* buffer = NULL;
   int errorCode = GetLastError();

   FormatMessage(
	FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
	NULL,
	errorCode,
	MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
	(LPTSTR)&buffer,
	0,
	NULL);

   UnicodeString errorMessage = buffer != NULL ? *buffer : UnicodeString("");

   //delete [] buffer;
   LocalFree(buffer);

   delete [] data;
   delete gLoad;
   throw EInvalidGraphic(errorMessage);
  }

  //FBitmap->PixelFormat = pf24bit;
  gLoad->Close();

  // totul e ok
  delete [] data;
  delete gLoad;
}
//---------------------------------------------------------------------------

void __fastcall TBitmapEx::SaveToFile(const AnsiString FileName)
{
  return;
}
//---------------------------------------------------------------------------

__fastcall TBitmapEx::~TBitmapEx()
{
}
//---------------------------------------------------------------------------

//------------------------------------------------------------------------//
//                                                                        //
//                                                                        //
// componenta care extinde TImage                                         //
//                                                                        //
//                                                                        //
//------------------------------------------------------------------------//
static inline void ValidCtrCheck(TImageEx *)
{
  // ValidCtrCheck is used to assure that the components created do not have
  // any pure virtual functions.
  new TImageEx(NULL);
}
//---------------------------------------------------------------------------

// Descriere   : constructor
// Param       : Owner - proprietarul obiectului
// Rezultat    : n/a
// Comentarii  : n/a
__fastcall TImageEx::TImageEx(TComponent* Owner)
 : TGraphicControl(Owner)
{
  Width        = 128;
  Height       = 128;
  FCenter      = false;
  FAutoSize    = false;
  FStretch     = false;
  FFit         = false;

  try
  {
    FBitmap = new TBitmapEx();
  }
  catch(...)
  {
    FBitmap = NULL;
  }

  if (FBitmap)
  {
    FBitmap->Width = 16;
    FBitmap->Height = 16;
    //FBitmap->PixelFormat = pf24bit;
    //FBitmap->HandleType = bmDIB;
  }

}
//---------------------------------------------------------------------------

void __fastcall TImageEx::SetFit(bool value)
{
  FFit = value;
  Invalidate();
  //Paint();
}
//---------------------------------------------------------------------------

void __fastcall TImageEx::SetTransparent(bool value)
{
  if (!FBitmap)
  {
    return;
  }

  FBitmap->Transparent = value;
  Invalidate();
  //Paint();
}
//---------------------------------------------------------------------------

bool __fastcall TImageEx::GetTransparent()
{
  if (FBitmap)
  {
    return FBitmap->Transparent;
  }
  else
  {
    return false;
  }
}
//---------------------------------------------------------------------------

void __fastcall TImageEx::SetStretch(bool value)
{
  FStretch = value;
  Invalidate();
  //Paint();
}
//---------------------------------------------------------------------------

void __fastcall TImageEx::SetAutoSize(bool value)
{
  if (!FBitmap)
  {
    return;
  }

  FAutoSize = value;

  if (FAutoSize)
  {
    Width  = FBitmap->Width;
    Height = FBitmap->Height;
  }

  Invalidate();
  //Paint();
}
//---------------------------------------------------------------------------

void __fastcall TImageEx::SetCenter(bool value)
{
  FCenter = value;
  Invalidate();
  //Paint();
}
//---------------------------------------------------------------------------

// Descriere   : desenez Canvasul
// Param       : n/a
// Rezultat    : n/a
// Comentarii  : n/a
void __fastcall TImageEx::Paint(void)
{
  Types::TRect rect;
  float          ratio;
  int            newBmpHeight, newBmpWidth;

  if (FBitmap)
  {
    // fit in window draw style - since we dont care for the performance, we'll use canvas draw
    if (FFit)
    {
      if ((Width > FBitmap->Width) && (Height > FBitmap->Height))
      {
        // exact ca Center
        Canvas->Draw(Width / 2 - FBitmap->Width / 2, Height / 2 - FBitmap->Height / 2, FBitmap);
      }
      else
      {
        ratio = (float)FBitmap->Width / FBitmap->Height;
        if (ratio > (float)Width / Height)
        {
          rect.Left    = 0;
          rect.Right   = Width;
          newBmpHeight = Width / ratio;
          rect.Top     = Height / 2 - newBmpHeight / 2;
          rect.Bottom  = Height / 2 + newBmpHeight / 2;
        }
        else
        {
          rect.Top     = 0;
          rect.Bottom  = Height;

          newBmpWidth  = Height * ratio;
          rect.Left    = Width / 2 - newBmpWidth / 2;
          rect.Right   = Width / 2 + newBmpWidth / 2;
        }

        Canvas->StretchDraw(rect, FBitmap);
      }
      return;
    }

    // stretch draw style
    if (FStretch)
    {
      rect.Left   = 0;
      rect.Top    = 0;
      rect.Right  = Width;
      rect.Bottom = Height;

      Canvas->StretchDraw(rect, FBitmap);
      return;
    }

    // center image in window
    if (FCenter)
    {
      Canvas->Draw(Width / 2 - FBitmap->Width / 2, Height / 2 - FBitmap->Height / 2, FBitmap);
      return;
    }

    // normal draw
    Canvas->Draw(0, 0, FBitmap);
  }
  else
  {
    // no bitmap available, clear
    Clear();
  }
}
//---------------------------------------------------------------------------

void __fastcall TImageEx::Clear()
{
  if (FBitmap != NULL)
  {
    Canvas->Pen->Color   = clBtnFace;
    Canvas->Pen->Style   = psSolid;
    Canvas->Brush->Color = clBtnFace;
    Canvas->Brush->Style = bsSolid;
    Canvas->Rectangle(0, 0, Width, Height);

    FBitmap->Assign(NULL);

    Paint();
  }
}
//---------------------------------------------------------------------------

void __fastcall TImageEx::SetBitmap(TBitmapEx *newBitmap)
{
  if ((!newBitmap) || (!FBitmap))
  {
    return;
  }

  FBitmap->Assign(newBitmap);
  Paint();
}
//---------------------------------------------------------------------------

TBitmapEx* __fastcall TImageEx::GetBitmap()
{
  if (!FBitmap)
  {
    return NULL;
  }

  return FBitmap;
}
//---------------------------------------------------------------------------

TCanvas* __fastcall TImageEx::GetCanvas()
{
  if (!FBitmap->Canvas)
  {
    // creez canvasul ?
  }

  return FBitmap->Canvas;
}
//---------------------------------------------------------------------------

__fastcall TImageEx::~TImageEx()
{
  if (FBitmap)
  {
    delete FBitmap;
  }
}
//---------------------------------------------------------------------------


//------------------------------------------------------------------------//
//                                                                        //
//                                                                        //
// componenta de dialog pentru open file, cu preview al fisierului grafic //
//                                                                        //
//                                                                        //
//------------------------------------------------------------------------//
static inline void ValidCtrCheck(TOpenImageEx *)
{
        new TOpenImageEx(NULL);
}
//---------------------------------------------------------------------------

__fastcall TOpenImageEx::TOpenImageEx(TComponent* Owner)
        : TOpenDialog(Owner)
{
  FPanel    = new TPanel(this);
  FOutLine  = new TPanel(this);
  FBitmapEx = new TBitmapEx();
  FPaintBox = new TPaintBox(this);

  Filter    = "Microsoft Bitmaps (*.BMP)|*.bmp|ZSoft Pcx (*.PCX)|*.pcx|Amiga Iff (*.IFF)|*.iff|Silicon Graphics Image (*.SGI)|*.sgi|Piron Images (*.pImg, *.pImage)|*.pImg;*.pImage|All known graphic files|*.bmp;*.pcx;*.iff;*.sgi;*.pImg;*.pImage";

  FPanel->Name          = "PicturePanel";
  FPanel->Caption       = "";
  FPanel->BevelOuter    = bvNone;
  FPanel->BorderWidth   = 6;
  FPanel->TabOrder      = 1;

  FOutLine->Name        = "OutLinePanel";
  FOutLine->Caption     = "";
  FOutLine->Align       = alClient;
  FOutLine->BevelOuter  = bvLowered;
  FOutLine->BevelInner  = bvRaised;
  FOutLine->TabOrder    = 0;
  FOutLine->Parent      = FPanel;

  FPaintBox->Name       = "PaintBox";
  FPaintBox->Align      = alClient;
  FPaintBox->Parent     = FOutLine;

  TemplateConst = L"DLGTEMPLATE";
}
//---------------------------------------------------------------------------

bool __fastcall TOpenImageEx::Execute()
{
  if (NewStyleControls && !Options.Contains(ofOldStyleDialog))
  {
	Template = UnicodeString(L"DLGTEMPLATE").c_str();
  }
  else
  {
    Template = NULL;
  }

  return TOpenDialog::Execute();
}
//---------------------------------------------------------------------------

__fastcall TOpenImageEx::~TOpenImageEx()
{
  delete FPaintBox;
  delete FBitmapEx;
  delete FOutLine;
  delete FPanel;
}
//---------------------------------------------------------------------------

void __fastcall TOpenImageEx::DoShow(void)
{
  RECT staticRect, previewRect;

  staticRect = GetStaticRect();
  if (GetClientRect(Handle, &previewRect) == 0)
  {
    return;
  }

  previewRect.left  = staticRect.right;
  previewRect.right = previewRect.left + 168;
  previewRect.top   = previewRect.top + 4;

  FPanel->ParentWindow  = Handle;

  FPanel->BoundsRect = previewRect;

  TOpenDialog::DoShow();
}
//---------------------------------------------------------------------------

void __fastcall TOpenImageEx::DoClose(void)
{
  TOpenDialog::DoClose();
  Application->HideHint();
}
//---------------------------------------------------------------------------

// Descriere   : daca se schimba selectia din dialog
// Param       : n/a
// Rezultat    : n/a
// Comentarii  : inca ar mai fi niste kestii de testat - ca sa nu dea exceptii aiurea
void __fastcall TOpenImageEx::DoSelectionChange(void)
{
  if (FileExists(FileName) && (FileGetAttr(FileName.c_str()) != faDirectory))
  {
    try
    {
      FBitmapEx->LoadFromFile(FileName);
    }
	catch(...)
    {
      return;
    }

    // paint into the paintbox
    DoThePaint();
  }

  TOpenDialog::DoSelectionChange();
}
//---------------------------------------------------------------------------

void __fastcall TOpenImageEx::DoThePaint(void)
{
  int            pbWidth = FPaintBox->Width, pbHeight = FPaintBox->Height, newBmpWidth, newBmpHeight;
  Types::TRect rect;
  float          ratio;

  DoClear();

  if ((pbWidth > FBitmapEx->Width) && (pbHeight > FBitmapEx->Height))
  {
    // exact ca Center
    FPaintBox->Canvas->Draw(pbWidth / 2 - FBitmapEx->Width / 2, pbHeight / 2 - FBitmapEx->Height / 2, FBitmapEx);
  }
  else
  {
    ratio = (float)FBitmapEx->Width / FBitmapEx->Height;
    if (ratio > (float)pbWidth / pbHeight)
    {
      rect.Left    = 0;
      rect.Right   = pbWidth;
      newBmpHeight = pbWidth / ratio;
      rect.Top     = pbHeight / 2 - newBmpHeight / 2;
      rect.Bottom  = pbHeight / 2 + newBmpHeight / 2;
    }
    else
    {
      rect.Top     = 0;
      rect.Bottom  = pbHeight;

      newBmpWidth  = pbHeight * ratio;
      rect.Left    = pbWidth / 2 - newBmpWidth / 2;
      rect.Right   = pbWidth / 2 + newBmpWidth / 2;
    }

    FPaintBox->Canvas->StretchDraw(rect, FBitmapEx);
  }  
}
//---------------------------------------------------------------------------

void __fastcall TOpenImageEx::DoClear(void)
{
  FPaintBox->Canvas->Pen->Color   = clBtnFace;
  FPaintBox->Canvas->Pen->Style   = psSolid;
  FPaintBox->Canvas->Brush->Color = clBtnFace;
  FPaintBox->Canvas->Brush->Style = bsSolid;
  FPaintBox->Canvas->Rectangle(0, 0, FPaintBox->Width, FPaintBox->Height);
}
//---------------------------------------------------------------------------


