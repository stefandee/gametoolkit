//---------------------------------------------------------------------------

#ifndef FScriptH
#define FScriptH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "CSPIN.h"
#include <Buttons.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------

class CXSpriteDesigner;
//---------------------------------------------------------------------------

class TFormScript : public TForm
{
__published:	// IDE-managed Components
    TGroupBox *gbParams;
    TGroupBox *GroupBox1;
    TMemo *memoScript;
    TMemo *memoErrors;
    TButton *Button1;
    TOpenDialog *OpenDialog;
    void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
    CXSpriteDesigner* mSprite;

public:		// User declarations
    __fastcall TFormScript(TComponent* Owner);
    
    void __fastcall OnScriptStringEditChange(TObject *Sender);
    void __fastcall OnScriptFileBrowse(TObject *Sender);

    bool Init(CXSpriteDesigner* sprite);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormScript *FormScript;
//---------------------------------------------------------------------------
#endif
