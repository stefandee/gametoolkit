//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FScript.h"
#include "XSpriteDesigner.h"
#include "Script.h"
#include "CSPIN.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"
TFormScript *FormScript;
//---------------------------------------------------------------------------

__fastcall TFormScript::TFormScript(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

bool TFormScript::Init(CXSpriteDesigner* sprite)
{
  mSprite = sprite;

  if (!mSprite)
  {
    return false;
  }

  memoErrors->Lines->Clear();
  memoScript->Lines->Text = AnsiString(mSprite->GetScript().c_str());

  mSprite->InitScript();

  // remove all controls from the group
  while(gbParams->ControlCount > 0)
  {
    TControl* control = gbParams->Controls[0];

    control->Parent = 0;

    delete control;
  }

  // TODO: test code
  std::string paramsCountAsStr = mSprite->GetScriptVar(SCRIPT_VAR_PARAM_COUNT);

  if (paramsCountAsStr == "")
  {
    return false;
  }

  int paramsCount = atoi(paramsCountAsStr.c_str());

  int controlLeft = 4;
  int controlTop  = 28;

  for(int i = 0; i < paramsCount; i++)
  {
    std::string paramName         = mSprite->GetScriptParam(i, 0);
    std::string paramValue        = mSprite->GetScriptParam(i, 1);
    std::string paramDefaultValue = mSprite->GetScriptParam(i, 2);
    std::string paramType         = mSprite->GetScriptParam(i, 3);

    if (paramName == "" ||
        paramType == "" ||
        paramValue == "" ||
        paramDefaultValue == "")
    {
      continue;
    }

    if (paramType != "INT" &&
        paramType != "DOUBLE" &&
        paramType != "PATH" &&
        paramType != "FILENAME" &&
        paramType != "STRING" &&
        paramType != "ENUM")
    {
      continue;
    }

    TLabel* label  = new TLabel(gbParams);
    label->Left    = controlLeft;
    label->Top     = controlTop;
    label->Caption = (paramName.c_str());
    label->Parent  = gbParams;

    if (paramType == "INT")
    {
      TCSpinEdit* spinEdit = new TCSpinEdit(gbParams);
      spinEdit->Left   = controlLeft + 128;
      spinEdit->Top    = controlTop - 8;
      spinEdit->Width  = 120;
      spinEdit->Value  = atoi(paramDefaultValue.c_str());
      spinEdit->Parent = gbParams;
      spinEdit->Tag    = i;
    }

    if (paramType == "DOUBLE")
    {
      TEdit* edit = new TEdit(gbParams);
      edit->Left   = controlLeft + 128;
      edit->Top    = controlTop - 8;
      edit->Width  = 120;
      edit->Tag    = i;

      // TODO: verify it's really a float
      edit->Text   = paramDefaultValue.c_str();
      edit->Parent = gbParams;
    }

    if (paramType == "STRING")
    {
      TEdit* edit = new TEdit(gbParams);
      edit->Left   = controlLeft + 128;
      edit->Top    = controlTop - 8;
      edit->Width  = 120;
      edit->Text   = paramDefaultValue.c_str();
      edit->Parent = gbParams;
      edit->Tag    = i;
    }

    if (paramType == "PATH")
    {
      TEdit* edit    = new TEdit(gbParams);
      edit->Left     = controlLeft + 128;
      edit->Top      = controlTop - 8;
      edit->Width    = 120;
      edit->Text     = paramDefaultValue.c_str();
      edit->Parent   = gbParams;
      edit->Tag      = i;
      edit->OnExit = OnScriptStringEditChange;

      TSpeedButton* btn = new TSpeedButton(gbParams);
      btn->Left = controlLeft + 252;
      btn->Top  = controlTop - 8;
      btn->Caption = "...";
      btn->Parent = gbParams;
      //btn->OnClick = OnScriptPathBrowse;

      // TODO: attach a file dialog to the button then fill the edit with the path
    }

    if (paramType == "FILENAME")
    {
      TEdit* edit = new TEdit(gbParams);
      edit->Left   = controlLeft + 128;
      edit->Top    = controlTop - 8;
      edit->Width  = 120;
      edit->Text   = paramValue.c_str();
      edit->Parent = gbParams;
      edit->Tag    = i;
      edit->OnExit = OnScriptStringEditChange;

      TSpeedButton* btn = new TSpeedButton(gbParams);
      btn->Left = controlLeft + 252;
      btn->Top  = controlTop - 8;
      btn->Caption = "...";
      btn->Parent = gbParams;
      btn->OnClick = OnScriptFileBrowse;

      // TODO: attach a file dialog to the button then fill the edit with the file name
    }

    if (paramType == "ENUM")
    {
      TComboBox* combo = new TComboBox(gbParams);
      combo->Left      = controlLeft + 128;
      combo->Top       = controlTop - 8;
      combo->Width     = 120;
      combo->Parent    = gbParams;
      combo->Tag       = i;

      AnsiString items = paramDefaultValue.c_str();

      int pos = items.AnsiPos("|");

      while(pos != 0)
      {
        combo->Items->Add(items.SubString(1, pos - 1));

        items = items.SubString(pos + 1, items.Length() - pos + 1);

        pos = items.AnsiPos("|");
      }

      combo->Items->Add(items);

      combo->ItemIndex = 0;
      combo->Style = csDropDownList;
    }

    controlTop += 28;
  }

  return true;
}
//---------------------------------------------------------------------------

void __fastcall TFormScript::Button1Click(TObject *Sender)
{
  if (!mSprite)
  {
    return;
  }

  mSprite->ExecuteScript();

  memoErrors->Lines->Text = AnsiString(mSprite->GetScriptError().c_str());
}
//---------------------------------------------------------------------------

void __fastcall TFormScript::OnScriptStringEditChange(TObject *Sender)
{
  TEdit* edit = dynamic_cast<TEdit*>(Sender);

  if (edit)
  {
    mSprite->SetScriptParam(edit->Tag, 1, edit->Text.c_str());
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormScript::OnScriptFileBrowse(TObject *Sender)
{
  if (!mSprite)
  {
    return;
  }

  TSpeedButton* btn = dynamic_cast<TSpeedButton*>(Sender);

  if (!btn)
  {
    // TODO: drop an error message
    return;
  }

  TEdit* edit = 0;

  for(int i = 0; i < gbParams->ControlCount; i++)
  {
    edit = dynamic_cast<TEdit*>(gbParams->Controls[i]);

    if (edit)
    {
      if (edit->Tag == btn->Tag)
      {
        break;
      }
    }
  }

  if (!edit)
  {
    return;
  }

  OpenDialog->InitialDir = ExtractFileDir(AnsiString(edit->Text));
  
  if (OpenDialog->Execute())
  {
    mSprite->SetScriptParam(btn->Tag, 1, OpenDialog->FileName.c_str());
    
    edit->Text = OpenDialog->FileName;
  }
}
//---------------------------------------------------------------------------

