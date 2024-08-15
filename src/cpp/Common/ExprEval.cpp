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
//---------------------------------------------------------------------------
//#include <SysUtils.hpp>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#pragma hdrstop

#include "ExprEval.h"
#include "Platform.h"
//---------------------------------------------------------------------------

#ifdef PPT_COMPILER_BORLAND
#pragma package(smart_init)
#endif

#define EVALA_KW_POW        254
#define EVALA_NOTHING       255
#define EVALA_MULTIPLY      256
#define EVALA_DIVIDE        257
#define EVALA_MINUS         258
#define EVALA_PLUS          259
#define EVALA_REAL          260
#define EVALA_INT           261
#define EVALA_BRACKET_LEFT  264  // open bracket
#define EVALA_BRACKET_RIGHT 265  // close bracket
#define EVALA_EOS           266

// Descriere   : nimic interesant
// Param       :
// Rezultat    :
// Comentarii  :
CPExprEval::CPExprEval()
{
  mAv.value   = (char*)new char[EVAL_DATA_SIZE + 1];
  mAv.lexCode = EVALA_NOTHING;
  mExprIndex  = -1;
  mExpr       = "";
}
//---------------------------------------------------------------------------

double CPExprEval::Evaluate(std::string _e)
{
  double lValue = 0.0;

  if (_e.empty())
  {
    throw CPEvalError("String is empty.");
  }

  mExpr      = _e;
  mExprIndex = -1;

  if (!EvalAsin(&lValue))
  {
    throw CPEvalError("Error in expression.");
  }

  return lValue;
}
//---------------------------------------------------------------------------

int  CPExprEval::NextChar()
{
  if (++mExprIndex >= mExpr.length())
  {
    return 256;
  }

  return mExpr[mExprIndex];
}
//---------------------------------------------------------------------------

int  CPExprEval::BackChar()
{
  if (--mExprIndex < 0)
  {
    return -1;
  }

  return 1;
}
//---------------------------------------------------------------------------

bool CPExprEval::EvalAlex()
{
  int           ch, fractionarPart;
  int           resNameIndex = 0;

  // prelucrare initiala
  mAv.lexCode = EVALA_NOTHING;
  memset((char*)mAv.value, '\0', EVAL_DATA_SIZE);

  do
  {
    ch = NextChar();

    // DIGIT ?
    if (isdigit(ch))
    {
      // prelucrez cifra
      //((char*)av.value)[resNameIndex] = (char)ch;

      while(isdigit(ch))
      {
        ((char*)mAv.value)[resNameIndex] = (char)ch;
        resNameIndex++;
        if (resNameIndex > EVAL_DATA_SIZE)
        {
          return false;
        }
        ch = NextChar();
      }

      // a fost punct ?
      if (ch == '.')
      {
        // avem numar real, adaug punctul
        ((char*)mAv.value)[resNameIndex++] = (char)ch;

        // citesc in continuare
        fractionarPart = 0;
        ch = NextChar();
        while(isdigit(ch))
        {
          ((char*)mAv.value)[resNameIndex] = (char)ch;
          resNameIndex++;
          if (resNameIndex > EVAL_DATA_SIZE)
          {
            return false;
          }
          fractionarPart++;
          ch = NextChar();
        }

        if (fractionarPart == 0)
        {
          return false;
        }

        if (ch != 256)
        {
          BackChar();
        }

        mAv.lexCode = EVALA_REAL;
        return true;
      }

      if (ch != 256)
      {
        BackChar();
      }

      mAv.lexCode = EVALA_INT;
      return true;
    }

    // SEMNE SPECIALE
    switch(ch)
    {
      case '(' :
        mAv.lexCode = EVALA_BRACKET_LEFT;
        return true;

      case ')' :
        mAv.lexCode = EVALA_BRACKET_RIGHT;
        return true;

      case '+' :
        mAv.lexCode = EVALA_PLUS;
        return true;

      case '-' :
        mAv.lexCode = EVALA_MINUS;
        return true;

      case '/' :
        mAv.lexCode = EVALA_DIVIDE;
        return true;

      case '*' :
        mAv.lexCode = EVALA_MULTIPLY;
        return true;

      case 256 :
        mAv.lexCode = EVALA_EOS;
        return true;
    }
  }
  while(ch == 13 || ch == 10 || ch == 32);

  return false;
}
//---------------------------------------------------------------------------

bool CPExprEval::EvalAsin(double* _v)
{
  if (!EvalAlex())
  {
    return false;
  }

  return ExprStatica(_v);
}
//---------------------------------------------------------------------------


bool CPExprEval::ExprStatica(double* _v)
{
  int lOpCode = -1;
  double lV = 0.0;

  while(1)
  {
    if (!TermenStatic(_v))
    {
      return false;
    }

    // in mAv.lexCode ar trebui sa am acum un cod de operatie
    switch(lOpCode)
    {
      case EVALA_PLUS  :
        lV += *_v;
        break;

      case EVALA_MINUS :
        lV -= *_v;
        break;

      default :
        lV = *_v;
    }

    switch(mAv.lexCode)
    {
      case EVALA_EOS   :
        *_v = lV;
        return true;

      case EVALA_PLUS  :
        lOpCode = EVALA_PLUS;
        break;

      case EVALA_MINUS :
        lOpCode = EVALA_MINUS;
        break;

      default:
        *_v = lV; // din cauza parantezei
        return false;
    }

    if (!EvalAlex())
    {
      return false;
    }
  }
}
//---------------------------------------------------------------------------

bool CPExprEval::TermenStatic (double* _v)
{
  int lOpCode = -1;
  double lV = 1.0;

  while(1)
  {
    if (!FactorStatic(_v))
    {
      return false;
    }

    // in mAv.lexCode ar trebui sa am acum un cod de operatie
    switch(lOpCode)
    {
      case EVALA_MULTIPLY  :
        lV *= *_v;
        break;

      case EVALA_DIVIDE :

        if (*_v == 0)
        {
          return false;
        }

        lV /= *_v;
        break;

      default :
        lV = *_v;
    }

    switch(mAv.lexCode)
    {
      case EVALA_EOS   :
        *_v = lV;
        return true;

      case EVALA_DIVIDE  :
        lOpCode = EVALA_DIVIDE;
        break;

      case EVALA_MULTIPLY :
        lOpCode = EVALA_MULTIPLY;
        break;

      default:
        // atomul nu poate fi procesat de aceasta functie; ExprStatica il va
        // procesa atunci
        *_v = lV;
        return true;
    }

    if (!EvalAlex())
    {
      return false;
    }
  }
}
//---------------------------------------------------------------------------

bool CPExprEval::FactorStatic (double* _v)
{
  switch(mAv.lexCode)
  {
    case EVALA_INT :
      try
      {
        // nu e portabila
        // *_v = StrToInt(AnsiString((char*)mAv.value));
        *_v = atoi((const char*)mAv.value);
      }
      catch(...)
      {
        *_v = 0.0;
        return false;
      }

      break;

    case EVALA_REAL :
      try
      {
        // nu e portabila
        // *_v = StrToFloat(AnsiString((char*)mAv.value));
        *_v = atof((const char*)mAv.value);
      }
      catch(...)
      {
        *_v = 0.0;
        return false;
      }

      break;

    case EVALA_BRACKET_LEFT :

      if (!EvalAlex())
      {
        return false;
      }

      if (!ExprStatica(_v) && (mAv.lexCode != EVALA_BRACKET_RIGHT))
      {
        return false;
      }

      break;


    default :
      return false;
  }

  if (!EvalAlex())
  {
    return false;
  }

  return true;
}
//---------------------------------------------------------------------------

CPExprEval::~CPExprEval()
{
  delete [] (char*)mAv.value;
}
//---------------------------------------------------------------------------
