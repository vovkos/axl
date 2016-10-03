// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LEX_PARSEERROR_H

#include "axl_lex_Token.h"

namespace axl {
namespace lex {

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

class ParseErrorProvider: public err::ErrorProvider
{
public:
	virtual 
	sl::String 
	getErrorDescription (const err::ErrorRef& error);
};

//.............................................................................

// utility

inline 
void 
registerParseErrorProvider ()
{
	err::getErrorMgr ()->registerProvider (
		g_parseErrorGuid, 
		sl::getSimpleSingleton <ParseErrorProvider> ()
		);
}

inline
size_t
pushSrcPosError (
	const char* filePath,
	int line,
	int col = 0
	)
{
	return err::pushPackError <sl::PackSeq_3 <const char*, int, int> > (
		g_parseErrorGuid,
		ParseErrorCode_SrcPos, 
		filePath, 
		line, 
		col
		);
}

inline
size_t
pushSrcPosError (
	const char* filePath,
	const LineCol& lineCol
	)
{
	return pushSrcPosError (filePath, lineCol.m_line, lineCol.m_col);
}

inline
size_t
pushSrcPosError (const SrcPos& srcPos)
{
	return pushSrcPosError (srcPos.m_filePath, srcPos.m_line, srcPos.m_col);
}

inline
bool
isLastSrcPosError ()
{
	return err::getLastError ()->isStackTopKindOf (g_parseErrorGuid, ParseErrorCode_SrcPos);
}

inline
void
ensureSrcPosError (
	const char* filePath,
	int line,
	int col = 0
	)
{
	if (!isLastSrcPosError ())
		pushSrcPosError (filePath, line, col);
}

inline
void
ensureSrcPosError (
	const char* filePath,
	const LineCol& lineCol
	)
{
	ensureSrcPosError (filePath, lineCol.m_line, lineCol.m_col);
}

inline
void
ensureSrcPosError (const SrcPos& srcPos)
{
	ensureSrcPosError (srcPos.m_filePath, srcPos.m_line, srcPos.m_col);
}

inline
size_t
setSyntaxError ()
{
	return err::setError (g_parseErrorGuid, ParseErrorCode_InvalidSyntax);
}

inline
size_t
setSyntaxError (const char* location)
{
	return err::setPackError <sl::Pack <const char*> > (
		g_parseErrorGuid,
		ParseErrorCode_InvalidSyntaxIn, 
		location
		);
}

inline
size_t
setExpectedTokenError (
	const char* expectedToken,
	const char* actualToken
	)
{
	return err::setPackError <sl::PackSeq_2 <const char*, const char*> > (
		g_parseErrorGuid,
		ParseErrorCode_ExpectedToken, 
		expectedToken,
		actualToken
		);
}

inline
size_t
setUnexpectedTokenError (const char* token)
{
	return err::setPackError <sl::Pack <const char*> > (
		g_parseErrorGuid,
		ParseErrorCode_UnexpectedToken, 
		token
		);
}

inline
size_t
setUnexpectedTokenError (
	const char* token,
	const char* location
	)
{
	return err::setPackError <sl::PackSeq_2 <const char*, const char*> > (
		g_parseErrorGuid,
		ParseErrorCode_UnexpectedTokenIn, 
		token,
		location
		);
}

//.............................................................................

} // namespace lex
} // namespace axl

