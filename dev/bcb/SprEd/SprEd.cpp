//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>

#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP

//---------------------------------------------------------------------------
USEFORM("..\..\..\src\cpp\SprEd\cell.cpp", CellForm);
USEFORM("..\..\..\src\cpp\SprEd\SpecReplace.cpp", FormReplace);
USEFORM("..\..\..\src\cpp\SprEd\newbmp.cpp", NewForm);
USEFORM("..\..\..\src\cpp\SprEd\m_spred.cpp", MainForm);
USEFORM("..\..\..\src\cpp\SprEd\FAbout.cpp", FormAbout);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->CreateForm(__classid(TMainForm), &MainForm);
		Application->CreateForm(__classid(TCellForm), &CellForm);
		Application->CreateForm(__classid(TFormAbout), &FormAbout);
		Application->CreateForm(__classid(TNewForm), &NewForm);
		Application->CreateForm(__classid(TFormReplace), &FormReplace);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
