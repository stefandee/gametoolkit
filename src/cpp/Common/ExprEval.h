//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    // 
// Copyright (C) 1998, 1999, 2000, 2001 Stefan Dicu & Tudor Girba            //
//                                                                           //
// PPTactical Engine is free software; you can redistribute it               //
// and/or modify it under the terms of the GNU General Public License as     // 
// published by the Free Software Foundation; either version 2 of the        //
// License, or (at your option) any later version.                           //
//                                                                           //  
// PPTactical Engine is distributed in the hope that it will be useful, but  //
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY// 
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License  //
// for more details.                                                         //
//                                                                           // 
// You should have received a copy of the GNU General Public License         //
// along with PPTactical Engine; if not, write to the Free Software          //
// Foundation Inc. 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   // 
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//  Unit              : ExprEval.h (expression evaluator)
//
//  Versiune          : 0.9
//
//  Descriere         :
//    * evalueaza expresii matematice dintr'un sir de caractere
//
//  Istorie           :
//    [28.07.2000] - [Karg] - a fost creat unitul
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
#ifndef ExprEvalH
#define ExprEvalH
//---------------------------------------------------------------------------

#include <stdexcept>

#define EVAL_DATA_SIZE 4096

// atom-valoare2
typedef struct
{
  __int32 lexCode;
  void*   value;
} TCfgAtomValue2;

// clasa de exceptie - daca exista erori de evaluare, se arunca exceptia
class CPEvalError : public std::exception {
   public:
     explicit CPEvalError(const std::string& what_arg) {};
};

// clasa care evalueaza un sir(cu operatori) la expresia lui numerica
class CPExprEval
{
  private:
    int            mExprIndex;
	std::string    mExpr;
	TCfgAtomValue2 mAv;

  protected:
    bool TermenStatic (double* _v);
    bool ExprStatica  (double* _v);
    bool FactorStatic (double* _v);

    int  NextChar     ();
    int  BackChar     ();
    bool EvalAlex     ();
    bool EvalAsin     (double* _v);

  public:
    CPExprEval();

	virtual double Evaluate(std::string _e);

    ~CPExprEval();
};

#endif

