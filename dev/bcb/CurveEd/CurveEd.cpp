//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>

#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP

//---------------------------------------------------------------------------
USEFORM("..\..\..\src\cpp\CurveEd\FVisualFactory.cpp", FormVisualFactory);
USEFORM("..\..\..\src\cpp\CurveEd\FEditCurveStream.cpp", FormEditObjectStream);
USEFORM("..\..\..\src\cpp\CurveEd\FEditCurve2D.cpp", FormEditCurve2D);
USEFORM("..\..\..\src\cpp\CurveEd\FAbout.cpp", FormAbout);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->CreateForm(__classid(TFormEditObjectStream), &FormEditObjectStream);
		Application->CreateForm(__classid(TFormAbout), &FormAbout);
		Application->CreateForm(__classid(TFormEditCurve2D), &FormEditCurve2D);
		Application->CreateForm(__classid(TFormVisualFactory), &FormVisualFactory);
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
