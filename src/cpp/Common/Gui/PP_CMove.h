//---------------------------------------------------------------------------
#ifndef PP_CMoveH
#define PP_CMoveH
//---------------------------------------------------------------------------

#include "basic3d.h"
#include "PP_CWinControl.h"

class CPP_CMove : public CPIWinControl
{
  private:
    bool      mMoving;
    CVector3D mSpeed, mAccel;
    TPos3D    mPos, mStart, mStop;

    float     mTick, mTime;

    void ComputeMoveParams();

    StandardGUICallback mHandleOnStopMove;
    StandardGUICallback mHandleOnStartMove;
    //void (__closure* mHandleOnStopMove)(CPIObject *Sender);
    //void (__closure* mHandleOnStartMove)(CPIObject *Sender);

  protected:
    void OnStartMove();
    void OnStopMove();

  public:
    CPP_CMove(CPIWinControl *Parent = NULL);

    //virtual void Paint();
    virtual void Update();

    virtual void SetTop (int value);
    void         SetTopDestination(int value);

    virtual void SetLeft (int value);
    void         SetLeftDestination(int value);

    float GetTime() { return mTime; }
    void  SetTime(float _t);

    virtual void SetHandleOnStartMove (StandardGUICallback callback)  { mHandleOnStartMove = callback; }
    virtual void SetHandleOnStopMove (StandardGUICallback callback)  { mHandleOnStopMove = callback; }
    //virtual void SetHandleOnStartMove (void (__closure* value)(CPIObject *Sender));
    //virtual void SetHandleOnStopMove (void (__closure* value)(CPIObject *Sender));

    ~CPP_CMove();
};

#endif
