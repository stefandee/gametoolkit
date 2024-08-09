//---------------------------------------------------------------------------
#pragma hdrstop

#include "PP_CListBox.h"
#include "Paths.h"
#include "basepath.h"

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

CPIListBox::CPIListBox(CPIWinControl *Parent) : CPIWinControl(Parent)
{
  //mHandleOnDrawItem    = NULL;
  //mHandleOnMeasureItem = NULL;

  SetHandleOnLeftMouseDown(std::bind(&CPIListBox::LeftMouseDown, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

  // variabilele interne
  mColumns        = 0;
  mIntegralHeight = false;
  mItemHeight     = 10;
  mStyle          = LBS_STD;
  mAlign          = LA_CENTER;
  mScrollBy       = 4;
  mItemIndex      = -1;
  mTopIndex       = 0;
  mBackGround     = 0x400000;
  mY              = 0.0;

  //mItems = new CPList<CPString>(256);
  mFont  = new CPFont(PATH_FONTS + FILE_FONT_SMALL);
  mFont->SetHeight(24);

  // adaug un scroll bar
  mVScrollBar = new CPIVScrollBar(this);
  mVScrollBar->SetLeft(GetWidth() - mVScrollBar->GetWidth());
  mVScrollBar->SetTop(0);
  mVScrollBar->SetHeight(GetHeight());

  mVScrollBar->SetHandleOnScroll(std::bind(&CPIListBox::OnScroll, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

  mVScrollBar->SetPos(0);
  AddControl(mVScrollBar);
}
//---------------------------------------------------------------------------

void CPIListBox::SetFont(CPFont* _value)
{
  delete mFont;
  mFont = _value;
}
//---------------------------------------------------------------------------


void CPIListBox::OnDrawItem(int _index, const CPRect& _rect, TPI_ListOwnerState _state)
{
   if (mHandleOnDrawItem)
       mHandleOnDrawItem(this, _index, _rect, _state);
}
//---------------------------------------------------------------------------

void CPIListBox::OnMeasureItem(int _index, int& _height)
{
   if (mHandleOnMeasureItem)
       mHandleOnMeasureItem(this, _index, _height);
}
//---------------------------------------------------------------------------

/*
void CPIListBox::SetHandleOnDrawItem (void (__closure* value)(CPIObject *Sender, int _index, TRect& _rect, TPI_ListOwnerState _state))
{
   mHandleOnDrawItem = value;
}
//---------------------------------------------------------------------------

void CPIListBox::SetHandleOnMeasureItem (void (__closure* value)(CPIObject *Sender, int _index, int& _height))
{
   mHandleOnMeasureItem = value;
}
//---------------------------------------------------------------------------
*/

int  CPIListBox::ItemAtPos(TPos2D _p)
{
  return ItemAtPos(_p.x, _p.y);
}
//---------------------------------------------------------------------------

int  CPIListBox::ItemAtPos(int x, int y)
{
  int   lL = GetAbsoluteLeft(), lT = GetAbsoluteTop();
  CPRect lRect;
  int   lItem;

  lItem = mTopIndex;

  switch(mStyle)
  {
    case LBS_STD :
      while(lItem < mItems.size() && lItem < mTopIndex + GetHeight() / mFont->GetHeight() + 1)
      {
        lRect = ItemRect(lItem);

        if ( (lRect.left   + lL < x) &&
             (lRect.right  + lL > x) &&
             (lRect.top    + lT < y) &&
             (lRect.bottom + lT >= y))
        {
          return lItem;
        }

        lItem++;
      }
      break;

    case LBS_DRAWFIXED :
      while(lItem < mItems.size() && lItem < mTopIndex + GetHeight() / mItemHeight + 1)
      {
        lRect = ItemRect(lItem);

        if ( (lRect.left   + lL < x) &&
             (lRect.right  + lL > x) &&
             (lRect.top    + lT < y) &&
             (lRect.bottom + lT >= y))
        {
          return lItem;
        }

        lItem++;
      }
      break;

    case LBS_DRAWVARIABLE:
      // conditia de while e o ciudatenie - altfel ar trebui reluat un calcul
      while(lItem < mItems.size() && lRect.bottom != GetHeight() - 1)
      {
        lRect = ItemRect(lItem);

        if ( (lRect.left   + lL < x) &&
             (lRect.right  + lL > x) &&
             (lRect.top    + lT < y) &&
             (lRect.bottom + lT >= y))
        {
          return lItem;
        }

        lItem++;
      }
      break;
  }

  return -1;
}
//---------------------------------------------------------------------------

CPRect CPIListBox::ItemRect(int _item)
{
  int lTextTop, lItem, lTopClip, lBottomClip;

  if (mTopIndex > _item)
  {
    return CPRect(-1, -1, -1, -1);
  }

  switch(mStyle)
  {
    case LBS_STD     :
      lTextTop = mTopIndex * mFont->GetHeight() - mY;
      lItem    = mTopIndex;

      while (lItem < mItems.size())
      {
        // clip sus - se poate optimiza (scos in afara, aranjat bucla)
        lTopClip = lTextTop;
        if (lTextTop < 0)
        {
          lTopClip = 0;
        }

        // clip jos
        lBottomClip = lTextTop + mFont->GetHeight();
        if (lBottomClip > GetHeight())
        {
          lBottomClip = GetHeight() - 1;
        }

        lTextTop += mFont->GetHeight();

        if (lItem == _item)
        {
          return CPRect(0, lTopClip, GetEWidth() - 1, lBottomClip);
        }

        lItem++;
      }
      break;

    case LBS_DRAWFIXED    :
      lTextTop = mTopIndex * mItemHeight - mY;
      lItem    = mTopIndex;

      while (lItem < mItems.size())
      {
        // clip sus - se poate optimiza (scos in afara, aranjat bucla)
        lTopClip = lTextTop;
        if (lTextTop < 0)
        {
          lTopClip = 0;
        }

        // clip jos
        lBottomClip = lTextTop + mItemHeight;
        if (lBottomClip > GetHeight())
        {
          lBottomClip = GetHeight() - 1;
        }

        lTextTop += mItemHeight;

        if (lItem == _item)
        {
          return CPRect(0, lTopClip, GetEWidth() - 1, lBottomClip);
        }

        lItem++;
      }
      break;

    case LBS_DRAWVARIABLE :
      int lTempHeight;

      lTextTop = 0;

      for(int i = 0; i < mTopIndex; i++)
      {
        OnMeasureItem(lItem, lTempHeight);
        lTextTop += lTempHeight;
      }

      lTextTop = lTextTop - mY;
      lItem    = mTopIndex;

      while (lItem < mItems.size())
      {
        // clip sus - se poate optimiza (scos in afara, aranjat bucla)
        lTopClip = lTextTop;
        if (lTextTop < 0)
        {
          lTopClip = 0;
        }

        OnMeasureItem(lItem, lTempHeight);

        // clip jos
        lBottomClip = lTextTop + lTempHeight;
        if (lBottomClip > GetHeight())
        {
          lBottomClip = GetHeight() - 1;
        }

        lTextTop += lTempHeight;

        if (lItem == _item)
        {
          return CPRect(0, lTopClip, GetEWidth() - 1, lBottomClip);
        }

        lItem++;
      }
      break;
  }

  return CPRect(-1, -1, -1, -1);
}
//---------------------------------------------------------------------------

void  CPIListBox::Clear()
{
  mItems.clear();
}
//---------------------------------------------------------------------------

void CPIListBox::SetTopIndex(int _ti)
{
  if ((_ti < 0) ||
      (_ti >= mItems.size()))
  {
    return;
  }

  mTopIndex = _ti;

  switch(mStyle)
  {
    case LBS_STD :
      mY = mTopIndex * mFont->GetHeight();
      break;

    case LBS_DRAWFIXED :
      mY = mTopIndex * mItemHeight;
      break;

    case LBS_DRAWVARIABLE :

      int lTempHeight;

      mY = 0.0;

      for(int i = 0; i < mTopIndex; i++)
      {
        OnMeasureItem(i, lTempHeight);
        mY += lTempHeight;
      }
      break;
  }
}
//---------------------------------------------------------------------------

void CPIListBox::ScrollUp()
{
  if (mItems.size() <= 0)
  {
    return;
  }

  mY -= mScrollBy;

  if (mY < 0)
  {
    mY         = 0.0;
    mItemIndex = 0;
    return;
  }

  switch(mStyle)
  {
    case LBS_STD :
      if (mY < mTopIndex * mFont->GetHeight())
      {
        mTopIndex += (mY - mTopIndex * mFont->GetHeight()) / mFont->GetHeight();
      }
      break;

    case LBS_DRAWFIXED :
      if (mY < mTopIndex * mItemHeight)
      {
        mTopIndex += (mY - mTopIndex * mItemHeight) / mItemHeight;
      }
      break;

    case LBS_DRAWVARIABLE :
      int lTempHeight, lFullHeight = 0, lItem = 0;

      while(lFullHeight < mY && lItem < mItems.size() - 1)
      {
        OnMeasureItem(lItem, lTempHeight);
        lFullHeight += lTempHeight;
        lItem++;
      }

      mTopIndex = lItem;
      break;
  }

}
//---------------------------------------------------------------------------

void CPIListBox::ScrollDown()
{
  if (mItems.size() <= 0)
  {
    return;
  }

  mY += mScrollBy;

  switch(mStyle)
  {
    case LBS_STD :
      if (mY > (mTopIndex + 1) * mFont->GetHeight())
      {
        mTopIndex += (mY - mTopIndex * mFont->GetHeight()) / mFont->GetHeight();
      }

      if (mY > (mItems.size() - 1) * mFont->GetHeight())
      {
        mY = (mItems.size() - 1) * mFont->GetHeight();
      }
      break;

    case LBS_DRAWFIXED :
      if (mY > (mTopIndex + 1) * mItemHeight)
      {
        mTopIndex += (mY - mTopIndex * mItemHeight) / mItemHeight;
      }

      if (mY > (mItems.size() - 1) * mItemHeight)
      {
        mY = (mItems.size() - 1) * mItemHeight;
      }
      break;

    case LBS_DRAWVARIABLE :
      int lTempHeight, lFullHeight = 0, lItem = 0;

      while(lFullHeight < mY && lItem < mItems.size() - 1)
      {
        OnMeasureItem(lItem, lTempHeight);
        lFullHeight += lTempHeight;
        lItem++;
      }

      mTopIndex = lItem;

      if (lItem == mItems.size() - 1)
      {
        mY = lFullHeight;
      }

      break;
  }
}
//---------------------------------------------------------------------------

void CPIListBox::Update()
{
  CPIWinControl::Update();

  // updatez scrollbarul
  mVScrollBar->SetMin(0);
  mVScrollBar->SetMax(mItems.size() - 1);
}
//---------------------------------------------------------------------------

void CPIListBox::Paint()
{
  SetClipRegion();

  // desenez backgroundul - doar de test
  GetGraphicInstance()->SetColor(mBackGround);
  GetGraphicInstance()->FillRectangle(CPRect(GetAbsoluteLeft(), GetAbsoluteTop(), GetAbsoluteLeft() + GetEWidth() - 1, GetAbsoluteTop() + GetHeight() - 1));

  // desenul efectiv
  switch(mStyle)
  {
    case LBS_STD     :
      PaintAsStandard();
      break;

    case LBS_DRAWFIXED    :
      PaintAsFixed();
      break;

    case LBS_DRAWVARIABLE :
      PaintAsVariable();
      break;
  }

  CPIWinControl::Paint();
}
//---------------------------------------------------------------------------

void CPIListBox::PaintAsStandard()
{
  int     lL = GetAbsoluteLeft(), lT = GetAbsoluteTop(), lTextTop;
  int     lAlign, lDrawAlign, lItem, lTopClip, lBottomClip;

  // acum desenez textul
  GetGraphicInstance()->SetFont(mFont);
  GetGraphicInstance()->SetColor(0xFFFFFFFF);

  // font align
  switch(mAlign)
  {
    case LA_LEFT :
      lAlign = PPDT_LEFT;
      break;

    case LA_CENTER :
      lAlign = PPDT_CENTER;
      break;

    case LA_RIGHT :
      lAlign = PPDT_RIGHT;
      break;
  }

  lTextTop = mTopIndex * mFont->GetHeight() - mY;
  lItem    = mTopIndex;

  while (lTextTop < GetHeight() && (lItem < mItems.size()))
  {
    // clip sus - se poate optimiza (scos in afara, aranjat bucla)
    lTopClip = lTextTop;
    lDrawAlign = lAlign | PPDT_BOTTOM | PPDT_SINGLELINE;
    if (lTextTop < 0)
    {
      lTopClip = 0;
    }

    // clip jos
    lBottomClip = lTextTop + mFont->GetHeight();
    if (lBottomClip > GetHeight())
    {
      lDrawAlign = lAlign | PPDT_TOP | PPDT_SINGLELINE;
      lBottomClip = GetHeight() - 1;
    }

    // desen
    GetGraphicInstance()->TextOut2(
      //CPRect(lL, lT + lTopClip, lL + GetEWidth() - 1, lT + lBottomClip),
      lL, lT + lTopClip,
      mItems[lItem].c_str(),
      lDrawAlign);

    // daca e selectat
    if (mItemIndex == lItem)
    {
        GetGraphicInstance()->SetColor(0xFFFFFFFF);
        GetGraphicInstance()->Rectangle(CPRect(lL, lT + lTopClip, lL + GetEWidth() - 1, lT + lBottomClip));
    }

    lTextTop += mFont->GetHeight();
    lItem++;
  }
}
//---------------------------------------------------------------------------

void CPIListBox::PaintAsFixed()
{
  int     lL = GetAbsoluteLeft(), lT = GetAbsoluteTop(), lTextTop;
  int     lItem, lTopClip, lBottomClip;

  lTextTop = mTopIndex * mItemHeight - mY;
  lItem    = mTopIndex;

  // acum desenez textul
  GetGraphicInstance()->SetFont(mFont);
  GetGraphicInstance()->SetColor(0xFFFFFFFF);

  while (lTextTop < GetHeight() && (lItem < mItems.size()))
  {
    // clip sus - se poate optimiza (scos in afara, aranjat bucla)
    lTopClip = lTextTop;
    if (lTextTop < 0)
    {
      lTopClip = 0;
    }

    // clip jos
    lBottomClip = lTextTop + mItemHeight;
    if (lBottomClip > GetHeight())
    {
      lBottomClip = GetHeight() - 1;
    }

    OnDrawItem(lItem, CPRect(lL, lT + lTopClip, lL + GetEWidth() - 1, lT + lBottomClip), LOS_DISABLED);

    lTextTop += mItemHeight;
    lItem++;
  }
}
//---------------------------------------------------------------------------

void CPIListBox::PaintAsVariable()
{
  int     lL = GetAbsoluteLeft(), lT = GetAbsoluteTop(), lTextTop;
  int     lItem, lTopClip, lBottomClip, lTempHeight;


  // calcul pentru locatia curenta a textului in pixeli
  lTextTop = 0;

  for(int i = 0; i < mTopIndex; i++)
  {
    OnMeasureItem(i, lTempHeight);
    lTextTop += lTempHeight;
  }

  lItem    = mTopIndex;

  // acum desenez textul
  GetGraphicInstance()->SetFont(mFont);
  GetGraphicInstance()->SetColor(0xFFFFFFFF);

  while (lTextTop < GetHeight() && (lItem < mItems.size()))
  {
    // clip sus - se poate optimiza (scos in afara, aranjat bucla)
    lTopClip = lTextTop;
    if (lTextTop < 0)
    {
      lTopClip = 0;
    }

    OnMeasureItem(lItem, lTempHeight);

    // clip jos
    lBottomClip = lTextTop + lTempHeight;
    if (lBottomClip > GetHeight())
    {
      lBottomClip = GetHeight() - 1;
    }

    OnDrawItem(lItem, CPRect(lL, lT + lTopClip, lL + GetEWidth() - 1, lT + lBottomClip), LOS_DISABLED);

    lTextTop += lTempHeight;
    lItem++;
  }
}
//---------------------------------------------------------------------------

void CPIListBox::SetItemHeight(int _ih)
{
  if (_ih <= 0)
  {
    mItemHeight = mFont->GetHeight();
    return;
  }

  mItemHeight = _ih;
}
//---------------------------------------------------------------------------

void CPIListBox::SetStyle(TPI_ListBoxStyle _s)
{
  mY = 0.0;
  mTopIndex = 0;

  mStyle = _s;

  switch(mStyle)
  {
    case LBS_DRAWFIXED    :
      SetHandleOnDrawItem(std::bind(&CPIListBox::DrawItemFixed, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
      break;

    case LBS_DRAWVARIABLE :
      SetHandleOnDrawItem(std::bind(&CPIListBox::DrawItemVariable, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
      SetHandleOnMeasureItem(std::bind(&CPIListBox::MeasureItemVariable, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
      break;
  }
}
//---------------------------------------------------------------------------

void CPIListBox::DrawItemFixed(CPIObject *Sender, int _index, const CPRect& _rect, TPI_ListOwnerState _state)
{
  int lAlign, lDrawAlign;

  // din cauza textului
  switch(mAlign)
  {
    case LA_LEFT :
      lAlign = PPDT_LEFT;
      break;

    case LA_CENTER :
      lAlign = PPDT_CENTER;
      break;

    case LA_RIGHT :
      lAlign = PPDT_RIGHT;
      break;
  }

  if (_rect.bottom == GetHeight() - 1)
  {
    lDrawAlign = lAlign | PPDT_BOTTOM | PPDT_SINGLELINE;
  }
  else
  {
    lDrawAlign = lAlign | PPDT_TOP | PPDT_SINGLELINE;
  }

  // desen
  // has no member DrawTextA ???
  /*GetGraphicInstance()->DrawText(
    NULL,
    mItems->Get(_index)->c_str(),
    _rect,
    lDrawAlign);*/

  // daca e selectat
  if (mItemIndex == _index)
  {
    GetGraphicInstance()->SetColor(0xFFFFFFFF);
    GetGraphicInstance()->Rectangle(_rect);
  }
}
//---------------------------------------------------------------------------

void CPIListBox::DrawItemVariable(CPIObject *Sender, int _index, const CPRect& _rect, TPI_ListOwnerState _state)
{
  DrawItemFixed(this, _index, _rect, _state);
}
//---------------------------------------------------------------------------

void CPIListBox::MeasureItemVariable(CPIObject *Sender, int _index, int& _height)
{
  _height = 18 + _index * 3;
}
//---------------------------------------------------------------------------

void CPIListBox::LeftMouseDown(CPIObject *Sender, int _x, int _y)
{
  mItemIndex = ItemAtPos(_x, _y);
}
//---------------------------------------------------------------------------

// Descriere   : get effective width
// Param       :
// Rezultat    :
// Comentarii  : din cauza ca ScrollBarul maninca o bucata de latime,
//               textul nu poate folosi latimea controlului (GetWidth) pentru
//               a se desena; de accea, din GetWidth trebuie scazut ScrollBar->GetWidth()
int CPIListBox::GetEWidth()
{
  return GetWidth() - mVScrollBar->GetWidth();
}
//---------------------------------------------------------------------------

void CPIListBox::SetWidth(int _w)
{
  if (_w < mVScrollBar->GetWidth())
  {
    return;
  }

  CPIWinControl::SetWidth(_w);

  // aliniat la dreapta listului
  mVScrollBar->SetLeft(GetWidth() - mVScrollBar->GetWidth());
}
//---------------------------------------------------------------------------

void CPIListBox::SetHeight(int _h)
{
  mVScrollBar->SetHeight(_h);
  CPIWinControl::SetHeight(mVScrollBar->GetHeight());
}
//---------------------------------------------------------------------------

void CPIListBox::OnScroll(CPIObject* Sender, TPP_ScrollBarCode _code, int _position)
{
  switch(_code)
  {
    case SBC_LINEUP :
      ScrollUp();
      break;
    case SBC_LINEDOWN :
      ScrollDown();
      break;
    default :
      SetTopIndex(_position);
  }
}
//---------------------------------------------------------------------------

void CPIListBox::SetItemIndex(int _ii)
{
  if ((_ii < -1) || (_ii > mItems.size() - 1))
  {
    return;
  }

  mItemIndex = _ii;
}
//---------------------------------------------------------------------------

CPIListBox::~CPIListBox()
{
  delete mFont;
  //delete mItems;
}
//---------------------------------------------------------------------------

