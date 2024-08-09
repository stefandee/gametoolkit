//---------------------------------------------------------------------------

#ifndef TImgExEditorFormH
#define TImgExEditorFormH
//---------------------------------------------------------------------------

#include <SysUtils.hpp>
#include <StdCtrls.hpp>
#include <Graphics.hpp>
#include <Forms.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <System.hpp>
#include "ImageEx.h"

// clasa pentru editarea TBitmap'ului; am facut intii o forma (new application) si
// apoi am copiat headerul aici si sursa in TImageEx.cpp
class TImgExEditorForm : public TForm
{
__published:
  TButton      *btnOk;
  TPanel       *pInner;
  TButton      *btnLoad;
  TPanel       *pOuter;
  TButton      *btnCancel;
  TButton      *btnClear;
  TImageEx     *ImageEx;
  TOpenImageEx *OpenImageEx;
  void __fastcall btnClearClick(TObject *Sender);
  void __fastcall btnLoadClick(TObject *Sender);
private:

public:
  __fastcall TImgExEditorForm(TComponent* Owner);
  __fastcall TImgExEditorForm(TComponent* AOwner, int Dummy);
  __fastcall TImgExEditorForm(HWND ParentWindow);
};

#endif
