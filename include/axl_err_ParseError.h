// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_err_ErrorMgr.h"
#include "axl_lex_Token.h"
#include "axl_rtl_Pack.h"

namespace axl {
namespace err {

//.............................................................................

// {56FC601E-5D2C-4bbe-B455-3D8B81AAD8FF}
AXL_RTL_DEFINE_GUID (GUID_ParseError, 0x56fc601e, 0x5d2c, 0x4bbe, 0xb4, 0x55, 0x3d, 0x8b, 0x81, 0xaa, 0xd8, 0xff);

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum EParseError
{
	EParseError_SrcPos,
	EParseError_InvalidSyntax,
	EParseError_InvalidSyntaxIn,
	EParseError_ExpectedToken,
	EParseError_UnexpectedToken,
	EParseError_UnexpectedTokenIn,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CParseErrorProvider: public IErrorProvider
{
public:
	virtual 
	rtl::CString 
	GetErrorDescription (const TError* pError);

	static 
	void 
	Register ()
	{
		GetErrorMgr ()->RegisterProvider (
			GUID_ParseError, 
			rtl::GetSimpleSingleton <CParseErrorProvider> ()
			);
	}
};

//.............................................................................

// utility

inline
CError
PushSrcPosError (
	const char* pFilePath,
	int Line,
	int Col = 0
	)
{
	return PushPackError <rtl::CPackSeqT_3 <const char*, int, int> > (
		GUID_ParseError,
		EParseError_SrcPos, 
		pFilePath, 
		Line, 
		Col
		);
}

inline
CError
PushSrcPosError (
	const char* pFilePath,
	const lex::CLineCol& LineCol
	)
{
	return PushSrcPosError (pFilePath, LineCol.m_Line, LineCol.m_Col);
}

inline
CError
PushSrcPosError (const lex::CSrcPos& SrcPos)
{
	return PushSrcPosError (SrcPos.m_FilePath, SrcPos.m_Line, SrcPos.m_Col);
}

inline
CError
EnsureSrcPosError (
	const char* pFilePath,
	int Line,
	int Col = 0
	)
{
	CError Error = GetError ();
	return Error->IsKind (GUID_ParseError, EParseError_SrcPos) ? Error : PushSrcPosError (pFilePath, Line, Col);
}

inline
CError
EnsureSrcPosError (
	const char* pFilePath,
	const lex::CLineCol& LineCol
	)
{
	return EnsureSrcPosError (pFilePath, LineCol.m_Line, LineCol.m_Col);
}

inline
CError
EnsureSrcPosError (const lex::CSrcPos& SrcPos)
{
	return EnsureSrcPosError (SrcPos.m_FilePath, SrcPos.m_Line, SrcPos.m_Col);
}

inline
CError
SetSyntaxError ()
{
	return SetError (GUID_ParseError, EParseError_InvalidSyntax);
}

inline
CError
SetSyntaxError (const char* pLocation)
{
	return SetPackError <rtl::CPackT <const char*> > (
		GUID_ParseError,
		EParseError_InvalidSyntaxIn, 
		pLocation
		);
}

inline
CError
SetExpectedTokenError (
	const char* pExpectedToken,
	const char* pActualToken
	)
{
	return SetPackError <rtl::CPackSeqT_2 <const char*, const char*> > (
		GUID_ParseError,
		EParseError_ExpectedToken, 
		pExpectedToken,
		pActualToken
		);
}

inline
CError
SetUnexpectedTokenError (const char* pToken)
{
	return SetPackError <rtl::CPackT <const char*> > (
		GUID_ParseError,
		EParseError_UnexpectedToken, 
		pToken
		);
}

inline
CError
SetUnexpectedTokenError (
	const char* pToken,
	const char* pLocation
	)
{
	return SetPackError <rtl::CPackSeqT_2 <const char*, const char*> > (
		GUID_ParseError,
		EParseError_UnexpectedTokenIn, 
		pToken,
		pLocation
		);
}

//.............................................................................

} // namespace err
} // namespace axl

