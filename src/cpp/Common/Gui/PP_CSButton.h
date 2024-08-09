//-----------------------------------------------------------------------------
//  Unit              : Pure Power Starship Button
//
//  Versiune          : 0.9
//
//  Descriere         :
//    * butonul general din joc
//
//  Istorie           :
//    [13.01.2001] - [Karg] - unit creat
//---------------------------------------------------------------------------
#ifndef PP_CSButtonH
#define PP_CSButtonH
//---------------------------------------------------------------------------

#include "PP_CButton.h"
#include "PP_Font.h"
//#include "Fx_Drain.h"
//#include "PP_InterfaceDef.h"

enum TPP_SButtonOrientation {SBO_LEFT, SBO_RIGHT};

class CPP_CSButton : public CPIButton
{
  private:
    CPString    mCaption;
    CPFont*     mFont;
    __int32     mFontColor;
    __int32     mFontColorDisabled;
    __int32     mBackColor;
    __int32     mBackColorDisabled;
    float       mOpacity;
    TPP_SButtonOrientation mOrientation;
    CPGISprite* mBtnSprite;
    //CFx_Drain*  mFxDrain;

    void MouseMove(CPIObject *Sender, int X, int Y); 

  protected:

  public:
    CPP_CSButton(CPIWinControl *Parent = NULL);

    CPString ClassName() {return "CPP_SButton"; };
    virtual bool IsDerivedFrom(CPString ClassName);

    virtual bool IsPointInside (CPPoint point);

    virtual void Update();
    virtual void Paint();

    // get-set crap
    virtual void SetHeight(int _v);
    virtual void SetWidth(int _v);

    CPFont* GetFont() { return mFont; }
    void  SetFont(CPFont* _value);

    CPString GetCaption() { return mCaption; }
    void     SetCaption(CPString _caption) { mCaption = _caption; }

    __int32 GetBackColor() { return mBackColor; };
    void    SetBackColor(__int32 _bc) { mBackColor = _bc; };

    __int32 GetBackColorDisabled() { return mBackColorDisabled; };
    void    SetBackColorDisabled(__int32 _bc) { mBackColorDisabled = _bc; };

    __int32 GetFontColor() { return mFontColor; };
    void    SetFontColor(__int32 _fc) { mFontColor = _fc; };             

    __int32 GetFontColorDisabled() { return mFontColorDisabled; };
    void    SetFontColorDisabled(__int32 _fc) { mFontColorDisabled = _fc; };

    TPP_SButtonOrientation GetOrientation() { return mOrientation; };
    void    SetOrientation(TPP_SButtonOrientation _o) { mOrientation = _o; };

    float   GetOpacity() { return mOpacity; };
    void    SetOpacity(float _o);

    virtual ~CPP_CSButton();
};

#endif
