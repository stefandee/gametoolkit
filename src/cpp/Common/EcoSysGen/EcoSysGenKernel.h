//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    //
// Copyright (C) 1998-2004 Stefan Dicu & Tudor Girba                         //
//                                                                           //
// PPTactical Engine is free software; you can redistribute it               //
// and/or modify it under the terms of the GNU Lesser General Public License //
// as published by the Free Software Foundation; either version 2.1 of the   //
// License, or (at your option) any later version.                           //
//                                                                           //
// PPTactical Engine is distributed in the hope that it will be useful, but  //
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY//
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public    //
// License for more details.                                                 //
//                                                                           //
// You should have received a copy of the GNU Lesser General Public License  //
// along with PPTactical Engine; if not, write to the Free Software          //
// Foundation Inc. 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   //
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  Unit              : EcoSystem Generation using Kernel Deformation
//
//  Version           : 1.0
//
//  Description       :
//    * uses the algorithm described in paper "Generating Spatial Distributions
//      for Multilevel Models of Plant Communities" by Brendan Lane and Przemyslaw
//      Prusinkiewicz, pages 6-10
//
//  History           :
//    [13.10.2004] - [Karg] - created
//-----------------------------------------------------------------------------

#ifndef EcoSysGenKernelH
#define EcoSysGenKernelH
//---------------------------------------------------------------------------

#include "EcoSysGen.h"
#include "Curve2D.h"
#include <vector>

class EcoSysItemKernelIn : public EcoSysItemIn
{
  public:
    EcoSysItemKernelIn();
    EcoSysItemKernelIn(const EcoSysItemKernelIn&);
    EcoSysItemKernelIn& operator = (const EcoSysItemKernelIn&);
    virtual EcoSysItemIn* Clone() { return new EcoSysItemKernelIn(*this); }

    Curve2D* mCurve;
};
//---------------------------------------------------------------------------

class EcoSysGenKernel : public EcoSysGen
{
  public:
    EcoSysGenKernel() : mInitialValue(.5), mSteps(50), mMapLogicSize(CPPoint(50, 50)) {}
    ~EcoSysGenKernel() {}

  public:
    std::vector<EcoSysItemOut> Generate();

  public:
    void SetSteps(int _v) { mSteps = _v; }
    int  GetSteps() { return mSteps; }

    void SetMapLogicSize(CPPoint _v) { mMapLogicSize = _v; }
    CPPoint GetMapLogicSize() { return mMapLogicSize; }

    float GetInitialValue() { return mInitialValue; }
    void  SetInitialValue(float _v) { mInitialValue = _v; }

  protected:
    void ApplyKernelFunction(Curve2D* _curve, float** _kernel, CPPoint _center);
    void ComputeC(std::vector<float>& _c, float** _kernel, float _yt, int _index);
    float ComputeX(std::vector<float>& _c);
    float ComputeY(std::vector<float>& _r, int* _index);
    void  ComputeR(std::vector<float>& _r, float** _kernel);

  private:
    int     mSteps;
    CPPoint mMapLogicSize;
    float   mInitialValue;
};
//---------------------------------------------------------------------------

#endif
