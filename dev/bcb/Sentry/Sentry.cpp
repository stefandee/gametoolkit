//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("..\..\..\src\cpp\Sentry\FMain.cpp", FormMain);
USEFORM("..\..\..\src\cpp\Sentry\FOptions.cpp", FormOptions);
USEFORM("..\..\..\src\cpp\Sentry\FScript.cpp", FormScript);
USEFORM("..\..\..\src\cpp\Sentry\FAbout.cpp", FormAbout);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
    try
    {
         Application->Initialize();
         Application->MainFormOnTaskBar = true;
         Application->CreateForm(__classid(TFormMain), &FormMain);
         Application->CreateForm(__classid(TFormAbout), &FormAbout);
         Application->CreateForm(__classid(TFormOptions), &FormOptions);
         Application->CreateForm(__classid(TFormScript), &FormScript);
         Application->CreateForm(__classid(TFormMain), &FormMain);
         Application->CreateForm(__classid(TFormOptions), &FormOptions);
         Application->CreateForm(__classid(TFormScript), &FormScript);
         Application->CreateForm(__classid(TFormAbout), &FormAbout);
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
