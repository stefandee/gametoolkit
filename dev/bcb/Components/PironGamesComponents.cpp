//---------------------------------------------------------------------------

#include <System.hpp>
#pragma hdrstop

#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP

#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Package source.
//---------------------------------------------------------------------------


#pragma argsused
extern "C" int _libmain(unsigned long reason)
{
	return 1;
}
//---------------------------------------------------------------------------
