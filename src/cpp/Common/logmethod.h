//---------------------------------------------------------------------------

#ifndef logMethodH
#define logMethodH
//---------------------------------------------------------------------------

#include "easylogging++.h"

class LogMethodImpl
{
  public:
    LogMethodImpl(const char* _msg) { VLOG(9) << _msg << "\n\r" << "{"; }
    ~LogMethodImpl() { VLOG(9) << "}";}

  protected:
    LogMethodImpl();

  private:
};

#define LogMethod(message) LogMethodImpl aInstance(message);

#endif
