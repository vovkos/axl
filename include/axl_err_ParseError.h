// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
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

class CParseErrorProvider: public CErrorProvider
{
public:
	virtual 
	rtl::CString 
	getErrorDescription (const TError* error);
};

//.............................................................................

// utility

inline 
void 
registerParseErrorProvider ()
{
	getErrorMgr ()->registerProvider (
		GUID_ParseError, 
		rtl::getSimpleSingleton <CParseErrorProvider> ()
		);
}

inline
CError
pushSrcPosError (
	const char* filePath,
	int line,
	int col = 0
	)
{
	return pushPackError <rtl::CPackSeqT_3 <const char*, int, int> > (
		GUID_ParseError,
		EParseError_SrcPos, 
		filePath, 
		line, 
		col
		);
}

inline
CError
pushSrcPosError (
	const char* filePath,
	const lex::CLineCol& lineCol
	)
{
	return pushSrcPosError (filePath, lineCol.m_line, lineCol.m_col);
}

inline
CError
pushSrcPosError (const lex::CSrcPos& srcPos)
{
	return pushSrcPosError (srcPos.m_filePath, srcPos.m_line, srcPos.m_col);
}

inline
CError
ensureSrcPosError (
	const char* filePath,
	int line,
	int col = 0
	)
{
	CError error = getError ();
	return error->isKind (GUID_ParseError, EParseError_SrcPos) ? error : pushSrcPosError (filePath, line, col);
}

inline
CError
ensureSrcPosError (
	const char* filePath,
	const lex::CLineCol& lineCol
	)
{
	return ensureSrcPosError (filePath, lineCol.m_line, lineCol.m_col);
}

inline
CError
ensureSrcPosError (const lex::CSrcPos& srcPos)
{
	return ensureSrcPosError (srcPos.m_filePath, srcPos.m_line, srcPos.m_col);
}

inline
CError
setSyntaxError ()
{
	return setError (GUID_ParseError, EParseError_InvalidSyntax);
}

inline
CError
setSyntaxError (const char* location)
{
	return setPackError <rtl::CPackT <const char*> > (
		GUID_ParseError,
		EParseError_InvalidSyntaxIn, 
		location
		);
}

inline
CError
setExpectedTokenError (
	const char* expectedToken,
	const char* actualToken
	)
{
	return setPackError <rtl::CPackSeqT_2 <const char*, const char*> > (
		GUID_ParseError,
		EParseError_ExpectedToken, 
		expectedToken,
		actualToken
		);
}

inline
CError
setUnexpectedTokenError (const char* token)
{
	return setPackError <rtl::CPackT <const char*> > (
		GUID_ParseError,
		EParseError_UnexpectedToken, 
		token
		);
}

inline
CError
setUnexpectedTokenError (
	const char* token,
	const char* location
	)
{
	return setPackError <rtl::CPackSeqT_2 <const char*, const char*> > (
		GUID_ParseError,
		EParseError_UnexpectedTokenIn, 
		token,
		location
		);
}

//.............................................................................

} // namespace err
} // namespace axl

