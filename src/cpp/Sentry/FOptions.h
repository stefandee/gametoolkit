//---------------------------------------------------------------------------

#ifndef FOptionsH
#define FOptionsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------

#include "AppConfig.h"
#include "CSPIN.h"
//---------------------------------------------------------------------------

class TFormOptions : public TForm
{
__published:	// IDE-managed Components
    TPageControl *PageControl1;
    TTabSheet *TabGeneral;
    TTabSheet *TabModules;
    TTabSheet *TabFrames;
    TTabSheet *TabAnims;
    TButton *BtnOk;
    TButton *BtnCancel;
    TLabel *Label1;
    TShape *Shape1;
    TColorDialog *ColorDialog;
    TLabel *Label2;
    TShape *Shape2;
    TLabel *Label3;
    TShape *Shape3;
    TLabel *Label4;
    TShape *Shape4;
    TLabel *Label5;
    TShape *Shape5;
    TCSpinEdit *CSpinEdit1;
    TCSpinEdit *CSpinEdit2;
    TLabel *Label6;
    TLabel *Label7;
    TLabel *Label8;
    TCSpinEdit *CSpinEdit3;
    TTabSheet *TabFLogic;
    TButton *btnReset;
    TLabel *Label9;
    TShape *Shape6;
    TShape *Shape7;
    TLabel *Label10;
    TLabel *Label11;
    TCSpinEdit *CSpinEdit4;
    TLabel *Label12;
    TCSpinEdit *CSpinEdit5;
    TLabel *Label13;
    TShape *Shape8;
    TLabel *Label14;
    TCSpinEdit *CSpinEdit6;
    TLabel *Label15;
    TShape *Shape10;
    TLabel *Label16;
    TShape *Shape11;
    TLabel *Label17;
    TShape *Shape9;
    TLabel *Label18;
    TShape *Shape12;
    TShape *Shape13;
    TLabel *Label19;
    TLabel *Label20;
    TShape *Shape14;
    TLabel *Label21;
    TCSpinEdit *CSpinEdit7;
    TCSpinEdit *CSpinEdit8;
    TLabel *Label22;
    TLabel *Label23;
    TShape *Shape15;
    TLabel *Label24;
    TCSpinEdit *CSpinEdit9;
    TLabel *Label25;
    TLabel *Label26;
    TShape *Shape16;
    TShape *Shape17;
    TLabel *Label27;
    TShape *Shape18;
    TShape *Shape19;
    TLabel *Label28;
    TLabel *Label29;
    TShape *Shape20;
    TCSpinEdit *CSpinEdit10;
    TLabel *Label30;
    TLabel *Label31;
    TCSpinEdit *CSpinEdit11;
    TShape *Shape21;
    TLabel *Label32;
    TLabel *Label33;
    TShape *Shape22;
    TLabel *Label34;
    TShape *Shape23;
    TLabel *Label35;
    TCSpinEdit *CSpinEdit12;
    TTabSheet *TabColorMap;
    TLabel *Label36;
    TShape *Shape24;
    TCSpinEdit *CSpinEdit13;
    TLabel *Label37;
    TLabel *Label38;
    TCSpinEdit *CSpinEdit14;
    TLabel *Label39;
    TShape *Shape25;
    TShape *Shape26;
    TLabel *Label40;
    TTabSheet *Scripts;
    TLabel *Label41;
    TEdit *edScriptsPath;
    TButton *btnBrowseScriptsPath;
    TOpenDialog *OpenScriptsPathDialog;
    void __fastcall Shape1MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall BtnOkClick(TObject *Sender);
    void __fastcall btnResetClick(TObject *Sender);
    void __fastcall btnBrowseScriptsPathClick(TObject *Sender);

private:	// User declarations
    AppConfig* mAppConfig;

    void AppConfigToGUI();

public:		// User declarations
    __fastcall TFormOptions(TComponent* Owner);

    void SetAppConfig(AppConfig& appConfig);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormOptions *FormOptions;
//---------------------------------------------------------------------------
#endif
