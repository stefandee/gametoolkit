//---------------------------------------------------------------------------

#ifndef FNameH
#define FNameH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TFormName : public TForm
{
__published:	// IDE-managed Components
        TEdit *edName;
        TLabel *Label1;
        TButton *btnOk;
        TButton *btnCancel;
private:	// User declarations
public:		// User declarations
        __fastcall TFormName(TComponent* Owner);
        AnsiString GetText();
        void SetText(AnsiString _v);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormName *FormName;
//---------------------------------------------------------------------------
#endif
