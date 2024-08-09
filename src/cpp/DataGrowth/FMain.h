//---------------------------------------------------------------------------

#ifndef FMainH
#define FMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
#include "DataFile.h"
#include <Grids.hpp>
//---------------------------------------------------------------------------

const AnsiString gApplicationName = "Data Growth Tool";

class TFormMain : public TForm
{
__published:	// IDE-managed Components
        TMainMenu *MainMenu;
        TMenuItem *File1;
        TMenuItem *Help1;
        TMenuItem *About1;
        TMenuItem *Open1;
        TMenuItem *Save1;
        TMenuItem *SaveAs1;
        TMenuItem *Exit1;
        TMenuItem *N1;
        TMenuItem *N2;
        TMenuItem *Tool1;
        TMenuItem *Addfile1;
        TMenuItem *Deleteentry1;
        TStatusBar *StatusBar1;
        TGroupBox *GroupBox1;
        TBevel *Bevel1;
        TPopupMenu *PopupMenuList;
        TMenuItem *AddFile2;
        TMenuItem *DeleteEntry2;
        TOpenDialog *OpenDialog;
        TSaveDialog *SaveDialog;
        TMenuItem *NewDataFile1;
        TMenuItem *N3;
        TMenuItem *Close1;
        TMenuItem *N4;
        TOpenDialog *OpenDataDialog;
        TStringGrid *Grid;
        TMenuItem *ReplaceEntry1;
        void __fastcall NewDataFile1Click(TObject *Sender);
        void __fastcall Open1Click(TObject *Sender);
        void __fastcall Close1Click(TObject *Sender);
        void __fastcall Addfile1Click(TObject *Sender);
        void __fastcall Save1Click(TObject *Sender);
        void __fastcall SaveAs1Click(TObject *Sender);
        void __fastcall Deleteentry1Click(TObject *Sender);
        void __fastcall ReplaceEntry1Click(TObject *Sender);
private:	// User declarations
        UnicodeString mFileForEdit;
        void UpdateControls();
        void SetFileName(AnsiString _v);
        void UpdateList();
        AnsiString GetId(AnsiString _filename);

        //
        // Command line
        //
        void HandleCommandLine();
        void PrintHelp();
        void MyTerminate();

        CDataFile mDataFile;

public:		// User declarations
        __fastcall TFormMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormMain *FormMain;
//---------------------------------------------------------------------------
#endif
