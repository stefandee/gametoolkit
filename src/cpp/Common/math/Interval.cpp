//---------------------------------------------------------------------------

#pragma hdrstop

#include "Interval.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

void Interval::Serialize(PP::Stream &a)
{
  a.writeDouble(&mMin);
  a.writeDouble(&mMax);
}
//---------------------------------------------------------------------------

void Interval::DeSerialize(PP::Stream &a)
{
  a.readDouble(&mMin);
  a.readDouble(&mMax);
}
//---------------------------------------------------------------------------


