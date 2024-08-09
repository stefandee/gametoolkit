//---------------------------------------------------------------------------
#pragma hdrstop

#include "PP_CMove.h"

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

CPP_CMove::CPP_CMove(CPIWinControl *Parent) : CPIWinControl(Parent)
{
  mStart.y = GetTop();
  mStop.y  = GetTop();
  mStart.x = GetLeft();
  mStop.x  = GetLeft();
  mTime    = 1.0;
  mTick    = 0.0;

  ComputeMoveParams();
}
//---------------------------------------------------------------------------

CPP_CMove::~CPP_CMove()
{
}
//---------------------------------------------------------------------------

void CPP_CMove::ComputeMoveParams()
{
  if ((mStop.x - mStart.x == 0) &&
      (mStop.y - mStart.y == 0))
  {
    mMoving = false;
    return;
  }

  // calculul acceleratiei initiale
  CVector3D lNewAccel(-2.0 * (mStop.x - mStart.x) / (mTime * mTime), -2.0 * (mStop.y - mStart.y) / (mTime * mTime), 1.0);
  mAccel = lNewAccel;

  // calculul vitezei initiale
  CVector3D lNewSpeed(-mAccel.GetX() * mTime, -mAccel.GetY() * mTime, 1.0);
  mSpeed = lNewSpeed;

  // calculul pozitiei initiale la t=0
  mPos.x    = mStart.x + mSpeed.GetX() * mTick + mAccel.GetX() * mTick * mTick / 2.0;
  mPos.y    = mStart.y + mSpeed.GetY() * mTick + mAccel.GetY() * mTick * mTick / 2.0;

  mTick     = 0.0;
  mMoving = true;
}
//---------------------------------------------------------------------------

void CPP_CMove::SetTopDestination(int value)
{
  mStop.y = value;

  ComputeMoveParams();
}
//---------------------------------------------------------------------------

void CPP_CMove::SetTop (int value)
{
  CPIWinControl::SetTop(value);

  mStart.y = value;
  ComputeMoveParams();
}
//---------------------------------------------------------------------------

void CPP_CMove::SetLeftDestination(int value)
{
  mStop.x = value;

  ComputeMoveParams();
}
//---------------------------------------------------------------------------

void CPP_CMove::SetLeft (int value)
{
  CPIWinControl::SetLeft(value);

  mStart.x = value;
  ComputeMoveParams();
}
//---------------------------------------------------------------------------

void  CPP_CMove::SetTime(float _t)
{
  if (_t <= 0)
  {
    mTime = 1.0;
    return;
  }

  mTime = _t;
  ComputeMoveParams();
}
//---------------------------------------------------------------------------

void CPP_CMove::Update()
{
  if (mMoving)
  {
    // onstartmove !!!

    mTick += 0.03;
    mPos.x = mStart.x + mSpeed.GetX() * mTick + mAccel.GetX() * mTick * mTick / 2;
    mPos.y = mStart.y + mSpeed.GetY() * mTick + mAccel.GetY() * mTick * mTick / 2;

    Move(mPos.x, mPos.y);

    if (mTick >= mTime)
    {
      mMoving = false;
      OnStopMove();
    }
  }

  CPIWinControl::Update();
}
//---------------------------------------------------------------------------

void CPP_CMove::OnStartMove()
{
   if (mHandleOnStartMove)
       mHandleOnStartMove(this);
}
//---------------------------------------------------------------------------

void CPP_CMove::OnStopMove()
{
   if (mHandleOnStopMove)
       mHandleOnStopMove(this);
}
//---------------------------------------------------------------------------

/*
void CPP_CMove::SetHandleOnStartMove (void (__closure* value)(CPIObject *Sender))
{
   mHandleOnStartMove = value;
}
//---------------------------------------------------------------------------

void CPP_CMove::SetHandleOnStopMove (void (__closure* value)(CPIObject *Sender))
{
   mHandleOnStopMove = value;
}
//---------------------------------------------------------------------------
*/

