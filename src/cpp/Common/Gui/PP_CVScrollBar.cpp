//---------------------------------------------------------------------------
#pragma hdrstop

#include "PP_CVScrollBar.h"
//#include "PPC_Cursor.h"
#include "PPIG_CScreen.h"
#include "Paths.h"
#include "basepath.h"

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

CPIVScrollBar::CPIVScrollBar(CPIWinControl* Sender) : CPIControl(Sender)
{
  mMin          = 0;
  mMax          = 10;
  mPos          = 0;
  mThumbChange  = 1;
  mClickChange  = 1;
  //mStyle        = SBS_HORIZONTAL;
  mThumbStyle   = SBT_FIXED;
  mState        = SBST_NONE;
  mBtnSprite    = GetGraphicInstance()->AddSprite(CPString(PATH_GUI_GFX) + "scrollbar.png", 4, 2);
  mThumbSprite  = GetGraphicInstance()->AddSprite(CPString(PATH_GUI_GFX) + "vthumb.png", 1, 1);

  SetWidth(mBtnSprite->GetAnimWidth());

  SetHandleOnLeftMouseDown(std::bind(&CPIVScrollBar::LeftMouseDown, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
  SetHandleOnLeftMouseUp(std::bind(&CPIVScrollBar::LeftMouseUp, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
  SetHandleOnMouseMove(std::bind(&CPIVScrollBar::MouseMove, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}
//---------------------------------------------------------------------------

int CPIVScrollBar::GetThumbPos()
{
  switch(mThumbStyle)
  {
    case SBT_FIXED :
      return ((float)mPos / (mMax - mMin)) * (GetHeight() - 2 * GetBtnHeight() - GetThumbHeight()) + GetBtnHeight() + GetThumbHeight() / 2;

    case SBT_VARIABLE :
      return ((float)mPos / (mMax - mMin)) * (GetHeight() - 2 * GetBtnHeight() - GetThumbHeight()) + GetBtnHeight() + GetThumbHeight() / 2;
  }

  return GetBtnHeight();
}
//---------------------------------------------------------------------------

int CPIVScrollBar::GetBtnWidth()
{
  if (mBtnSprite)
  {
    return mBtnSprite->GetAnimWidth();
  }

  return 8;
}
//---------------------------------------------------------------------------

int CPIVScrollBar::GetBtnHeight()
{
  if (mBtnSprite)
  {
    return mBtnSprite->GetAnimHeight();
  }

  return 8;
}
//---------------------------------------------------------------------------

int CPIVScrollBar::GetThumbWidth()
{
  if (mThumbSprite)
  {
    return mThumbSprite->GetAnimWidth();
  }

  return 8;
}
//---------------------------------------------------------------------------

int CPIVScrollBar::GetThumbHeight()
{
  switch(mThumbStyle)
  {
    case SBT_FIXED :
      if (mThumbSprite)
      {
        return mThumbSprite->GetAnimHeight();
      }

    case SBT_VARIABLE :
      return ((float)mThumbChange / (mMax - mMin)) * (GetHeight() - 2 * GetBtnHeight());
  }

  return 0;
}
//---------------------------------------------------------------------------

void CPIVScrollBar::DrawButtons()
{
  int lL = GetAbsoluteLeft(), lT = GetAbsoluteTop();

  if (!mBtnSprite)
  {
    return;
  }

  if (mState == SBST_LINEUP)
  {
    GetGraphicInstance()->PaintSprite(lL, lT, mBtnSprite, 0, 1);
  }
  else
  {
    GetGraphicInstance()->PaintSprite(lL, lT, mBtnSprite, 0, 0);
  }

  if (mState == SBST_LINEDOWN)
  {
    GetGraphicInstance()->PaintSprite(lL, lT + GetHeight() - GetBtnHeight(), mBtnSprite, 1, 1);
  }
  else
  {
    GetGraphicInstance()->PaintSprite(lL, lT + GetHeight() - GetBtnHeight(), mBtnSprite, 1, 0);
  }
}
//---------------------------------------------------------------------------

void CPIVScrollBar::DrawTrackBar()
{
  int lL = GetAbsoluteLeft(), lT = GetAbsoluteTop();
  int lBH = mBtnSprite->GetAnimHeight();

  GetGraphicInstance()->SetColor(0xFF808080);
  GetGraphicInstance()->FillRectangle(CPRect(lL, lT + lBH, lL + GetWidth(), lT + GetHeight() - lBH));
}
//---------------------------------------------------------------------------

void CPIVScrollBar::DrawThumb()
{
  int lL  = GetAbsoluteLeft(), lT = GetAbsoluteTop();
  int lTW = GetThumbWidth(), lTH = GetThumbHeight();
  int lBW = GetBtnWidth();

  // calculez pozitia thumbului
  switch(mThumbStyle)
  {
    case SBT_FIXED :
      if (!mThumbSprite)
      {
        return;
      }

      GetGraphicInstance()->PaintSprite(
        lL + lBW / 2 - lTW / 2,
        lT + GetThumbPos() - lTH / 2,
        mThumbSprite,
        0, 0);
      break;

    case SBT_VARIABLE :
      GetGraphicInstance()->SetColor(0x909090);
      GetGraphicInstance()->FillRectangle(
        CPRect(lL, lT + GetThumbPos() - GetThumbHeight() / 2, lL + lBW, lT + GetThumbPos() + GetThumbHeight() / 2)
        );
      break;
  }
}
//---------------------------------------------------------------------------

// Descriere   : daca se apasa butonul stinga al mousului
// Param       : clari
// Rezultat    : se seteaza starea controlului
// Comentarii  :
void CPIVScrollBar::LeftMouseDown(CPIObject* Sender, int _x, int _y)
{
  int lL  = GetAbsoluteLeft(), lT  = GetAbsoluteTop(),
      lTW = GetThumbWidth(),   lTH = GetThumbHeight(),
      lBW = GetBtnWidth(),     lBH = GetBtnHeight();

  // verific daca s'a apasat butonul "up"
  if ((lL < _x) && (_x < lL + lBW) &&
      (lT < _y) && (_y < lT + lBH))
  {
    GetMainScreenInstance()->SetMouseCaptureControl(this);
    mState = SBST_LINEUP;

    // se genereaza eventul corespunzator
    OnScroll(SBC_LINEUP, mPos);
    return;
  }

  // verific daca s'a apasat butonul "down"
  if ((lL < _x) && (_x < lL + lBW) &&
      (lT + GetHeight() - lBH < _y) && (_y < lT + GetHeight()))
  {
    GetMainScreenInstance()->SetMouseCaptureControl(this);
    mState = SBST_LINEDOWN;

    // se genereaza eventul corespunzator
    OnScroll(SBC_LINEDOWN, mPos);
    return;
  }

  // verific daca s'a apasat pe thumb tab

  // fac verificarea in functie de stilul thumbului
  switch(mThumbStyle)
  {
    case SBT_FIXED :
      if ((lL + lBW / 2 - lTW / 2 < _x) && (_x < lL + lBW / 2 + lTW / 2) &&
          (lT + GetThumbPos() - lTH / 2 < _y) && (_y < lT + GetThumbPos() + lTH / 2))
      {
        GetMainScreenInstance()->SetMouseCaptureControl(this);
        mState = SBST_TRACK;
        return;
      }
      break;

    case SBT_VARIABLE :
      if ((lL < _x) && (_x < lL + lBW) &&
          (lT + GetThumbPos() - GetThumbHeight() / 2 < _y) && (_y < lT + GetThumbPos() + GetThumbHeight() / 2))
      {
        GetMainScreenInstance()->SetMouseCaptureControl(this);
        mState = SBST_TRACK;
        return;
      }
      break;
  }

  // verific daca s'a apasat pe track bar
  if ((lL < _x) && (_x < lL + lBW))
  {
    if (_y < lT + GetThumbPos())
    {
      GetMainScreenInstance()->SetMouseCaptureControl(this);
      mState = SBST_PAGEDOWN;

      // se genereaza eventul corespunzator
      OnScroll(SBC_PAGEDOWN, mPos);
    }
    else
    {
      GetMainScreenInstance()->SetMouseCaptureControl(this);
      mState = SBST_PAGEUP;

      // se genereaza eventul corespunzator
      OnScroll(SBC_PAGEUP, mPos);
    }
  }

  // s'a apasat altundeva
}
//---------------------------------------------------------------------------

void CPIVScrollBar::LeftMouseUp(CPIObject* Sender, int _x, int _y)
{
  if (mState == SBST_TRACK)
  {
    // se genereaza eventul corespunzator
    OnScroll(SBC_POSITION, mPos);
  }

  mState = SBST_NONE;
  GetMainScreenInstance()->SetMouseCaptureControl(NULL);
}
//---------------------------------------------------------------------------

void CPIVScrollBar::MouseMove(CPIObject* Sender, int _x, int _y)
{
  int lTH = GetThumbHeight(), lBH = GetBtnHeight();

  if (mState == SBST_TRACK)
  {
    if (IsPointInside(CPPoint(_x, _y)))
    {
      SetPos((float)(_y - GetAbsoluteTop() - lBH - lTH / 2) * (mMax - mMin) / (GetHeight() - 2 * lBH - lTH));
      OnScroll(SBC_TRACK, mPos);
    }
  }
}
//---------------------------------------------------------------------------

void CPIVScrollBar::OnChange()
{
  if (mHandleOnChange)
  {
    mHandleOnChange(this);
  }
}
//---------------------------------------------------------------------------

void CPIVScrollBar::OnScroll(TPP_ScrollBarCode _code, int& _position)
{
  if (mHandleOnScroll)
  {
    mHandleOnScroll(this, _code, _position);
    SetPos(_position);
  }
}
//---------------------------------------------------------------------------

void CPIVScrollBar::Paint()
{
  SetClipRegion();

  DrawTrackBar();
  DrawButtons();
  DrawThumb();
}
//---------------------------------------------------------------------------

void CPIVScrollBar::Update()
{
  CPIControl::Update();

  switch(mState)
  {
    case SBST_LINEUP :
      SetPos(GetPos() - mClickChange);
      OnScroll(SBC_LINEUP, mPos);
      break;

    case SBST_LINEDOWN :
      SetPos(GetPos() + mClickChange);
      OnScroll(SBC_LINEDOWN, mPos);
      break;

    case SBST_PAGEUP :
      SetPos(GetPos() + mThumbChange);
      OnScroll(SBC_PAGEUP, mPos);
      break;

    case SBST_PAGEDOWN :
      SetPos(GetPos() - mThumbChange);
      OnScroll(SBC_PAGEDOWN, mPos);
      break;
  }
}
//---------------------------------------------------------------------------

void CPIVScrollBar::SetWidth(int _w)
{
  CPIControl::SetWidth(mBtnSprite->GetAnimWidth());
}
//---------------------------------------------------------------------------

void CPIVScrollBar::SetHeight(int _h)
{
  switch(mThumbStyle)
  {
    case SBT_FIXED :
      if (_h < GetBtnHeight() * 2 + GetThumbHeight())
      {
        return;
      }
      break;

    case SBT_VARIABLE :
      if (_h < GetBtnHeight() * 2 + 6)
      {
        return;
      }
      break;
  }

  CPIControl::SetHeight(_h);
}
//---------------------------------------------------------------------------

void CPIVScrollBar::Resize(int _w, int _h)
{
  SetWidth(_w);
  SetHeight(_h);
}
//---------------------------------------------------------------------------

/*
void CPIVScrollBar::SetHandleOnChange (void (__closure* value)(CPIObject *Sender))
{
  mHandleOnChange = value;
}
//---------------------------------------------------------------------------

void CPIVScrollBar::SetHandleOnScroll (void (__closure* value)(CPIObject *Sender, TPP_ScrollBarCode _code, int& _position))
{
  mHandleOnScroll = value;
}
//---------------------------------------------------------------------------
*/

void CPIVScrollBar::SetMin(int _m)
{
  int lTmp;

  // nu pot fi ambele 0
  if ((_m == 0) && (mMax == 0))
  {
    return;
  }

  mMin = _m;

  if (mMin > mMax)
  {
    lTmp = mMin;
    mMin = mMax;
    mMax = lTmp;
  }

  SetThumbChange(mThumbChange);
}
//---------------------------------------------------------------------------

void CPIVScrollBar::SetMax(int _m)
{
  int lTmp;

  // nu pot fi ambele 0
  if ((_m == 0) && (mMin == 0))
  {
    return;
  }

  mMax = _m;

  if (mMin > mMax)
  {
    lTmp = mMin;
    mMin = mMax;
    mMax = lTmp;
  }

  SetThumbChange(mThumbChange);
}
//---------------------------------------------------------------------------

void CPIVScrollBar::SetPos(int _p)
{
  mPos = _p;

  if (mPos < mMin)
  {
    mPos = mMin;
  }

  if (mPos > mMax)
  {
    mPos = mMax;
  }

  OnChange();
}
//---------------------------------------------------------------------------

void CPIVScrollBar::SetThumbChange(int _tc)
{
  if (_tc > mMax - mMin)
  {
    mThumbChange = mMax - mMin;
    return;
  }

  mThumbChange = _tc;
}
//---------------------------------------------------------------------------

void CPIVScrollBar::SetClickChange(int _cc)
{
  if (_cc > mMax - mMin)
  {
    mClickChange = mMax - mMin;
    return;
  }

  mClickChange = _cc;
}
//---------------------------------------------------------------------------

CPIVScrollBar::~CPIVScrollBar()
{
}
//---------------------------------------------------------------------------


