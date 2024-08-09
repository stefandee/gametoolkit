//---------------------------------------------------------------------------
#ifndef PP_CSMenuH
#define PP_CSMenuH
//---------------------------------------------------------------------------

#include "PP_CSButton.h"
#include "PP_CMove.h"

class CPP_CSMenu : public CPP_CMove
{
  private:

  protected:
    CPP_CSButton* mBtns[16];
    int           mBtnsNumber, mSpacing;
    TPP_SButtonOrientation mOrientation;

  public:
    CPP_CSMenu(int _number, CPIWinControl *Parent = NULL);

    virtual void Paint();

    void SetBtnHandle(int _index, StandardGUICallback callback);

    void SetBtnOrientation(TPP_SButtonOrientation _orientation);
    TPP_SButtonOrientation  GetBtnOrientation() { return  mOrientation; };

    void SetBtnEnabled(int _index, bool _enabled);
    void SetBtnCaption(int _index, CPString _caption);

    void SetBtnTag(int _index, int _tag);

    virtual void SetMenuWidth(int value);

    int GetBtnsNumber() { return mBtnsNumber; };

    int  GetSpacing() { return mSpacing; };
    virtual void SetSpacing(int _spacing);

    ~CPP_CSMenu();
};

#endif
