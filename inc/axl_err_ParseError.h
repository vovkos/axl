// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2010. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_err_ErrorMgr.h"
#include "axl_lex_Token.h"
#include "axl_rtl_Pack.h"

namespace axl {
namespace err {

//.............................................................................

// {56FC601E-5D2C-4bbe-B455-3D8B81AAD8FF}
AXL_DEFINE_GUID (GUID_ParseErrorA, 0x56fc601e, 0x5d2c, 0x4bbe, 0xb4, 0x55, 0x3d, 0x8b, 0x81, 0xaa, 0xd8, 0xff);

// {EDE23520-CAD0-4F61-8BAD-071520CCC3C6}
AXL_DEFINE_GUID (GUID_ParseErrorW, 0xede23520, 0xcad0, 0x4f61, 0x8b, 0xad, 0x07, 0x15, 0x20, 0xcc, 0xc3, 0xc6);

//.............................................................................

enum EParseError
{
	EParseError_SrcPos,
	EParseError_InvalidSyntax,
	EParseError_InvalidSyntaxIn,
	EParseError_ExpectedToken,
	EParseError_UnexpectedToken,
	EParseError_UnexpectedTokenIn,
};

//.............................................................................

class CParseErrorProviderA: public IErrorProvider
{
public:
	AXL_OBJ_SIMPLE_CLASS (CParseErrorProviderA, IErrorProvider)

public:
	virtual 
	rtl::CString 
	GetErrorDescription (const TError* pError);

	static 
	void 
	Register ()
	{
		GetErrorMgr ()->RegisterProvider (
			GUID_ParseErrorA, 
			rtl::GetSimpleSingleton <CParseErrorProviderA> ()
			);
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CParseErrorProviderW: public IErrorProvider
{
public:
	AXL_OBJ_SIMPLE_CLASS (CParseErrorProviderW, IErrorProvider)

public:
	virtual 
	rtl::CString 
	GetErrorDescription (const TError* pError);

	static 
	void 
	Register ()
	{
		GetErrorMgr ()->RegisterProvider (
			GUID_ParseErrorW, 
			rtl::GetSimpleSingleton <CParseErrorProviderW> ()
			);
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#ifdef _UNICODE
#define GUID_ParseError GUID_ParseErrorW
typedef CParseErrorProviderW CParseErrorProvider;
#else
#define GUID_ParseError GUID_ParseErrorA
typedef CParseErrorProviderA CParseErrorProvider;
#endif

//.............................................................................

// utility

inline
CError
PushSrcPosError (
	const tchar_t* pFilePath,
	long Line,
	long Col = 0
	)
{
	return PushPackError <rtl::CPackSeqT_3 <const tchar_t*, long, long> > (
		GUID_ParseError,
		EParseError_SrcPos, 
		pFilePath, 
		Line, 
		Col
		);
}

inline
CError
PushSrcPosError (const lex::CSrcPos& SrcPos)
{
	return PushSrcPosError (SrcPos.m_FilePath, SrcPos.m_Line, SrcPos.m_Col);
}

inline
CError
SetSyntaxError ()
{
	return SetError (GUID_ParseError, EParseError_InvalidSyntax);
}

inline
CError
SetSyntaxError (const tchar_t* pLocation)
{
	return SetPackError <rtl::CPackT <const tchar_t*> > (
		GUID_ParseError,
		EParseError_InvalidSyntaxIn, 
		pLocation
		);
}

inline
CError
SetExpectedTokenError (
	const tchar_t* pExpectedToken,
	const tchar_t* pActualToken
	)
{
	return SetPackError <rtl::CPackSeqT_2 <const tchar_t*, const tchar_t*> > (
		GUID_ParseError,
		EParseError_ExpectedToken, 
		pExpectedToken,
		pActualToken
		);
}

inline
CError
SetUnexpectedTokenError (const tchar_t* pToken)
{
	return SetPackError <rtl::CPackT <const tchar_t*> > (
		GUID_ParseError,
		EParseError_UnexpectedToken, 
		pToken
		);
}

inline
CError
SetUnexpectedTokenError (
	const tchar_t* pToken,
	const tchar_t* pLocation
	)
{
	return SetPackError <rtl::CPackSeqT_2 <const tchar_t*, const tchar_t*> > (
		GUID_ParseError,
		EParseError_UnexpectedTokenIn, 
		pToken,
		pLocation
		);
}

//.............................................................................

} // namespace err
} // namespace axl

