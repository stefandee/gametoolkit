//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TestBenchMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "StringGridX"
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------


void __fastcall TForm1::StringGridX1PopMoveUp(TObject *Sender)
{
  Application->MessageBox("hello from clone move up", "bla");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::StringGridX1PopClone(TObject *Sender)
{
  Application->MessageBox("hello from clone pop", "bla");
}
//---------------------------------------------------------------------------

