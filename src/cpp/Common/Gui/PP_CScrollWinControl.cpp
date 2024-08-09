//---------------------------------------------------------------------------
#pragma hdrstop

#include "PP_CScrollWinControl.h"

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

CPIScrollWinControl::CPIScrollWinControl(CPIWinControl* Parent) : CPIWinControl(Parent)
{
  mAutoScroll = false;
  mHScrollBar = new CPIHScrollBar(this);
  mVScrollBar = new CPIVScrollBar(this);

  mHScrollBar->SetVisible(false);
  mVScrollBar->SetVisible(false);

  mHScrollBar->SetHandleOnScroll(std::bind(&CPIScrollWinControl::OnHScroll, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
  mVScrollBar->SetHandleOnScroll(std::bind(&CPIScrollWinControl::OnVScroll, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

  mHScrollBar->SetThumbStyle(SBT_FIXED);
  mVScrollBar->SetThumbStyle(SBT_FIXED);

  /*
  mHScrollBar->SetWidth(GetWidth());
  mHScrollBar->SetLeft(0);
  mHScrollBar->SetTop(GetHeight() - mHScrollBar->SetHeight());

  mVScrollBar->SetHeight(GetHeight());
  mVScrollBar->SetLeft(GetWidth() - mVScrollBar->SetWidth());
  mVScrollBar->SetTop(0);
  */

  AddControl(mHScrollBar);
  AddControl(mVScrollBar);

  mHScrollBar->BringToFront();
  mVScrollBar->BringToFront();

  mHPos = 0;
  mVPos = 0;

  SetWidth(75);
  SetHeight(75);
}
//---------------------------------------------------------------------------

void CPIScrollWinControl::ComputeLimits()
{
  int lHMin, lHMax, lVMin, lVMax;
  CPIControlList* lList = GetControls();

  // metoda anterioara
  CPIWinControl::Update();

  // trebuie aflat range'ul in care sint incadrate controalele
  // in caz ca exista controale miscatoare (behehe!)
  lHMin = lList->GetControl(0)->GetLeft();
  lHMax = lList->GetControl(0)->GetLeft();
  lVMin = lList->GetControl(0)->GetTop();
  lVMax = lList->GetControl(0)->GetTop();

  for(int i = 0; i < GetControls()->Count(); i++)
  {
    if ( (lList->GetControl(i) != mHScrollBar) &&
         (lList->GetControl(i) != mVScrollBar))
    {
      if (lHMin > lList->GetControl(i)->GetLeft())
      {
        lHMin = lList->GetControl(i)->GetLeft();
      }

      if (lHMax < lList->GetControl(i)->GetLeft())
      {
        lHMax = lList->GetControl(i)->GetLeft();
      }

      if (lVMin > lList->GetControl(i)->GetTop())
      {
        lVMin = lList->GetControl(i)->GetTop();
      }

      if (lVMax < lList->GetControl(i)->GetTop())
      {
        lVMax = lList->GetControl(i)->GetTop();
      }
    }
  }

  // o data range'urile alflate, scrollbarurile trebuie updatate dupa cum
  // urmeaza
  mHScrollBar->SetMin(lHMin);
  mHScrollBar->SetMax(lHMax);

  mVScrollBar->SetMin(lVMin);
  mVScrollBar->SetMax(lVMax);
}
//---------------------------------------------------------------------------

/*
void CPIScrollWinControl::Update()
{
  CPIWinControl::Update();

  /*

  logWriteILn(lHMin);
  logWriteILn(lHMax);
  logWriteILn(lVMin);
  logWriteILn(lVMax);
  */
//}
//---------------------------------------------------------------------------

void CPIScrollWinControl::Paint()
{
  SetClipRegion();

  GetGraphicInstance()->SetColor(0xFF303030);
  GetGraphicInstance()->FillRectangle(GetBoundsRect());

  CPIWinControl::Paint();
}
//---------------------------------------------------------------------------

void CPIScrollWinControl::SetAutoScroll(bool _v)
{
  mAutoScroll = _v;

  if (mAutoScroll)
  {
    mHScrollBar->SetVisible(true);
    mVScrollBar->SetVisible(true);
  }
  else
  {
    mHScrollBar->SetVisible(false);
    mVScrollBar->SetVisible(false);
  }
}
//---------------------------------------------------------------------------

void CPIScrollWinControl::OnHScroll(CPIObject *Sender, TPP_ScrollBarCode _code, int& _position)
{
  ScrollControls(_position - mHPos, 0);
  mHPos = _position;
}
//---------------------------------------------------------------------------

void CPIScrollWinControl::OnVScroll(CPIObject *Sender, TPP_ScrollBarCode _code, int& _position)
{
  ScrollControls(0, _position - mVPos);
  mVPos = _position;
}
//---------------------------------------------------------------------------

void CPIScrollWinControl::ScrollControls(int _h, int _v)
{
  CPIControlList* lList = GetControls();

  for(int i = 0; i < GetControls()->Count(); i++)
  {
    if ((lList->GetControl(i) != mHScrollBar) &&
        (lList->GetControl(i) != mVScrollBar))
    {
      lList->GetControl(i)->SetLeft(lList->GetControl(i)->GetLeft() + _h);
      lList->GetControl(i)->SetTop(lList->GetControl(i)->GetTop() + _v);
    }
  }
}
//---------------------------------------------------------------------------

void CPIScrollWinControl::SetWidth(int _v)
{
  CPIWinControl::SetWidth(_v);

  mHScrollBar->SetWidth(GetWidth() - mVScrollBar->GetWidth());
  mHScrollBar->SetLeft(0);
  //mHScrollBar->SetTop(GetHeight() - mHScrollBar->SetHeight());

  mVScrollBar->SetHeight(GetHeight() - mHScrollBar->GetHeight());
  mVScrollBar->SetLeft(GetWidth() - mVScrollBar->GetWidth());
  //mVScrollBar->SetTop(0);
}
//---------------------------------------------------------------------------

void CPIScrollWinControl::SetHeight(int _v)
{
  CPIWinControl::SetHeight(_v);

  mHScrollBar->SetWidth(GetWidth() - mVScrollBar->GetWidth());
  //mHScrollBar->SetLeft(0);
  mHScrollBar->SetTop(GetHeight() - mHScrollBar->GetHeight());

  mVScrollBar->SetHeight(GetHeight() - mHScrollBar->GetHeight());
  //mVScrollBar->SetLeft(GetWidth() - mVScrollBar->SetWidth());
  mVScrollBar->SetTop(0);
}
//---------------------------------------------------------------------------

void CPIScrollWinControl::Resize(int _w, int _h)
{
  CPIWinControl::Resize(_w, _h);

  mHScrollBar->SetWidth(GetWidth() - mVScrollBar->GetWidth());
  mHScrollBar->SetLeft(0);
  mHScrollBar->SetTop(GetHeight() - mHScrollBar->GetHeight());

  mVScrollBar->SetHeight(GetHeight() - mHScrollBar->GetHeight());
  mVScrollBar->SetLeft(GetWidth() - mVScrollBar->GetWidth());
  mVScrollBar->SetTop(0);
}
//---------------------------------------------------------------------------

void CPIScrollWinControl::AddControl (CPIControl *Control)
{
  CPIWinControl::AddControl(Control);


}
//---------------------------------------------------------------------------

void CPIScrollWinControl::RemoveControl (CPIControl *Control)
{
}
//---------------------------------------------------------------------------

void CPIScrollWinControl::RemoveAllControls ()
{
}
//---------------------------------------------------------------------------

void CPIScrollWinControl::FreeControl (CPIControl *Control)
{
}
//---------------------------------------------------------------------------

void CPIScrollWinControl::FreeAllControls ()
{
}
//---------------------------------------------------------------------------

CPIScrollWinControl::~CPIScrollWinControl()
{
}
//---------------------------------------------------------------------------

