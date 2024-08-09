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
#ifndef m_spredH
#define m_spredH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
#include <ExtDlgs.hpp>
#include <ComCtrls.hpp>
#include "CfgBin.h"
#include "ImageEx.h"
//---------------------------------------------------------------------------

#define CFG_FILE_NAME      "spred.pCfg"
#define HISTORY_MENU_INDEX 5
#define MAX_HISTORY_LINKS  6

class TMainForm : public TForm
{
__published:	// IDE-managed Components
        TMainMenu *MainMenu;
        TMenuItem *File1;
        TMenuItem *New1;
        TMenuItem *Save1;
        TMenuItem *Load1;
        TMenuItem *N1;
        TMenuItem *Exit1;
        TMenuItem *Help1;
        TMenuItem *About1;
        TImage *Sprite;
        TPopupMenu *PopupMenu;
        TMenuItem *Replace1;
        TMenuItem *Clear1;
        TMenuItem *N2;
        TMenuItem *Load2;
        TMenuItem *Savebitmap1;
        TSavePictureDialog *SavePictureDialog;
        TMenuItem *N3;
        TMenuItem *Resize1;
        TMenuItem *Newbitmap1;
    TMenuItem *N4;
    TMenuItem *Insertrowbefore1;
    TMenuItem *Insertrowafter1;
    TMenuItem *Insertcolumnbefore1;
    TMenuItem *InsertColumnafter1;
    TMenuItem *N5;
    TMenuItem *Deleterow1;
    TMenuItem *Deletecolumn1;
    TStatusBar *StatusBar1;
    TMenuItem *Saveas1;
    TMenuItem *Savebitmapas1;
        TMenuItem *Replace;
    TMenuItem *N6;
        TOpenImageEx *OpenPictureDialog;
        void __fastcall New1Click(TObject *Sender);
        void __fastcall Replace1Click(TObject *Sender);
        void __fastcall SpriteMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall Load1Click(TObject *Sender);

    void __fastcall Savebitmap1Click(TObject *Sender);
    void __fastcall Resize1Click(TObject *Sender);

    void __fastcall Clear1Click(TObject *Sender);

    void __fastcall Savebitmapas1Click(TObject *Sender);

    void __fastcall ReplaceClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall About1Click(TObject *Sender);
    
private:	// User declarations
        void __fastcall History1Click(TObject *Sender);
        void __fastcall History2Click(TObject *Sender);
        void __fastcall History3Click(TObject *Sender);
        void __fastcall History4Click(TObject *Sender);
        void __fastcall History5Click(TObject *Sender);
        void FillByRows();
        void FillByCols();

public:		// User declarations
        int xCellSize, yCellSize;
        int xCellSel, yCellSel;
        int xBmpSize, yBmpSize;
        AnsiString fileName;
        bool modified, newFile;
        // AnsiString appDir;
        CCfgBin cfgFile;
        int historyItems, historyCrtItems;

        __fastcall TMainForm(TComponent* Owner);
        bool LoadSpriteBmp(AnsiString spriteFileName);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
