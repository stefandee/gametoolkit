//-----------------------------------------------------------------------------
//  Unit              : Special Effects (Fx.h)
//
//  Versiune          : 1.0
//
//  Descriere         :
//    * contine clasa de baza pentru bufferele de efecte speciale grafice
//    * de vreme ce efectele speciale sint pentru folosirea lor in jocuri si nicidecum
//      pentru folosirea lor full screen, nu sint optimizate pentru viteza
//
//  Istorie           :
//    [26.02.2001] - [Karg] - unit creat
//-----------------------------------------------------------------------------

#ifndef FxH
#define FxH
//---------------------------------------------------------------------------

#include "PP_Graphic.h"

class CFx
{
  private:

  protected:
    CPGISurface*  mFxCanvas;
    int           mFxWidth, mFxHeight;
    bool          mEnabled;

    void CreateCanvas();

  public:
    CFx();

    virtual void Update();
    virtual void Paint();

    virtual ~CFx();

    // set-get
    virtual CPGISurface* GetCanvas();

    virtual void SetWidth(int _v);
    virtual int  GetWidth();

    virtual void SetHeight(int _v);
    virtual int  GetHeight();

    virtual void SetEnabled(bool _v) { mEnabled = _v; };
    virtual bool GetEnabled() { return mEnabled; };
};


#endif
