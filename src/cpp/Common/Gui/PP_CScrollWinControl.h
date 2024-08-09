//---------------------------------------------------------------------------
#ifndef PP_CScrollWinControlH
#define PP_CScrollWinControlH
//---------------------------------------------------------------------------

#include "PP_CHScrollBar.h"
#include "PP_CVScrollBar.h"
#include "PP_CWinControl.h"

class CPIScrollWinControl : public CPIWinControl
{
  private:
  protected:
    bool           mAutoScroll;
    CPIHScrollBar* mHScrollBar;
    CPIVScrollBar* mVScrollBar;
    int            mHPos, mVPos;

    void OnHScroll(CPIObject *Sender, TPP_ScrollBarCode _code, int& _position);
    void OnVScroll(CPIObject *Sender, TPP_ScrollBarCode _code, int& _position);

    virtual void ScrollControls(int _h, int _v);

    void ComputeLimits();
  public:
    CPIScrollWinControl(CPIWinControl* Parent = NULL);

    //virtual void Update();
    virtual void Paint();

    virtual void AddControl (CPIControl *Control);
    virtual void RemoveControl (CPIControl *Control);
    virtual void RemoveAllControls ();
    virtual void FreeControl (CPIControl *Control);
    virtual void FreeAllControls ();

    ~CPIScrollWinControl();

    // set-get
    void SetAutoScroll(bool _v);
    bool GetAutoScroll()        { return mAutoScroll; }

    CPIVScrollBar* GetVScrollBar() { return mVScrollBar; }
    CPIHScrollBar* GetHScrollBar() { return mHScrollBar; }

    virtual void SetWidth(int _v);
    virtual void SetHeight(int _v);
    virtual void Resize(int _w, int _h);

    void HasVScroll(bool _v) { mVScrollBar->SetVisible(_v); };
    void HasHScroll(bool _v) { mHScrollBar->SetVisible(_v); };
};


#endif
