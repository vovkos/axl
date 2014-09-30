#include "pch.h"
#include "axl_exe_ArgPacker.h"

namespace axl {
namespace exe {

//.............................................................................
	
void
ArgPackerSeq::Agent::construct (void* _p)
{
	size_t count = m_sequence.getCount ();

	uchar_t* p = (uchar_t*) _p;

	for (size_t i = 0; i < count; i++)
	{
		m_sequence [i]->construct (p);
		p += m_sequence [i]->getSize ();
	}
}

void
ArgPackerSeq::Agent::destruct (void* _p)
{
	size_t count = m_sequence.getCount ();

	uchar_t* p = (uchar_t*) _p;

	for (size_t i = 0; i < count; i++)
	{
		m_sequence [i]->destruct (p);
		p += m_sequence [i]->getSize ();
	}
}

void
ArgPackerSeq::Agent::copy (
	void* _p,
	const void* _pSrc
	)
{
	size_t count = m_sequence.getCount ();

	uchar_t* p = (uchar_t*) _p;
	uchar_t* src = (uchar_t*) _pSrc;

	for (size_t i = 0; i < count; i++)
	{
		m_sequence [i]->copy (p, src);

		p += m_sequence [i]->getSize ();
		src += m_sequence [i]->getSize ();
	}
}

//.............................................................................

axl_va_list
ArgPackerSeq::packV (
	void* _p,
	size_t* size,
	ref::Ptr <obj::IType>* agent,
	axl_va_list va
	)
{
	size_t count = m_sequence.getCount ();
	size_t totalSize = 0;
	uchar_t* p = (uchar_t*) _p;

	if (!p)
	{
		for (size_t i = 0; i < count; i++)
		{
			size_t size = 0;
			va = m_sequence [i]->packV (NULL, &size, NULL, va);
			totalSize += size;
		}
	}
	else
	{
		ref::Ptr <Agent> agent = AXL_REF_NEW (Agent);
		agent->m_sequence.reserve (count);

		for (size_t i = 0; i < count; i++)
		{
			size_t size = 0;
			ref::Ptr <obj::IType> subAgent;

			va = m_sequence [i]->packV (p, &size, &subAgent, va);

			agent->m_sequence.append (subAgent);

			p += size;
			totalSize += size;
		}

		*agent = agent;
	}

	*size = totalSize;
	return va;
}

size_t
ArgPackerSeq::appendFormat (const char* format)
{
	const char* pF = format;
	
	for (; *pF; pF++)
	{
		if (*pF != '%')
			continue;

		pF++;

		switch (*pF)
		{
		case 'd': 
		case 'i': 
		case 'o':
		case 'u':
		case 'x':
		case 'X':
			append <Arg <intptr_t> > ();
			break;

		case 'e': 
		case 'E': 
		case 'f':
		case 'g':
		case 'G':
			append <Arg <double> > ();
			break;

		case 'p':
			append <Arg <void*> > ();
			break;
			
#ifdef _UNICODE

		case 's': 
			append <ArgStringW> ();
			break;

		case 'S': 
			append <ArgStringA> ();
			break;

		case 'c': 
			append <Arg <wchar_t> > ();
			break;

		case 'C': 
			append <Arg <char> > ();
			break;

#else // _UNICODE

		case 's': 
			append <ArgStringA> ();
			break;

		case 'S': 
			append <ArgStringW> ();
			break;

		case 'c': 
			append <Arg <char> > ();
			break;

		case 'C': 
			append <Arg <wchar_t> > ();
			break;

#endif // _UNICODE
		}
	}

	return m_sequence.getCount ();
}

//.............................................................................

} // namespace exe
} // namespace axl
