//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("..\..\..\src\cpp\Sentry\FMain.cpp", FormMain);
USEFORM("..\..\..\src\cpp\Sentry\FAbout.cpp", FormAbout);
USEFORM("..\..\..\src\cpp\Sentry\FOptions.cpp", FormOptions);
USEFORM("..\..\..\src\cpp\Sentry\FScript.cpp", FormScript);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
         Application->Initialize();
         Application->Title = "Sentry - Sprite Editor";
         Application->CreateForm(__classid(TFormMain), &FormMain);
         Application->CreateForm(__classid(TFormAbout), &FormAbout);
         Application->CreateForm(__classid(TFormOptions), &FormOptions);
         Application->CreateForm(__classid(TFormScript), &FormScript);
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

