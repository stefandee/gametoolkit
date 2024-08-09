//---------------------------------------------------------------------------


#pragma hdrstop

#include "EcoSysGenKernel.h"
#include "PP_Stdlib.h"
#include "CurvePolynome.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

EcoSysItemKernelIn::EcoSysItemKernelIn() : EcoSysItemIn()
{
  TemplateParamList<double> lParamList;

  lParamList.SetName("Explicit");
  lParamList.SetParam("A0", 1.);
  lParamList.SetParam("N", 1);

  mCurve = new CurvePolynome(lParamList);

  mCurve->SetName("Default Kernel Deformation");
}
//---------------------------------------------------------------------------

EcoSysItemKernelIn::EcoSysItemKernelIn(const EcoSysItemKernelIn& _obj)
{
  mColorCode = _obj.mColorCode;
  mSize      = _obj.mSize;
  mCode      = _obj.mCode;
  mFilePath  = _obj.mFilePath;

  // we should have a clone-like method for Curve2D classes?
  // bleah - lazy - now there is one
  if (_obj.mCurve)
  {
    mCurve = _obj.mCurve->Clone();
  }
  else
  {
    mCurve = 0;
  }
}
//---------------------------------------------------------------------------

EcoSysItemKernelIn& EcoSysItemKernelIn::operator = (const EcoSysItemKernelIn& _obj)
{
  if (&_obj == this)
  {
    return (*this);
  }

  mColorCode = _obj.mColorCode;
  mSize      = _obj.mSize;
  mCode      = _obj.mCode;
  mFilePath  = _obj.mFilePath;

  // we should have a clone-like method for Curve2D classes?
  // bleah - lazy - now there is one
  if (_obj.mCurve)
  {
    mCurve = _obj.mCurve->Clone();
  }
  else
  {
    mCurve = 0;
  }

  return *this;
}
//---------------------------------------------------------------------------

std::vector<EcoSysItemOut> EcoSysGenKernel::Generate()
{
  // for now
  std::vector<EcoSysItemOut> lResult;

  // allocate the kernel matrix
  float** mKernelMatrix;

  mKernelMatrix = new float* [mMapLogicSize.y];

  for(int i = 0; i < mMapLogicSize.y; i++)
  {
    mKernelMatrix[i] = new float[mMapLogicSize.x];
  }

  for(int i = 0; i < mMapLogicSize.y; i++)
  {
    for(int j = 0; j < mMapLogicSize.x; j++)
    {
      mKernelMatrix[i][j] = mInitialValue;
    }
  }


  // allocate the partial sums vector
  std::vector<float> lR;
  lR.resize(mMapLogicSize.y);

  // allocate the conditional distribution vector
  std::vector<float> lC;
  lC.resize(mMapLogicSize.x);

  for(int i = 0; i < mSteps; i++)
  {
    // compute the R vector
    ComputeR(lR, mKernelMatrix);

    float lPlantLocationX, lPlantLocationY;

    // find the Y coordinate of the plant
    int lIndex;

    lPlantLocationY = ComputeY(lR, &lIndex);

    // compute the C vector
    ComputeC(lC, mKernelMatrix, lPlantLocationY, lIndex);

    // find the X coordinate of the plant
    lPlantLocationX = ComputeX(lC);

    // chose the plant now
    std::vector<EcoSysItemIn*> lItems = GetItems();

    //int lSizeIn = lItems.size();

    int lIndexIn = gfRandom(lItems.size());

    // setup the output
    EcoSysItemOut item;

    item.mSize      = lItems[lIndexIn]->mSize;
    item.mCode      = lItems[lIndexIn]->mCode;
    item.mColorCode = lItems[lIndexIn]->mColorCode;
    item.mFilePath  = lItems[lIndexIn]->mFilePath;
    item.mLocation  = CPPoint(lPlantLocationX, lPlantLocationY);

    lResult.push_back(item);

    // apply the kernel function (the curve that is part of EcoSysItemKernelIn)
    EcoSysItemKernelIn* lKIn = (EcoSysItemKernelIn*)(lItems[lIndexIn]);

    Curve2D* lCurve = lKIn->mCurve;

    ApplyKernelFunction(lCurve, mKernelMatrix, item.mLocation);
  }

  return lResult;
}
//---------------------------------------------------------------------------

void EcoSysGenKernel::ComputeR(std::vector<float>& _r, float** _kernel)
{
  for(unsigned int k = 0; k < _r.size(); k++)
  {
    _r[k] = 0.;

    for(unsigned int i = 0; i < k; i++)
    {
      for(int j = 0; j < mMapLogicSize.x; j++)
      {
        _r[k] += _kernel[i][j];
      }
    }
  }
}
//---------------------------------------------------------------------------

float EcoSysGenKernel::ComputeY(std::vector<float>& _r, int* _index)
{
  float u = _r[_r.size() - 1] * gfRandom48();

  int lIndex = 0;

  for(unsigned int i = 0; i < _r.size() - 1; i++)
  {
    if (_r[i] < u && u < _r[i + 1])
    {
      lIndex = i;
      break;
    }
  }

  // as interpolation
  *_index = lIndex;

  return lIndex + (u - _r[lIndex + 1]) / (_r[lIndex + 1] - _r[lIndex]);
}
//---------------------------------------------------------------------------

float EcoSysGenKernel::ComputeX(std::vector<float>& _c)
{
  float u = _c[_c.size() - 1] * gfRandom48();

  int lIndex = 0;

  for(unsigned int i = 0; i < _c.size() - 1; i++)
  {
    if (_c[i] < u && u < _c[i + 1])
    {
      lIndex = i;
      break;
    }
  }

  // as interpolation
  return lIndex + (u - _c[lIndex + 1]) / (_c[lIndex + 1] - _c[lIndex]);
}
//---------------------------------------------------------------------------

void EcoSysGenKernel::ComputeC(std::vector<float>& _c, float** _kernel, float _yt, int _index)
{
  for(unsigned int k = 0; k < _c.size(); k++)
  {
    _c[k] = 0.;

    float lSum1 = 0.;
    float lSum2 = 0.;

    for(unsigned int j = 0; j < k; j++)
    {
      lSum1 += _kernel[_index + 1][j];
      lSum2 += _kernel[_index][j];
    }

    _c[k] = (_yt - _index) * lSum1 + (_index + 1 - _yt) * lSum2;
  }
}
//---------------------------------------------------------------------------

void EcoSysGenKernel::ApplyKernelFunction(Curve2D* _curve, float** _kernel, CPPoint _center)
{
  std::vector<double>       lResult;

  if (!_curve)
  {
    return;
  }

  for(int x = 0; x < mMapLogicSize.x; x++)
  {
    for(int y = 0; y < mMapLogicSize.y; y++)
    {
      TemplateParamList<double> lParams;
      double lDist = _center.Distance(CPPoint(x, y));

      lParams.AddParam("X", lDist);

      lResult = _curve->Compute(lParams);

      _kernel[y][x] = _kernel[y][x] * lResult[1];
    }
  }
}
//---------------------------------------------------------------------------

