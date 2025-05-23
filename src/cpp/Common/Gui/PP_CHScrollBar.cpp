//---------------------------------------------------------------------------
#pragma hdrstop

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

//#include "PPC_Cursor.h"
// TODO: temporary
#include "PPIG_CScreen.h"

#include "PP_CHScrollBar.h"
#include "Paths.h"
#include "basepath.h"

CPIHScrollBar::CPIHScrollBar(CPIWinControl* Sender) : CPIControl(Sender)
{
  mMin          = 0;
  mMax          = 10;
  mPos          = 0;
  mThumbChange  = 1;
  mClickChange  = 1;
  mThumbStyle   = SBT_FIXED;
  mState        = SBST_NONE;
  mBtnSprite    = GetGraphicInstance()->AddSprite(CPString(PATH_GUI_GFX) + "scrollbar.png", 4, 2);
  mThumbSprite  = GetGraphicInstance()->AddSprite(CPString(PATH_GUI_GFX) + "hthumb.png", 1, 1);

  SetWidth(GetBtnWidth());
  SetHeight(GetBtnHeight());

  SetHandleOnLeftMouseDown(std::bind(&CPIHScrollBar::LeftMouseDown, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
  SetHandleOnLeftMouseUp(std::bind(&CPIHScrollBar::LeftMouseUp, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
  SetHandleOnMouseMove(std::bind(&CPIHScrollBar::MouseMove, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}
//---------------------------------------------------------------------------

int CPIHScrollBar::GetThumbPos()
{
  switch(mThumbStyle)
  {
    case SBT_FIXED :
      return ((float)mPos / (mMax - mMin)) * (GetWidth() - 2 * GetBtnWidth() - GetThumbWidth()) + GetBtnWidth() + GetThumbWidth() / 2;

    case SBT_VARIABLE :
      return ((float)mPos / (mMax - mMin)) * (GetWidth() - 2 * GetBtnWidth() - GetThumbWidth()) + GetBtnWidth() + GetThumbWidth() / 2;
  }

  return GetBtnWidth();
}
//---------------------------------------------------------------------------

int CPIHScrollBar::GetBtnWidth()
{
  if (mBtnSprite)
  {
    return mBtnSprite->GetAnimWidth();
  }

  return 8;
}
//---------------------------------------------------------------------------

int CPIHScrollBar::GetBtnHeight()
{
  if (mBtnSprite)
  {
    return mBtnSprite->GetAnimHeight();
  }

  return 8;
}
//---------------------------------------------------------------------------

int CPIHScrollBar::GetThumbWidth()
{
  switch(mThumbStyle)
  {
    case SBT_FIXED :
      if (mThumbSprite)
      {
        return mThumbSprite->GetAnimWidth();
      }

    case SBT_VARIABLE :
      return ((float)mThumbChange / (mMax - mMin)) * (GetWidth() - 2 * GetBtnWidth());
  }

  return 0;
}
//---------------------------------------------------------------------------

int CPIHScrollBar::GetThumbHeight()
{
  if (mThumbSprite)
  {
    return mThumbSprite->GetAnimHeight();
  }

  return 8;
}
//---------------------------------------------------------------------------

void CPIHScrollBar::DrawButtons()
{
  int lL = GetAbsoluteLeft(), lT = GetAbsoluteTop();

  if (!mBtnSprite)
  {
    return;
  }

  // se poate face si cu switch (si se mai elimina niste linii in plus)
  if (mState == SBST_LINEUP)
  {
    GetGraphicInstance()->PaintSprite(lL, lT, mBtnSprite, 2, 1);
  }
  else
  {
    GetGraphicInstance()->PaintSprite(lL, lT, mBtnSprite, 2, 0);
  }

  if (mState == SBST_LINEDOWN)
  {
    GetGraphicInstance()->PaintSprite(lL + GetWidth() - GetBtnWidth(), lT, mBtnSprite, 3, 1);
  }
  else
  {
    GetGraphicInstance()->PaintSprite(lL + GetWidth() - GetBtnWidth(), lT, mBtnSprite, 3, 0);
  }
}
//---------------------------------------------------------------------------

void CPIHScrollBar::DrawTrackBar()
{
  int lL  = GetAbsoluteLeft(), lT = GetAbsoluteTop();
  int lBW = GetBtnWidth();

  GetGraphicInstance()->SetColor(0x808080);
  GetGraphicInstance()->FillRectangle(CPRect(lL + lBW, lT, lL + GetWidth() - lBW, lT + GetBtnHeight()));
}
//---------------------------------------------------------------------------

void CPIHScrollBar::DrawThumb()
{
  // BUG POSSIBLE
  int lL  = GetAbsoluteLeft(), lT = GetAbsoluteTop();
  int lTW = GetThumbWidth(), lTH = GetThumbHeight();
  int lBH = GetBtnHeight();

  // calculez pozitia thumbului si il desenez
  switch(mThumbStyle)
  {
    case SBT_FIXED :
      if (!mThumbSprite)
      {
        return;
      }

      GetGraphicInstance()->PaintSprite(
        lL + GetThumbPos() - lTW / 2,
        lT + lBH / 2 - lTH / 2,
        mThumbSprite,
        0, 0);
      break;

    case SBT_VARIABLE :
      GetGraphicInstance()->SetColor(0x909090);
      GetGraphicInstance()->FillRectangle(
        //Rect(lL, lT + GetThumbPos() - GetThumbHeight() / 2, lL + lBW, lT + GetThumbPos() + GetThumbHeight() / 2),
        CPRect(lL + GetThumbPos() - GetThumbWidth() / 2, lT, lL + GetThumbPos() + GetThumbWidth() / 2, lT + lBH));

      break;
  }
}
//---------------------------------------------------------------------------

// Descriere   : daca se apasa butonul stinga al mousului
// Param       : clari
// Rezultat    : se seteaza starea controlului
// Comentarii  :
void CPIHScrollBar::LeftMouseDown(CPIObject* Sender, int _x, int _y)
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
  if ((lT < _y) && (_y < lT + lBH) &&
      (lL + GetWidth() - lBW < _x) && (_x < lL + GetWidth()))
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
      if ((lT + lBH / 2 - lTH / 2 < _y) && (_y < lT + lBH / 2 + lTH / 2) &&
          (lL + GetThumbPos() - lTW / 2 < _x) && (_x < lL + GetThumbPos() + lTW / 2))
      {
        GetMainScreenInstance()->SetMouseCaptureControl(this);
        mState = SBST_TRACK;
        return;
      }
      break;

    case SBT_VARIABLE :
      /*
      if ((lL < _x) && (_x < lL + lBW) &&
          (lT + GetThumbPos() - GetThumbHeight() / 2 < _y) && (_y < lT + GetThumbPos() + GetThumbHeight() / 2))
      */

      if ((lT < _y) && (_y < lT + lBH) &&
          (lL + GetThumbPos() - GetThumbWidth() / 2 < _x) && (_x < lL + GetThumbPos() + GetThumbWidth() / 2))
      {
        GetMainScreenInstance()->SetMouseCaptureControl(this);
        mState = SBST_TRACK;
        return;
      }
      break;
  }

  // verific daca s'a apasat pe track bar
  if ((lT < _y) && (_y < lT + lBH))
  {
    if (_x < lL + GetThumbPos())
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

void CPIHScrollBar::LeftMouseUp(CPIObject* Sender, int _x, int _y)
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

void CPIHScrollBar::MouseMove(CPIObject* Sender, int _x, int _y)
{
  int lTW = GetThumbWidth(), lBW = GetBtnWidth();

  if (mState == SBST_TRACK)
  {
    if (IsPointInside(CPPoint(_x, _y)))
    {
      // ???
      SetPos((float)(_x - GetAbsoluteLeft() - lBW - lTW / 2) * (mMax - mMin) / (GetWidth() - 2 * lBW - lTW));
      OnScroll(SBC_TRACK, mPos);
    }
  }
}
//---------------------------------------------------------------------------

void CPIHScrollBar::OnChange()
{
  if (mHandleOnChange)
  {
    mHandleOnChange(this);
  }
}
//---------------------------------------------------------------------------

void CPIHScrollBar::OnScroll(TPP_ScrollBarCode _code, int& _position)
{
  if (mHandleOnScroll)
  {
    mHandleOnScroll(this, _code, _position);
    SetPos(_position);
  }
}
//---------------------------------------------------------------------------

void CPIHScrollBar::Paint()
{
  SetClipRegion();

  DrawTrackBar();
  DrawButtons();
  DrawThumb();
}
//---------------------------------------------------------------------------

void CPIHScrollBar::Update()
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

void CPIHScrollBar::SetWidth(int _w)
{
  switch(mThumbStyle)
  {
    case SBT_FIXED :
      if (_w < GetBtnWidth() * 2 + GetThumbWidth())
      {
        return;
      }
      break;

    case SBT_VARIABLE :
      if (_w < GetBtnWidth() * 2 + 6)
      {
        return;
      }
      break;
  }

  CPIControl::SetWidth(_w);
}
//---------------------------------------------------------------------------

void CPIHScrollBar::SetHeight(int _h)
{
  CPIControl::SetHeight(mBtnSprite->GetAnimHeight());
}
//---------------------------------------------------------------------------

void CPIHScrollBar::Resize(int _w, int _h)
{
  SetWidth(_w);
  SetHeight(_h);
}
//---------------------------------------------------------------------------

/*
void CPIHScrollBar::SetHandleOnChange (void (__closure* value)(CPIObject *Sender))
{
  mHandleOnChange = value;
}
//---------------------------------------------------------------------------

void CPIHScrollBar::SetHandleOnScroll (void (__closure* value)(CPIObject *Sender, TPP_ScrollBarCode _code, int& _position))
{
  mHandleOnScroll = value;
}
//---------------------------------------------------------------------------
*/

void CPIHScrollBar::SetMin(int _m)
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

  if (mPos < mMin)
  {
    mPos = mMin;
  }

  SetThumbChange(mThumbChange);
}
//---------------------------------------------------------------------------

void CPIHScrollBar::SetMax(int _m)
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

  if (mPos > mMax)
  {
    mPos = mMax;
  }

  SetThumbChange(mThumbChange);
}
//---------------------------------------------------------------------------

void CPIHScrollBar::SetPos(int _p)
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

void CPIHScrollBar::SetThumbChange(int _tc)
{
  if (_tc > mMax - mMin)
  {
    mThumbChange = mMax - mMin;
    return;
  }

  mThumbChange = _tc;
}
//---------------------------------------------------------------------------

void CPIHScrollBar::SetClickChange(int _cc)
{
  if (_cc > mMax - mMin)
  {
    mClickChange = mMax - mMin;
    return;
  }

  mClickChange = _cc;
}
//---------------------------------------------------------------------------

CPIHScrollBar::~CPIHScrollBar()
{
}
//---------------------------------------------------------------------------


