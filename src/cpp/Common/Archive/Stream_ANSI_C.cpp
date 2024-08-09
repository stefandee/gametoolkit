#include <stdexcept>
#include <stdio.h>
#pragma hdrstop

// Stream_ANSI_C.cpp
#include "Stream_ANSI_C.h"
#include "Platform.h"
#include "BitOp.h"
#include "easylogging++.h"

namespace PP
{

class Stream_ANSI_C_impl
{
public:
    Stream_ANSI_C_impl(const char* filename, const char* mode)

    {
        mFile = fopen(filename, mode);

        if (!mFile)
        {
            //std::string sErr("Cannot open file: ");
            //sErr += filename;
            //throw std::invalid_argument(sErr);
            LOG(ERROR) << "Cannot open file: " << filename << " with mode " << mode;
        }
    }

    ~Stream_ANSI_C_impl()
    {
        fclose(mFile);
    }

    FILE* mFile;
};

Stream_ANSI_C::Stream_ANSI_C(const char* filename, bool _empty)
{
    CPString lFixedPath = FixPath(CPString(filename));

    //logWrite("opening: ");
    //logWriteLn(lFixedPath.c_str());

    if (_empty)
    {
        mPimpl = new Stream_ANSI_C_impl(lFixedPath.c_str(), "w+b");
    }
    else
    {
        mPimpl = new Stream_ANSI_C_impl(lFixedPath.c_str(), "r+b");
    }
}

Stream_ANSI_C::~Stream_ANSI_C()
{
    delete mPimpl;
}

size_t Stream_ANSI_C::read(void* pMem, size_t nBytes)
{
    return fread(pMem, 1, nBytes, mPimpl->mFile);
}

bool Stream_ANSI_C::rewind()
{
    ::rewind(mPimpl->mFile);
    return true;
}

bool Stream_ANSI_C::seek(size_t offset)
{
    return fseek(mPimpl->mFile, offset, SEEK_SET) == 0;
}

bool Stream_ANSI_C::skip(size_t size)
{
    return fseek(mPimpl->mFile, size, SEEK_CUR) == 0;
}

size_t Stream_ANSI_C::length()
{
    int lCurrentPos = ftell(mPimpl->mFile);
    int lFileSize   = fseek(mPimpl->mFile, 0, SEEK_END);

    fseek(mPimpl->mFile, lCurrentPos, SEEK_SET);

    return lFileSize;
}

int Stream_ANSI_C::tell()
{
    return ftell(mPimpl->mFile);
}

size_t Stream_ANSI_C::write(void* pMem, size_t nBytes)
{
    return fwrite(pMem, 1, nBytes, mPimpl->mFile);
}

// TODO: apply endianess transform to these functions
bool  Stream_ANSI_C::readInt   (int*    _v)
{
    bool lResult = fread(_v, 1, sizeof(int), mPimpl->mFile);;

#ifdef PPT_BIGENDIAN_IO
    *_v = Swap32(*_v);
#endif

    return lResult;
}
//---------------------------------------------------------------------------

bool  Stream_ANSI_C::readShort   (short*    _v)
{
    bool lResult = fread(_v, 1, sizeof(short), mPimpl->mFile);;

#ifdef PPT_BIGENDIAN_IO
    *_v = Swap16(*_v);
#endif

    return lResult;
}
//---------------------------------------------------------------------------
bool  Stream_ANSI_C::readDouble(double* _v)
{
    bool lResult = fread(_v, 1, sizeof(double), mPimpl->mFile);

    /* XXX: can not dynamic_cast double to type unsigned char
      #ifdef PPT_BIGENDIAN_IO
        SwapBytes(dynamic_cast<unsigned char*>(_v), sizeof(double));
      #endif
    */
    return lResult;
}
//---------------------------------------------------------------------------

bool  Stream_ANSI_C::readFloat (float*  _v)
{
    bool lResult = fread(_v, 1, sizeof(float), mPimpl->mFile);

#ifdef PPT_BIGENDIAN_IO
    int lInt;

    memcpy(&lInt, _v, 4);
    lInt = Swap32(lInt);
    memcpy(_v, &lInt, 4);
#endif

    return lResult;
}
//---------------------------------------------------------------------------

bool  Stream_ANSI_C::readBool  (bool*   _v)
{
    int lValue = 0, lResult;

    lResult = fread(&lValue, 1, sizeof(int), mPimpl->mFile);

#ifdef PPT_BIGENDIAN_IO
    lValue = Swap32(lValue);
#endif

    *_v = (lValue != 0);
    return lResult;
}
//---------------------------------------------------------------------------

bool  Stream_ANSI_C::writeInt   (int*    _v)
{
#ifdef PPT_BIGENDIAN_IO
    *_v = Swap32(*_v);
#endif

    return fwrite(_v, 1, sizeof(int), mPimpl->mFile);
}
//---------------------------------------------------------------------------

bool  Stream_ANSI_C::writeDouble(double* _v)
{
    /* XXX: Can not dynamic_cast double to unsigned char
      #ifdef PPT_BIGENDIAN_IO
        SwapBytes(dynamic_cast<unsigned char*>(_v), sizeof(double));
      #endif
    */
    return fwrite(_v, 1, sizeof(double), mPimpl->mFile);
}
//---------------------------------------------------------------------------

// Description :
// Param       :
// Result      :
// Comments    : assuming that the float is 4 bytes long (assumption is the mother
//               of all fuckups
bool  Stream_ANSI_C::writeFloat (float*  _v)
{
#ifdef PPT_BIGENDIAN_IO
    int lInt;

    memcpy(&lInt, _v, 4);
    lInt = Swap32(lInt);
    memcpy(_v, &lInt, 4);
#endif

    return fwrite(_v, 1, sizeof(float), mPimpl->mFile);
}
//---------------------------------------------------------------------------

bool  Stream_ANSI_C::writeBool  (bool*   _v)
{
    int lValue = (int)*_v;

#ifdef PPT_BIGENDIAN_IO
    lValue = Swap32(lValue);
#endif

    return fwrite(&lValue, 1, sizeof(int), mPimpl->mFile);
}
//---------------------------------------------------------------------------

bool Stream_ANSI_C::valid()
{
    return (mPimpl->mFile != NULL);
}

}	// end of namespace PP


