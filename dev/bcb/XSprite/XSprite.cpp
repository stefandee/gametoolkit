//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>

#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP
//---------------------------------------------------------------------------
USEFORM("..\..\..\src\cpp\XSprite\FMain.cpp", FormMain);
USEFORM("..\..\..\src\cpp\XSprite\FName.cpp", FormName);
USEFORM("..\..\..\src\cpp\XSprite\FActions.cpp", FormActions);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
    try
    {
         Application->Initialize();
         Application->MainFormOnTaskBar = true;
         Application->CreateForm(__classid(TFormMain), &FormMain);
         Application->CreateForm(__classid(TFormName), &FormName);
         Application->CreateForm(__classid(TFormActions), &FormActions);
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
