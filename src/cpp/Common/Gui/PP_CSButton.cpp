//---------------------------------------------------------------------------
#pragma hdrstop

#include "PP_CSButton.h"
#include "PPIG_CScreen.h"
#include "PP_CSButton.h"
#include "PP_Graphic.h"
#include "Paths.h"
#include "basepath.h"
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

CPP_CSButton::CPP_CSButton(CPIWinControl *Parent) : CPIButton(Parent)
{
#if PPT_USE_SDL
  mFont = new CPFont(PATH_FONTS + FILE_FONT_BIG, 14, true);
#else
  mFont = new CPFont(PATH_FONTS + FILE_FONT_BIG);
  mFont->SetSize(14);
#endif // PPT_USE_SDL

  mFontColor         = 0xD3D0D0;
  mFontColorDisabled = 0x808080;
  mOpacity           = 1.0;
  mCaption           = "";
  mBackColor         = 0x000040;
  mBackColorDisabled = 0x000000;
  mOrientation       = SBO_LEFT;
  mBtnSprite         = GetGraphicInstance()->AddSprite(CPString(PATH_GUI_GFX) + "button.png", 2, 2);

  /*
  mFxDrain           = new CFx_Drain();
  mFxDrain->SetEnabled(false);
  mFxDrain->SetDensity(32);
  mFxDrain->SetHeadColor(0XFFD0D0);
  mFxDrain->SetTailColor(0x883600);
  mFxDrain->SetTailLength(8);
  */

  SetHeight(32);
  SetWidth(64);

  SetHandleOnMouseMove(std::bind(&CPP_CSButton::MouseMove, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}
//---------------------------------------------------------------------------

void CPP_CSButton::MouseMove(CPIObject *Sender, int X, int Y)
{
  if (IsPointInside(CPPoint(X, Y)))
  {
    GetMainScreenInstance()->SetMouseCaptureControl(this);
    //mFxDrain->SetEnabled(true);
  }
  else
  {
    GetMainScreenInstance()->SetMouseCaptureControl(NULL);
    //mFxDrain->SetEnabled(false);
  }
}
//---------------------------------------------------------------------------

bool CPP_CSButton::IsDerivedFrom(CPString className)
{
   return ((className == ClassName()) || (CPIControl::IsDerivedFrom(className)));
}
//---------------------------------------------------------------------------

void CPP_CSButton::Update()
{
  CPIControl::Update();

  //mFxDrain->Update();
}
//---------------------------------------------------------------------------

bool CPP_CSButton::IsPointInside (CPPoint point)
{
  int lL = GetAbsoluteLeft(), lT = GetAbsoluteTop();

  if (GetWidth() <= 64)
  {
    return false;
  }

  if (point.x >= lL + 32 &&
      point.x <= lL + GetWidth() - 32 &&
      point.y >= lT &&
      point.y <= lT + 32)
  {
    return true;
  }

  return false;
}
//---------------------------------------------------------------------------

void CPP_CSButton::Paint()
{
  int lL = GetAbsoluteLeft(), lT = GetAbsoluteTop();
  TColor32 lBackColor;
  TColor32 lFontColor;
  CPGIGraphicSystem* lGI = GetGraphicInstance();

  SetClipRegion();

  if (GetEnabled())
  {
    lFontColor = mFontColor;
    lBackColor = mBackColor;
  }
  else
  {
    lFontColor = mFontColorDisabled;
    lBackColor = mBackColorDisabled;
  }

  GetGraphicInstance()->SetFont(mFont);

  switch (mOrientation)
  {
    case SBO_LEFT  :
      lGI->SetColor(lBackColor);
      lGI->FillRectangle(CPRect(lL + 8, lT + 6, lL + GetWidth() - 6, lT + 26));

      // fx'ul, daca e activat
      /*
      if (mFxDrain->GetEnabled())
      {
        TRect lDstRect = Rect(lL + 8, lT + 6, lL + mFxDrain->GetWidth() + 8, lT + mFxDrain->GetHeight() + 6);
        TRect lSrcRect = Rect(0, 0, mFxDrain->GetWidth(), mFxDrain->GetHeight());
        mFxDrain->Paint();
        GetGraphicInstance()->PaintSurfaceToSurface(NULL, lDstRect, mFxDrain->GetCanvas(), lSrcRect);
      }
      */

      lGI->SetColor(lFontColor);
      lGI->TextOut2(lL + 32 + (GetWidth() - 64) / 2, lT + 8, mCaption, PPDT_CENTER);

      lGI->PaintSprite(lL, lT, mBtnSprite, 1, 1);
      lGI->PaintSprite(lL + GetWidth() - mBtnSprite->GetAnimWidth(), lT, mBtnSprite, 1, 0);
      break;

    case SBO_RIGHT :
      lGI->SetColor(lBackColor);
      lGI->FillRectangle(CPRect(lL + 6, lT + 6, lL + GetWidth() - 8, lT + 26));

      // fx'ul, daca e activat
      /*
      if (mFxDrain->GetEnabled())
      {
        TRect lDstRect = Rect(lL + 6, lT + 6, lL + 6 + mFxDrain->GetWidth(), lT + mFxDrain->GetHeight() + 6);
        TRect lSrcRect = Rect(0, 0, mFxDrain->GetWidth(), mFxDrain->GetHeight());
        mFxDrain->Paint();
        GetGraphicInstance()->PaintSurfaceToSurface(NULL, lDstRect, mFxDrain->GetCanvas(), lSrcRect);
      }
      */

      lGI->SetColor(lFontColor);
      lGI->TextOut2(lL + 32 + (GetWidth() - 64) / 2, lT + 8, mCaption, PPDT_CENTER);

      lGI->PaintSprite(lL, lT, mBtnSprite, 0, 0);
      lGI->PaintSprite(lL + GetWidth() - mBtnSprite->GetAnimWidth(), lT, mBtnSprite, 0, 1);
      break;
  }

}
//---------------------------------------------------------------------------

void CPP_CSButton::SetHeight(int _v)
{
  CPIControl::SetHeight(_v);

  //mFxDrain->SetHeight(20);
}
//---------------------------------------------------------------------------

void CPP_CSButton::SetWidth(int _v)
{
  CPIControl::SetWidth(_v);

  switch(mOrientation)
  {
    case SBO_LEFT  :
      //mFxDrain->SetWidth(GetWidth() - 11);
      break;

    case SBO_RIGHT :
      //mFxDrain->SetWidth(GetWidth() - 13);
      break;
  }
}
//---------------------------------------------------------------------------

void CPP_CSButton::SetFont(CPFont* _value)
{
   if (!_value)
   {
     return;
   }

   delete mFont;
   mFont = _value;
}
//---------------------------------------------------------------------------

void CPP_CSButton::SetOpacity(float _o)
{
  if (_o < 0.0)
  {
    mOpacity = 0.0;
    return;
  }

  if (_o > 1.0)
  {
    mOpacity = 0.0;
    return;
  }

  mOpacity = _o;
}
//---------------------------------------------------------------------------

CPP_CSButton::~CPP_CSButton()
{
  delete mFont;
  //delete mFxDrain;
}
//---------------------------------------------------------------------------
