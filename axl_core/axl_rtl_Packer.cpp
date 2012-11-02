#include "stdafx.h"
#include "axl_rtl_Packer.h"

namespace axl {
namespace rtl {

//.............................................................................

va_list
CPackerSeq::PackV (
	void* _p,
	size_t* pSize,
	va_list va
	)
{
	size_t Count = m_Sequence.GetCount ();
	size_t TotalSize = 0;

	if (!_p)
	{
		for (size_t i = 0; i < Count; i++)
		{
			size_t Size = 0;
			va = m_Sequence [i]->PackV (NULL, &Size, va);
			TotalSize += Size;
		}
	}
	else
	{
		uchar_t* p = (uchar_t*) _p;

		for (size_t i = 0; i < Count; i++)
		{
			size_t Size = 0;

			va = m_Sequence [i]->PackV (p, &Size, va);

			p += Size;
			TotalSize += Size;
		}
	}

	*pSize = TotalSize;
	return va;
}

size_t
CPackerSeq::AppendFormat (const char* pFormat)
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
			Append <CPackT <intptr_t> > ();
			break;

		case 'e': 
		case 'E': 
		case 'f':
		case 'g':
		case 'G':
			Append <CPackT <double> > ();
			break;

		case 'p':
			Append <CPackT <size_t> > ();
			break;
			
#ifdef _UNICODE

		case 's': 
			Append <CPackStringW> ();
			break;

		case 'S': 
			Append <CPackStringA> ();
			break;

		case 'c': 
			Append <CPackT <wchar_t> > ();
			break;

		case 'C': 
			Append <CPackT <char> > ();
			break;

#else // _UNICODE

		case 's': 
			Append <CPackStringA> ();
			break;

		case 'S': 
			Append <CPackStringW> ();
			break;

		case 'c': 
			Append <CPackT <char> > ();
			break;

		case 'C': 
			Append <CPackT <wchar_t> > ();
			break;

#endif // _UNICODE

		case 'B':
			Append <CPackT <uchar_t> > ();
			break;

		case 'W':
			Append <CPackT <ushort_t> > ();
			break;

		case 'D':
			Append <CPackT <ulong_t> > ();
			break;

		case 'Z':
			Append <CPackT <size_t> > ();
			break;

		case 'P':
			Append <CPackPtrSize> ();
			break;

		case 'R':
			Append <CPackLastError> ();
			break;
		}
	}

	return m_Sequence.GetCount ();
}

//.............................................................................
	
size_t
CPacker::AppendV (
	IPacker* pPacker,
	va_list va
	)
{
	bool Result;

	size_t Size;
	pPacker->PackV (NULL, &Size, va);

	size_t OldSize = m_Buffer.GetCount ();
	size_t NewSize = OldSize + Size;

	Result = m_Buffer.SetCount (NewSize);
	if (!Result)
		return OldSize;
	
	pPacker->PackV (m_Buffer + OldSize, &Size, va);
	return NewSize;
}

size_t
CPacker::Append (
	const void* p,
	size_t Size
	)
{
	bool Result;

	size_t OldSize = m_Buffer.GetCount ();
	size_t NewSize = OldSize + Size;

	Result = m_Buffer.SetCount (NewSize);
	if (!Result)
		return OldSize;

	memcpy (m_Buffer + OldSize, p, Size);
	return NewSize;
}

//.............................................................................

} // namespace err
} // namespace axl
