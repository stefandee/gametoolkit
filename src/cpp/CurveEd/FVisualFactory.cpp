//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FVisualFactory.h"
#include "Curve2DFactory.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma resource "*.dfm"
TFormVisualFactory *FormVisualFactory;
//---------------------------------------------------------------------------

__fastcall TFormVisualFactory::TFormVisualFactory(TComponent* Owner)
        : TForm(Owner)
{
  cbType->ItemIndex = 0;
}
//---------------------------------------------------------------------------

void __fastcall TFormVisualFactory::btnOkClick(TObject *Sender)
{
  //
}
//---------------------------------------------------------------------------

Curve2D* TFormVisualFactory::GetObject()
{
  switch(cbType->ItemIndex)
  {
    case 0:
      return Curve2DFactory().Get("Curve2DLinear");

    case 1:
      return Curve2DFactory().Get("Curve2DElipsoid");

    case 2:
      return Curve2DFactory().Get("Curve2DScript");

    case 3:
      return Curve2DFactory().Get("Curve2DComposite");

    case 4:
      return Curve2DFactory().Get("Curve2DPolynome");
  }

  return 0;
}
//---------------------------------------------------------------------------
