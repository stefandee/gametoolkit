//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FName.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormName *FormName;
//---------------------------------------------------------------------------

__fastcall TFormName::TFormName(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

UnicodeString TFormName::GetText()
{
  TReplaceFlags lFlags;

  lFlags << rfReplaceAll;

  return StringReplace(edName->Text, " ", "", lFlags);
}
//---------------------------------------------------------------------------

void TFormName::SetText(UnicodeString _v)
{
  TReplaceFlags lFlags;

  lFlags << rfReplaceAll;
  
  edName->Text = StringReplace(_v, " ", "", lFlags);
}
//---------------------------------------------------------------------------
