//---------------------------------------------------------------------------

#ifndef FActionsH
#define FActionsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>
#include "Action.h"
#include <Dialogs.hpp>
#include <vector>
//---------------------------------------------------------------------------

class MyStringGrid : public TStringGrid
{
  public:
    virtual void MyDeleteRow(int _pos) { TStringGrid::DeleteRow(_pos); };
    virtual void MyDeleteColumn(int _pos) { TStringGrid::DeleteColumn(_pos); };
};

class TFormActions : public TForm
{
__published:	// IDE-managed Components
        TStringGrid *stringGrid;
        TPopupMenu *popupMenu;
        TMenuItem *Save1;
        TMenuItem *N1;
        TMenuItem *Addaction1;
        TMenuItem *Removeaction1;
        TMenuItem *Add10actions1;
        TMenuItem *RemoveAllACtions1;
        TMenuItem *N2;
        TMenuItem *Correct1;
        TMenuItem *N3;
        TMenuItem *PastefromClipboard1;
        TMenuItem *N4;
        TMenuItem *Export1;
        TSaveDialog *dlgSave;
        TSaveDialog *dlgExport;
        void __fastcall Addaction1Click(TObject *Sender);
        void __fastcall Add10actions1Click(TObject *Sender);
        void __fastcall stringGridContextPopup(TObject *Sender,
          TPoint &MousePos, bool &Handled);
        void __fastcall Removeaction1Click(TObject *Sender);
        void __fastcall Save1Click(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall Correct1Click(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall PastefromClipboard1Click(TObject *Sender);
        void __fastcall Export1Click(TObject *Sender);
private:	// User declarations
        TPoint mLastPopupPoint;
        std::vector<CAction> mActions;
        UnicodeString mFileName;

        void DeleteRow(int _pos);
        void DeleteColumn(int _pos);

        void LoadActions(UnicodeString _fileName);
        void SaveActions(UnicodeString _fileName);

        void Export(UnicodeString _fileName);

        void ListToScreen();
        void ScreenToList();
        UnicodeString BuildSpeedsList(int _action, bool _what);
        void StringToSpeeds(int, UnicodeString, UnicodeString);

public:		// User declarations
        __fastcall TFormActions(TComponent* Owner);

        void SetFileName(UnicodeString _fileName);

        char* GetData(UnicodeString _fileName, int* _size);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormActions *FormActions;
//---------------------------------------------------------------------------
#endif
