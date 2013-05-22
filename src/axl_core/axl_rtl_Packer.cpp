#include "pch.h"
#include "axl_rtl_Packer.h"

namespace axl {
namespace rtl {

//.............................................................................

axl_va_list
CPackerSeq::Pack_va (
	void* _p,
	size_t* pSize,
	axl_va_list va
	)
{
	size_t Count = m_Sequence.GetCount ();
	size_t TotalSize = 0;

	if (!_p)
	{
		for (size_t i = 0; i < Count; i++)
		{
			size_t Size = 0;
			va = m_Sequence [i]->Pack_va (NULL, &Size, va);
			TotalSize += Size;
		}
	}
	else
	{
		uchar_t* p = (uchar_t*) _p;

		for (size_t i = 0; i < Count; i++)
		{
			size_t Size = 0;

			va = m_Sequence [i]->Pack_va (p, &Size, va);

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
			Append <CPackT <int> > ();
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
			
		case 's': 
			Append <CPackString> ();
			break;

		case 'c': 
			Append <CPackT <char> > ();
			break;

		case 'S': 
			Append <CPackString_w> ();
			break;

		case 'C': 
			Append <CPackT <wchar_t> > ();
			break;

		case 'B':
			Append <CPackT <uint8_t> > ();
			break;

		case 'W':
			Append <CPackT <uint16_t> > ();
			break;

		case 'D':
			Append <CPackT <uint32_t> > ();
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
CPacker::Append_va (
	IPacker* pPacker,
	axl_va_list va
	)
{
	bool Result;

	size_t Size;
	pPacker->Pack_va (NULL, &Size, va);

	size_t OldSize = m_Buffer.GetCount ();
	size_t NewSize = OldSize + Size;

	Result = m_Buffer.SetCount (NewSize);
	if (!Result)
		return OldSize;
	
	pPacker->Pack_va (m_Buffer + OldSize, &Size, va);
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
