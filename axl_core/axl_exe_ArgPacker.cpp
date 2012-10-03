#include "stdafx.h"
#include "axl_exe_ArgPacker.h"

namespace axl {
namespace exe {

//.............................................................................
	
void
CArgPackerSeq::CAgent::Construct (void* _p)
{
	size_t Count = m_Sequence.GetCount ();

	uchar_t* p = (uchar_t*) _p;

	for (size_t i = 0; i < Count; i++)
	{
		m_Sequence [i]->Construct (p);
		p += m_Sequence [i]->GetSize ();
	}
}

void
CArgPackerSeq::CAgent::Destruct (void* _p)
{
	size_t Count = m_Sequence.GetCount ();

	uchar_t* p = (uchar_t*) _p;

	for (size_t i = 0; i < Count; i++)
	{
		m_Sequence [i]->Destruct (p);
		p += m_Sequence [i]->GetSize ();
	}
}

void
CArgPackerSeq::CAgent::Copy (
	void* _p,
	const void* _pSrc
	)
{
	size_t Count = m_Sequence.GetCount ();

	uchar_t* p = (uchar_t*) _p;
	uchar_t* pSrc = (uchar_t*) _pSrc;

	for (size_t i = 0; i < Count; i++)
	{
		m_Sequence [i]->Copy (p, pSrc);

		p += m_Sequence [i]->GetSize ();
		pSrc += m_Sequence [i]->GetSize ();
	}
}

//.............................................................................

va_list
CArgPackerSeq::PackV (
	void* _p,
	size_t* pSize,
	ref::CPtrT <obj::IClass>* pAgent,
	va_list va
	)
{
	size_t Count = m_Sequence.GetCount ();
	size_t TotalSize = 0;
	uchar_t* p = (uchar_t*) _p;

	if (!p)
	{
		for (size_t i = 0; i < Count; i++)
		{
			size_t Size = 0;
			va = m_Sequence [i]->PackV (NULL, &Size, NULL, va);
			TotalSize += Size;
		}
	}
	else
	{
		ref::CPtrT <CAgent> Agent = AXL_REF_NEW (CAgent);
		Agent->m_Sequence.Reserve (Count);

		for (size_t i = 0; i < Count; i++)
		{
			size_t Size = 0;
			ref::CPtrT <obj::IClass> SubAgent;

			va = m_Sequence [i]->PackV (p, &Size, &SubAgent, va);

			Agent->m_Sequence.Append (SubAgent);

			p += Size;
			TotalSize += Size;
		}

		*pAgent = Agent;
	}

	*pSize = TotalSize;
	return va;
}

size_t
CArgPackerSeq::AppendFormat (const char* pFormat)
{
 	const char* pF = pFormat;
	
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
			Append <CArgT <intptr_t> > ();
			break;

		case 'e': 
		case 'E': 
		case 'f':
		case 'g':
		case 'G':
			Append <CArgT <double> > ();
			break;

		case 'p':
			Append <CArgT <void*> > ();
			break;
			
#ifdef _UNICODE

		case 's': 
			Append <CArgStringW> ();
			break;

		case 'S': 
			Append <CArgStringA> ();
			break;

		case 'c': 
			Append <CArgT <wchar_t> > ();
			break;

		case 'C': 
			Append <CArgT <char> > ();
			break;

#else // _UNICODE

		case 's': 
			Append <CArgStringA> ();
			break;

		case 'S': 
			Append <CArgStringW> ();
			break;

		case 'c': 
			Append <CArgT <char> > ();
			break;

		case 'C': 
			Append <CArgT <wchar_t> > ();
			break;

#endif // _UNICODE
		}
	}

	return m_Sequence.GetCount ();
}

//.............................................................................

} // namespace exe
} // namespace axl
