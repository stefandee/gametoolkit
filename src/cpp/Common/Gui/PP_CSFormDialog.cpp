//---------------------------------------------------------------------------
#pragma hdrstop

#include "PP_CSFormDialog.h"
#include "Paths.h"
#include "basepath.h"

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

CPP_CSFormDialog::CPP_CSFormDialog(CPIWinControl *Parent) : CPICommonDialog(Parent)
{
  // membrii
  mButtons = DB_OKCANCEL;
  mText    = CPString("MESAJ");
  mTitle   = CPString("TITLU");

  // controalele
  mScreen = new CPP_CMove(this);
  mScreen->SetLeft(0);
  mScreen->SetTime(0.25);
  AddControl(mScreen);

  mImgDialog = new CPIImage(mScreen);
  mImgDialog->SetLeft(0);
  mImgDialog->SetTop(0);
  mImgDialog->SetAutoSizeToSprite(true);
  mImgDialog->LoadFromFile(CPString(PATH_GUI_GFX) + "dialog.png");
  mScreen->AddControl(mImgDialog);

  // message label
  CPFont* lFont1 = new CPFont(PATH_FONTS + FILE_FONT_SMALL, 16, true);

  mLabelMsg = new CPILabel(mScreen);
  mLabelMsg->SetFont(lFont1);
  mLabelMsg->SetText(mText);
  mLabelMsg->SetAutoSize(true);

  // because of center horizontal alignment, otherwise the clipping will cut off the text. should definitely have a better solution
  mLabelMsg->SetParentClip(true);
  //mLabelMsg->SetWidth(mImgDialog->GetWidth());
  //mLabelMsg->SetHeight(20);
  mLabelMsg->SetLeft(mImgDialog->GetWidth() / 2);
  mLabelMsg->SetTextAllignment(PPDT_CENTER);
  mLabelMsg->SetTop(300);
  mLabelMsg->SetColor(0xD3D0D0);
  mScreen->AddControl(mLabelMsg);
  mLabelMsg->BringToFront();

  // title label
  CPFont* lFont2 = new CPFont(PATH_FONTS + FILE_FONT_BIG, 18, true);

  mLabelTitle = new CPILabel(mScreen);
  mLabelTitle->SetFont(lFont2);
  mLabelTitle->SetText(mTitle);
  mLabelTitle->SetAutoSize(true);
  mLabelTitle->SetParentClip(true);
  //mLabelTitle->SetWidth(mImgDialog->GetWidth());
  //mLabelTitle->SetHeight(100);
  mLabelTitle->SetLeft(mImgDialog->GetWidth() / 2);
  mLabelTitle->SetTextAllignment(PPDT_CENTER);
  mLabelTitle->SetTop(203);
  //mLabelTitle->SetWidth(100);
  //mLabelTitle->SetHeight(32);
  mLabelTitle->SetColor(0xE3E0E0);

  mScreen->AddControl(mLabelTitle);
  mLabelTitle->BringToFront();

  // si din nou ecranul
  mScreen->SetWidth(mImgDialog->GetWidth());

  mMenu = NULL;
  SetButtons(DB_OKCANCEL);

  SetWidth(mScreen->GetWidth());
  SetLeft(400 - mScreen->GetWidth() / 2);
  SetTop(0);
  SetHeight(600);
}
//---------------------------------------------------------------------------

void CPP_CSFormDialog::ShowModal(CPIControl *parent)
{
  mScreen->SetTop(-mImgDialog->GetHeight() - mMenu->GetWidth());
  mScreen->SetTopDestination(0);
  mScreen->SetHandleOnStopMove(NULL);

  CPIForm::ShowModal(parent);
}
//---------------------------------------------------------------------------

void CPP_CSFormDialog::SetButtons(TDialogButtons _v)
{
  mButtons = _v;

  if (mMenu)
  {
    mScreen->FreeControl(mMenu);
  }

  switch(mButtons)
  {
    case DB_OK :
      mMenu = new CPP_CSLinkMenu(1, mScreen);
      mMenu->SetBtnCaption(0, "OK");
      mMenu->SetBtnTag(0, PMR_OK);
      mMenu->SetBtnHandle(0, std::bind(&CPP_CSFormDialog::DefaultBtnHandle, this, std::placeholders::_1));
      break;

    case DB_OKCANCEL :
      mMenu = new CPP_CSLinkMenu(2, mScreen);
      mMenu->SetBtnCaption(0, "OK");
      mMenu->SetBtnTag(0, PMR_OK);
      mMenu->SetBtnHandle(0, std::bind(&CPP_CSFormDialog::DefaultBtnHandle, this, std::placeholders::_1));
      mMenu->SetBtnCaption(1, "CANCEL");
      mMenu->SetBtnTag(1, PMR_CANCEL);
      mMenu->SetBtnHandle(1, std::bind(&CPP_CSFormDialog::DefaultBtnHandle, this, std::placeholders::_1));
      break;

    case DB_YESNO :
      mMenu = new CPP_CSLinkMenu(2, mScreen);
      mMenu->SetBtnCaption(0, "YES");
      mMenu->SetBtnTag(0, PMR_YES);
      mMenu->SetBtnHandle(0, std::bind(&CPP_CSFormDialog::DefaultBtnHandle, this, std::placeholders::_1));
      mMenu->SetBtnCaption(1, "NO");
      mMenu->SetBtnTag(1, PMR_NO);
      mMenu->SetBtnHandle(1, std::bind(&CPP_CSFormDialog::DefaultBtnHandle, this, std::placeholders::_1));
      break;

    case DB_RETRYCANCEL :
      mMenu = new CPP_CSLinkMenu(2, mScreen);
      mMenu->SetBtnCaption(0, "RETRY");
      mMenu->SetBtnTag(0, PMR_OK);
      mMenu->SetBtnHandle(0, std::bind(&CPP_CSFormDialog::DefaultBtnHandle, this, std::placeholders::_1));
      mMenu->SetBtnCaption(1, "CANCEL");
      mMenu->SetBtnTag(1, PMR_CANCEL);
      mMenu->SetBtnHandle(1, std::bind(&CPP_CSFormDialog::DefaultBtnHandle, this, std::placeholders::_1));
      break;

    case DB_YESNOCANCEL :
      mMenu = new CPP_CSLinkMenu(3, mScreen);
      mMenu->SetBtnCaption(0, "YES");
      mMenu->SetBtnTag(0, PMR_YES);
      mMenu->SetBtnHandle(0, std::bind(&CPP_CSFormDialog::DefaultBtnHandle, this, std::placeholders::_1));
      mMenu->SetBtnCaption(1, "NO");
      mMenu->SetBtnTag(1, PMR_NO);
      mMenu->SetBtnHandle(1, std::bind(&CPP_CSFormDialog::DefaultBtnHandle, this, std::placeholders::_1));
      mMenu->SetBtnCaption(2, "CANCEL");
      mMenu->SetBtnTag(2, PMR_CANCEL);
      mMenu->SetBtnHandle(2, std::bind(&CPP_CSFormDialog::DefaultBtnHandle, this, std::placeholders::_1));
      break;
  }

  mMenu->SetWidth(214);
  mMenu->SetHeight(100);
  mMenu->SetMenuWidth(150);
  mMenu->SetLeft(mImgDialog->GetWidth() - 214 - 80);
  mMenu->SetLeftDestination(mImgDialog->GetWidth() - 214 - 80);
  mMenu->SetTop(mImgDialog->GetHeight() - 5);
  mMenu->SetTopDestination(mImgDialog->GetHeight() - 5);
  mMenu->SetBtnOrientation(SBO_RIGHT);
  mMenu->SetLinkOrientation(SLMO_UP);
  mMenu->SendToBack();

  mScreen->SetHeight(mImgDialog->GetHeight() + mMenu->GetHeight());
  mScreen->AddControl(mMenu);
  mScreen->BringControlToFront(mMenu);
}
//---------------------------------------------------------------------------

void CPP_CSFormDialog::SetText(CPString _v)
{
  mText = _v;
  mLabelMsg->SetText(mText);
}
//---------------------------------------------------------------------------

void CPP_CSFormDialog::SetTitle(CPString _v)
{
  mTitle = _v;
  mLabelTitle->SetText(mTitle);
}
//---------------------------------------------------------------------------

void CPP_CSFormDialog::DefaultBtnHandle(CPIObject *Sender)
{
  mScreen->SetTop(mScreen->GetTop());
  mScreen->SetTopDestination(-mImgDialog->GetHeight() - mMenu->GetWidth());
  mScreen->SetHandleOnStopMove(std::bind(&CPP_CSFormDialog::CloseStopMoveHandle, this, std::placeholders::_1));

  mBtnTag = Sender->GetTag();
}
//---------------------------------------------------------------------------

void CPP_CSFormDialog::CloseStopMoveHandle(CPIObject *Sender)
{
  if (mScreen->GetTop() < 0)
  {
    SetModalResult(mBtnTag);
    OnClose();
  }
}
//---------------------------------------------------------------------------

CPP_CSFormDialog::~CPP_CSFormDialog()
{
}
//---------------------------------------------------------------------------


