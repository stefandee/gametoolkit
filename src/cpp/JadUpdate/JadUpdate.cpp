//---------------------------------------------------------------------------

#include <vcl.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <tchar.h>
#pragma hdrstop
//---------------------------------------------------------------------------

#include "Jad.h"

using std::cout;
using std::endl;
using std::string;

#pragma argsused

void PrintInfo()
{
  cout << "JadUpdate tool" << endl << "(c) 2003-2005 Karg" << endl << "Part of the Lethal Metal Project" << endl;
}

void PrintHelp()
{
  cout << endl << "Usage: " << endl;
  cout << "JadUpdate -jad jadfile|manifest -v version [-help]" << endl << endl;
  cout << "-jad jadfile\tthe jad file to update" << endl;
  cout << "-v version\tupdate the MIDlet-Version with version value" << endl;
  cout << "-help\t prints this help" << endl;
}

void ExitWithError(const char* errorInfo)
{
   cout << errorInfo << endl << endl;

   PrintHelp();

   exit(EXIT_FAILURE);
}

AnsiString gJadFile;
AnsiString gVersion;

//int main(int argc, char* argv[])
int _tmain(int argc, _TCHAR* argv[])
{
  PrintInfo();

  if (argc <= 1)
  {
    PrintHelp();
    return 0;
  }

  bool hasJad = false;
  bool hasVersion = false;

  for(int i = 1; i < argc; i++)
  {
     if (AnsiString(argv[i]) == AnsiString("-help"))
     {
       PrintHelp();
       return 0;
     }

     if (AnsiString(argv[i]) == AnsiString("-jad"))
     {
       if (!hasJad)
       {
         if (i >= argc - 1)
         {
           ExitWithError("Missing jad file in option.");
         }

         i++;
         gJadFile = argv[i];
         hasJad = true;
         continue;
       }
       else
       {
         ExitWithError("Wrong command line.");
       }
     }

     if (AnsiString(argv[i]) == AnsiString("-v"))
     {
       if (!hasVersion)
       {
         if (i >= argc - 1)
         {
           ExitWithError("Missing version in option.");
         }

         i++;
         gVersion   = argv[i];
         hasVersion = true;
         continue;
       }
       else
       {
         ExitWithError("Wrong command line.");
       }
     }

     ExitWithError((AnsiString(argv[i]) + AnsiString(" option is not recognized.")).c_str());
  }

  if (!hasJad)
  {
     ExitWithError("Really need a jad file.");
  }

  if (!hasVersion)
  {
     ExitWithError("Need a property to update.");
  }

  Jad jad;

  if (!jad.Open(gJadFile))
  {
    cout << "Cannot open the jad file." << endl;
    exit(EXIT_FAILURE);
  }

  if (!jad.Update(AnsiString("MIDlet-Version"), gVersion))
  {
    cout << "Jad file does not contain the specified property." << endl;
    exit(EXIT_FAILURE);
  }

  jad.Flush();

  //cout << "Working with " << gJadFile.c_str() << endl;
  //cout << "Version " << gVersion.c_str() << endl;

  //getch();
  return 0;
}
//---------------------------------------------------------------------------
