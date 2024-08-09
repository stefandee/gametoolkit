// Stream_ANSI_C.h
//#pragma once

//-----------------------------------------------------------------------------
//  Unit              : Ansi Stream
//
//  Version           : 1.0
//
//  Description       :
//    * reads and writes data from/to a file stream
//    * it handles little endian/big endian by defines (as all disk data
//      is in little endian format); not the best solution, but because
//      big endian systems are compile&run-only (as data is created on a windows
//      machine), this does not matter too much
//
//  History           :
//    [?2003] - [tamlin] - created
//    [07.01.2005] - [Karg] - added endian management by defines
//-----------------------------------------------------------------------------

#ifndef PP_COMMON_STREAM_ANSI_C_H
#define PP_COMMON_STREAM_ANSI_C_H

#include "Stream.h"

namespace PP {

// Stream is not really a "stream", since you can rewind() it,
// but the name kind of stuck.

class Stream_ANSI_C : public Stream
{
public:
	Stream_ANSI_C(const char* filename, bool _empty = false);
	virtual ~Stream_ANSI_C();

	// returns number of bytes read.
	virtual size_t read(void* pMem, size_t nBytes);

	// TMN: If we can't rewind, then the brown stuff hit the rotating
	// ventilation device.
	virtual bool   rewind();
	virtual bool   seek(size_t offset);
	virtual bool   skip(size_t size);
	virtual size_t length();
    virtual int    tell();
    virtual size_t write(void* pMem, size_t nBytes);
    virtual bool valid();

  public: // typed io
        virtual bool  readInt   (int*    _v);
		virtual bool  readShort (short*  _v);
        virtual bool  readDouble(double* _v);
        virtual bool  readFloat (float*  _v);
        virtual bool  readBool  (bool*   _v);

        virtual bool  writeInt   (int*    _v);
        virtual bool  writeDouble(double* _v);
        virtual bool  writeFloat (float*  _v);
        virtual bool  writeBool  (bool*   _v);

private:
	Stream_ANSI_C(const Stream_ANSI_C& rhs);	// no impl.
	void operator=(const Stream_ANSI_C& rhs);	// no impl.

	class Stream_ANSI_C_impl* mPimpl;
};

}	// end of namespace PP

#endif	// PP_COMMON_STREAM_ANSI_C_H
