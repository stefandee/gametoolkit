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
//  Unit              : Curve from script
//
//  Version           : 1.0
//
//  Description       :
//    * set a script for the curve, in CSL, as a string
//    * set the parameters, depending on the curve and the equations type it may
//    * there are callbacks for the CSL script to get the parameters; the CSL
//      script also employs the use of its math library
//    * this class must have copy constructors and assignment ops ;)
//    * when writing a script for a curve, mark the following rules:
//      - if you want a parametric curve, implement in the script the XParam and YParam functions
//      - if you want an explicit curve, implement YFromX (if you have y = f(x)) or
//        XFromY (if you have x = f(y)) functions
//      - there is no support for implicit curves; you must obtain one of the other
//        representations of the curve
//
//  History           :
//    [04.11.2004] - [Karg] - created
//-----------------------------------------------------------------------------

#ifndef CurveScriptH
#define CurveScriptH
//---------------------------------------------------------------------------

#include "Curve2D.h"
#include "ZCsl.hpp"

class CurveScript : public Curve2D
{
  public:
    CurveScript() : mCompiler(0), mModuleName("Curve-C"), mValidCsl(false) {}
    CurveScript(TemplateParamList<double> _paramList) : mCompiler(0), mModuleName("Curve-C"), mValidCsl(false), Curve2D(_paramList) {}
    CurveScript(Interval _defInterval, TemplateParamList<double> _paramList) : mCompiler(0), mModuleName("Curve-C"), mValidCsl(false), Curve2D(_defInterval, _paramList) {}
    CurveScript(const CurveScript& _v);
    CurveScript& operator= (const CurveScript& _v);
    virtual ~CurveScript();
    virtual CPString ClassName () {return "Curve2DScript"; }
    virtual Curve2D* Clone() { return new CurveScript(*this); }

  public:
    virtual std::vector<double> Compute(TemplateParamList<double>& _paramList);

    void     SetScript(CPString _script);
    bool     SetScriptFromFile(const char* _fileName);
    CPString GetScript() { return mScript; }

    bool     IsValidCsl() { return mValidCsl; }

  public: // ser-deser
    virtual void        Serialize(PP::Stream &a);
    virtual void        DeSerialize(PP::Stream &a);

  protected: // script compiler methods
    virtual void InitCsl();
    virtual void InitHandlers();
    virtual void addModuleFunc(const char* szMemberName, const ZCsl_callback_t& callback);

  protected: // script handlers & script methods
    ZCsl_callback_t  mHandleGetParam;

    ZString mcGetParam(ZCsl* csl);

  protected: // computation


  private:
    ZCsl*                 mCompiler;
    ZString               mModuleName;
    bool                  mValidCsl;
    CPString              mScript;
};

#endif
