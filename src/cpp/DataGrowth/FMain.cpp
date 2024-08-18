//---------------------------------------------------------------------------

#include <vcl.h>
#include <io.h>
#include <fcntl.h>
#include <iostream>
#include <conio.h>
#include <fstream>
#pragma hdrstop

using namespace std;

#include "easylogging++.h"

#include "FMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormMain *FormMain;
//---------------------------------------------------------------------------

__fastcall TFormMain::TFormMain(TComponent* Owner) : TForm(Owner)
{
  HandleCommandLine();

  SetFileName("");
  UpdateControls();

  Grid->Cells[0][0] = "Id";
  Grid->Cells[1][0] = "Position";
  Grid->Cells[2][0] = "Size";
}
//---------------------------------------------------------------------------

void TFormMain::UpdateControls()
{
  if (mFileForEdit == "")
  {
    GroupBox1->Enabled = false;
    Tool1->Enabled = false;
    Save1->Enabled = false;
    SaveAs1->Enabled = false;
  }
  else
  {
    GroupBox1->Enabled = true;
    Tool1->Enabled = true;
    Save1->Enabled = true;
    SaveAs1->Enabled = true;
  }
}
//---------------------------------------------------------------------------

void TFormMain::UpdateList()
{
  Grid->RowCount  = mDataFile.GetEntryCount() + 1;

  if (Grid->RowCount == 1)
  {
    Grid->RowCount = 2;
  }

  Grid->FixedRows = 1;

  int lTotalSize = 0;

  for(int i = 0; i < mDataFile.GetEntryCount(); i++)
  {
    CDataEntry* lData = mDataFile.GetEntry(i);

    //Memo->Lines->Add(AnsiString("[") + AnsiString(lData->GetId().c_str()) + AnsiString("] - ") + "[location: " + AnsiString(lData->GetLocation()) + "] - [size: " + AnsiString(lData->GetDataSize()) + "]");
    Grid->Cells[0][i + 1] = AnsiString(lData->GetId().c_str());
    Grid->Cells[1][i + 1] = AnsiString(lData->GetLocation());
    Grid->Cells[2][i + 1] = AnsiString(lData->GetDataSize());

    lTotalSize += lData->GetDataSize();
  }

  StatusBar1->SimpleText = AnsiString("Data file size: ") + AnsiString(lTotalSize);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::NewDataFile1Click(TObject *Sender)
{
  mDataFile.DeleteAllEntries();
  SetFileName("noname.data");
  UpdateControls();
  UpdateList();
}
//---------------------------------------------------------------------------

void TFormMain::SetFileName(AnsiString _v)
{
  mFileForEdit = _v;

  if (_v != "")
  {
    FormMain->Caption = AnsiString("Data Growth Tool - ") + mFileForEdit;
  }
  else
  {
    FormMain->Caption = AnsiString("Data Growth Tool");
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Open1Click(TObject *Sender)
{
  if (OpenDialog->Execute())
  {
    mDataFile.DeleteAllEntries();
    mDataFile.ReadFromFile(OpenDialog->FileName.c_str());
    SetFileName(OpenDialog->FileName);
    UpdateList();
    UpdateControls();
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Close1Click(TObject *Sender)
{
  mDataFile.DeleteAllEntries();
  mFileForEdit = "";
  UpdateControls();
  UpdateList();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Addfile1Click(TObject *Sender)
{
  // blabla
  if (OpenDataDialog->Execute())
  {
    StatusBar1->SimpleText = "Reading files...";

	// open each selected file
    for(int i = 0; i < OpenDataDialog->Files->Count; i++)
    {
      // open the file and read it in a char*
	  int lHandle, lFileLength;

	  lHandle = FileOpen(OpenDataDialog->Files->Strings[i], fmOpenRead);

      if (lHandle == -1)
	  {
		Application->MessageBox(L"Could not open the file, aborting", L"Warning", MB_OK);
        return;
      }

	  lFileLength = FileSeek(lHandle, 0, 2); // filelength(lHandle);

      FileSeek(lHandle, 0, 0);

	  char* lData;

      try
	  {
        lData = new char[lFileLength];
      }
      catch(...)
      {
		Application->MessageBox(L"Could not allocate memory, aborting", L"Warning", MB_OK);
        return;
      }

	  if (FileRead(lHandle, lData, lFileLength) == -1)
      {
		Application->MessageBox(L"Could read file, aborting", L"Warning", MB_OK);

        delete lData;

        return;
      }

      FileClose(lHandle);


      CDataEntry* lEntry = new CDataEntry();

      lEntry->SetId(GetId(OpenDataDialog->Files->Strings[i]).c_str());
      lEntry->SetLocation(0);
      lEntry->SetData(lData, lFileLength);

      mDataFile.AddEntry(lEntry);
    }

    UpdateList();
  }
}
//---------------------------------------------------------------------------

AnsiString TFormMain::GetId(AnsiString _filename)
{
  return UpperCase(ExtractFileName(_filename)).SubString(0, 4);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Save1Click(TObject *Sender)
{
  if (mFileForEdit == "noname.data")
  {
    if (SaveDialog->Execute())
    {
	  mDataFile.WriteToFile(SaveDialog->FileName.c_str());
      SetFileName(SaveDialog->FileName);
    }
  }
  else
  {
    mDataFile.WriteToFile(mFileForEdit.c_str());
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SaveAs1Click(TObject *Sender)
{
  if (SaveDialog->Execute())
  {
    mDataFile.WriteToFile(SaveDialog->FileName.c_str());
    SetFileName(SaveDialog->FileName);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Deleteentry1Click(TObject *Sender)
{
  // get the selected string grids lines
  mDataFile.DeleteEntriesInRange(Grid->Selection.Top - 1, Grid->Selection.Bottom);

  UpdateList();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ReplaceEntry1Click(TObject *Sender)
{
  int x1 = Grid->Selection.Top;
  int x2 = Grid->Selection.Bottom;
  int x3 = x2 - x1 + 1;

  if (x3 == 1)
  {
    if (OpenDataDialog->Execute())
	{
      // open the file and read it in a char*
      int lHandle, lFileLength;

	  lHandle = FileOpen(OpenDataDialog->Files->Strings[0], fmOpenRead);

      if (lHandle == -1)
      {
		Application->MessageBox(L"Could not open the file, aborting", L"Warning", MB_OK);
		return;
	  }

	  lFileLength = FileSeek(lHandle, 0, 2);
	  FileSeek(lHandle, 0, 0);

	  char* lData;

	  try
	  {
		lData = new char[lFileLength];
	  }
	  catch(...)
	  {
		Application->MessageBox(L"Could not allocate memory, aborting", L"Warning", MB_OK);
        return;
      }

	  if (FileRead(lHandle, lData, lFileLength) == -1)
      {
        Application->MessageBox(L"Could read file, aborting", L"Warning", MB_OK);

        delete lData;

        return;
      }

      FileClose(lHandle);


      CDataEntry* lEntry = mDataFile.GetEntry(x1 - 1);

      lEntry->SetId(GetId(OpenDataDialog->Files->Strings[0]).c_str());
      lEntry->SetLocation(0);
      lEntry->SetData(lData, lFileLength);

      UpdateList();
    }
  }
}
//---------------------------------------------------------------------------

void TFormMain::PrintHelp()
{
  LOG(INFO) << "Command line usage for DataGrowth tool:";
  LOG(INFO) << "datagrowth inputbatchfile outputdatafile";
}
//---------------------------------------------------------------------------

void TFormMain::HandleCommandLine()
{
  el::Loggers::setVerboseLevel(5);

  VLOG(5) << "DataGrowth Tool";
  VLOG(5) << "---------------";

  // VLOG(5) << logTimeMark();

  // no command line usage
  if (ParamCount() == 0)
  {
    return;
  }

  if (ParamCount() != 2)
  {
	LOG(ERROR) << "Invalid number of command line arguments.";
	PrintHelp();

    MyTerminate();
    return;
  }

  UnicodeString inputFileName  = ParamStr(1);
  UnicodeString outputFileName = ParamStr(2);

  // check the two paths
  if (!FileExists(inputFileName))
  {
	LOG(ERROR) << "Input file does not exists -> " << inputFileName.c_str();

	MyTerminate();
	return;
  }

  // the data file
  CDataFile dataFile;

  // open the input file
  ifstream inFile;

  inFile.open(inputFileName.c_str(), ios_base::in);

  char line[2048];

  // the path to the input file is obtained
  // if an item in the input file does not have an
  // absolute path, then add the input file path
  // to the name

  VLOG(5) << GetCurrentDir().c_str();

  //ChDir(ExtractFilePath(inputFileName));

  if (!SetCurrentDir(ExtractFilePath(inputFileName)))
  {
	LOG(ERROR) << "Could not change dir.";

	MyTerminate();
	return;
  }

  while(inFile.getline(line, 1024))
  {
    // open the file and read it in a char*
    int lHandle, lFileLength;

    lHandle = open(line, O_RDONLY | O_BINARY);

    if (lHandle == -1)
    {
	  LOG(ERROR) << "Could not open file -> " << line;

	  MyTerminate();
	  return;
	}

	lFileLength = filelength(lHandle);

	char* lData;

	try
	{
	  lData = new char[lFileLength];
	}
	catch(...)
	{
	  LOG(ERROR) << "Could not allocate memory";

	  MyTerminate();
	  return;
	}

	if (read(lHandle, lData, lFileLength) == -1)
	{
	  LOG(ERROR) << "Could not read file.";

	  delete lData;

	  MyTerminate();
	  return;
	}

	close(lHandle);


	CDataEntry* lEntry = new CDataEntry();

	//logWriteLn(ExtractFileName(line).c_str());

	lEntry->SetId(GetId(ExtractFileName(line).c_str()).c_str());
	lEntry->SetLocation(0);
	lEntry->SetData(lData, lFileLength);

	dataFile.AddEntry(lEntry);
  }

  dataFile.WriteToFile(outputFileName.c_str());

  MyTerminate();
}
//---------------------------------------------------------------------------

void TFormMain::MyTerminate()
{
  Application->Minimize();
  Application->Terminate();

  //while(true) {};
}
//---------------------------------------------------------------------------

