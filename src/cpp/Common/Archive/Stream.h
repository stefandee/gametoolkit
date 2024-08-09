// Stream.h
//#pragma once
#ifndef PP_COMMON_STREAM_H
#define PP_COMMON_STREAM_H

#include <stdlib.h>	// size_t
#include "PP_String.h"

namespace PP {

// Stream is not really a "stream", since you can rewind() it,
// but the name kind of stuck.
class Stream
{
public:
	virtual ~Stream() = 0;

	// returns number of bytes read.
	virtual size_t read(void* pMem, size_t nBytes) = 0;

	// If we can't rewind, then the brown stuff hit the rotating
	// ventilation device.
	virtual bool   rewind() = 0;
	virtual bool   seek(size_t /* off_t is non-ANSI */ offset) = 0;
	virtual bool   skip(size_t size) = 0;  // skips n bytes of input
    virtual size_t length() = 0;
    virtual int    tell() = 0;
    virtual size_t write(void* pMem, size_t nBytes) = 0;
    virtual bool valid() = 0;

  public: // typed io
        virtual bool  readInt   (int*    _v) = 0;
		virtual bool  readShort (short*  _v) = 0;
        virtual bool  readDouble(double* _v) = 0;
        virtual bool  readFloat (float*  _v) = 0;
        virtual bool  readBool  (bool*   _v) = 0;

        virtual bool  writeInt   (int*    _v) = 0;
        virtual bool  writeDouble(double* _v) = 0;
        virtual bool  writeFloat (float*  _v) = 0;
        virtual bool  writeBool  (bool*   _v) = 0;

  public:
        static CPString FixPath(CPString _path);

  protected:
	Stream();

  private:
	Stream(const Stream& rhs);	// no impl.
	void operator=(const Stream& rhs);	// no impl.
};

}	// end of namespace PP

#endif	// PP_COMMON_STREAM_H
