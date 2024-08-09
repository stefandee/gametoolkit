//-----------------------------------------------------------------------------
//  Unit              : List Box Component
//
//  Versiune          : 1.0
//
//  Descriere         :
//    * standard list box - ca cea din VCL
//    * se poate face un state - pentru fiecare tip de lista (standard, owner)
//
//  Istorie           :
//    [20.01.2001] - [Karg] - unit creat
//    [31.01.2001] - [Karg] - scris functionalitatile de baza ale componentei
//    [01.02.2001] - [Karg]  - adaugat mai multe proprietati
//    [03.02.2001] - [Karg]  - standard si drawfixed sint gata
//    [04.02.2001] - [Karg]  - adaugat cod pentru drawvariable - scrolldown si paintasvariable au ramas
//                           - descoperit bug la scrollup - de corectat
//    [04.02.2001] - [Karg]  - in mare, controlul e terminat; in viitorul apropiat
//                             e posibil sa sufere un refactoring (cu stari si restul)
//---------------------------------------------------------------------------
#ifndef PP_CListBoxH
#define PP_CListBoxH
//---------------------------------------------------------------------------

#include "PP_CWinControl.h"
#include "PP_CVScrollBar.h"
#include "PP_Font.h"
#include "KList.h"
#include "basic3d.h"
#include "basic2d.h"

enum TPI_ListBoxStyle { LBS_STD, LBS_DRAWFIXED, LBS_DRAWVARIABLE};
enum TPI_ListAlign { LA_LEFT, LA_CENTER, LA_RIGHT};
enum TPI_ListOwnerState {LOS_SELECTED, LOS_DISABLED, LOS_FOCUSED};

typedef std::function <void(CPIObject*,int,const CPRect&,TPI_ListOwnerState)> ListDrawItemCallback; /**< List measure item event (perhaps it can be reused) */

class CPIListBox : public CPIWinControl
{
  private:
    CPIVScrollBar*
            mVScrollBar;
    __int32 mBackGround;
    int     mColumns;
    bool    mIntegralHeight;
    int     mItemHeight;
    TPI_ListBoxStyle
            mStyle;
    TPI_ListAlign
            mAlign;
    CPFont*
            mFont;
    int     mScrollBy;

    ListDrawItemCallback mHandleOnDrawItem;
    MeasureItemCallback mHandleOnMeasureItem;

    //void (__closure* mHandleOnDrawItem)(CPIObject *Sender, int _index, TRect& _rect, TPI_ListOwnerState _state);
    //void (__closure* mHandleOnMeasureItem)(CPIObject *Sender, int _index, int& _height);

  protected:
    int               mItemIndex;
    int               mTopIndex;
    std::vector<CPString> mItems;
    float             mY;          // variabila interna, folosita pentru desen -
                                   // asta ar fi trebuit sa fie o functie (asa sint doua
                                   // variabile si e dificil sa le tin "legate")

    virtual int  GetEWidth();

    void OnDrawItem(int _index, const CPRect& _rect, TPI_ListOwnerState _state);
    void OnMeasureItem(int _index, int& _height);

    void LeftMouseDown(CPIObject *Sender, int _x, int _y);
    void DrawItemFixed(CPIObject *Sender, int _index, const CPRect& _rect, TPI_ListOwnerState _state);
    void DrawItemVariable(CPIObject *Sender, int _index, const CPRect& _rect, TPI_ListOwnerState _state);
    void MeasureItemVariable(CPIObject *Sender, int _index, int& _height);

    virtual void PaintAsStandard();
    virtual void PaintAsFixed();
    virtual void PaintAsVariable();

    virtual void OnScroll(CPIObject* Sender, TPP_ScrollBarCode _code, int _position);

  public:
    CPIListBox(CPIWinControl *Parent = NULL);

    // paint related
    int    ItemAtPos(TPos2D _p);
    int    ItemAtPos(int x, int y);
    CPRect ItemRect(int _item);

    // item related
    //void  DeleteItem(int _itemIndex);
    void  Clear();

    virtual void SetHandleOnDrawItem (ListDrawItemCallback callback) { mHandleOnDrawItem = callback; }
    virtual void SetHandleOnMeasureItem (MeasureItemCallback callback) { mHandleOnMeasureItem = callback; }
    //virtual void SetHandleOnDrawItem (void (__closure* value)(CPIObject *Sender, int _index, CPRect& _rect, TPI_ListOwnerState _state));
    //virtual void SetHandleOnMeasureItem (void (__closure* value)(CPIObject *Sender, int _index, int& _height));

    std::vector<CPString>& Items() { return mItems; };

    virtual void Update();
    virtual void Paint();

    void ScrollUp();
    void ScrollDown();

    virtual void SetWidth(int _w);
    virtual void SetHeight(int _h);

    ~CPIListBox();

    // get-set
    void SetBackColor(__int32 _c) { mBackGround = _c; };
    __int32 GetBackColor() { return mBackGround; };

    void SetColumns(int _c) { mColumns = _c; };
    int GetColumns() { return mColumns; };

    void SetIHeight(bool _ih) { mIntegralHeight = _ih; };
    bool GetIHeight() { return mIntegralHeight; };

    void SetItemHeight(int _ih);
    int GetItemHeight() { return mItemHeight; };

    void SetStyle(TPI_ListBoxStyle _s);
    TPI_ListBoxStyle GetStyle() { return mStyle; };

    void SetAlign(TPI_ListAlign _a) { mAlign = _a; }
    TPI_ListAlign GetAlign() { return mAlign; }

    void SetItemIndex(int _ii);
    int GetItemIndex() { return mItemIndex; };

    void SetTopIndex(int _ti);
    int GetTopIndex() { return mTopIndex; };

    void SetScrollBy(int _s) { mScrollBy = _s; };
    int GetScrollBy() { return mScrollBy; };

    void SetFont(CPFont* _value);
};

#endif
