//---------------------------------------------------------------------------

#pragma hdrstop

#include <iostream>
#include <conio.h>
#include <string>

#include "tinyxml.h"
#include "XmlToCsl.h"
//---------------------------------------------------------------------------

using std::wcout;
using std::cout;
using std::endl;
using std::string;
using std::wstring;
//---------------------------------------------------------------------------

#pragma argsused

void printHeader()
{
  wcout << L"StringTool - A Localization Manager" << endl;
  wcout << L"Part of the Lethal Metal project - http://sourceforge.net/projects/lethalmetal" << endl;
  wcout << L"(c) 2006 Stefan Dicu/Piron Games" << endl << endl;
}
//---------------------------------------------------------------------------

void printUsage()
{
  printHeader();

  wcout << endl << L"Usage: " << endl;
  wcout << L"StringTool -input fileName.xml [-output folder] -script filename.csl" << endl;
}
//---------------------------------------------------------------------------

//int main(int argc, char* argv[])
int _tmain(int argc, _TCHAR* argv[])
{
    // test when normally running the program
    if (argc == 1)
	{
      printUsage();
      exit(EXIT_FAILURE);
    }

	std::wstring inputFile, scriptFile, outputDir;

	for(int i = 1; i < argc; i++)
    {
	  if (wstring(argv[i]) == wstring(L"-input"))
      {
        if (i < argc - 1)
        {
		  inputFile = argv[i + 1];
          i+=1;
          continue;
        }
        else
        {
          printUsage();
          exit(EXIT_FAILURE);
        }
	  }


	  if (wstring(argv[i]) == wstring(L"-script"))
	  {
		if (i < argc - 1)
		{
		  scriptFile = argv[i + 1];
		  i+=1;
		  continue;
		}
		else
		{
		  printUsage();
		  exit(EXIT_FAILURE);
		}
	  }

	  if (wstring(argv[i]) == wstring(L"-output"))
      {
        if (i < argc - 1)
        {
          outputDir = argv[i + 1];
          i+=1;
          continue;
        }
        else
		{
          printUsage();
          exit(EXIT_FAILURE);
        }
      }

      // unknown option
      printUsage();
      exit(EXIT_FAILURE);
    }

    printHeader();

	wcout << "input file: " << inputFile << endl;
	wcout << "script file: " << scriptFile << endl;

    CXmlToCsl converter;

    if (!converter.Run(inputFile, outputDir, scriptFile))
    {
      wcout << L"Fatal: " << converter.ErrorDesc() << endl;
      exit(EXIT_FAILURE);
      //getch();
    }

    //getch();
    return 0;
}
//---------------------------------------------------------------------------

