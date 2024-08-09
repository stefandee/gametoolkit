//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USEFORM("..\..\..\src\cpp\xsprite\FMain.cpp", FormMain);
USEFORM("..\..\..\src\cpp\xsprite\FName.cpp", FormName);
USEFORM("..\..\..\src\cpp\xsprite\FActions.cpp", FormActions);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TFormMain), &FormMain);
                 Application->CreateForm(__classid(TFormName), &FormName);
                 Application->CreateForm(__classid(TFormActions), &FormActions);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------
