//-----------------------------------------------------------------------------
//  Unit              : Tab Control
//
//  Versiune          : 1.0
//
//  Descriere         :
//    * echivalent cu TTabControl/VCL - mai degraba ar fi echivalent cu TCustomTab/VCL,
//      din cauza ca lasa desenatul (stilul) in seama descendentilor;
//    * multiline nu este implementat inca
//
//  Istorie           :
//    [08.01.2001] - [Karg] - unit creat
//---------------------------------------------------------------------------
#ifndef PP_CTabH
#define PP_CTabH
//---------------------------------------------------------------------------

#include "PP_CPanel.h"
#include "PP_CCheckButton.h"
#include "PP_CMove.h"
#include "KList.h"

typedef std::function <void(CPIObject*,bool&)> TabChangingCallback; /**< Tab Changing callback, specific to the CPITab */

enum TPI_TabPosition { TP_UP, TP_BOTTOM };

class CPISheet : public CPIWinControl
{
  private   :
    CPString mCaption;

  protected :

  public    :
    CPISheet(CPIWinControl *Parent = NULL) {};

    virtual void Paint();

    void SetCaption(CPString _value) { mCaption = _value; };
    CPString GetCaption() { return mCaption; }

    ~CPISheet() {};
};
//---------------------------------------------------------------------------

class CPITab : public CPP_CMove/*public CPIWinControl*/
{
  private:
    bool mMultiLine, mSpread;
    int  mTabIndex, mTabHeight, mTabWidth;
    int  mTabLeft, mTabRight;

    TPI_TabPosition
         mTabPosition;

    CPFont*
         mFont;

    // events pointers
    StandardGUICallback mHandleOnTabArrange;
    StandardGUICallback mHandleOnTabChange;
    TabChangingCallback mHandleOnTabChanging;

    //void (__closure* mHandleOnTabArrange) (CPIObject *Sender);
    //void (__closure* mHandleOnTabChange)  (CPIObject *Sender);
    //void (__closure* mHandleOnTabChanging)  (CPIObject *Sender, bool &_allowChange);

    // internal handles - tin de CPITab numai
    void ScrollOnLeftClick  (CPIObject *Sender);
    void ScrollOnRightClick (CPIObject *Sender);
    void TabOnClick         (CPIObject *Sender);
    void OnArrange          (CPIObject *Sender);
    void OnChanging         (CPIObject *Sender, bool &_allowChange);

  protected:
    CPList<CPICheckButton>* mTabs;
    CPList<CPISheet>*  mPages;
    CPIButton*         mLeftBtn, *mRightBtn;

    // metode pentru descendenti
    virtual int  GetRTabWidth(int _index);
    virtual int  GetRTabHeight(int _index);
    virtual void LoadScrollBtnSprite();
    virtual int  TabsInView(int _startTab);

    // events
    virtual void OnTabArrange();
    virtual void OnTabChange();
    virtual void OnTabChanging(bool &_allowChange);

  public:
    CPITab(CPIWinControl *Parent = NULL);

    CPRect GetTabRect(int _index);
    CPRect GetClientRect();

    virtual void Paint();
    virtual void Update();

    // page related
    virtual void      AddPage(CPISheet* _page);
    virtual CPISheet* GetPage(int _index);
    virtual void      DeletePage(int _index);
    virtual void      SetImage(int _index, CPGISprite* _value);

    // page stuff
    virtual void      SelectNextPage();
    virtual void      SelectPrevPage();

    ~CPITab();

    // get-set
    int  GetTabWidth() { return mTabWidth; };
    void SetTabWidth(int _value);

    int  GetTabHeight() { return mTabHeight; };
    void SetTabHeight(int _value);

    int  GetTabIndex() { return mTabIndex; };
    void SetTabIndex(int _value);

    bool GetMultiLine() { return mMultiLine; };
    void SetMultiLine(bool _value) { mMultiLine = _value; };

    bool GetSpread() { return mSpread; };
    void SetSpread(bool _value) { mSpread = _value; };

    TPI_TabPosition GetTabPosition() { return mTabPosition; } ;
    void            SetTabPosition(TPI_TabPosition _value);

    CPFont* GetFont() { return mFont; }
    void    SetFont(CPFont* _value);

    // handles
    virtual void SetHandleOnTabArrange  (StandardGUICallback callback) { mHandleOnTabArrange = callback;  }
    virtual void SetHandleOnTabChange   (StandardGUICallback callback) { mHandleOnTabChange = callback;   }
    virtual void SetHandleOnTabChanging (TabChangingCallback callback) { mHandleOnTabChanging = callback; }

    //virtual void SetHandleOnTabArrange (void (__closure* value)(CPIObject *Sender));
    //virtual void SetHandleOnTabChange (void (__closure* value)(CPIObject *Sender));
    //virtual void SetHandleOnTabChanging (void (__closure* value)(CPIObject *Sender, bool &_allowChange));
};

#endif
