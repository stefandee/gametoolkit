//---------------------------------------------------------------------------
#pragma hdrstop

#include "PP_CSLinkMenu.h"
#include "Paths.h"
#include "basepath.h"
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

CPP_CSLinkMenu::CPP_CSLinkMenu(int _number, CPIWinControl* Parent) : CPP_CSMenu(_number, Parent)
{
  mSprConn         = GetGraphicInstance()->AddSprite(CPString(PATH_GUI_GFX) + "connectors.png", 8, 2);
  mLinkOrientation = SLMO_DOWN;
}
//---------------------------------------------------------------------------

void CPP_CSLinkMenu::Paint()
{
  int     lL = GetAbsoluteLeft(), lT = GetAbsoluteTop();

  if (!mSprConn)
  {
    return;
  }

  SetClipRegion();

  // intii desenez conectorii - se poate optimiza, dar nu e esential
  switch(mLinkOrientation)
  {
    case SLMO_UP :
      // coltul - in functie de orientarea sus-jos a meniului
      if (GetBtnOrientation() == SBO_RIGHT)
      {
        // T'urile
        for(int i = 0; i < mBtnsNumber - 1; i++)
        {
          // intre doi t sint conectori normali
          int lCrtTop = lT + mBtns[i]->GetTop() + mBtns[i]->GetHeight() / 2 - 16;
          for(int j = 0; j < mSpacing / 32 + 1; j++)
          {
            GetGraphicInstance()->PaintSprite(
              lL + GetWidth() / 2 + mBtns[i]->GetWidth() / 2,
              lCrtTop + 32 * (j + 1),
              mSprConn,
              2, 0);
          }

          GetGraphicInstance()->PaintSprite(
            lL + GetWidth() / 2 + mBtns[i]->GetWidth() / 2,
            lT + mBtns[i]->GetTop() + mBtns[i]->GetHeight() / 2 - 16,
            mSprConn,
            7, 1);
        }

        // coltul
        /*
        GetGraphicInstance()->PaintSpriteToSurface(
          NULL,
          lL + GetWidth() / 2 + mBtns[0]->GetWidth() / 2,
          lT + mBtns[mBtnsNumber - 1]->GetTop() - 4,
          mSprConn,
          1, 1);
        */

        GetGraphicInstance()->PaintSprite(
          lL + GetWidth() / 2 + mBtns[0]->GetWidth() / 2,
          lT + mBtns[mBtnsNumber - 1]->GetTop() - 6,
          mSprConn,
          1, 1);
      }
      else
      {
        // T'urile
        for(int i = 0; i < mBtnsNumber - 1; i++)
        {
          // intre doi t sint conectori normali
          int lCrtTop = lT + mBtns[i]->GetTop() + mBtns[i]->GetHeight() / 2 - 16;
          for(int j = 0; j < mSpacing / 32 + 1; j++)
          {
            GetGraphicInstance()->PaintSprite(
              lL + mBtns[i]->GetLeft() - 32,
              lCrtTop + 32 * (j + 1),
              mSprConn,
              2, 1);
          }

          GetGraphicInstance()->PaintSprite(
            lL + mBtns[i]->GetLeft() - 32,
            lT + mBtns[i]->GetTop() + mBtns[i]->GetHeight() / 2 - 16,
            mSprConn,
            6, 0);
        }

        // coltul
        GetGraphicInstance()->PaintSprite(
          lL + mBtns[0]->GetLeft() - 32,
          lT + mBtns[mBtnsNumber - 1]->GetTop() - 6,
          mSprConn,
          0, 1);
      }

      break;

    case SLMO_DOWN :
      // coltul - in functie de orientarea generala a meniului
      if (GetBtnOrientation() == SBO_RIGHT)
      {
        // T'urile
        for(int i = 1; i < mBtnsNumber; i++)
        {
          // intre doi t sint conectori normali
          int lCrtTop = lT + mBtns[i]->GetTop() + mBtns[i]->GetHeight() / 2 - 16;
          for(int j = 0; j < mSpacing / 32 + 1; j++)
          {
            GetGraphicInstance()->PaintSprite(
              lL + GetWidth() / 2 + mBtns[i]->GetWidth() / 2,
              lCrtTop - 32 * (j + 1),
              mSprConn,
              2, 0);
          }

          GetGraphicInstance()->PaintSprite(
            lL + GetWidth() / 2 + mBtns[i]->GetWidth() / 2,
            lT + mBtns[i]->GetTop() + mBtns[i]->GetHeight() / 2 - 16,
            mSprConn, 7, 1);
        }

        // coltul
        GetGraphicInstance()->PaintSprite(
          lL + GetWidth() / 2 + mBtns[0]->GetWidth() / 2,
          lT + mBtns[0]->GetTop() + 6,
          mSprConn,
          1, 0);
      }
      else
      {
        // T'urile
        for(int i = 1; i < mBtnsNumber; i++)
        {
          // intre doi t sint conectori normali
          int lCrtTop = lT + mBtns[i]->GetTop() + mBtns[i]->GetHeight() / 2 - 16;
          for(int j = 0; j < mSpacing / 32 + 1; j++)
          {
            GetGraphicInstance()->PaintSprite(
              lL + mBtns[i]->GetLeft() - 32,
              lCrtTop - 32 * (j + 1),
              mSprConn,
              2, 1);
          }

          GetGraphicInstance()->PaintSprite(
            lL + mBtns[i]->GetLeft() - 32,
            lT + mBtns[i]->GetTop() + mBtns[i]->GetHeight() / 2 - 16,
            mSprConn,
            6, 0);
        }

        // coltul
        GetGraphicInstance()->PaintSprite(
          lL + mBtns[0]->GetLeft() - 32,
          lT + mBtns[0]->GetTop() + 6,
          mSprConn,
          0, 0);
      }
      break;
  }

  switch(GetBtnOrientation())
  {
    case SBO_RIGHT :

      break;

    case SBO_LEFT :

      break;
  }

  CPP_CSMenu::Paint();
}
//---------------------------------------------------------------------------

CPP_CSLinkMenu::~CPP_CSLinkMenu()
{
  //GetGraphicInstance()->ReleaseSprite(mSprConn);
}
//---------------------------------------------------------------------------
