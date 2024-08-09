//---------------------------------------------------------------------------

#ifndef FVisualFactoryH
#define FVisualFactoryH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Curve2D.h"
//---------------------------------------------------------------------------

class TFormVisualFactory : public TForm
{
__published:	// IDE-managed Components
        TButton *btnOk;
        TButton *btnCancel;
        TComboBox *cbType;
        TLabel *Label1;
        void __fastcall btnOkClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFormVisualFactory(TComponent* Owner);

        Curve2D* GetObject();
};
//---------------------------------------------------------------------------
extern PACKAGE TFormVisualFactory *FormVisualFactory;
//---------------------------------------------------------------------------
#endif
