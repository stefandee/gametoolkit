//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    //
// Copyright (C) 1998, 1999, 2000, 2001 Stefan Dicu & Tudor Girba            //
//                                                                           //
// PPTactical Engine is free software; you can redistribute it               //
// and/or modify it under the terms of the GNU Lesser General Public License as     //
// published by the Free Software Foundation; either version 2.1 of the        //
// License, or (at your option) any later version.                           //
//                                                                           //
// PPTactical Engine is distributed in the hope that it will be useful, but  //
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY//
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License  //
// for more details.                                                         //
//                                                                           //
// You should have received a copy of the GNU Lesser General Public License         //
// along with PPTactical Engine; if not, write to the Free Software          //
// Foundation Inc. 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   //
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#include <dir.h>
#endif

#ifdef _MSC_VER
#include <direct.h>
#endif

#ifdef __SUNPRO_CC
#include <unistd.h>
#endif

#ifdef __GNUG__
#include <unistd.h>
#endif

#pragma hdrstop

#include "Paths.h"
#include "Platform.h"
#include "basepath.h"

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

static CPString gamePath = "";

CPString getGamePath()
{
  return gamePath;
}

void initGamePath()
{
  char lCwdStr[1024];

  getcwd(lCwdStr, 1024);
  gamePath = CPString(lCwdStr);

  #ifdef IN_LEVEL1
    for(int i = gamePath.Length() - 1; i >= 0; i--)
    {
      if (gamePath[i] == PPT_PATHSEPARATOR)
      {
        CPString lTmpStr;

        lTmpStr  = gamePath.SubString(0, i+1);
        gamePath = lTmpStr;
        return;
      }
    }
  #else
    char lChars[2];
    lChars[0] = PPT_PATHSEPARATOR;
    lChars[1] = '\0';

    gamePath.Add(CPString(lChars));
  #endif
}


