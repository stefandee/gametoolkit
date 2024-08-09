//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FAbout.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "THttpLabel"
#pragma resource "*.dfm"
TFormAbout *FormAbout;
//---------------------------------------------------------------------------

__fastcall TFormAbout::TFormAbout(TComponent* Owner)
    : TForm(Owner)
{
  MemoInfo->Lines->LoadFromFile("Sentry.notes");
}
//---------------------------------------------------------------------------


