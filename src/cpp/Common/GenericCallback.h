#ifndef COMMON_GENERIC_CALLBACK_H
#define COMMON_GENERIC_CALLBACK_H

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


// Some magic to stuff to make a generic callback class that can hold
// member function pointers with different number and types of arguments.

class PP_voidret_callback_t
{
public:
	PP_voidret_callback_t() :	m_p(0), m_pmf(0) { }
	template <class T, typename A1>
		PP_voidret_callback_t(T* pObj, void (T::*pmf)(A1))
		:  m_p(pObj), m_pmf(*reinterpret_cast<void**>(&pmf)) {}
	template <class T, typename A1, typename A2>
		PP_voidret_callback_t(T* pObj, void (T::*pmf)(A1, A2))
		:  m_p(pObj), m_pmf(*reinterpret_cast<void**>(&pmf)) {}
	template <class T, typename A1, typename A2, typename A3>
		PP_voidret_callback_t(T* pObj, void (T::*pmf)(A1, A2, A3))
		:  m_p(pObj), m_pmf(*reinterpret_cast<void**>(&pmf)) {}
	template <class T, typename A1, typename A2, typename A3, typename A4>
		PP_voidret_callback_t(T* pObj, void (T::*pmf)(A1, A2, A3, A4))
		:  m_p(pObj), m_pmf(*reinterpret_cast<void**>(&pmf)) {}

	template <class T, typename A1>
		void set(T* pObj, void (T::*pmf)(A1))
			{ m_p = pObj; m_pmf = *reinterpret_cast<void**>(&pmf); }
	template <class T, typename A1, typename A2>
		void set(T* pObj, void (T::*pmf)(A1, A2))
			{ m_p = pObj; m_pmf = *reinterpret_cast<void**>(&pmf); }
	template <class T, typename A1, typename A2, typename A3>
		void set(T* pObj, void (T::*pmf)(A1, A2, A3))
			{ m_p = pObj; m_pmf = *reinterpret_cast<void**>(&pmf); }
	template <class T, typename A1, typename A2, typename A3, typename A4>
		void set(T* pObj, void (T::*pmf)(A1, A2, A3, A4))
			{ m_p = pObj; m_pmf = *reinterpret_cast<void**>(&pmf); }

	template <typename A1>
	void callback(const A1& a1)
	{
		struct foo_t { };
		typedef void (foo_t::*pmf)(A1);
		foo_t* pFake = static_cast<foo_t*>(m_p);
		pmf pmfFake = *reinterpret_cast<pmf*>(&m_pmf);
		(pFake->*pmfFake)(a1);
		
	}
	template <typename A1, typename A2>
	void callback(const A1& a1, const A2& a2)
	{
		struct foo_t { };
		typedef void (foo_t::*pmf)(A1, A2);
		foo_t* pFake = static_cast<foo_t*>(m_p);
		pmf pmfFake = *reinterpret_cast<pmf*>(&m_pmf);
		(pFake->*pmfFake)(a1, a2);
	}
	template <typename A1, typename A2, typename A3>
	void callback(const A1& a1, const A2& a2, const A3& a3)
	{
	    struct foo_t { };
		typedef void (foo_t::*pmf)(A1,A2,A3);		
		foo_t* pFake = static_cast<foo_t*>(m_p);
		pmf pmfFake = *reinterpret_cast<pmf*>(&m_pmf);
		(pFake->*pmfFake)(a1, a2, a3);
	}

	template <typename A1, typename A2, typename A3, typename A4>
	void callback(const A1& a1, const A2& a2, const A3& a3, const A4& a4)
	{
	    struct foo_t { };
		typedef void (foo_t::*pmf)(A1, A2, A3, A4);
		foo_t* pFake = static_cast<foo_t*>(m_p);
		pmf pmfFake = *reinterpret_cast<pmf*>(&m_pmf);
		(pFake->*pmfFake)(a1, a2, a3, a4);
	}

	bool valid() const { return m_p != 0 && m_pmf != 0; }

private:
	void* m_p;
	void* m_pmf;
};


#endif	// COMMON_GENERIC_CALLBACK_H

