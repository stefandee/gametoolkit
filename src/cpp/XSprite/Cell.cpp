//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Cell.h"

//---------------------------------------------------------------------------

CCell::CCell()
{
  mPoint.x = -1;
  mPoint.y = -1;
  mRect.left = mRect.bottom = mRect.right = mRect.top;
  mName = "newcell"; 
}
//---------------------------------------------------------------------------

#pragma package(smart_init)
