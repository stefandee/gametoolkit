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
/*---------------------------------------------------------------------------
 application: Pure Power

 description: Interface Defines
 last modify: 17 04 00
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#ifndef PP_GuiDefH
#define PP_GuiDefH

//type mouse buttons
typedef struct
{
   bool mLeft, mRight, mMiddle;
}TMouseButtons;
//---------------------------------------------------------------------------

enum TDialogButtons { DB_OK, DB_YESNO, DB_OKCANCEL, DB_YESNOCANCEL, DB_RETRYCANCEL };
//---------------------------------------------------------------------------

//Piron Interface Class Names
#define PICN_OBJECT           "CPIObject"
#define PICN_CONTROL          "CPIControl"
#define PICN_WINCONTROL       "CPIWinControl"
#define PICN_DESKTOP          "CPIDesktop"
#define PICN_PANEL            "CPIPanel"
#define PICN_FORM             "CPIForm"
#define PICN_LABEL            "CPILabel"
#define PICN_BUTTON           "CPIButton"
#define PICN_CHECKBUTTON      "CPICheckButton"
#define PICN_CHECKBOX         "CPICheckBox"
#define PICN_EDITCONTROL      "CPIEditControl"
#define PICN_TRACKBAR         "CPITrackBar"
//---------------------------------------------------------------------------

//Piron Modal Results
#define PMR_OK          1
#define PMR_CANCEL      2
#define PMR_YES         3
#define PMR_NO          4
//---------------------------------------------------------------------------

//Piron Text Allignment
#define PTA_LEFT        0
#define PTA_RIGHT       1
#define PTA_CENTER      2
#define PTA_JUSTIFY     3

//---------------------------------------------------------------------------
#endif

