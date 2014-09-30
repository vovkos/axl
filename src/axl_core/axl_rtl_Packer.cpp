#include "pch.h"
#include "axl_rtl_Packer.h"

namespace axl {
namespace rtl {

//.............................................................................

axl_va_list
CPackerSeq::pack_va (
	void* _p,
	size_t* size,
	axl_va_list va
	)
{
	size_t count = m_sequence.getCount ();
	size_t totalSize = 0;

	if (!_p)
	{
		for (size_t i = 0; i < count; i++)
		{
			size_t size = 0;
			va = m_sequence [i]->pack_va (NULL, &size, va);
			totalSize += size;
		}
	}
	else
	{
		uchar_t* p = (uchar_t*) _p;

		for (size_t i = 0; i < count; i++)
		{
			size_t size = 0;

			va = m_sequence [i]->pack_va (p, &size, va);

			p += size;
			totalSize += size;
		}
	}

	*size = totalSize;
	return va;
}

size_t
CPackerSeq::appendFormat (const char* formatString)
{
	const char* pF = formatString;
	
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
			append <CPackT <int> > ();
			break;

		case 'e': 
		case 'E': 
		case 'f':
		case 'g':
		case 'G':
			append <CPackT <double> > ();
			break;

		case 'p':
			append <CPackT <size_t> > ();
			break;
			
		case 's': 
			append <CPackString> ();
			break;

		case 'c': 
			append <CPackT <char> > ();
			break;

		case 'S': 
			append <CPackString_w> ();
			break;

		case 'C': 
			append <CPackT <wchar_t> > ();
			break;

		case 'B':
			append <CPackT <uint8_t> > ();
			break;

		case 'W':
			append <CPackT <uint16_t> > ();
			break;

		case 'D':
			append <CPackT <uint32_t> > ();
			break;

		case 'Z':
			append <CPackT <size_t> > ();
			break;

		case 'P':
			append <CPackPtrSize> ();
			break;

		case 'R':
			append <CPackLastError> ();
			break;
		}
	}

	return m_sequence.getCount ();
}

//.............................................................................
	
size_t
CPackage::append_va (
	CPacker* packer,
	axl_va_list va
	)
{
	bool result;

	size_t size;
	packer->pack_va (NULL, &size, va);

	size_t oldSize = m_buffer.getCount ();
	size_t newSize = oldSize + size;

	result = m_buffer.setCount (newSize);
	if (!result)
		return oldSize;
	
	packer->pack_va (m_buffer + oldSize, &size, va);
	return newSize;
}

size_t
CPackage::append (
	const void* p,
	size_t size
	)
{
	bool result;

	size_t oldSize = m_buffer.getCount ();
	size_t newSize = oldSize + size;

	result = m_buffer.setCount (newSize);
	if (!result)
		return oldSize;

	memcpy (m_buffer + oldSize, p, size);
	return newSize;
}

//.............................................................................

} // namespace err
} // namespace axl
