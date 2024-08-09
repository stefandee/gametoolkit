//---------------------------------------------------------------------------

#include <vcl.h>
#include <fstream>
#include <stdio.h>
#include <fstream>
#pragma hdrstop

using namespace std;

#include "FMain.h"
#include "FName.h"
#include "FActions.h"
#include "logFile.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormMain *FormMain;
//---------------------------------------------------------------------------

__fastcall TFormMain::TFormMain(TComponent* Owner)
        : TForm(Owner)
{
  HandleCommandLine();
  
  mBmp = new Graphics::TBitmap();
  mState = X_NONE;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Open1Click(TObject *Sender)
{
  if (dlgOpen->Execute())
  {
    try
    {
      mBmp->LoadFromFile(dlgOpen->FileName);
    }
    catch(...)
    {
      Application->MessageBox("Cannot create bitmap!", "Error", MB_OK);
      return;
    }

    mState = X_NORMAL;

    ComputeScrollLimits();
    mCells.clear();

    sbH->Position = 0;
    sbV->Position = 0;

    // build file names
    mBmpFileName = dlgOpen->FileName;
    mDatFileName = ChangeFileExt(mBmpFileName, ".dat");

    // load from file (if any)
    Load(mDatFileName.c_str());

    MakeCombo();
    
    paintSprite->OnPaint(this);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::paintSpritePaint(TObject *Sender)
{
  if (mState == X_NONE)
  {
    return;
  }

  paintSprite->Canvas->Brush->Color = (TColor)0x000000;
  paintSprite->Canvas->FillRect(Rect(0, 0, paintSprite->Width, paintSprite->Height));

  // draw the bitmap on the canvas using the zoom specified by the trackZoom component
  // and panning regarding the scroll values
  paintSprite->Canvas->CopyRect(
    Rect(-sbH->Position, -sbV->Position, trackZoom->Position * mBmp->Width - sbH->Position, trackZoom->Position * mBmp->Height - sbV->Position),
    mBmp->Canvas,
    Rect(0, 0, mBmp->Width, mBmp->Height)
  );

  if (mSelecting)
  {
    TRect lRect;

    lRect.left = std::min(mStartX, mCurrentX);
    lRect.right = std::max(mStartX, mCurrentX);

    lRect.top = std::min(mStartY, mCurrentY);
    lRect.bottom = std::max(mStartY, mCurrentY);

    paintSprite->Canvas->Brush->Color = shapeBox->Brush->Color;
    paintSprite->Canvas->FrameRect(lRect);
  }

  if (cbCells->ItemIndex != -1)
  {
    CCell* lCell = mCells[cbCells->ItemIndex];

    TCellRect lRect = lCell->GetRect();
    TCellPoint lPoint = lCell->GetPoint();

    paintSprite->Canvas->Brush->Color = shapeBox->Brush->Color;
    paintSprite->Canvas->FrameRect(Rect(lRect.left * trackZoom->Position - sbH->Position, lRect.top * trackZoom->Position - sbV->Position, lRect.right * trackZoom->Position - sbH->Position, lRect.bottom * trackZoom->Position - sbV->Position));

    paintSprite->Canvas->Brush->Color = shapePoint->Brush->Color;
    paintSprite->Canvas->FrameRect(Rect(lPoint.x * trackZoom->Position - sbH->Position, lPoint.y * trackZoom->Position - sbV->Position, lPoint.x * trackZoom->Position - sbH->Position + trackZoom->Position, lPoint.y * trackZoom->Position - sbV->Position + trackZoom->Position));
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::trackZoomChange(TObject *Sender)
{
  ComputeScrollLimits();
  paintSprite->OnPaint(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::shapeBoxMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if (Button == mbLeft)
  {
    dlgColor->Color = shapeBox->Brush->Color;

    if (dlgColor->Execute())
    {
      shapeBox->Brush->Color = dlgColor->Color;
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::shapePointMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if (Button == mbLeft)
  {
    dlgColor->Color = shapePoint->Brush->Color;

    if (dlgColor->Execute())
    {
      shapePoint->Brush->Color = dlgColor->Color;
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::sbHChange(TObject *Sender)
{
  paintSprite->OnPaint(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::sbVChange(TObject *Sender)
{
  paintSprite->OnPaint(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::FormResize(TObject *Sender)
{
  ComputeScrollLimits();
}
//---------------------------------------------------------------------------

void TFormMain::ComputeScrollLimits()
{
  sbH->Max = std::max(sbH->Min + 1, mBmp->Width  * trackZoom->Position - paintSprite->Width);
  sbV->Max = std::max(sbV->Min + 1, mBmp->Height * trackZoom->Position - paintSprite->Height);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::paintSpriteMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if (mState == X_NONE)
  {
    return;
  }

  switch(rgZone->ItemIndex)
  {
    case 0:
      mStartX = mCurrentX = X - X % trackZoom->Position - sbH->Position % trackZoom->Position;
      mStartY = mCurrentY = Y - Y % trackZoom->Position - sbV->Position % trackZoom->Position;
      mSelecting = true;
      break;

    case 1:
      break;
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::paintSpriteMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
  if (mState == X_NONE)
  {
    return;
  }

  switch(rgZone->ItemIndex)
  {
    case 0:
      if (mSelecting)
      {
        mCurrentX = X - X % trackZoom->Position + trackZoom->Position - sbH->Position % trackZoom->Position;
        mCurrentY = Y - Y % trackZoom->Position + trackZoom->Position - sbV->Position % trackZoom->Position;

        paintSprite->OnPaint(this);
      }

      break;

    case 1:
      break;
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::paintSpriteMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if (mState == X_NONE)
  {
    return;
  }

  if (cbCells->ItemIndex == -1)
  {
    mSelecting = false;
    return;
  }

  CCell* lCell;

  mCurrentX = X - X % trackZoom->Position + trackZoom->Position;
  mCurrentY = Y - Y % trackZoom->Position + trackZoom->Position;

  // setup the current cell
  lCell = mCells[cbCells->ItemIndex];

  switch(rgZone->ItemIndex)
  {
    case 0:
      // set the current cell
      mSelecting = false;

      TCellRect lRect;

      mStartX   = (mStartX + sbH->Position) / trackZoom->Position;
      mStartY   = (mStartY + sbV->Position) / trackZoom->Position;
      mCurrentX = (mCurrentX + sbH->Position) / trackZoom->Position;
      mCurrentY = (mCurrentY + sbV->Position) / trackZoom->Position;

      lRect.left   = std::min(mStartX, mCurrentX);
      lRect.right  = std::max(mStartX, mCurrentX);
      lRect.top    = std::min(mStartY, mCurrentY);
      lRect.bottom = std::max(mStartY, mCurrentY);

      // range checks and response
      if (lRect.bottom > mBmp->Height)
      {
        lRect.bottom = mBmp->Height;
      }

      if (lRect.right > mBmp->Width)
      {
        lRect.right = mBmp->Width;
      }

      lCell->SetRect(lRect);
      paintSprite->OnPaint(this);

      // save to file
      Save(mDatFileName.c_str());

      break;

    case 1:
      TCellPoint lPoint;

      mSelecting = false;
      mCurrentX = (mCurrentX + sbH->Position) / trackZoom->Position;
      mCurrentY = (mCurrentY + sbV->Position) / trackZoom->Position;

      lPoint.x = mCurrentX;
      lPoint.y = mCurrentY;

      lCell->SetPoint(lPoint);
      paintSprite->OnPaint(this);

      // save to file
      Save(mDatFileName.c_str());
      
      break;
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnNewCellClick(TObject *Sender)
{
  if (mState == X_NONE)
  {
    return;
  }

  // create and add a new cell
  CCell* lCell = new CCell();

  lCell->SetName("bla_cell");
  mCells.push_back(lCell);

  MakeCombo();

  // save to file
  Save(mDatFileName.c_str());

  cbCells->ItemIndex = mCells.size() - 1;
}
//---------------------------------------------------------------------------

void TFormMain::MakeCombo()
{
  cbCells->Clear();

  for(unsigned int i = 0; i < mCells.size(); i++)
  {
    cbCells->Items->Add(mCells[i]->GetName().c_str());
  }
}
//---------------------------------------------------------------------------

bool TFormMain::Save(std::string _fileName)
{
  FILE* lFile;

  lFile = fopen(_fileName.c_str(), "w+t");

  if (!lFile)
  {
    return false;
  }

  for(unsigned int i = 0; i < mCells.size(); i++)
  {
    CCell* lCell = mCells[i];

    TCellRect lRect = lCell->GetRect();
    TCellPoint lPoint = lCell->GetPoint();

    fprintf(lFile, "%s\t%d\t%d\t%d\t%d\t%d\t%d\n", lCell->GetName().c_str(), (int)lRect.left, (int)lRect.top, (int)lRect.right, (int)lRect.bottom, (int)lPoint.x, (int)lPoint.y);

  }

  fclose(lFile);

  return true;
}
//---------------------------------------------------------------------------

bool TFormMain::Load(std::string _fileName)
{
  FILE* lFile;

  lFile = fopen(_fileName.c_str(), "r+t");

  if (!lFile)
  {
    return false;
  }

  mCells.clear();

  while(!feof(lFile))
  {
    char lName[200];
    TCellRect lRect;
    TCellPoint lPoint;

    int lLeft, lRight, lTop, lBottom, lX, lY;

    fscanf(lFile, "%s\t%d\t%d\t%d\t%d\t%d\t%d\n", lName, &lLeft, &lTop, &lRight, &lBottom, &lX, &lY);

    lRect.left   = (int)lLeft;
    lRect.top    = (int)lTop;
    lRect.right  = (int)lRight;
    lRect.bottom = (int)lBottom;
    lPoint.x     = (int)lX;
    lPoint.y     = (int)lY;

    CCell* lCell = new CCell();

    lCell->SetName(std::string(lName));
    lCell->SetRect(lRect);
    lCell->SetPoint(lPoint);

    mCells.push_back(lCell);
  }

  fclose(lFile);

  return true;
}
//---------------------------------------------------------------------------

bool TFormMain::ExportAsDefinition(std::string _fileName)
{
  // open the file
  std::fstream lFile;

  lFile.open(_fileName.c_str(), std::ios_base::out | std::ios_base::binary);

  // exit if file cannot be opened
  if (lFile.fail())
  {
    return false;
  }

  // write the number of cell items
  unsigned char lSize = mCells.size();

  lFile.write(&lSize, sizeof(char));

  for(unsigned int i = 0; i < mCells.size(); i++)
  {
    TCellRect lRect = mCells[i]->GetRect();
    TCellPoint lPoint = mCells[i]->GetPoint();

    lFile.write((char*)(&lRect.left), sizeof(short int));
    lFile.write((char*)(&lRect.top), sizeof(short int));
    lFile.write((char*)(&lRect.right), sizeof(short int));
    lFile.write((char*)(&lRect.bottom), sizeof(short int));
    lFile.write((char*)(&lPoint.x), sizeof(short int));
    lFile.write((char*)(&lPoint.y), sizeof(short int));
  }

  lFile.close();
  return true;
}
//---------------------------------------------------------------------------

bool TFormMain::ExportAsSprite(std::string _fileName)
{
  // open the file
  std::fstream lFile;

  lFile.open(_fileName.c_str(), std::ios_base::out | std::ios_base::binary);

  // exit if file cannot be opened
  if (lFile.fail())
  {
    return false;
  }

  // write the number of cell items
  unsigned char lSize = mCells.size();

  lFile.write(&lSize, sizeof(char));

  for(unsigned int i = 0; i < mCells.size(); i++)
  {
    TCellRect lRect = mCells[i]->GetRect();
    TCellPoint lPoint = mCells[i]->GetPoint();

    // recompute the hot spot as relative coordinates to the cell rect top-left corner
    lPoint.x = lPoint.x - lRect.left;
    lPoint.y = lPoint.y - lRect.top;

    // first write definitions
    int lWidth  = lRect.right  - lRect.left + 1;
    int lHeight = lRect.bottom - lRect.top + 1;

    lFile.write((char*)(&lWidth), sizeof(short int));
    lFile.write((char*)(&lHeight), sizeof(short int));
    lFile.write((char*)(&lPoint.x), sizeof(short int));
    lFile.write((char*)(&lPoint.y), sizeof(short int));

    // now write the pixels as 4444 (ARGB)
    for (int y = lRect.top; y <= lRect.bottom; y++)
    {
      for (int x = lRect.left; x <= lRect.right; x++)
      {
        int lColor = (int)mBmp->Canvas->Pixels[x][y];

        int lR = lColor & 0x0000FF;
        int lG = (lColor & 0x00FF00) >> 8;
        int lB = (lColor & 0xFF0000) >> 16;

        // compute a 4444 color value out of the original RGB color
        short int lColor444 = 0;

        if (lColor != 0xFFFFFF)
        {
          lColor444 = (lB >> 4) | ((lG >> 4) << 4) | ((lR >> 4) << 8) | (15 << 12);
        }

        // and write it to file
        lFile.write((char*)(&lColor444), sizeof(short int));
      }
    }
  }

  lFile.close();
  return true;
}
//---------------------------------------------------------------------------

bool TFormMain::ExportAsIndexedSprite(std::string _fileName)
{
  // open the file
  std::fstream lFile;

  lFile.open(_fileName.c_str(), std::ios_base::out | std::ios_base::binary);

  // exit if file cannot be opened
  if (lFile.fail())
  {
    return false;
  }

  // write the number of cell items
  unsigned char lSize = mCells.size();

  lFile.write(&lSize, sizeof(char));

  // compute the pallete for this sprite
  int lPallete[256];
  unsigned char lColorCount = 0;

  for(int x = 0 ; x < mBmp->Width; x++)
  {
    for(int y = 0 ; y < mBmp->Height; y++)
    {
      int lColor = (int)mBmp->Canvas->Pixels[x][y];

      bool lColorFound = false;

      for (int i = 0; i < lColorCount; i++)
      {
        if (lPallete[i] == lColor)
        {
          lColorFound = true;
          break;
        }
      }

      if (!lColorFound)
      {
        lPallete[lColorCount++] = lColor;
      }
    }
  }

  lFile.write(&lColorCount, sizeof(char));

  for(short int j = 0; j < lColorCount; j++)
  {
    unsigned char lR = (lPallete[j] & 0x0000FF);
    unsigned char lG = (lPallete[j] & 0x00FF00) >> 8;
    unsigned char lB = (lPallete[j] & 0xFF0000) >> 16;

    lFile.write(&lR, sizeof(char));
    lFile.write(&lG, sizeof(char));
    lFile.write(&lB, sizeof(char));
  }

  for(unsigned int i = 0; i < mCells.size(); i++)
  {
    TCellRect lRect = mCells[i]->GetRect();
    TCellPoint lPoint = mCells[i]->GetPoint();

    // recompute the hot spot as relative coordinates to the cell rect top-left corner
    lPoint.x = lPoint.x - lRect.left;
    lPoint.y = lPoint.y - lRect.top;

    // first write definitions
    int lWidth  = lRect.right  - lRect.left;// + 1;
    int lHeight = lRect.bottom - lRect.top;// + 1;

    lFile.write((char*)(&lWidth), sizeof(short int));
    lFile.write((char*)(&lHeight), sizeof(short int));
    lFile.write((char*)(&lPoint.x), sizeof(short int));
    lFile.write((char*)(&lPoint.y), sizeof(short int));

    // now write the pixels as 8-bit indexed
    for (int y = lRect.top; y < lRect.bottom; y++)
    {
      for (int x = lRect.left; x < lRect.right; x++)
      {
        int lColor = (int)mBmp->Canvas->Pixels[x][y];

        unsigned char lColorIndex;

        for(lColorIndex = 0; lColorIndex < lColorCount; lColorIndex++)
        {
          if (lPallete[lColorIndex] == lColor)
          {
            break;
          }
        }

        // and write the index to file
        lFile.write(&lColorIndex, sizeof(char));
      }
    }
  }

  lFile.close();
  return true;
}
//---------------------------------------------------------------------------

bool TFormMain::ExportAs4BitIndexedSprite(std::string _fileName)
{
  // open the file
  std::fstream lFile;

  lFile.open(_fileName.c_str(), std::ios_base::out | std::ios_base::binary);

  // exit if file cannot be opened
  if (lFile.fail())
  {
    return false;
  }

  // write the number of cell items
  unsigned char lSize = mCells.size();

  lFile.write(&lSize, sizeof(char));

  // compute the pallete for this sprite
  int lPallete[16];
  unsigned char lColorCount = 0;

  // assume the sprite has less or exactly than 16 colors
  // no check on the color count for now
  for(int x = 0 ; x < mBmp->Width; x++)
  {
    for(int y = 0 ; y < mBmp->Height; y++)
    {
      int lColor = (int)mBmp->Canvas->Pixels[x][y];

      bool lColorFound = false;

      for (int i = 0; i < lColorCount; i++)
      {
        if (lPallete[i] == lColor)
        {
          lColorFound = true;
          break;
        }
      }

      if (!lColorFound)
      {
        lPallete[lColorCount++] = lColor;
      }
    }
  }

  lFile.write(&lColorCount, sizeof(char));

  // output the pallete
  for(short int j = 0; j < lColorCount; j++)
  {
    unsigned char lR = (lPallete[j] & 0x0000FF);
    unsigned char lG = (lPallete[j] & 0x00FF00) >> 8;
    unsigned char lB = (lPallete[j] & 0xFF0000) >> 16;

    lFile.write(&lR, sizeof(char));
    lFile.write(&lG, sizeof(char));
    lFile.write(&lB, sizeof(char));
  }

  for(unsigned int i = 0; i < mCells.size(); i++)
  {
    TCellRect lRect = mCells[i]->GetRect();
    TCellPoint lPoint = mCells[i]->GetPoint();

    // recompute the hot spot as relative coordinates to the cell rect top-left corner
    lPoint.x = lPoint.x - lRect.left;
    lPoint.y = lPoint.y - lRect.top;

    // first write definitions
    short int lWidth  = lRect.right  - lRect.left;// + 1;
    short int lHeight = lRect.bottom - lRect.top;// + 1;

    lFile.write((char*)(&lWidth), sizeof(short int));
    lFile.write((char*)(&lHeight), sizeof(short int));
    lFile.write((char*)(&lPoint.x), sizeof(short int));
    lFile.write((char*)(&lPoint.y), sizeof(short int));

    // the color
    unsigned char lOutputColor = 0;
    int lNibbleCount = 0;
    int lBytesWrote = 0;

    // now write the pixels as 4-bit indexed
    for (int y = lRect.top; y < lRect.bottom; y++)
    {
      for (int x = lRect.left; x < lRect.right; x++)
      {
        int lColor = (int)mBmp->Canvas->Pixels[x][y];

        unsigned char lColorIndex;

        for(lColorIndex = 0; lColorIndex < lColorCount; lColorIndex++)
        {
          if (lPallete[lColorIndex] == lColor)
          {
            break;
          }
        }

        if (lNibbleCount == 0)
        {
          // put the color index as the low nibble
          lOutputColor = lColorIndex;

          lNibbleCount++;
        }
        else
        {
          // put the color index as the high nibble
          lOutputColor |= (lColorIndex << 4);

          // and write the composed to file
          lFile.write(&lOutputColor, sizeof(char));
          lOutputColor = 0;
          lNibbleCount = 0;
          lBytesWrote++;
        }
      }
    }

    if (lNibbleCount == 1)
    {
      lFile.write(&lOutputColor, sizeof(char));
    }
  }

  lFile.close();
  return true;
}
//---------------------------------------------------------------------------

bool TFormMain::ExportAs4BitIndexedSpriteAndActions(std::string _fileName)
{
  // open the file
  std::fstream lFile;

  lFile.open(_fileName.c_str(), std::ios_base::out | std::ios_base::binary);

  // exit if file cannot be opened
  if (lFile.fail())
  {
    return false;
  }

  // write the number of cell items
  unsigned char lSize = mCells.size();

  lFile.write(&lSize, sizeof(char));

  // compute the pallete for this sprite
  int lPallete[16];
  unsigned char lColorCount = 0;

  // assume the sprite has less or exactly than 16 colors
  // no check on the color count for now
  for(int x = 0 ; x < mBmp->Width; x++)
  {
    for(int y = 0 ; y < mBmp->Height; y++)
    {
      int lColor = (int)mBmp->Canvas->Pixels[x][y];

      bool lColorFound = false;

      for (int i = 0; i < lColorCount; i++)
      {
        if (lPallete[i] == lColor)
        {
          lColorFound = true;
          break;
        }
      }

      if (!lColorFound)
      {
        lPallete[lColorCount++] = lColor;
      }
    }
  }

  lFile.write(&lColorCount, sizeof(char));

  // output the pallete
  for(short int j = 0; j < lColorCount; j++)
  {
    unsigned char lR = (lPallete[j] & 0x0000FF);
    unsigned char lG = (lPallete[j] & 0x00FF00) >> 8;
    unsigned char lB = (lPallete[j] & 0xFF0000) >> 16;

    lFile.write(&lR, sizeof(char));
    lFile.write(&lG, sizeof(char));
    lFile.write(&lB, sizeof(char));
  }

  for(unsigned int i = 0; i < mCells.size(); i++)
  {
    TCellRect lRect = mCells[i]->GetRect();
    TCellPoint lPoint = mCells[i]->GetPoint();

    // recompute the hot spot as relative coordinates to the cell rect top-left corner
    lPoint.x = lPoint.x - lRect.left;
    lPoint.y = lPoint.y - lRect.top;

    // first write definitions
    short int lWidth  = lRect.right  - lRect.left;// + 1;
    short int lHeight = lRect.bottom - lRect.top;// + 1;

    lFile.write((char*)(&lWidth), sizeof(short int));
    lFile.write((char*)(&lHeight), sizeof(short int));
    lFile.write((char*)(&lPoint.x), sizeof(short int));
    lFile.write((char*)(&lPoint.y), sizeof(short int));

    // the color
    unsigned char lOutputColor = 0;
    int lNibbleCount = 0;
    int lBytesWrote = 0;

    // now write the pixels as 4-bit indexed
    for (int y = lRect.top; y < lRect.bottom; y++)
    {
      for (int x = lRect.left; x < lRect.right; x++)
      {
        int lColor = (int)mBmp->Canvas->Pixels[x][y];

        unsigned char lColorIndex;

        for(lColorIndex = 0; lColorIndex < lColorCount; lColorIndex++)
        {
          if (lPallete[lColorIndex] == lColor)
          {
            break;
          }
        }

        if (lNibbleCount == 0)
        {
          // put the color index as the low nibble
          lOutputColor = lColorIndex;

          lNibbleCount++;
        }
        else
        {
          // put the color index as the high nibble
          lOutputColor |= (lColorIndex << 4);

          // and write the composed to file
          lFile.write(&lOutputColor, sizeof(char));
          lOutputColor = 0;
          lNibbleCount = 0;
          lBytesWrote++;
        }
      }
    }

    if (lNibbleCount == 1)
    {
      lFile.write(&lOutputColor, sizeof(char));
    }
  }

  // get the actions data
  int   lActionsDataSize;
  char* lActionsData = FormActions->GetData(ChangeFileExt(mBmpFileName, ".actions"), &lActionsDataSize);

  lFile.write(lActionsData, lActionsDataSize);

  lFile.close();

  return true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::cbCellsChange(TObject *Sender)
{
  paintSprite->OnPaint(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnEditClick(TObject *Sender)
{
  if (mState == X_NONE)
  {
    return;
  }

  if (cbCells->ItemIndex == -1)
  {
    return;
  }

  FormName->SetText(mCells[cbCells->ItemIndex]->GetName().c_str());

  if (FormName->ShowModal() == mrOk)
  {
    mCells[cbCells->ItemIndex]->SetName(FormName->GetText().c_str());

    // save to file
    Save(mDatFileName.c_str());

    int lOldIndex = cbCells->ItemIndex ;

    MakeCombo();

    cbCells->ItemIndex = lOldIndex;
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Export1Click(TObject *Sender)
{
  if (mState == X_NONE)
  {
    return;
  }

  if (mCells.size() == 0)
  {
    return;
  }

  // set the file filter
  dlgSave->FilterIndex = 1;
  
  if (dlgSave->Execute())
  {
    ExportAsDefinition(dlgSave->FileName.c_str());
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Exportassprite1Click(TObject *Sender)
{
  // export the sprite definition and data in one file
  if (mState == X_NONE)
  {
    return;
  }

  if (mCells.size() == 0)
  {
    return;
  }

  // set the file filter
  dlgSave->FilterIndex = 2;

  if (dlgSave->Execute())
  {
    ExportAsSprite(dlgSave->FileName.c_str());
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Exportasindexedsprite1Click(TObject *Sender)
{
  // export the sprite definition and data in one file
  if (mState == X_NONE)
  {
    return;
  }

  if (mCells.size() == 0)
  {
    return;
  }

  // set the file filter
  dlgSave->FilterIndex = 2;

  if (dlgSave->Execute())
  {
    ExportAsIndexedSprite(dlgSave->FileName.c_str());
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Exit1Click(TObject *Sender)
{
  if ( Application->MessageBox("Really exit?", "Question", MB_OKCANCEL) == IDOK)
  {
    Application->Terminate();
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ActionEditor1Click(TObject *Sender)
{
  // export the sprite definition and data in one file
  if (mState == X_NONE)
  {
    return;
  }
  
  FormActions->SetFileName(ChangeFileExt(mBmpFileName, ".actions"));
  FormActions->ShowModal();
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::Exportas4bitindexedsprite1Click(TObject *Sender)
{
  // export the sprite definition and data in one file
  if (mState == X_NONE)
  {
    return;
  }

  if (mCells.size() == 0)
  {
    return;
  }

  // set the file filter
  dlgSave->FilterIndex = 2;

  if (dlgSave->Execute())
  {
    ExportAs4BitIndexedSprite(dlgSave->FileName.c_str());
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Exportas4bitindexedspriteactions1Click(
      TObject *Sender)
{
  // export the sprite definition and data in one file
  /*
  if (mState == X_NONE)
  {
    return;
  }

  if (mCells.size() == 0)
  {
    return;
  }

  // set the file filter
  dlgSave->FilterIndex = 2;

  if (dlgSave->Execute())
  {
    ExportAs4BitIndexedSpriteAndActions(dlgSave->FileName.c_str());
  }
  */
}
//---------------------------------------------------------------------------

void TFormMain::PrintHelp()
{
  logWriteLn("Command line usage for XSprite tool:");
  logWriteLn("xsprite inputbatchfile outputpath");
}
//---------------------------------------------------------------------------

void TFormMain::HandleCommandLine()
{
  logWriteLn("XSprite Tool");
  logWriteLn("---------------");
  logTimeMark();

  // no command line usage
  if (ParamCount() == 0)
  {
    return;
  }

  if (ParamCount() != 2)
  {
    logWriteLn("Invalid number of command line arguments.");
    PrintHelp();

    MyTerminate();
    return;
  }

  AnsiString inputFileName  = ParamStr(1);
  AnsiString outputPath     = ParamStr(2);

  // check the two paths
  if (!FileExists(inputFileName))
  {
    logWrite("Input file does not exists -> ");
    logWriteLn(inputFileName.c_str());

    MyTerminate();
    return;
  }

  // open the input file
  ifstream inFile;

  inFile.open(inputFileName.c_str(), ios_base::in);

  char line[2048];

  // the path to the input file is obtained
  // if an item in the input file does not have an
  // absolute path, then add the input file path
  // to the name

  if (!SetCurrentDir(ExtractFilePath(inputFileName)))
  {
    logWriteLn("Could not change dir.");

    MyTerminate();
    return;
  }

  while(inFile.getline(line, 1024))
  {
    mBmp = new Graphics::TBitmap();

    try
    {
      mBmp->LoadFromFile(line);
    }
    catch(...)
    {
      logWrite("Cannot load bitmap -> ");
      logWriteLn(line);

      delete mBmp;
      continue;
    }

    // load dat from file (if any)
    Load(ChangeFileExt(line, ".dat").c_str());

    // output file path
    AnsiString outputFile = outputPath + ChangeFileExt(ExtractFileName(line), ".spr");

    // export
    if (!ExportAs4BitIndexedSprite(outputFile.c_str()))
    {
      logWrite("Cannot export -> ");
      logWriteLn(outputFile.c_str());
    }

    // clean
    delete mBmp;
  }

  MyTerminate();
}
//---------------------------------------------------------------------------

void TFormMain::MyTerminate()
{
  Application->Minimize();
  Application->Terminate();

  //while(true) {};
}
//---------------------------------------------------------------------------

