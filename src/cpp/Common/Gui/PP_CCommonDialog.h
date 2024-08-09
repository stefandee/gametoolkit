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
//  Unit              : Common Dialog
//
//  Version           : 1.0
//
//  Description       :
//    * ancestor for dialog classes
//
//  History           :
//    [06.10.2004] - [Karg] - unit created
//-----------------------------------------------------------------------------

#ifndef PP_CCommonDialogH
#define PP_CCommonDialogH
//---------------------------------------------------------------------------

#include "PP_CForm.h"

class CPICommonDialog : public CPIForm
{
  public: //c-d
    CPICommonDialog(CPIWinControl *Parent = NULL);
    virtual ~CPICommonDialog();

    virtual CPString ClassName() {return CPString("CPICOMMONDIALOG"); }
    bool IsDerivedFrom(CPString className);

    virtual bool Execute();

  public:
    virtual void OnClose();

  public:
    virtual void Update();
    virtual void Paint();
    virtual void Close();   //simulates the click on the close
};

#endif
