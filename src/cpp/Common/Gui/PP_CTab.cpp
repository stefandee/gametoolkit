//---------------------------------------------------------------------------
#pragma hdrstop

#include "PP_CTab.h"
#include "Paths.h"
#include "basepath.h"

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

CPITab::CPITab(CPIWinControl *Parent) : CPP_CMove(Parent)/*CPIWinControl(Parent)*/
{
  // alloc pages
  mPages  = new CPList<CPISheet>(64);
  mTabs   = new CPList<CPICheckButton>(64);

  // setari
  mMultiLine   = false;
  mSpread      = false;
  mTabIndex    = -1;
  mTabHeight   = 0;
  mTabWidth    = 0;
  mTabPosition = TP_UP;

  // fontul
  mFont        = new CPFont(PATH_FONTS + FILE_FONT_SMALL);
  mFont->SetSize(8);

  // butoanele de scroll - initial dezactivate (0 itemuri)
  mLeftBtn  = new CPIButton(this);
  mRightBtn = new CPIButton(this);

  mLeftBtn->Hide();
  mRightBtn->Hide();

  mLeftBtn->SetAutoSizeToSprite(true);
  mRightBtn->SetAutoSizeToSprite(true);

  mLeftBtn->SetHandleOnLeftMouseClick(std::bind(&CPITab::ScrollOnLeftClick, this, std::placeholders::_1));
  mRightBtn->SetHandleOnLeftMouseClick(std::bind(&CPITab::ScrollOnRightClick, this, std::placeholders::_1));

  LoadScrollBtnSprite();

  AddControl(mLeftBtn);
  AddControl(mRightBtn);

  // variabile interne
  mTabLeft  = 0;
  mTabRight = 0;

  SetHandleOnTabArrange(std::bind(&CPITab::OnArrange, this, std::placeholders::_1));
}
//---------------------------------------------------------------------------

CPITab::~CPITab()
{
  delete mPages;
  delete mTabs;
  delete mFont;
}
//---------------------------------------------------------------------------


int CPITab::GetRTabWidth(int _index)
{
  if (mPages->Count() <= _index)
  {
    return -1;
  }

  if (mTabWidth == 0)
  {
    // autosize la latimea sprite'ului (daca are)
    if (mTabs->Get(_index)->GetSprite())
    {
      return mTabs->Get(_index)->GetSprite()->GetAnimWidth();
    }

    return mPages->Get(_index)->GetCaption().Length() * mFont->GetSize() + 2;
  }

  return mTabWidth;
}
//---------------------------------------------------------------------------

int CPITab::GetRTabHeight(int _index)
{
  if (mPages->Count() <= _index)
  {
    return -1;
  }

  if (mTabHeight == 0)
  {
    // autosize la inaltimea sprite'ului (daca are)
    if (mTabs->Get(_index)->GetSprite())
    {
      return mTabs->Get(_index)->GetSprite()->GetAnimHeight();
    }

    return mFont->GetSize();
  }

  return mTabHeight;
}
//---------------------------------------------------------------------------

void CPITab::LoadScrollBtnSprite()
{
  mLeftBtn->SetSprite(GetGraphicInstance()->AddSprite(CPString(PATH_GUI_GFX) + "tab_left_scroll.png", 1, 2));
  mRightBtn->SetSprite(GetGraphicInstance()->AddSprite(CPString(PATH_GUI_GFX) + "tab_right_scroll.png", 1, 2));
}
//---------------------------------------------------------------------------

int CPITab::TabsInView(int _startTab)
{
  int lTabs = 0, lLeft = 0, lStartTab = _startTab;

  if (mPages->Count() <= 0)
  {
    return 0;
  }

  if (_startTab < 0 || _startTab >= mPages->Count())
  {
    return 0;
  }

  // numaratoarea
  while (lLeft < GetWidth() && lStartTab < mPages->Count())
  {
    lLeft += GetRTabWidth(lStartTab);
    lStartTab++;
    lTabs++;
  }

  return lTabs;
}
//---------------------------------------------------------------------------

void CPITab::SelectNextPage()
{
  if (mPages->Count() <= 0)
  {
    return;
  }

  if (!mMultiLine)
  {
    mTabIndex++;

    if (mTabIndex > mPages->Count() - 1)
    {
      VLOG(5) << "END OF SCROLL - BACK TO START!!!";
      mTabIndex = 0;
      mTabLeft  = 0;
    }
    else
    {
      if (mTabIndex >= mTabRight)
      {
        if (mTabIndex < mPages->Count() - 1)
        {
          mTabLeft++;
        }

        OnTabArrange();
      }
    }

    mTabs->Get(mTabIndex)->OnLeftMouseClick();
    return;
  }

  // pentru MultiLine tab
  // ???
}
//---------------------------------------------------------------------------

void CPITab::SelectPrevPage()
{
  if (mPages->Count() <= 0)
  {
    return;
  }

  if (!mMultiLine)
  {
    mTabIndex--;

    if (mTabIndex < 0)
    {
      mTabIndex = mPages->Count() - 1;
      mTabLeft  = mPages->Count() - 1;
    }
    else
    {
      if (mTabIndex < mTabLeft)
      {
        if (mTabLeft > 0)
        {
          mTabLeft--;
        }
      }
    }

    mTabs->Get(mTabIndex)->OnLeftMouseClick();
    OnTabArrange();
    return;
  }

  // pentru MultiLine tab
  // ???
}
//---------------------------------------------------------------------------

void CPITab::OnArrange(CPIObject *Sender)
{
  int lLeft, lTab, lTabTop, lPageTop;

  VLOG(9) << "Starting onarrange handle....";

  if (mPages->Count() <= 0)
  {
    return;
  }

  if (!mMultiLine)
  {
    VLOG(9) << "Arranging tabs...";

    switch(mTabPosition)
    {
      case TP_UP :
        lTabTop  = 0;
        lPageTop = GetRTabHeight(0);
        break;

      case TP_BOTTOM :
        lTabTop  = GetHeight() - GetRTabHeight(0);
        lPageTop = 0;
        break;
    }

    lTab  = mTabLeft;
    lLeft = 0;

    // dezactivez toate butoanele
    for(int i = 0; i < mPages->Count(); i++)
    {
      mTabs->Get(i)->Hide();
    }

    // aranjez butoanele de tab
    while (lLeft < GetWidth() && lTab < mPages->Count())
    {
      mTabs->Get(lTab)->SetLeft(lLeft);
      mTabs->Get(lTab)->SetTop(lTabTop);

      if (GetTabWidth() != 0)
      {
        mTabs->Get(lTab)->SetWidth(GetRTabWidth(lTab));
      }

      if (GetTabHeight() != 0)
      {
        mTabs->Get(lTab)->SetHeight(GetRTabHeight(lTab));
      }

      mTabs->Get(lTab)->Show();

      lLeft += GetRTabWidth(lTab);
      lTab++;
    }

    mTabRight = lTab - 1;

    // aranjez paginile
    for(int i = 0; i < mPages->Count(); i++)
    {
      mPages->Get(i)->SetLeft(0);
      mPages->Get(i)->SetTop(lPageTop);
      mPages->Get(i)->SetWidth(GetWidth());
      mPages->Get(i)->SetHeight(GetHeight() - lTabTop);
      mPages->Get(i)->Hide();
    }

    // fac vizibila pagina curenta
    mPages->Get(mTabIndex)->Show();

    // activez cele doua butoane de scroll si le setez pozitia
    //if (lLeft > GetWidth())
    {
      mLeftBtn->SetLeft(GetWidth() - mRightBtn->GetWidth() - mLeftBtn->GetWidth());
      mRightBtn->SetLeft(GetWidth() - mRightBtn->GetWidth());
      mLeftBtn->SetTop(lTabTop);
      mRightBtn->SetTop(lTabTop);
      mLeftBtn->Show();
      mRightBtn->Show();
      mLeftBtn->SendToBack();
      mRightBtn->SendToBack();
    }
    /*
    else
    {
      mLeftBtn->Hide();
      mRightBtn->Hide();
    }
    */

    return;
  }

  // intotdeauna Spread la MultiLine
  // algoritmul de aranjare a butoanelor pe linii e urmatorul :
  // - se porneste cu mLeftTab si se cauta mRightTab (adunind latimea)
  // - intotdeauna mLeftTab, mTabIndex (si, evident, mRightTab) se afla pe aceesi linie
  // - se merge apoi (cu for) de la butonul 0 pina la butonul mLeftTab - 1 si se aranjeaza pe linii
  //   (tinind un counter cu pozitia pe Y - Top)
  // - apoi se merge (tot cu for) de la butonul mRightTab + 1 pina la ultimul buton,
  //   facind aceeasi aranjare

  lLeft = 0;
  lTab  = mTabLeft;

  while (lLeft < GetWidth() && lTab < mPages->Count())
  {
    if (GetTabWidth() != 0)
    {
      mTabs->Get(lTab)->SetWidth(GetRTabWidth(lTab));
    }

    if (GetTabHeight() != 0)
    {
      mTabs->Get(lTab)->SetHeight(GetRTabHeight(lTab));
    }

    mTabs->Get(lTab)->Show();

    lLeft += GetRTabWidth(lTab);
    lTab++;
  }

  mTabRight = lTab - 1;

  // etc etc

}
//---------------------------------------------------------------------------

CPRect CPITab::GetTabRect(int _index)
{
  // TODO: return something useful
  return CPRect();
}
//---------------------------------------------------------------------------

CPRect CPITab::GetClientRect()
{
  // TODO: return something useful
  return CPRect();
}
//---------------------------------------------------------------------------

void CPITab::Paint()
{
  SetClipRegion();

  /*
  GetGraphicInstance()->FillRectangle(
    NULL,
    //Rect(GetAbsoluteLeft(), GetAbsoluteTop(), GetAbsoluteLeft() + GetWidth(), GetAbsoluteTop() + GetHeight()),
    GetBoundsRect(),
    0x000050);
  */

  CPIWinControl::Paint();
}
//---------------------------------------------------------------------------

void CPITab::Update()
{
  CPP_CMove::Update();

  // la fiecare frame se calculeaza pozitiile - pentru descendenti
  // OnArrange();
}
//---------------------------------------------------------------------------

void CPITab::AddPage(CPISheet* _page)
{
  if (mPages->Count() > 64)
  {
    return;
  }

  // adaug pagina in listele interne
  _page->SetParent(this);
  _page->Hide();
  mPages->Add(_page);
  AddControl(_page);

  // si butonul
  CPICheckButton* lTab = new CPICheckButton(this);

  lTab->SetHandleOnLeftMouseClick(std::bind(&CPITab::TabOnClick, this, std::placeholders::_1));

  lTab->SetText(_page->GetCaption());
  lTab->GetFont()->SetSize(7);
  lTab->SetGroupIndex(1);

  lTab->Hide();
  mTabs->Add(lTab);
  AddControl(lTab);

  // evident, tot timpul trebuie sa existe un buton selectat
  if (mTabIndex < 0)
  {
    SetTabIndex(0);
  }

  OnTabArrange();
}
//---------------------------------------------------------------------------

CPISheet* CPITab::GetPage(int _index)
{
  if (mPages->Count() <= _index)
  {
    return NULL;
  }

  return mPages->Get(_index);
}
//---------------------------------------------------------------------------

void CPITab::SetImage(int _index, CPGISprite* _value)
{
  if (mPages->Count() <= _index)
  {
    return;
  }

  mTabs->Get(_index)->SetSprite(_value);

  VLOG(9) << "Image set - go to arrange now";

  OnTabArrange();
}
//---------------------------------------------------------------------------

void CPITab::DeletePage(int _index)
{
  if (mPages->Count() <= _index)
  {
    return;
  }

  RemoveControl(mPages->Get(_index));
  mPages->Delete(_index);

  RemoveControl(mTabs->Get(_index));
  mTabs->Delete(_index);

  if (mPages->Count() > 0)
  {
    SetTabIndex(0);
  }
  else
  {
    // nu mai exista nici un tab
    mTabIndex = -1;
  }

  OnTabArrange();
}
//---------------------------------------------------------------------------

void CPITab::SetTabWidth(int _value)
{
  if (_value < 0)
  {
    return;
  }

  mTabWidth = _value;

  OnTabArrange();
}
//---------------------------------------------------------------------------

void CPITab::SetTabHeight(int _value)
{
  if (_value < 0)
  {
    return;
  }

  mTabHeight = _value;

  OnTabArrange();
}
//---------------------------------------------------------------------------

void CPITab::SetTabIndex(int _value)
{
  if (_value > mPages->Count() - 1)
  {
    _value = 0;
  }

  if (_value < 0)
  {
    _value = mPages->Count() - 1;
  }

  mTabIndex = _value;
  mTabLeft  = mTabIndex;

  mTabs->Get(mTabIndex)->OnLeftMouseClick();

  OnTabArrange();
}
//---------------------------------------------------------------------------

void CPITab::SetFont(CPFont* _value)
{
  if (!mFont)
  {
    return;
  }

  delete mFont;
  mFont = _value;
}
//---------------------------------------------------------------------------

void CPITab::SetTabPosition(TPI_TabPosition _value)
{
  mTabPosition = _value;

  OnTabArrange();
}
//---------------------------------------------------------------------------

void CPITab::ScrollOnLeftClick(CPIObject *Sender)
{
  if (mPages->Count() <= 0)
  {
    return;
  }

  if (!mMultiLine)
  {
    if (mTabLeft > 0)
    {
      mTabLeft--;
      OnTabArrange();
    }
  }
}
//---------------------------------------------------------------------------

void CPITab::ScrollOnRightClick(CPIObject *Sender)
{
  if (mPages->Count() <= 0)
  {
    return;
  }

  if (!mMultiLine)
  {
    if (mTabLeft + TabsInView(mTabLeft) < mPages->Count())
    {
      mTabLeft++;
      OnTabArrange();
    }
  }
}
//---------------------------------------------------------------------------

void CPITab::TabOnClick(CPIObject *Sender)
{
  int i;
  // in momentul asta noul tab e selectat - doar trebuie sa ma plimb printre
  // ele sa vad care e

  for(i = 0; i < mPages->Count(); i++)
  {
    if (mTabs->Get(i)->GetChecked())
    {
      break;
    }
  }

  // generez eventul OnChanging si verific daca valoarea returnata e T/F
  // daca e FALSE, atunci nu se va face schimbarea tabului
  bool lAllowChange;

  OnTabChanging(lAllowChange);

  if (!lAllowChange)
  {
    // selectez vechiul tab
    mTabs->Get(mTabIndex)->OnLeftMouseClick();
    return;
  }

  mTabIndex = i;

  if (mMultiLine)
  {
    // ???
  }
  else
  {
    // ciudatenie - nu cred ca asa se comporta un tab
    if (i == mTabRight)
    {
      if (i < mPages->Count() - 1)
      {
        mTabLeft++;
      }
    }
  }

  OnTabArrange();
}
//---------------------------------------------------------------------------

void CPITab::OnTabArrange()
{
   if (mHandleOnTabArrange)
   {
     mHandleOnTabArrange(this);
   }
}
//---------------------------------------------------------------------------

void CPITab::OnTabChange()
{
   if (mHandleOnTabChange)
   {
     mHandleOnTabChange(this);
   }
}
//---------------------------------------------------------------------------

void CPITab::OnTabChanging(bool &_allowChange)
{
   if (mHandleOnTabChanging)
   {
     mHandleOnTabChanging(this, _allowChange);
   }
   else
   {
     _allowChange = true;
   }
}
//---------------------------------------------------------------------------

/*
void CPITab::SetHandleOnTabArrange (void (__closure* value)(CPIObject *Sender))
{
   mHandleOnTabArrange = value;
}
//---------------------------------------------------------------------------

void CPITab::SetHandleOnTabChange (void (__closure* value)(CPIObject *Sender))
{
  mHandleOnTabChange = value;
}
//---------------------------------------------------------------------------

void CPITab::SetHandleOnTabChanging (void (__closure* value)(CPIObject *Sender, bool &_allowChange))
{
  mHandleOnTabChanging = value;
}
//---------------------------------------------------------------------------
*/

void CPISheet::Paint()
{
  SetClipRegion();

   if (GetSprite() != NULL)
   {
      CPGIGraphicSystem *lGSI;   //local Graphic System Instance
      try
      {
         lGSI = GetGraphicInstance();
      }
      catch(...)
      {
         LOG(FATAL) << "CPISheet::Paint - UNABLE TO QUERY GRAPHICS !!!!!!!!!!";
         throw;
      }
      lGSI->PaintSprite(GetAbsoluteLeft(), GetAbsoluteTop(), GetSprite(), GetAnimCounter(), GetAnimType());
   }

   /*
  GetGraphicInstance()->FillRectangle(
    NULL,
    //Rect(GetAbsoluteLeft(), GetAbsoluteTop(), GetAbsoluteLeft() + GetWidth(), GetAbsoluteTop() + GetHeight()),
    GetBoundsRect(),
    0x905800);
  */
}
//---------------------------------------------------------------------------


