//-----------------------------------------------------------------------------
//  Unit              :
//
//  Version           :
//
//  Description       :
//    *
//  Hystory           :
//    [] - [] -
//-----------------------------------------------------------------------------

#ifndef ZFileLibH
#define ZFileLibH
//---------------------------------------------------------------------------

#include "ZLibrary.h"
#include "GenericCallback.h"

const int FILELIB_INVALID_HANDLE = -1;
const int FILELIB_PARAM_INT    = 0;
const int FILELIB_PARAM_DOUBLE = 1;
const int FILELIB_PARAM_BYTE   = 2;

class ZFileLib : public ZLibrary
{
  public:
   ZFileLib(                    // constructor
      ZCsl* _Parent,            // parent
      const ZString& _Name      // library name
      //ZLibrary* _Prev         // previous library
      );

   virtual void InitLibrary();

  private:
    // callback handlers
    ZCsl_callback_t mHandleFileOpen;
    ZCsl_callback_t mHandleFileCreate;
    ZCsl_callback_t mHandleFileClose;
    ZCsl_callback_t mHandleFileRead;
    ZCsl_callback_t mHandleFileWrite;
    ZCsl_callback_t mHandleFileWriteParam;
    ZCsl_callback_t mHandleFileSeek;
    ZCsl_callback_t mHandleFilePos;
    ZCsl_callback_t mHandleFileExists;

    // the actual implementation
    ZString FileOpen(ZCsl* csl);
    ZString FileCreate(ZCsl* csl);
    ZString FileClose(ZCsl* csl);
    ZString FileRead(ZCsl* csl);
    ZString FileWrite(ZCsl* csl);
    ZString FileWriteParam(ZCsl* csl);
    ZString FileSeek(ZCsl* csl);
    ZString FilePos(ZCsl* csl);
    ZString FileExists(ZCsl* csl);
};

#endif

