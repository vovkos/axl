// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_RTL_STRING_H

#include "axl_ref_Buf.h"
#include "axl_rtl_BitIdx.h"

namespace axl {
namespace rtl {

//.............................................................................

enum EStrCmp
{
	EStrCmp_NoCase  = 1,
	EStrCmp_Partial = 2
};

//.............................................................................

template <typename T>
struct CStringDetailsT 
{
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <>
struct CStringDetailsT <char>
{
	typedef char C;
	typedef wchar_t C2;
	typedef CStringDetailsT <wchar_t> CDetails2;
	
	static 
	size_t 
	GetLength (const char* p)
	{ 
		return strlen (p); 
	}

	static 
	const char* 
	GetEmptyLiteral ()
	{ 
		return ""; 
	}

	static 
	size_t 
	FormatV (
		char* pBuffer, 
		size_t BufferLength, 
		const char* pFormat, 
		va_list va
		)
	{ 
		return _vsnprintf (pBuffer, BufferLength, pFormat, va); 
	}

	static 
	int 
	Cmp (
		const char* p1, 
		size_t Length1,
		const char* p2,
		size_t Length2,
		int Flags
		)
	{ 
		size_t Length = min (Length1, Length2);

		if (Flags & EStrCmp_NoCase)
		{
			for (size_t i = 0; i < Length; i++)
			{
				char c1 = tolower (p1 [i]);
				char c2 = tolower (p2 [i]);

				if (c1 < c2)
					return -1;
				else if (c1 > c2)
					return 1;
			}
		}
		else
		{
			for (size_t i = 0; i < Length; i++)
			{
				char c1 = p1 [i];
				char c2 = p2 [i];

				if (c1 < c2)
					return -1;
				else if (c1 > c2)
					return 1;
			}
		}

		return 
			Length1 == Length2 || (Flags & EStrCmp_Partial) ? 0 :
			Length1 < Length2 ? -1 : 1;
	}

	static 
	size_t
	Find (
		const char* pString,
		size_t Length,
		char c,
		int Flags
		)
	{ 
		const char* p = pString;
		const char* pEnd = p + Length;

		if (Flags & EStrCmp_NoCase)
		{
			c = tolower (c);

			for (; p < pEnd; p++)
				if (tolower (*p) == c)
					return p - pString;
		}
		else
		{
			for (; p < pEnd; p++)
				if (*p == c)
					return p - pString;
		}

		return -1;
	}

	static 
	size_t
	Find (
		const char* pString,
		size_t Length,
		const char* pPattern, 
		size_t PatternLength,
		int Flags
		)
	{ 
		const char* p = pString;
		const char* pEnd = p + Length;
		const char* pPatEnd = pPattern + PatternLength;
		
		if (Flags & EStrCmp_NoCase)
		{
			for (; p < pEnd; p++)
			{
				const char *p1 = p;
				const char *p2 = pPattern;

				for (; p1 < pEnd && p2 < pPatEnd; p1++, p2++)
				{
					if (tolower (*p1) != tolower (*p2))
						break;
				}

				if (p2 == pPatEnd || p1 == pEnd && (Flags & EStrCmp_Partial) != 0)
					return p - pString;
			}
		}
		else
		{
			for (; p < pEnd; p++)
			{
				const char *p1 = p;
				const char *p2 = pPattern;

				for (; p1 < pEnd && p2 < pPatEnd; p1++, p2++)
				{
					if (*p1 != *p2)
						break;
				}

				if (p2 == pPatEnd || p1 == pEnd && (Flags & EStrCmp_Partial) != 0)
					return p - pString;
			}
		}

		return -1;
	}

	static 
	void 
	Fill (
		char* p, 
		char c, 
		size_t Count
		)
	{ 
		memset (p, c, Count * sizeof (char)); 
	}

	static 
	size_t 
	Convert (
		char* pDst, 
		const wchar_t* pSrc, 
		size_t Length
		)
	{ 
		return wcstombs (pDst, pSrc, Length); 
	}

	static 
	long 
	StrToLong (
		const char* p,
		char** ppEnd = NULL,
		int Radix = 10
		)
	{
		return strtol (p, ppEnd, Radix);
	}

	static 
	ulong_t 
	StrToULong (
		const char* p,
		char** ppEnd = NULL,
		int Radix = 10
		)
	{
		return strtoul (p, ppEnd, Radix);
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <>
struct CStringDetailsT <wchar_t>
{
	typedef wchar_t C;
	typedef char C2;
	typedef CStringDetailsT <char> CDetails2;

	static 
	size_t 
	GetLength (const wchar_t* p)
	{ 
		return wcslen (p); 
	}

	static 
	const wchar_t* 
	GetEmptyLiteral ()
	{ 
		return L""; 
	}

	static 
	size_t 
	FormatV (
		wchar_t* pBuffer, 
		size_t BufferLength, 
		const wchar_t* pFormat, 
		va_list va
		)
	{ 
		return _vsnwprintf (pBuffer, BufferLength, pFormat, va); 
	}

	static 
	int 
	Cmp (
		const wchar_t* p1, 
		size_t Length1,
		const wchar_t* p2,
		size_t Length2,
		int Flags
		)
	{ 
		size_t Length = min (Length1, Length2);

		if (Flags & EStrCmp_NoCase)
		{
			for (size_t i = 0; i < Length; i++)
			{
				wchar_t c1 = towlower (p1 [i]);
				wchar_t c2 = towlower (p2 [i]);

				if (c1 < c2)
					return -1;
				else if (c1 > c2)
					return 1;
			}
		}
		else
		{
			for (size_t i = 0; i < Length; i++)
			{
				wchar_t c1 = p1 [i];
				wchar_t c2 = p2 [i];

				if (c1 < c2)
					return -1;
				else if (c1 > c2)
					return 1;
			}
		}

		return 
			Length1 == Length2 || (Flags & EStrCmp_Partial) ? 0 :
			Length1 < Length2 ? -1 : 1;
	}

	static 
	size_t
	Find (
		const wchar_t* pString,
		size_t Length,
		wchar_t c,
		int Flags
		)
	{ 
		const wchar_t* p = pString;
		const wchar_t* pEnd = p + Length;

		if (Flags & EStrCmp_NoCase)
		{
			c = towlower (c);

			for (; p < pEnd; p++)
				if (towlower (*p) == c)
					return p - pString;
		}
		else
		{
			for (; p < pEnd; p++)
				if (*p == c)
					return p - pString;
		}

		return -1;
	}

	static 
	size_t
	Find (
		const wchar_t* pString,
		size_t Length,
		const wchar_t* pPattern, 
		size_t PatternLength,
		int Flags
		)
	{ 
		const wchar_t* p = pString;
		const wchar_t* pEnd = p + Length;
		const wchar_t* pPatEnd = pPattern + PatternLength;
		
		if (Flags & EStrCmp_NoCase)
		{
			for (; p < pEnd; p++)
			{
				const wchar_t *p1 = p;
				const wchar_t *p2 = pPattern;

				for (; p1 < pEnd && p2 < pPatEnd; p1++, p2++)
				{
					if (towlower (*p1) != towlower (*p2))
						break;
				}

				if (p2 == pPatEnd || p1 == pEnd && (Flags & EStrCmp_Partial) != 0)
					return p - pString;
			}
		}
		else
		{
			for (; p < pEnd; p++)
			{
				const wchar_t *p1 = p;
				const wchar_t *p2 = pPattern;

				for (; p1 < pEnd && p2 < pPatEnd; p1++, p2++)
				{
					if (*p1 != *p2)
						break;
				}

				if (p2 == pPatEnd || p1 == pEnd && (Flags & EStrCmp_Partial) != 0)
					return p - pString;
			}
		}

		return -1;
	}
	
	static 
	void 
	Fill (
		wchar_t* p, 
		wchar_t c, 
		size_t Count
		)
	{
		wchar_t* pEnd = p + Count;
		for (; p < pEnd; p++)
			*p = c;
	}

	static 
	size_t 
	Convert (
		wchar_t* pDst, 
		const char* pSrc, 
		size_t Length
		)
	{ 
		return mbstowcs (pDst, pSrc, Length); 
	}

	static 
	long 
	StrToLong (
		const wchar_t* p,
		wchar_t** ppEnd = NULL,
		int Radix = 10
		)
	{
		return wcstol (p, ppEnd, Radix);
	}

	static 
	ulong_t 
	StrToULong (
		const wchar_t* p,
		wchar_t** ppEnd = NULL,
		int Radix = 10
		)
	{
		return wcstoul (p, ppEnd, Radix);
	}
};

//.............................................................................

template <typename T>
class CStringT
{
public:
	class CHdr: public ref::IRefCount
	{
	public:
		size_t m_MaxLength;
		size_t m_Length;
	};

	typedef CStringDetailsT <T> CDetails;
	typedef typename CDetails::C C;
	typedef typename CDetails::C2 C2;
	typedef typename CDetails::CDetails2 CDetails2;
	typedef CStringT <C2> CString2;

protected:
	enum EConst
	{ 
		EConst_FormatBufferSize = 1024,
	};

protected:
	C* m_p;

public:
	CStringT ()
	{ 
		m_p = NULL;
	}

	CStringT (const CStringT& Src)
	{ 
		m_p = NULL;
		Copy (Src); 
	}

	CStringT (const CString2& Src)
	{ 
		m_p = NULL;
		Copy (Src); 
	}

	CStringT (
		const C* p, 
		size_t Length = -1
		)
	{ 
		m_p = NULL;
		Copy (p, Length); 
	}

	CStringT (
		const C2* p, 
		size_t Length = -1
		)
	{ 
		m_p = NULL;
		Copy (p, Length); 
	}

	CStringT (
		C c, 
		size_t Count = 1
		)
	{ 
		m_p = NULL;
		Copy (c, Count); 
	}

	CStringT (
		C2 c, 
		size_t Count = 1
		)
	{ 
		m_p = NULL;
		Copy (c, Count); 
	}

	CStringT (
		ref::EBuf Kind,
		void* p, 
		size_t Size
		)
	{
		m_p = NULL;
		SetBuffer (Kind, p, Size);
	}

	~CStringT ()
	{
		Release ();
	}

	operator const C* () const
	{ 
		// orginally was like this:
		// return m_p ? m_p : CDetails::GetEmptyLiteral (); 
		// but in this case there will be difference in passing CString as const char* or as vararg
		
		return m_p; 
	}

	operator C* () 
	{ 
		return m_p; 
	}

	bool 
	operator == (const CStringT& String) const
	{ 
		return Cmp (String) == 0; 
	}

	bool 
	operator == (const C* p) const
	{ 
		return Cmp (p) == 0; 
	}

	bool 
	operator != (const CStringT& String) const
	{ 
		return Cmp (String) != 0; 
	}

	bool 
	operator != (const C* p) const
	{ 
		return Cmp (p) != 0; 
	}

	CStringT& 
	operator = (const CStringT& Src)
	{ 
		Copy (Src);
		return *this;
	}

	CStringT& 
	operator = (const CString2& Src)
	{ 
		Copy (Src);
		return *this;
	}

	CStringT& 
	operator = (const C* p)
	{
		Copy (p, -1);
		return *this;
	}

	CStringT& 
	operator = (const C2* p)
	{
		Copy (p, -1);
		return *this;
	}

	CStringT& 
	operator = (C x)
	{
		Copy (x, 1);
		return *this;
	}

	CStringT& 
	operator = (C2 y)
	{
		Copy (y, 1);
		return *this;
	}

	CStringT& 
	operator += (const C* p)
	{
		Append (p, -1);
		return *this;
	}

	CStringT& 
	operator += (const C2* p)
	{
		Append (p, -1);
		return *this;
	}

	CStringT& 
	operator += (C x)
	{
		Append (x, 1);
		return *this;
	}

	CStringT& 
	operator += (C2 y)
	{
		Append (y, 1);
		return *this;
	}

	CStringT 
	operator + (const CStringT& String) const
	{ 
		CStringT Result = *this;
		Result.Append (String);
		return Result; 
	}

	CStringT 
	operator + (const C* p) const
	{ 
		CStringT Result = *this;
		Result.Append (p);
		return Result; 
	}

	CStringT 
	operator + (const CString2& String) const
	{ 
		CStringT Result = *this;
		Result.Append (String);
		return Result; 
	}

	CStringT 
	operator + (const C2* p) const
	{ 
		CStringT Result = *this;
		Result.Append (p);
		return Result; 
	}

	size_t 
	GetLength () const
	{ 
		return m_p ? GetHdr ()->m_Length : 0; 
	}

	size_t 
	GetBufferLength () const
	{ 
		return m_p ? GetHdr ()->m_MaxLength : 0; 
	}

	bool 
	IsEmpty () const
	{ 
		return GetLength () == 0; 
	}

	void 
	Release ()
	{
		if (!m_p)
			return;

		GetHdr ()->Release ();
		m_p = NULL;
	}

	void 
	Clear ()
	{ 
		SetLength (0, false); 
	}

	int 
	Cmp (
		int Flags,
		const C* p, 
		size_t Length = -1
		) const
	{ 
		if (Length == -1)
			Length = CDetails::GetLength (p);

		return CDetails::Cmp (m_p, GetLength (), p, Length, Flags);
	}

	int 
	Cmp (
		const C* p, 
		size_t Length = -1
		) const
	{
		return Cmp (0, p, Length);
	}

	int 
	Cmp (
		int Flags,
		const CStringT& String
		) const
	{ 
		return CDetails::Cmp (m_p, GetLength (), String, String.GetLength (), Flags);
	}

	int 
	Cmp (const CStringT& String) const
	{
		return Cmp (0, String);
	}

	size_t 
	Find (
		int Flags,
		const C* p, 
		size_t Length = -1
		) const
	{
		if (Length == -1)
			Length = CDetails::GetLength (p);

		return CDetails::Find (m_p, GetLength (), p, Length, Flags);
	}

	size_t 
	Find (
		const C* p, 
		size_t Length = -1
		) const
	{
		return Find (0, p, Length);
	}

	size_t 
	Find (
		int Flags,
		C c
		) const
	{
		return CDetails::Find (m_p, GetLength (), c, Flags);
	}

	size_t
	Find (C c) const
	{
		return Find (0, c);
	}

	C 
	GetBack (size_t Index = 0)
	{
		size_t Length = GetLength ();
		return Index < Length ? m_p [Length - Index] : 0;
	}

	size_t 
	ForceCopy (const CStringT& Src)
	{ 
		return Copy (Src, Src.GetLength ()); 
	}

	size_t 
	ForceCopy (const CString2& Src)
	{ 
		return Copy (Src, Src.GetLength ()); 
	}

	size_t
	Copy (const CStringT& Src)
	{
		if (m_p == Src.m_p)
			return GetLength ();

		if (Src.m_p)
		{
			if (Src.GetHdr ()->GetFree () == (mem::FFree) -1)
				return Copy (Src, Src.GetLength ()); 

			Src.GetHdr ()->AddRef ();
		}

		if (m_p)
			GetHdr ()->Release ();

		m_p = Src.m_p;
		return GetLength ();
	}

	size_t 
	Copy (const CString2& Src)
	{ 
		return Copy (Src, Src.GetLength ()); 
	}

	size_t 
	Copy (
		const C* p, 
		size_t Length = -1
		)
	{
		if (p == m_p && Length == -1)
			return GetLength ();

		if (!p)
		{
			Clear ();
			return 0;
		}

		if (Length == -1)
			Length = CDetails::GetLength (p);

		if (!SetLength (Length, false))
			return GetLength ();
			
		if (p != m_p)
			memcpy (m_p, p, Length * sizeof (C));

		return Length;
	}

	size_t 
	Copy (
		const C2* p, 
		size_t Length = -1
		)
	{
		if (!p)
		{
			Clear ();
			return 0;
		}

		if (Length == -1)
			Length = CDetails2::GetLength (p);

		if (!SetLength (Length, false))
			return GetLength ();
			
		CDetails::Convert (m_p, p, Length);
		return Length;
	}

	size_t 
	Copy (
		C c, 
		size_t Count = 1
		)
	{
		if (!Count)
		{
			Clear ();
			return 0;
		}

		if (!SetLength (Count, false))
			return GetLength ();
			
		CDetails::Fill (m_p, c, Count);
		return Count;
	}

	size_t 
	Copy (
		C2 _c, 
		size_t Count = 1
		)
	{
		C c;
		CDetails::Convert (&c, &_c, 1);
		return Copy (c, Count);
	}

	size_t 
	Append (const CStringT& Src)
	{ 
		return Append (Src, Src.GetLength ()); 
	}

	size_t 
	Append (const CString2& Src)
	{ 
		return Append (Src, Src.GetLength ()); 
	}

	size_t 
	Append (
		const C* p, 
		size_t Length = -1
		)
	{
		size_t OldLength = GetLength ();

		if (!p)
			return OldLength;

		if (Length == -1)
			Length = CDetails::GetLength (p);
		
		if (Length == 0)
			return OldLength;

		size_t NewLength = OldLength + Length;
		
		if (!SetLength (NewLength, true))
			return OldLength;
			
		memcpy (m_p + OldLength, p, Length * sizeof (C));
		return NewLength;
	}

	size_t 
	Append (
		const C2* p, 
		size_t Length = -1
		)
	{
		size_t OldLength = GetLength ();

		if (!p)
			return OldLength;

		if (Length == -1)
			Length = CDetails2::GetLength (p);

		if (Length == 0)
			return OldLength;

		size_t NewLength = OldLength + Length;
		if (!SetLength (NewLength, true))
			return OldLength;

		CDetails::Convert (m_p + OldLength, p, Length);
		return NewLength;
	}

	size_t 
	Append (
		C c, 
		size_t Count = 1
		)
	{
		size_t OldLength = GetLength ();

		if (Count == 0)
			return OldLength;

		size_t NewLength = OldLength + Count;
		if (!SetLength (NewLength, true))
			return OldLength;
			
		CDetails::Fill (m_p + OldLength, c, Count);
		return NewLength;
	}

	size_t 
	Append (
		C2 _c, 
		size_t Count = 1
		)
	{
		C c;
		CDetails::Convert (&c, &_c, 1);
		return Append (c, Count);
	}

	size_t 
	Replace (
		C From, 
		C To
		)
	{
		if (!EnsureExclusive ())
			return 0;

		C* p = m_p;
		C* pEnd = m_p + GetLength ();

		size_t Count = 0;
		for (; p < pEnd; p++)
			if (*p == From)
			{
				*p = To;
				Count++;
			}

		return Count;
	}

	size_t 
	FormatV (
		const C* pFormat, 
		va_list va
		)
	{
		C Buffer [EConst_FormatBufferSize];
		size_t Length = CDetails::FormatV (Buffer, sizeof (Buffer) / sizeof (char), pFormat, va);
		return Copy (Buffer, Length);
	}

	size_t 
	Format (
		const C* pFormat, 
		...
		)
	{ 
		return FormatV (pFormat, va_start_e (pFormat)); 
	}

	size_t 
	AppendFormatV (
		const C* pFormat, 
		va_list va
		)
	{
		C Buffer [EConst_FormatBufferSize];
		size_t Length = CDetails::FormatV (Buffer, sizeof (Buffer) / sizeof (char), pFormat, va);
		return Append (Buffer, Length);
	}

	size_t 
	AppendFormat (
		const C* pFormat, 
		...
		)
	{ 
		return AppendFormatV (pFormat, va_start_e (pFormat)); 
	}

	static 
	CStringT 
	FormatV_s (
		const C* pFormat, 
		va_list va
		)
	{
		CStringT Result;
		Result.FormatV (pFormat, va);
		return Result;
	}

	static 
	CStringT 
	Format_s (
		const C* pFormat, 
		...
		)
	{ 
		return FormatV_s (pFormat, va_start_e (pFormat)); 
	}

	size_t 
	ReduceLength (size_t Delta)
	{
		size_t Length = GetLength ();
		if (Length <= Delta)
		{
			Clear ();
			return 0;
		}

		Length -= Delta;
		SetLength (Length, true);
		return Length;
	}

	C* 
	GetBuffer ()
	{ 
		return EnsureExclusive () ? m_p : NULL; 
	}

	C* 
	GetBuffer (
		size_t Length, 
		bool SaveContents = false
		)
	{ 
		return SetLength (Length, SaveContents) ? m_p : NULL; 
	}

	C* 
	Reserve (
		size_t Length, 
		bool SaveContents = false
		)
	{ 
		size_t OldLength = GetLength ();
		
		if (!SetLength (Length, SaveContents))
			return NULL;

		SetLength (OldLength, SaveContents);
		return m_p; 
	}

	size_t 
	UpdateLength ()
	{
		if (!m_p)
			return 0;

		THdr* pHdr = GetHdr ();
		pHdr->m_Length = CDetails::GetLength (m_p);
		return pHdr->m_Length;
	}

	bool 
	EnsureExclusive ()
	{ 
		return m_p ? SetLength (GetLength (), true) : true; 
	}

protected:
	CHdr*
	GetHdr () const
	{
		return m_p ? (CHdr*) m_p - 1 : NULL;
	}

	bool 
	SetLength (
		size_t Length, 
		bool SaveContents
		)
	{
		CHdr* pOldHdr = GetHdr ();

		if (pOldHdr && 
			pOldHdr->m_MaxLength >= Length && 
			pOldHdr->GetRefCount () == 1)
		{
			pOldHdr->m_Length = Length;
			m_p [Length] = 0;
			return true;
		}

		if (Length == 0)
		{
			Release ();
			return true;
		}

		size_t MaxLength = rtl::GetMinPower2Gt (Length) - 1;
		ASSERT (MaxLength >= Length);

		size_t Size = (MaxLength + 1) * sizeof (C);
		ref::CPtrT <CHdr> NewHdr = AXL_REF_NEW_EXTRA (CHdr, Size);
		if (!NewHdr)
			return false;

		NewHdr->m_Length = Length;
		NewHdr->m_MaxLength = MaxLength;

		C* p = (C*) (NewHdr + 1);
		p [0] = p [Length] = 0;

		if (SaveContents && m_p)
		{
			size_t CopyLength = min (Length, pOldHdr->m_Length);
			memcpy (p, m_p, (CopyLength + 1) * sizeof (C));
		}

		if (pOldHdr)
			pOldHdr->Release ();

		m_p = p;

		NewHdr.Detach ();
		return true;
	}

	void
	SetBuffer (
		ref::EBuf Kind,
		void* p,
		size_t Size
		)
	{
		ASSERT (Size >= sizeof (CHdr) + sizeof (C));

		CHdr* pOldHdr = GetHdr ();
		
		ref::CPtrT <CHdr> NewHdr = AXL_REF_NEW_STATIC (CHdr, p);
		NewHdr->m_Length = 0;
		NewHdr->m_MaxLength = (Size - sizeof (CHdr)) / sizeof (C) - 1;
		NewHdr->SetFree (Kind == ref::EBuf_Static ? NULL : (mem::FFree) -1);

		if (pOldHdr)
			pOldHdr->Release ();

		m_p = (T*) (NewHdr + 1);
		m_p [0] = 0;

		NewHdr.Detach ();
	}
};

//.............................................................................

typedef CStringDetailsT <char>    CStringDetailsA;
typedef CStringDetailsT <wchar_t> CStringDetailsW;
typedef CStringDetailsT <tchar_t> CStringDetails;

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef CStringT <char>    CStringA;
typedef CStringT <wchar_t> CStringW;
typedef CStringT <tchar_t> CString;

//.............................................................................

} // namespace rtl
} // namespace axl

