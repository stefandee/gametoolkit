#ifndef COMMON_GENERIC_CALLBACK_H
#define COMMON_GENERIC_CALLBACK_H

#include <functional>

// Callback for ZCsl
class ZString;
class ZCsl;

// Callback class to be used with the Csl scripting
class ZCsl_callback_t
{
public:
	ZCsl_callback_t() : m_p(0), m_pmf(0) {}

	template <class T>
	ZCsl_callback_t(T* pObj, ZString (T::*pmf)(ZCsl*))
	:	m_p(pObj),
		m_pmf(*reinterpret_cast<fake_callback_t*>(&pmf))
		{}
	template <class T>
	void set(T* pObj, ZString (T::*pmf)(ZCsl*))
		{ m_p = pObj; m_pmf = *reinterpret_cast<fake_callback_t*>(&pmf); }

	ZString callback(ZCsl* a);
	bool valid() const { return m_p != 0 && m_pmf != 0; }

private:
	typedef ZString (*fake_callback_t)(ZCsl*);

	void*           m_p;
	fake_callback_t m_pmf;
};

#endif	// COMMON_GENERIC_CALLBACK_H

