//---------------------------------------------------------------------------
#ifndef Fx_Static3H
#define Fx_Static3H
//---------------------------------------------------------------------------

#include "Fx.h"

struct TFxStatic3_X
{
  int mXStart, mXEnd;
  TFxStatic3_X* mNext;
};

struct TFxStatic3_Y
{
  int mY;
  TFxStatic3_X* mHeadX;
  TFxStatic3_Y* mNext;
};

class CFx_Static3 : public CFx
{
  private:
    TFxStatic3_Y* mHeadY;

    void CreateMask();
  protected:

  public:
    CFx_Static3();

    void Update();
    void Paint();

    void SetMask(TPGISpriteReference _sprite, int _cw, int _ch);

    virtual ~CFx_Static3();
};

#endif
