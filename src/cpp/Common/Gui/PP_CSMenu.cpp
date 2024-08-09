//---------------------------------------------------------------------------
#pragma hdrstop

#include "PP_CSMenu.h"

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

CPP_CSMenu::CPP_CSMenu(int _number, CPIWinControl *Parent) : CPP_CMove(Parent)
{
  SetWidth(300);
  SetHeight(200);

  if (_number < 0)
  {
    _number = 1;
  }

  mBtnsNumber  = _number;
  mOrientation = SBO_RIGHT;
  mSpacing     = 8;

  try
  {
    for(int i = 0; i < mBtnsNumber; i++)
    {
      mBtns[i] = new CPP_CSButton(this);

      if (mOrientation == SBO_LEFT)
      {
        mBtns[i]->SetLeft(0);
      }
      else
      {
        mBtns[i]->SetLeft(GetWidth() / 2 - 200 / 2);
      }

      mBtns[i]->SetTop(i * (32 + mSpacing));
      mBtns[i]->SetWidth(200);
      mBtns[i]->SetBackColor(0x80000040);
      mBtns[i]->SetOrientation(mOrientation);
      mBtns[i]->SetCaption("Button");
      AddControl(mBtns[i]);
    }
  }
  catch(...)
  {
  }
}
//---------------------------------------------------------------------------

void CPP_CSMenu::SetMenuWidth(int value)
{
  if (value < 64 ||
      value > GetWidth())
  {
    return;
  }

  for(int i = 0; i < mBtnsNumber; i++)
  {
    mBtns[i]->SetLeft(GetWidth() / 2 - value / 2);
    mBtns[i]->SetWidth(value);
  }

  //CPIControl::SetWidth(value);
}
//---------------------------------------------------------------------------

void CPP_CSMenu::SetBtnHandle(int _index, StandardGUICallback callback)
{
  if ((_index >= mBtnsNumber) ||
     (_index < 0))
  {
    return;
  }

  mBtns[_index]->SetHandleOnLeftMouseClick(callback);
}
//---------------------------------------------------------------------------

void CPP_CSMenu::SetBtnCaption(int _index, CPString _caption)
{
  if ((_index >= mBtnsNumber) ||
     (_index < 0))
  {
    return;
  }

  mBtns[_index]->SetCaption(_caption);
}
//---------------------------------------------------------------------------

void CPP_CSMenu::SetBtnTag(int _index, int _tag)
{
  if ((_index >= mBtnsNumber) ||
     (_index < 0))
  {
    return;
  }

  mBtns[_index]->SetTag(_tag);
}


//---------------------------------------------------------------------------

void CPP_CSMenu::SetBtnEnabled(int _index, bool _enabled)
{
  if ((_index >= mBtnsNumber) ||
     (_index < 0))
  {
    return;
  }

  mBtns[_index]->SetEnabled(_enabled);
}
//---------------------------------------------------------------------------

void CPP_CSMenu::SetBtnOrientation(TPP_SButtonOrientation _orientation)
{
  mOrientation = _orientation;

  for(int i = 0; i < mBtnsNumber; i++)
  {
    mBtns[i]->SetOrientation(mOrientation);
  }
}
//---------------------------------------------------------------------------

void CPP_CSMenu::SetSpacing(int _spacing)
{
  if (_spacing < 0)
  {
    return;
  }

  mSpacing = _spacing;

  for(int i = 0; i < mBtnsNumber; i++)
  {
    mBtns[i]->SetTop(i * (32 + mSpacing));
  }

  //SetHeight(mBtnsNumber * (32 + mSpacing));
}
//---------------------------------------------------------------------------

void CPP_CSMenu::Paint()
{
  SetClipRegion();
  CPIWinControl::Paint();
}
//---------------------------------------------------------------------------

CPP_CSMenu::~CPP_CSMenu()
{
}
//---------------------------------------------------------------------------
