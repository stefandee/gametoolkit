//-----------------------------------------------------------------------------
//  Unit              : Visual Fx Drain (fx_drain.h)
//
//  Versiune          : 1.0
//
//  Descriere         :
//    * pixel drain (top -> bottom)
//
//  Istorie           :
//    [04.03.2001] - [Karg] - unit creat
//    [04.03.2001] - [Karg] - prima versiune de drains (se putea simula cu particle storm)
//    [05.03.2001] - [Karg] - a doua versiune (generarea trailului)
//-----------------------------------------------------------------------------
#ifndef Fx_DrainH
#define Fx_DrainH
//---------------------------------------------------------------------------

#include "Fx.h"
#include "basic3d.h"

#define FX_MAX_DRAINS 128

// structuri de date folosite
struct TFxDrain
{
  TPoint    mTrail[32], mHead;
  int       mTrailLength, mTrailGoal;      // in vectorul mTrail
  int       mSpeed;
};

class CFx_Drain : public CFx
{
  private:
    unsigned __int32 mHeadColor;
    unsigned __int32 mTailColor;
    int       mDrainsCount, mTailLength;
    TFxDrain  mDrains[FX_MAX_DRAINS];

    void GenerateDrain(int _index);
    
  protected:

  public:
    CFx_Drain();

    void Update();
    void Paint();

    ~CFx_Drain();

    // set - get
    void             SetHeadColor(unsigned __int32 _v) { mHeadColor = _v; };
    unsigned __int32 GetHeadColor() {return mHeadColor; };

    void             SetTailColor(unsigned __int32 _v) { mTailColor = _v; };
    unsigned __int32 GetTailColor() {return mTailColor; };

    void  SetDensity(int _v);
    int  GetDensity() { return mDrainsCount; }

    void SetTailLength(int _v) { mTailLength = _v; };
    int  GetTailLength(int _v) { return mTailLength; };

    virtual void SetHeight(int _v);
    virtual void SetWidth(int _v);
};

#endif
