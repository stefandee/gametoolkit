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
//-----------------------------------------------------------------------------
//  Unit              : Piron Interface Image
//
//  Versiune          : 0.9
//
//  Descriere         :
//    * echivalentul lui TImage din VCL - control care are o imagine pe fundal
//
//  Istorie           :
//    [16.01.2001] - [Karg] - unit creat
//-----------------------------------------------------------------------------
#ifndef PP_CImageH
#define PP_CImageH
//---------------------------------------------------------------------------

#include "PP_CControl.h"

class CPIImage : public CPIControl
{
  private:
    bool mCenter;
    bool mStretch;
    bool mTransparent;

  protected:

  public:
    CPIImage(CPIWinControl *Parent = NULL);

    virtual void LoadFromFile(CPString _fileName);

    virtual void Paint();

    //set-get
    bool GetCenter() { return mCenter; }
    void SetCenter(bool _c) { mCenter = _c; }

    bool GetStretch() { return mStretch; }
    void SetStretch(bool _s) { mStretch = _s; }

    /*
    bool GetStretch() { return mStretch; }
    void SetStretch(bool _s) { mStretch = _s; }
    */

    virtual ~CPIImage();
};


#endif
