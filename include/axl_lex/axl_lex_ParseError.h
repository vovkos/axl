//..............................................................................
//
//  This file is part of the AXL library.
//
//  AXL is distributed under the MIT license.
//  For details see accompanying license.txt file,
//  the public copy of which is also available at:
//  http://tibbo.com/downloads/archive/axl/license.txt
//
//..............................................................................

#pragma once

#define _AXL_LEX_PARSEERROR_H

#include "axl_lex_Token.h"

namespace axl {
namespace lex {

//..............................................................................

// {56FC601E-5D2C-4bbe-B455-3D8B81AAD8FF}
AXL_SL_DEFINE_GUID(g_parseErrorGuid, 0x56fc601e, 0x5d2c, 0x4bbe, 0xb4, 0x55, 0x3d, 0x8b, 0x81, 0xaa, 0xd8, 0xff);

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum ParseErrorCode
{
	ParseErrorCode_SrcPos,
	ParseErrorCode_InvalidSyntax,
	ParseErrorCode_InvalidSyntaxIn,
	ParseErrorCode_ExpectedToken,
	ParseErrorCode_UnexpectedToken,
	ParseErrorCode_UnexpectedTokenIn,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class ParseErrorProvider: public err::ErrorProvider
{
public:
	virtual
	sl::StringRef
	getErrorDescription(const err::ErrorRef& error);
};

//..............................................................................

// utility

inline
void
registerParseErrorProvider()
{
	err::getErrorMgr()->registerProvider(
		g_parseErrorGuid,
		sl::getSimpleSingleton<ParseErrorProvider> ()
		);
}

inline
size_t
pushSrcPosError(
	const sl::StringRef& filePath,
	int line,
	int col = 0
	)
{
	return err::pushPackError<sl::PackSeq_3<const char*, int, int> > (
		g_parseErrorGuid,
		ParseErrorCode_SrcPos,
		filePath.sz(),
		line,
		col
		);
}

inline
size_t
pushSrcPosError(
	const sl::StringRef& filePath,
	const LineCol& lineCol
	)
{
	return pushSrcPosError(filePath, lineCol.m_line, lineCol.m_col);
}

inline
size_t
pushSrcPosError(const SrcPos& srcPos)
{
	return pushSrcPosError(srcPos.m_filePath, srcPos.m_line, srcPos.m_col);
}

inline
bool
isSrcPosError(const err::ErrorHdr* error)
{
	return
		error->m_size >= sizeof(err::ErrorHdr)* 2 + sizeof(char) + sizeof(int)* 2 &&
		error->isStackTopKindOf(g_parseErrorGuid, ParseErrorCode_SrcPos);
}

void
decodeSrcPosError(
	sl::StringRef* filePath,
	LineCol* lineCol,
	const err::ErrorHdr* error
	);

inline
void
decodeSrcPosError(
	LineCol* lineCol,
	const err::ErrorHdr* error
	)
{
	decodeSrcPosError(NULL, lineCol, error);
}

inline
void
decodeSrcPosError(
	SrcPos* srcPos,
	const err::ErrorHdr* error
	)
{
	decodeSrcPosError(&srcPos->m_filePath, srcPos, error);
}

inline
bool
isLastSrcPosError()
{
	return isSrcPosError(err::getLastError());
}

inline
void
ensureSrcPosError(
	const sl::StringRef& filePath,
	int line,
	int col = 0
	)
{
	if (!isLastSrcPosError())
		pushSrcPosError(filePath, line, col);
}

inline
void
ensureSrcPosError(
	const sl::StringRef& filePath,
	const LineCol& lineCol
	)
{
	ensureSrcPosError(filePath, lineCol.m_line, lineCol.m_col);
}

inline
void
ensureSrcPosError(const SrcPos& srcPos)
{
	ensureSrcPosError(srcPos.m_filePath, srcPos.m_line, srcPos.m_col);
}

inline
size_t
setSyntaxError()
{
	return err::setError(g_parseErrorGuid, ParseErrorCode_InvalidSyntax);
}

inline
size_t
setSyntaxError(const sl::StringRef& location)
{
	return err::setPackError<sl::Pack<const char*> > (
		g_parseErrorGuid,
		ParseErrorCode_InvalidSyntaxIn,
		location.sz()
		);
}

inline
size_t
setExpectedTokenError(
	const sl::StringRef& expectedToken,
	const sl::StringRef& actualToken
	)
{
	return err::setPackError<sl::PackSeq_2<const char*, const char*> > (
		g_parseErrorGuid,
		ParseErrorCode_ExpectedToken,
		expectedToken.sz(),
		actualToken.sz()
		);
}

inline
size_t
setUnexpectedTokenError(const sl::StringRef& token)
{
	return err::setPackError<sl::Pack<const char*> > (
		g_parseErrorGuid,
		ParseErrorCode_UnexpectedToken,
		token.sz()
		);
}

inline
size_t
setUnexpectedTokenError(
	const sl::StringRef& token,
	const sl::StringRef& location
	)
{
	return err::setPackError<sl::PackSeq_2<const char*, const char*> > (
		g_parseErrorGuid,
		ParseErrorCode_UnexpectedTokenIn,
		token.sz(),
		location.sz()
		);
}

//..............................................................................

} // namespace lex
} // namespace axl
