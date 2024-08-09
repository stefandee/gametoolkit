//---------------------------------------------------------------------------
#ifndef PP_CVScrollBarH
#define PP_CVScrollBarH
//---------------------------------------------------------------------------

#include "PP_CScrollBar.h"
#include "PP_GuiCallbacks.h"

class CPIVScrollBar : public CPIControl
{
  private:
    int         mMin, mMax;
    int         mPos, mThumbChange, mClickChange;
    CPGISprite* mBtnSprite, *mThumbSprite;
    TPP_ScrollBarThumb
               mThumbStyle;
    TPP_ScrollBarState
               mState;

    StandardGUICallback mHandleOnChange;
    ScrollBarCallback mHandleOnScroll;

    //void (__closure* mHandleOnChange)(CPIObject *Sender);
    //void (__closure* mHandleOnScroll)(CPIObject *Sender, TPP_ScrollBarCode _code, int& _position);

  protected:

    virtual int GetThumbPos();
    virtual int GetBtnWidth();
    virtual int GetBtnHeight();
    virtual int GetThumbWidth();
    virtual int GetThumbHeight();

    virtual void DrawButtons();
    virtual void DrawTrackBar();
    virtual void DrawThumb();
    virtual void LeftMouseDown(CPIObject* Sender, int _x, int _y);
    virtual void LeftMouseUp(CPIObject* Sender, int _x, int _y);
    virtual void MouseMove(CPIObject* Sender, int _x, int _y);

    void OnChange();
    void OnScroll(TPP_ScrollBarCode _code, int& _position);

  public:
    CPIVScrollBar(CPIWinControl* Sender = NULL);

    virtual void Paint();
    virtual void Update();
    virtual void SetWidth(int _w);
    virtual void SetHeight(int _h);
    virtual void Resize(int _w, int _h);

    virtual void SetHandleOnChange (StandardGUICallback callback) { mHandleOnChange = callback; }
    virtual void SetHandleOnScroll (ScrollBarCallback callback) { mHandleOnScroll = callback; }

    //virtual void SetHandleOnChange (void (__closure* value)(CPIObject *Sender));
    //virtual void SetHandleOnScroll (void (__closure* value)(CPIObject *Sender, TPP_ScrollBarCode _code, int& _position));

    ~CPIVScrollBar();

    // get-set
    void SetMin(int _m);
    int  GetMin() { return mMin; }

    void SetMax(int _m);
    int  GetMax() { return mMax; }

    void SetPos(int _p);
    int  GetPos() { return mPos; }

    void SetThumbChange(int _tc);
    int  GetThumbChange() { return mThumbChange; }

    void SetClickChange(int _cc);
    int  GetClickChange() { return mClickChange; }

    void SetThumbStyle(TPP_ScrollBarThumb _s) { mThumbStyle = _s; }
    TPP_ScrollBarThumb  GetThumbStyle() { return mThumbStyle; }
};

#endif
