//---------------------------------------------------------------------------

#pragma hdrstop

#include "CurveComposite.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

std::vector<double> CurveComposite::Compute(TemplateParamList<double>& _paramList)
{
  std::vector<double> lComputed;

  for(unsigned int i = 0; i < mCurves.size(); i++)
  {
    if (!mCurves[i])
    {
      continue;
    }

    std::vector<double> lTempVector = mCurves[i]->Compute(_paramList);

    for(unsigned int j = 0; j < lTempVector.size(); j++)
    {
      lComputed.push_back(lTempVector[j]);
    }
  }

  return lComputed;
}
//---------------------------------------------------------------------------

