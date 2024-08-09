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
#ifndef newbmpH
#define newbmpH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TNewForm : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label1;
        TEdit *ed_xsize;
        TEdit *ed_ysize;
        TLabel *Label2;
        TLabel *cell_status;
        TButton *Button1;
        TButton *Button2;
        TEdit *ed_ybmp;
        TLabel *Label3;
        TLabel *Label4;
        TEdit *ed_xbmp;
        void __fastcall ed_xbmpExit(TObject *Sender);
        void __fastcall ed_ybmpExit(TObject *Sender);
        void __fastcall ed_xsizeExit(TObject *Sender);
        void __fastcall ed_ysizeExit(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TNewForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TNewForm *NewForm;
//---------------------------------------------------------------------------
#endif
