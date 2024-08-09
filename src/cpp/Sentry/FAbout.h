//---------------------------------------------------------------------------

#ifndef FAboutH
#define FAboutH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "THttpLabel.h"
//---------------------------------------------------------------------------
class TFormAbout : public TForm
{
__published:	// IDE-managed Components
    TMemo *MemoInfo;
    TGroupBox *GroupBox1;
    TButton *ButtonOk;
    TLabel *Label1;
    TLabel *Label2;
    THttpLabel *HttpLabel1;
private:	// User declarations
public:		// User declarations
    __fastcall TFormAbout(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormAbout *FormAbout;
//---------------------------------------------------------------------------
#endif
