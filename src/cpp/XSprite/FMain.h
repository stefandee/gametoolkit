//---------------------------------------------------------------------------

#ifndef FMainH
#define FMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
#include <ExtDlgs.hpp>
#include "EditorDef.h"
#include "Cell.h"
#include <vector>
//---------------------------------------------------------------------------
class TFormMain : public TForm
{
__published:	// IDE-managed Components
        TGroupBox *GroupBox1;
        TStatusBar *StatusBar1;
        TGroupBox *GroupBox2;
        TScrollBar *sbH;
        TScrollBar *sbV;
        TPaintBox *paintSprite;
        TGroupBox *GroupBox3;
        TComboBox *cbCells;
        TGroupBox *GroupBox4;
        TTrackBar *trackZoom;
        TLabel *Label1;
        TMainMenu *MainMenu1;
        TMenuItem *File1;
        TMenuItem *Open1;
        TMenuItem *N1;
        TMenuItem *Exit1;
        TMenuItem *Help1;
        TMenuItem *About1;
        TRadioGroup *rgZone;
        TLabel *Label2;
        TShape *shapeBox;
        TLabel *Label3;
        TShape *shapePoint;
        TButton *btnNewCell;
        TOpenPictureDialog *dlgOpen;
        TColorDialog *dlgColor;
        TButton *btnEdit;
        TMenuItem *Export1;
        TSaveDialog *dlgSave;
        TMenuItem *N2;
        TMenuItem *Exportassprite1;
        TMenuItem *Exportasindexedsprite1;
        TMenuItem *Actions1;
        TMenuItem *ActionEditor1;
        TMenuItem *Exportas4bitindexedsprite1;
        TMenuItem *Exportas4bitindexedspriteactions1;
        void __fastcall Open1Click(TObject *Sender);
        void __fastcall paintSpritePaint(TObject *Sender);
        void __fastcall trackZoomChange(TObject *Sender);
        void __fastcall shapeBoxMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall shapePointMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall sbHChange(TObject *Sender);
        void __fastcall sbVChange(TObject *Sender);
        void __fastcall FormResize(TObject *Sender);
        void __fastcall paintSpriteMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall paintSpriteMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
        void __fastcall paintSpriteMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnNewCellClick(TObject *Sender);
        void __fastcall cbCellsChange(TObject *Sender);
        void __fastcall btnEditClick(TObject *Sender);
        void __fastcall Export1Click(TObject *Sender);
        void __fastcall Exportassprite1Click(TObject *Sender);
        void __fastcall Exit1Click(TObject *Sender);
        void __fastcall ActionEditor1Click(TObject *Sender);
        void __fastcall Exportas4bitindexedsprite1Click(TObject *Sender);
        void __fastcall Exportasindexedsprite1Click(TObject *Sender);
        void __fastcall Exportas4bitindexedspriteactions1Click(
          TObject *Sender);
private:	// User declarations
    Graphics::TBitmap* mBmp;
    TXState mState;
    std::vector<CCell*> mCells;
    bool mSelecting;
    int mStartX, mStartY, mCurrentX, mCurrentY;

    void ComputeScrollLimits();
    void MakeCombo();

    bool Load(std::wstring _fileName);
    bool Save(std::wstring _fileName);

    bool ExportAsDefinition(std::wstring _fileName);
    bool ExportAsSprite(std::wstring _fileName);
    bool ExportAsIndexedSprite(std::wstring _fileName);
    bool ExportAs4BitIndexedSprite(std::wstring _fileName);
    bool ExportAs4BitIndexedSpriteAndActions(std::wstring _fileName);

    void PrintHelp();
    void HandleCommandLine();
    void MyTerminate();

    UnicodeString mBmpFileName, mDatFileName;

public:		// User declarations
        __fastcall TFormMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormMain *FormMain;
//---------------------------------------------------------------------------
#endif
