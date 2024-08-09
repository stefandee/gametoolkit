//---------------------------------------------------------------------------

#include <math.h>
#pragma hdrstop

#include "Action.h"

//---------------------------------------------------------------------------

CAction::CAction()
{
  mName   = "New action";
  mId     = 0;
  mUpdate = 1;
  mStartFrame = mEndFrame = 0;
  mXAlign = "LEFT";
  mYAlign = "TOP";
  mInactive = "NO";
  mEffect = "NORMAL";
  mRepeat = 0;
  mSpeeds.clear();
  mSpeeds.push_back(CSpeed(2,0));
}
//---------------------------------------------------------------------------

CAction::~CAction()
{
}
//---------------------------------------------------------------------------

void CAction::SetStartFrame(int _v)
{
  mStartFrame = _v;
  
  RecomputeSpeeds();
}
//---------------------------------------------------------------------------

void CAction::SetEndFrame(int _v)
{
  mEndFrame = _v;

  RecomputeSpeeds();
}
//---------------------------------------------------------------------------

void CAction::RecomputeSpeeds()
{
  int lSpeedsSize = abs(mStartFrame - mEndFrame) + 1;
  //int lOldSize = mSpeeds.size();

  mSpeeds.resize(lSpeedsSize);

  //if (lOldSize < lSpeedsSize)
  //{
  //  for(int i = lOldSize; i < lSpeedsSize; i++)
  //  {
  //    mSpeeds[i].SetX(0);
  //    mSpeeds[i].SetY(0);
  //  }
  //}
}
//---------------------------------------------------------------------------

#pragma package(smart_init)
