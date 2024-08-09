//---------------------------------------------------------------------------
#include <stdexcept>
#pragma hdrstop

#include "Stream_PhysFS.h"
//---------------------------------------------------------------------------

#include "Stream_PhysFS.h"
#include "Platform.h"
#include "BitOp.h"
#include "physfs.h"
#include "easylogging++.h"

namespace PP
{

class Stream_PhysFS_impl
{
public:
    Stream_PhysFS_impl(const char* filename, const std::string& mode)
    {
        // TODO: opening mode depending on the mode param
        if (mode == "w")
        {
            mFile = PHYSFS_openWrite(filename);
        }
        else if (mode == "r")
        {
            mFile = PHYSFS_openRead(filename);
        }

        if (!mFile)
        {
            std::string sErr("Cannot open file: ");
            sErr += filename;
            sErr += " with mode " + mode;

            LOG(FATAL) << sErr;

            throw std::invalid_argument(sErr);
        }
    }

    ~Stream_PhysFS_impl()
    {
        PHYSFS_close(mFile);
    }

    PHYSFS_file* mFile;
};

Stream_PhysFS::Stream_PhysFS(const char* filename, bool _empty)
{
    CPString lFixedPath = FixPath2(CPString(filename));

    //logWrite("opening: ");
    //logWriteLn(lFixedPath.c_str());

    if (_empty)
    {
        mPimpl = new Stream_PhysFS_impl(lFixedPath.c_str(), "w");
    }
    else
    {
        mPimpl = new Stream_PhysFS_impl(lFixedPath.c_str(), "r");
    }
}

Stream_PhysFS::~Stream_PhysFS()
{
    delete mPimpl;
}

size_t Stream_PhysFS::read(void* pMem, size_t nBytes)
{
    return PHYSFS_read(mPimpl->mFile, pMem, 1, nBytes);
}

bool Stream_PhysFS::rewind()
{
    PHYSFS_seek(mPimpl->mFile, 0);
    return true;
}

bool Stream_PhysFS::seek(size_t offset)
{
    return PHYSFS_seek(mPimpl->mFile, offset);
}

bool Stream_PhysFS::skip(size_t size)
{
    int currentPos = PHYSFS_tell(mPimpl->mFile);


    return PHYSFS_seek(mPimpl->mFile, currentPos + size) != -1;
}

size_t Stream_PhysFS::length()
{
    return PHYSFS_fileLength(mPimpl->mFile);
}

int Stream_PhysFS::tell()
{
    return PHYSFS_tell(mPimpl->mFile);
}

size_t Stream_PhysFS::write(void* pMem, size_t nBytes)
{
    return PHYSFS_write(mPimpl->mFile, pMem, 1, nBytes);
}

// TODO: apply endianess transform to these functions
bool  Stream_PhysFS::readInt   (int*    _v)
{
    bool lResult = PHYSFS_read(mPimpl->mFile, _v, 1, sizeof(int));

#ifdef PPT_BIGENDIAN_IO
    *_v = Swap32(*_v);
#endif

    return lResult;
}
//---------------------------------------------------------------------------

bool  Stream_PhysFS::readShort   (short*    _v)
{
    bool lResult = PHYSFS_read(mPimpl->mFile, _v, 1, sizeof(short));

#ifdef PPT_BIGENDIAN_IO
    *_v = Swap16(*_v);
#endif

    return lResult;
}
//---------------------------------------------------------------------------
bool  Stream_PhysFS::readDouble(double* _v)
{
    bool lResult = PHYSFS_read(mPimpl->mFile, _v, 1, sizeof(double));

    /* XXX: can not dynamic_cast double to type unsigned char
      #ifdef PPT_BIGENDIAN_IO
        SwapBytes(dynamic_cast<unsigned char*>(_v), sizeof(double));
      #endif
    */
    return lResult;
}
//---------------------------------------------------------------------------

bool  Stream_PhysFS::readFloat (float*  _v)
{
    bool lResult = PHYSFS_read(mPimpl->mFile, _v, 1, sizeof(float));

#ifdef PPT_BIGENDIAN_IO
    int lInt;

    memcpy(&lInt, _v, 4);
    lInt = Swap32(lInt);
    memcpy(_v, &lInt, 4);
#endif

    return lResult;
}
//---------------------------------------------------------------------------

bool  Stream_PhysFS::readBool  (bool*   _v)
{
    int lValue = 0, lResult;

    lResult = PHYSFS_read(mPimpl->mFile, &lValue, 1, sizeof(int));

#ifdef PPT_BIGENDIAN_IO
    lValue = Swap32(lValue);
#endif

    *_v = (lValue != 0);
    return lResult;
}
//---------------------------------------------------------------------------

bool  Stream_PhysFS::writeInt   (int*    _v)
{
#ifdef PPT_BIGENDIAN_IO
    *_v = Swap32(*_v);
#endif

    return PHYSFS_write(mPimpl->mFile, _v, 1, sizeof(int));
}
//---------------------------------------------------------------------------

bool  Stream_PhysFS::writeDouble(double* _v)
{
    /* XXX: Can not dynamic_cast double to unsigned char
      #ifdef PPT_BIGENDIAN_IO
        SwapBytes(dynamic_cast<unsigned char*>(_v), sizeof(double));
      #endif
    */
    return PHYSFS_write(mPimpl->mFile, _v, 1, sizeof(double));
}
//---------------------------------------------------------------------------

// Description :
// Param       :
// Result      :
// Comments    : assuming that the float is 4 bytes long (assumption is the mother
//               of all fuckups
bool  Stream_PhysFS::writeFloat (float*  _v)
{
#ifdef PPT_BIGENDIAN_IO
    int lInt;

    memcpy(&lInt, _v, 4);
    lInt = Swap32(lInt);
    memcpy(_v, &lInt, 4);
#endif

    return PHYSFS_write(mPimpl->mFile, _v, 1, sizeof(float));
}
//---------------------------------------------------------------------------

bool  Stream_PhysFS::writeBool  (bool*   _v)
{
    int lValue = (int)*_v;

#ifdef PPT_BIGENDIAN_IO
    lValue = Swap32(lValue);
#endif

    return PHYSFS_write(mPimpl->mFile, &lValue, 1, sizeof(int));
}
//---------------------------------------------------------------------------

CPString Stream_PhysFS::FixPath2(CPString _path)
{
    // FIXME: just a minor hack ;)
    _path.Replace('\\', '/');

    return CPString(_path);
}
//---------------------------------------------------------------------------

bool Stream_PhysFS::valid()
{
    return (mPimpl->mFile != NULL);
}
//---------------------------------------------------------------------------

}	// end of namespace PP
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif
