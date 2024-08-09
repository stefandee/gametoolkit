//---------------------------------------------------------------------------

#pragma hdrstop

#include "ZLibrary.h"
//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

ZLibrary::ZLibrary(ZCsl* _Parent, const ZString& _Name/*, ZLibrary* _Prev*/)
{
  mParent      = _Parent;
  mName        = _Name;
  mPrevLibrary = NULL;
  mNative      = true;
}
//---------------------------------------------------------------------------

void ZLibrary::InitLibrary()
{
  // place all your "mHandleThing = Thing" here
}
//---------------------------------------------------------------------------

ZLibrary::~ZLibrary()
{
  // de-allocation is done pretty strange, but this was the original CSL
  // convention
  if (mPrevLibrary) delete mPrevLibrary;
}
//---------------------------------------------------------------------------

