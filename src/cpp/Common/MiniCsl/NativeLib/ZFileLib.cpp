#include <fcntl.h>
#include <sys/stat.h>
#include <io.h>
#pragma hdrstop
//---------------------------------------------------------------------------

#pragma hdrstop
#include "Platform.h"
#include "ZFileLib.h"
#include "ZCsl.hpp"
#include "PP_Stdlib.h"
#include "ZBase.hpp"

#ifndef PPT_COMPILER_MICROSOFT
#include <sstream>
#else
#include <strstream>	// TMN: Note use of REALLY OLD header !!!
#endif

using namespace std;

//---------------------------------------------------------------------------
#ifdef PPT_COMPILER_BORLAND
#pragma package(smart_init)
#endif

ZFileLib::ZFileLib(ZCsl* _Parent, const ZString& _Name/*, ZLibrary* _Prev*/) : ZLibrary(_Parent, _Name/*, _Prev*/)
{
  //mNative = true;
}
//---------------------------------------------------------------------------

void ZFileLib::InitLibrary()
{
   ZString iModule("ZFileLib");
   // this is native library - no external dependencies
   ZString init(ZString(
      "const fileVersion = '1.0.0';\n"
      "const fileCompiler = '") + PPT_COMPILER +"';\n"
      "const fileLibtype = 'NATIVE';\n"
      "const fileBuilt = '"+ZString(__DATE__)+" "+__TIME__+"';\n"
      "const O_RDONLY = " + ZString(O_RDONLY) + ";\n"
      "const O_WRONLY = " + ZString(O_WRONLY) + ";\n"
      "const O_RDWR   = " + ZString(O_RDWR) + ";\n"
      "const O_APPEND = " + ZString(O_APPEND) + ";\n"
      "const O_CREAT = " + ZString(O_CREAT) + ";\n"
      "const O_TRUNC = " + ZString(O_TRUNC) + ";\n"
      "const O_EXCL = " + ZString(O_EXCL) + ";\n"
      "const O_BINARY = " + ZString(O_BINARY) + ";\n"
      "const O_TEXT = " + ZString(O_TEXT) + ";\n"
      "const SEEK_CUR = " + ZString(SEEK_CUR) + ";\n"
      "const SEEK_END = " + ZString(SEEK_END) + ";\n"
      "const SEEK_SET = " + ZString(SEEK_SET) + ";\n"
      "const S_IWRITE = " + ZString(S_IWRITE) + ";\n"
      "const S_IREAD = " + ZString(S_IREAD) + ";\n"
      "const FILELIB_PARAM_INT = " + ZString(FILELIB_PARAM_INT) + ";\n"
      "const FILELIB_PARAM_FLOAT = " + ZString(FILELIB_PARAM_DOUBLE) + ";\n"
      "const FILELIB_PARAM_BYTE = " + ZString(FILELIB_PARAM_BYTE) + ";\n"
      "const FILELIB_INVALID_HANDLE = " + ZString(FILELIB_INVALID_HANDLE) + ";\n"
   );

#if PPT_COMPILER_MICROSOFT
   std::istrstream str((char*)init);
#else
   std::istringstream str((char *) init);
#endif

   GetParent()->loadScript(iModule, &str);

   // handler assignment
   mHandleFileOpen    .set(this, &ZFileLib::FileOpen);
   mHandleFileCreate  .set(this, &ZFileLib::FileCreate);
   mHandleFileClose   .set(this, &ZFileLib::FileClose);
   mHandleFileRead    .set(this, &ZFileLib::FileRead);
   mHandleFileWrite   .set(this, &ZFileLib::FileWrite);
   mHandleFileWriteParam .set(this, &ZFileLib::FileWriteParam);
   mHandleFileSeek    .set(this, &ZFileLib::FileSeek);
   mHandleFilePos     .set(this, &ZFileLib::FilePos);
   mHandleFileExists  .set(this, &ZFileLib::FileExists);

   // add functions
   GetParent()->addFunc(iModule, "FileOpen(const fileName, const access, const mode)", mHandleFileOpen);
   GetParent()->addFunc(iModule, "FileCreate(const fileName, const mode)", mHandleFileCreate);
   GetParent()->addFunc(iModule, "FileClose(const handle)", mHandleFileClose);
   //GetParent()->addFunc(iModule, "FileRead(const handle, var buffer, const length )", mHandleFileRead);
   GetParent()->addFunc(iModule, "FileWrite(const handle, const buffer)", mHandleFileWrite);
   GetParent()->addFunc(iModule, "FileWriteParam(const handle, const buffer, const what)", mHandleFileWriteParam);
   GetParent()->addFunc(iModule, "FileSeek(const handle, const offset, const fromwhere)", mHandleFileSeek);
   GetParent()->addFunc(iModule, "FilePos(const handle)", mHandleFilePos);
   GetParent()->addFunc(iModule, "FileExists(const fileName)", mHandleFileExists);
}
//---------------------------------------------------------------------------

ZString ZFileLib::FileOpen(ZCsl* csl)
{
  ZString fileName = csl->get("fileName");
  int access       = csl->get("access").asInt();
  int mode         = csl->get("mode").asInt();

  int handle = open(fileName.buffer(), access, mode);

  //cout << handle << endl;
  return handle;
}
//---------------------------------------------------------------------------

ZString ZFileLib::FileCreate(ZCsl* csl)
{
  // TODO: implement me
  return "-1";
}
//---------------------------------------------------------------------------

ZString ZFileLib::FileClose(ZCsl* csl)
{
  int handle = csl->get("handle").asInt();

  return close(handle);
}
//---------------------------------------------------------------------------

ZString ZFileLib::FileRead(ZCsl* csl)
{
  // TODO: implement me
  return "-1";
}
//---------------------------------------------------------------------------

ZString ZFileLib::FileWrite(ZCsl* csl)
{
  int  handle    = csl->get("handle").asInt();
  ZString buffer    = csl->get("buffer");

  //cout << handle << "     " << buffer.buffer() << endl;

  return write(handle, buffer.buffer(), buffer.size());
}
//---------------------------------------------------------------------------

ZString ZFileLib::FileWriteParam(ZCsl* csl)
{
  int  handle = csl->get("handle").asInt();
  int  what   = csl->get("what").asInt();

  switch (what)
  {
    case FILELIB_PARAM_INT:
    {
        int buffer = csl->get("buffer").asInt();
        return write(handle, &buffer, sizeof(buffer));
    }

    case FILELIB_PARAM_DOUBLE:
    {
        double buffer = csl->get("buffer").asDouble();
        return write(handle, &buffer, sizeof(buffer));
    }

    case FILELIB_PARAM_BYTE:
    {
        char buffer = (char)csl->get("buffer").asInt();
        return write(handle, &buffer, sizeof(buffer));
    }
  }

  return -1;
}
//---------------------------------------------------------------------------

ZString ZFileLib::FileSeek(ZCsl* csl)
{
  int  handle    = csl->get("handle").asInt();
  long offset    = csl->get("offset").asInt();
  int  fromwhere = csl->get("fromwhere").asInt();

  return lseek(handle, offset, fromwhere);
}
//---------------------------------------------------------------------------

ZString ZFileLib::FilePos(ZCsl* csl)
{
  int  handle = csl->get("handle").asInt();

  return lseek(handle, 0, SEEK_CUR);
}
//---------------------------------------------------------------------------

ZString ZFileLib::FileExists(ZCsl* csl)
{
  // TODO: implement me
  return "-1";
}
//---------------------------------------------------------------------------

