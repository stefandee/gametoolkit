//---------------------------------------------------------------------------


#pragma hdrstop

#include "TImgExEditorForm.h"
#pragma resource "TImgExEditorForm.dfm"

//---------------------------------------------------------------------------

#pragma package(smart_init)


//------------------------------------------------------------------------//
//                                                                        //
//                                                                        //
// clasa pentru editarea TBitmap'ului; am facut intii o forma (new        //
// application) si apoi am copiat headerul aici si sursa in TImageEx.cpp  //
//                                                                        //
//                                                                        //
//------------------------------------------------------------------------//
__fastcall TImgExEditorForm::TImgExEditorForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

__fastcall TImgExEditorForm::TImgExEditorForm(TComponent* AOwner, int Dummy) : TForm(AOwner, Dummy)
{
}
//---------------------------------------------------------------------------

__fastcall TImgExEditorForm::TImgExEditorForm(HWND ParentWindow) : TForm(ParentWindow)
{
}
//---------------------------------------------------------------------------

void __fastcall TImgExEditorForm::btnClearClick(TObject *Sender)
{
  ImageEx->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TImgExEditorForm::btnLoadClick(TObject *Sender)
{
  if (OpenImageEx->Execute())
  {
    ImageEx->Clear();

    try
    {
      ImageEx->Bitmap->LoadFromFile(OpenImageEx->FileName);
      ImageEx->Paint();
    }
    catch(...)
    {
      // just do nothing
      Application->MessageBox(L"Error", L"Cannot load image", MB_OK);
    }
  }
}
//---------------------------------------------------------------------------
