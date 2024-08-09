//---------------------------------------------------------------------------

#ifndef FEditCurveStreamH
#define FEditCurveStreamH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <Grids.hpp>
#include <Dialogs.hpp>
#include <vector>
#include "ObjectStream.h"
#include "Curve2D.h"
#include "CFileHistory.h"
//---------------------------------------------------------------------------

enum TStreamEditState { SEDIT_NOT_OPENED, SEDIT_MODIFIED, SEDIT_NOT_MODIFIED, SEDIT_EMPTY, SEDIT_REQUEST_FILENAME };

const AnsiString NEW_FILE_NAME = "library.objlib";

const int HISTORY_MENU_MAX_ITEMS = 5;

class TFormEditObjectStream : public TForm
{
__published:	// IDE-managed Components
        TMainMenu *MainMenu;
        TMenuItem *File1;
        TMenuItem *Open1;
        TMenuItem *Save1;
        TMenuItem *About1;
        TMenuItem *About2;
        TToolBar *ToolBar;
        TToolButton *ToolButton1;
        TToolButton *ToolButton2;
        TToolButton *ToolButton3;
        TStatusBar *StatusBar;
        TStringGrid *StringGrid;
        TPopupMenu *PopupMenu;
        TMenuItem *AddRecord1;
        TMenuItem *RemoveRecord1;
        TMenuItem *N1;
        TMenuItem *Save2;
        TMenuItem *SaveAs1;
        TMenuItem *Load1;
        TMenuItem *Clear1;
        TMenuItem *N2;
        TMenuItem *Exit1;
        TMenuItem *InsertRecord1;
        TMenuItem *New1;
        TMenuItem *SaveAs2;
        TMenuItem *Edit1;
        TMenuItem *N3;
        TOpenDialog *OpenDialog;
        TSaveDialog *SaveDialog;
        TMenuItem *OpenRecent1;
        TMenuItem *N5;
        void __fastcall About2Click(TObject *Sender);
        void __fastcall Clear1Click(TObject *Sender);
        void __fastcall Exit1Click(TObject *Sender);
        void __fastcall AddRecord1Click(TObject *Sender);
        void __fastcall InsertRecord1Click(TObject *Sender);
        void __fastcall RemoveRecord1Click(TObject *Sender);
        void __fastcall New1Click(TObject *Sender);
        void __fastcall Edit1Click(TObject *Sender);
        void __fastcall Open1Click(TObject *Sender);
        void __fastcall SaveAs2Click(TObject *Sender);
        void __fastcall Save2Click(TObject *Sender);
        void __fastcall Save1Click(TObject *Sender);
        void __fastcall Load1Click(TObject *Sender);
private:	// User declarations
        TStreamEditState mState;

        // a vector must be instantiated here - containing objects
        // of the base class; this is the memory representation of
        // the objectstream (e.g. operations will be performed in memory,
        // then written to disk)
        std::vector<Curve2D*> mObjects;

        AnsiString mFileName;

        CFileHistory* mFileHistory;

protected:
        void __fastcall HistoryClick(TObject *Sender);
        
        virtual void SetState(TStreamEditState _v);
        virtual TStreamEditState GetState();

        virtual void SetupStringList();

        void ClearObjects();
        void ClearStringGrid();

        void SetFileName(AnsiString _fileName);
        AnsiString GetFileName();

        void Save(AnsiString _fileName);
        void Load(AnsiString _fileName);

public:		// User declarations
        __fastcall TFormEditObjectStream(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormEditObjectStream *FormEditObjectStream;
//---------------------------------------------------------------------------
#endif
