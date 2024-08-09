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
#ifndef SpecReplaceH
#define SpecReplaceH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <ExtDlgs.hpp>
#include "ImageEx.h"
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------

enum TFileFormat { _000, _0000};

class TFormReplace : public TForm
{
__published:	// IDE-managed Components
        TGroupBox *gbDimensions;
        TGroupBox *gbFile;
        TButton *btnOk;
        TButton *btnCancel;
        TComboBox *cbFormat;
        TLabel *lFormat;
        TLabel *lBase;
        TEdit *edBase;
        TEdit *edStart;
        TLabel *lFiles;
        TLabel *lRows;
        TEdit *edRows;
        TLabel *lColumns;
        TEdit *edCol;
        TEdit *edFirst;
        TLabel *Label1;
        TLabel *Label2;
        TEdit *edLast;
        TOpenImageEx *OpenSpecial;
   TRadioGroup *rgStyle;
        void __fastcall edBaseDblClick(TObject *Sender);
        void __fastcall edStartChange(TObject *Sender);
        void __fastcall edRowsChange(TObject *Sender);
        void __fastcall edColChange(TObject *Sender);
        void __fastcall cbFormatChange(TObject *Sender);
        void __fastcall edFirstChange(TObject *Sender);
        void __fastcall edLastChange(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
    
private:	// User declarations
public:		// User declarations
        AnsiString  pathToFiles;
        AnsiString  baseFile;
        TFileFormat format;
        int         startFile, firstFile, lastFile;
        int         rows, cols;

        __fastcall  TFormReplace(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormReplace *FormReplace;
//---------------------------------------------------------------------------
#endif
