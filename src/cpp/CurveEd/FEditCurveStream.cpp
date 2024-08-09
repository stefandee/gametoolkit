//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FEditCurveStream.h"
#include "FAbout.h"
#include "FVisualFactory.h"
#include "FEditCurve2D.h"
#include "Curve2DFactory.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma resource "*.dfm"
TFormEditObjectStream *FormEditObjectStream;
//---------------------------------------------------------------------------

__fastcall TFormEditObjectStream::TFormEditObjectStream(TComponent* Owner)
        : TForm(Owner)
{
  mObjects.clear();

  SetupStringList();

  SetState(SEDIT_NOT_OPENED);

  mFileHistory = new CFileHistory(OpenRecent1, &HistoryClick, HISTORY_MENU_MAX_ITEMS);
}
//---------------------------------------------------------------------------

void __fastcall TFormEditObjectStream::HistoryClick(TObject *Sender)
{
  AnsiString lFileName = ((TMenuItem*)Sender)->Caption;

  try
  {
  }
  catch(...)
  {
  }

  SetState(SEDIT_NOT_MODIFIED);
  SetFileName(lFileName);
}
//---------------------------------------------------------------------------

void __fastcall TFormEditObjectStream::About2Click(TObject *Sender)
{
  FormAbout->ShowModal();
}
//---------------------------------------------------------------------------

void TFormEditObjectStream::SetState(TStreamEditState _v)
{
  if ((mState == SEDIT_EMPTY || mState == SEDIT_REQUEST_FILENAME) && (_v == SEDIT_MODIFIED))
  {
    mState = SEDIT_REQUEST_FILENAME;
  }
  else
  {
    mState = _v;
  }  

  switch(mState)
  {
    case SEDIT_NOT_OPENED:
      ClearStringGrid();
      StringGrid->Enabled = false;
      Save1->Enabled      = false;
      SaveAs2->Enabled    = false;
      break;

    case SEDIT_MODIFIED:
    case SEDIT_REQUEST_FILENAME:
      StringGrid->Enabled    = true;
      Save1->Enabled         = true;
      SaveAs2->Enabled       = true;
      RemoveRecord1->Enabled = true;
      Clear1->Enabled        = true;
      Edit1->Enabled         = true;
      StatusBar->Panels->Items[0]->Text = "Modified";
      break;

    case SEDIT_NOT_MODIFIED:
      StringGrid->Enabled    = true;
      Save1->Enabled         = false;
      SaveAs2->Enabled       = true;
      RemoveRecord1->Enabled = true;
      Clear1->Enabled        = true;
      Edit1->Enabled         = true;
      StatusBar->Panels->Items[0]->Text = "Not modified";
      break;

    case SEDIT_EMPTY:
      StringGrid->Enabled    = true;
      Save1->Enabled         = true;
      SaveAs2->Enabled       = true;
      RemoveRecord1->Enabled = false;
      Clear1->Enabled        = false;
      Edit1->Enabled         = false;
      break;
  };
}
//---------------------------------------------------------------------------

TStreamEditState TFormEditObjectStream::GetState()
{
  return mState;
}
//---------------------------------------------------------------------------

// Description : deletes all the objects in mObjects and frees the memory
// Param       :
// Result      :
// Comments    :
void TFormEditObjectStream::ClearObjects()
{
  for(unsigned int i = 0; i < mObjects.size(); i++)
  {
    delete mObjects[i];
  }

  mObjects.clear();
}
//---------------------------------------------------------------------------

void TFormEditObjectStream::ClearStringGrid()
{
  for(unsigned int i = 1; i <= StringGrid->RowCount; i++)
  {
    StringGrid->Rows[i]->Clear();
  }
  
  StringGrid->RowCount  = 2;
  StringGrid->FixedRows = 1;
}
//---------------------------------------------------------------------------

void TFormEditObjectStream::SetupStringList()
{
  ClearStringGrid();

  StringGrid->ColCount    = 3;
  StringGrid->Cells[0][0] = "No.";
  StringGrid->Cells[1][0] = "Name";
  StringGrid->Cells[2][0] = "Type";

  for(int i = 0; i < mObjects.size(); i++)
  {
    StringGrid->Cells[0][i + 1] = AnsiString(i + 1);
    StringGrid->Cells[1][i + 1] = mObjects[i]->GetName().c_str();
    StringGrid->Cells[2][i + 1] = mObjects[i]->ClassName().c_str();

	StringGrid->RowCount++;
  }

  if (mObjects.size() > 0)
  {
    StringGrid->RowCount--;
  }

  //if (lRowCount > StringGrid->Row)
  //{
  //  StringGrid->Row = StringGrid->RowCount;
  //}
}
//---------------------------------------------------------------------------

void __fastcall TFormEditObjectStream::New1Click(TObject *Sender)
{
  if (mState == SEDIT_MODIFIED || mState == SEDIT_REQUEST_FILENAME)
  {
    // ask for saving
    int lResult = Application->MessageBox(L"List is modified. Save before exit?", L"Question", MB_YESNOCANCEL);

    if (lResult == IDCANCEL)
    {
      return;
    }

    if (lResult == IDYES)
    {
      // save
    }
  }

  // create a new list - erase the old list
  ClearObjects();
  SetupStringList();

  SetFileName(NEW_FILE_NAME);

  SetState(SEDIT_EMPTY);
}
//---------------------------------------------------------------------------

void __fastcall TFormEditObjectStream::Exit1Click(TObject *Sender)
{
  if (mState == SEDIT_MODIFIED || mState == SEDIT_REQUEST_FILENAME)
  {
    // ask for saving
    int lResult = Application->MessageBox(L"List is modified. Save before exit?", L"Question", MB_YESNOCANCEL);

    if (lResult == IDCANCEL)
    {
      return;
    }

    if (lResult == IDYES)
    {
      // save
      Save1Click(this);
    }
  }

  Application->Terminate();
}
//---------------------------------------------------------------------------

void __fastcall TFormEditObjectStream::AddRecord1Click(TObject *Sender)
{
  if (FormVisualFactory->ShowModal() == mrOk)
  {
    // get the edited object and add it in the vector
    mObjects.push_back(FormVisualFactory->GetObject());

    SetState(SEDIT_MODIFIED);
    SetupStringList();
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormEditObjectStream::InsertRecord1Click(TObject *Sender)
{
  if (FormVisualFactory->ShowModal() == mrOk)
  {
    std::vector<Curve2D*>::iterator it = mObjects.begin();

    it += StringGrid->Row - 1;

    // get the edited object and add it in the vector
    mObjects.insert(it, FormVisualFactory->GetObject());

    SetState(SEDIT_MODIFIED);
    SetupStringList();
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormEditObjectStream::RemoveRecord1Click(TObject *Sender)
{
  if (mState == SEDIT_EMPTY)
  {
    return;
  }
  
  delete mObjects[StringGrid->Row - 1];

  std::vector<Curve2D*>::iterator it = mObjects.begin();

  it += StringGrid->Row - 1;

  mObjects.erase(it);

  if (mObjects.size() == 0)
  {
    SetState(SEDIT_EMPTY);
  }
  else
  {
    SetState(SEDIT_MODIFIED);
  }

  SetupStringList();
}
//---------------------------------------------------------------------------

void __fastcall TFormEditObjectStream::Clear1Click(TObject *Sender)
{
  ClearObjects();

  SetState(SEDIT_MODIFIED);
}
//---------------------------------------------------------------------------

void __fastcall TFormEditObjectStream::Edit1Click(TObject *Sender)
{
  // popup the editor for individual objects
  // this may be the place to have a factory so that we may popup
  // a edit form based on the class id
  if (mState == SEDIT_EMPTY)
  {
    return;
  }

  int objectIndex = StringGrid->Row - 1;

  Curve2D* curve = mObjects[StringGrid->Row - 1];

  FormEditCurve2D->SetCurve(mObjects[StringGrid->Row - 1]->Clone());

  if (FormEditCurve2D->ShowModal() == mrOk)
  {
    delete mObjects[StringGrid->Row - 1];

    mObjects[StringGrid->Row - 1] = FormEditCurve2D->GetCurve();

    SetupStringList();
    SetState(SEDIT_MODIFIED);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormEditObjectStream::Open1Click(TObject *Sender)
{
  if (mState == SEDIT_MODIFIED)
  {
    // ask for saving
    int lResult = Application->MessageBox(L"List is modified. Save?", L"Question", MB_YESNOCANCEL);

    if (lResult == IDCANCEL)
    {
      return;
    }

    if (lResult == IDYES)
    {
      // save
      Save1Click(this);
    }
  }

  // will load the object stream from a file
  if (!OpenDialog->Execute())
  {
    return;
  }

  Load(OpenDialog->FileName);
}
//---------------------------------------------------------------------------

void __fastcall TFormEditObjectStream::Load1Click(TObject *Sender)
{
  Open1Click(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormEditObjectStream::Save1Click(TObject *Sender)
{
  Save2Click(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormEditObjectStream::Save2Click(TObject *Sender)
{
  if (mState == SEDIT_EMPTY || mState == SEDIT_REQUEST_FILENAME)
  {
    if (SaveDialog->Execute())
    {
      SetFileName(SaveDialog->FileName);
    }
    else
    {
      return;
    }
  }

  Save(GetFileName());
}
//---------------------------------------------------------------------------

void __fastcall TFormEditObjectStream::SaveAs2Click(TObject *Sender)
{
  SaveDialog->FileName = ExtractFilePath(GetFileName()) + "Copy of " + ExtractFileName(GetFileName());

  if (!SaveDialog->Execute())
  {
    return;
  }

  SetFileName(SaveDialog->FileName);
  Save(GetFileName());
}
//---------------------------------------------------------------------------

void TFormEditObjectStream::SetFileName(AnsiString _fileName)
{
  mFileName = _fileName;

  StatusBar->Panels->Items[1]->Text = mFileName;
}
//---------------------------------------------------------------------------

AnsiString TFormEditObjectStream::GetFileName()
{
  return mFileName;
}
//---------------------------------------------------------------------------

void TFormEditObjectStream::Save(AnsiString _fileName)
{
  try
  {
    PP::Stream_ANSI_C lA(CPString(_fileName.c_str()), true);

    ObjectStream<Curve2D, Curve2DFactory> stream(lA);

    int lSize = mObjects.size();

    lA.writeInt(&lSize);

    for(int i = 0; i < lSize; i++)
    {
      CSerString(mObjects[i]->ClassName()).Serialize(lA);

      mObjects[i]->Serialize(lA);

      //stream.Set(mObjects[i]);
    }
    //ComputeCurve()->Serialize(lA);

    SetState(SEDIT_NOT_MODIFIED);
  }
  catch(...)
  {
    Application->MessageBox(L"Cannot save the curve", L"Error", IDOK);
  }
}
//---------------------------------------------------------------------------

void TFormEditObjectStream::Load(AnsiString _fileName)
{
  try
  {
    PP::Stream_ANSI_C lA(CPString(_fileName.c_str()));

    ObjectStream<Curve2D, Curve2DFactory> stream(lA);

    ClearObjects();

    for(int i = 0; i < stream.Count(); i++)
    {
      mObjects.push_back(stream.Get(i));
    }

    SetState(SEDIT_NOT_MODIFIED);
    SetupStringList();

    SetFileName(OpenDialog->FileName);

	mFileHistory->Add(OpenDialog->FileName.c_str());
  }
  catch(...)
  {
    Application->MessageBox(L"Cannot load the list!", L"Error", IDOK);
  }
}
//---------------------------------------------------------------------------

