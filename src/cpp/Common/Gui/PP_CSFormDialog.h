//---------------------------------------------------------------------------
#ifndef PP_CSFormDialogH
#define PP_CSFormDialogH
//---------------------------------------------------------------------------

#include "PP_CCommonDialog.h"
#include "PP_CLabel.h"
#include "PP_CSLinkMenu.h"

/*
class CPP_CSDialogScreen : public CPIWinControl
{
  private:

  protected:

  public:
    CPP_CSDialogScreen();
    ~CPP_CSDialogScreen();
};
*/

class CPP_CSFormDialog : public CPICommonDialog
{
  private:
    TDialogButtons  mButtons;
    CPString        mText, mTitle;

    CPP_CSLinkMenu* mMenu;
    CPP_CMove*      mScreen;
    CPILabel*       mLabelMsg, *mLabelTitle;
    CPIImage*       mImgDialog;

    int             mBtnTag;

    // handlere interne pentru atasat componentelor
    void DefaultBtnHandle(CPIObject *Sender);
    void CloseStopMoveHandle(CPIObject *Sender);

  protected:

  public:
    CPP_CSFormDialog(CPIWinControl *Parent = NULL);

    //virtual void SetModal(bool value);
    virtual void ShowModal(CPIControl *parent);
    void SetButtons(TDialogButtons _v);
    void SetText(CPString _v);
    void SetTitle(CPString _v);

    virtual ~CPP_CSFormDialog();
};

#endif
