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
  TResourceStream *Stream = new TResourceStream((int)HInstance, "Notes", RT_RCDATA);

  __try {
    TStringList *List = new TStringList;
    __try {
      List->LoadFromStream(Stream);
      MemoInfo->Lines = List;
    }
    __finally {
      List->DisposeOf();
    }
  }
  __finally {
    Stream->DisposeOf();
  }

  //MemoInfo->Lines->LoadFromFile("Sentry.notes");
}
//---------------------------------------------------------------------------


