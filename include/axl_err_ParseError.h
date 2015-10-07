// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_err_ErrorMgr.h"
#include "axl_lex_Token.h"
#include "axl_sl_Pack.h"

namespace axl {
namespace err {

//.............................................................................

// {56FC601E-5D2C-4bbe-B455-3D8B81AAD8FF}
AXL_SL_DEFINE_GUID (g_parseErrorGuid, 0x56fc601e, 0x5d2c, 0x4bbe, 0xb4, 0x55, 0x3d, 0x8b, 0x81, 0xaa, 0xd8, 0xff);

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum ParseErrorCode
{
	ParseErrorCode_SrcPos,
	ParseErrorCode_InvalidSyntax,
	ParseErrorCode_InvalidSyntaxIn,
	ParseErrorCode_ExpectedToken,
	ParseErrorCode_UnexpectedToken,
	ParseErrorCode_UnexpectedTokenIn,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class ParseErrorProvider: public ErrorProvider
{
public:
	virtual 
	sl::String 
	getErrorDescription (const ErrorHdr* error);
};

//.............................................................................

// utility

inline 
void 
registerParseErrorProvider ()
{
	getErrorMgr ()->registerProvider (
		g_parseErrorGuid, 
		mt::getSimpleSingleton <ParseErrorProvider> ()
		);
}

inline
Error
pushSrcPosError (
	const char* filePath,
	int line,
	int col = 0
	)
{
	return pushPackError <sl::PackSeq_3 <const char*, int, int> > (
		g_parseErrorGuid,
		ParseErrorCode_SrcPos, 
		filePath, 
		line, 
		col
		);
}

inline
Error
pushSrcPosError (
	const char* filePath,
	const lex::LineCol& lineCol
	)
{
	return pushSrcPosError (filePath, lineCol.m_line, lineCol.m_col);
}

inline
Error
pushSrcPosError (const lex::SrcPos& srcPos)
{
	return pushSrcPosError (srcPos.m_filePath, srcPos.m_line, srcPos.m_col);
}

inline
Error
ensureSrcPosError (
	const char* filePath,
	int line,
	int col = 0
	)
{
	Error error = getLastError ();
	return error->isKind (g_parseErrorGuid, ParseErrorCode_SrcPos) ? error : pushSrcPosError (filePath, line, col);
}

inline
Error
ensureSrcPosError (
	const char* filePath,
	const lex::LineCol& lineCol
	)
{
	return ensureSrcPosError (filePath, lineCol.m_line, lineCol.m_col);
}

inline
Error
ensureSrcPosError (const lex::SrcPos& srcPos)
{
	return ensureSrcPosError (srcPos.m_filePath, srcPos.m_line, srcPos.m_col);
}

inline
Error
setSyntaxError ()
{
	return setError (g_parseErrorGuid, ParseErrorCode_InvalidSyntax);
}

inline
Error
setSyntaxError (const char* location)
{
	return setPackError <sl::Pack <const char*> > (
		g_parseErrorGuid,
		ParseErrorCode_InvalidSyntaxIn, 
		location
		);
}

inline
Error
setExpectedTokenError (
	const char* expectedToken,
	const char* actualToken
	)
{
	return setPackError <sl::PackSeq_2 <const char*, const char*> > (
		g_parseErrorGuid,
		ParseErrorCode_ExpectedToken, 
		expectedToken,
		actualToken
		);
}

inline
Error
setUnexpectedTokenError (const char* token)
{
	return setPackError <sl::Pack <const char*> > (
		g_parseErrorGuid,
		ParseErrorCode_UnexpectedToken, 
		token
		);
}

inline
Error
setUnexpectedTokenError (
	const char* token,
	const char* location
	)
{
	return setPackError <sl::PackSeq_2 <const char*, const char*> > (
		g_parseErrorGuid,
		ParseErrorCode_UnexpectedTokenIn, 
		token,
		location
		);
}

//.............................................................................

} // namespace err
} // namespace axl

