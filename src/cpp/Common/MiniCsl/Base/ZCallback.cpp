#include "ZCallback.h"
#include "ZString.hpp"

ZString ZCsl_callback_t::callback(ZCsl* a)
{
  struct foo_t { typedef ZString (foo_t::*pmf)(ZCsl*); };
  foo_t* pFake = static_cast<foo_t*>(m_p);
  foo_t::pmf pmfFake = *reinterpret_cast<foo_t::pmf*>(&m_pmf);
  return (pFake->*pmfFake)(a);
}
