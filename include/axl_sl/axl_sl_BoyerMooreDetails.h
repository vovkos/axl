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

#define _AXL_SL_BOYERMOOREDETAILS_H

#include "axl_sl_BoyerMooreSkipTables.h"
#include "axl_enc_Utf8.h"
#include "axl_enc_Utf16.h"
#include "axl_enc_Utf32.h"

namespace axl {
namespace sl {

//..............................................................................

template <
	typename C0,
	typename SkipTables0,
	bool IsReverse0,
	size_t BadSkipTableSize0 = 256
>
struct BoyerMooreDetailsBase {
	typedef C0 C;
	typedef SkipTables0 SkipTables;

	enum {
		IsReverse        = IsReverse0,
		BadSkipTableSize = BadSkipTableSize0,
	};
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef BoyerMooreDetailsBase<
	char,
	BoyerMooreBadSkipTableBase<char>,
	false
> BoyerMooreHorspoolBinDetails;

typedef BoyerMooreDetailsBase<
	char,
	BoyerMooreBadSkipTableBase<char>,
	true
> BoyerMooreHorspoolBinReverseDetails;

typedef BoyerMooreDetailsBase<
	char,
	BoyerMooreSkipTablesBase<char>,
	false
> BoyerMooreBinDetails;

typedef BoyerMooreDetailsBase<
	char,
	BoyerMooreSkipTablesBase<char>,
	true
> BoyerMooreBinReverseDetails;

//..............................................................................

template <
	typename SkipTables0,
	typename Encoding0,
	typename Decoder0,
	bool IsReverse0,
	typename CaseOp0,
	bool IsCaseFolded0,
	size_t BadSkipTableSize0 = 256,
	size_t DecoderBufferLength0 = 256
>
struct BoyerMooreTextDetailsBase: BoyerMooreDetailsBase<
	utf32_t,
	SkipTables0,
	IsReverse0,
	BadSkipTableSize0
> {
	typedef Encoding0 Encoding;
	typedef Decoder0 Decoder;
	typedef CaseOp0 CaseOp;

	enum {
		IsCaseFolded        = IsCaseFolded0,
		DecoderBufferLength = DecoderBufferLength0
	};
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef BoyerMooreTextDetailsBase<
	BoyerMooreBadSkipTableBase<utf32_t>,
	enc::Utf8,
	enc::Utf8::Decoder,
	false,
	sl::Nop<utf32_t>,
	false
> BoyerMooreHorspoolTextDetails_utf8;

typedef BoyerMooreTextDetailsBase<
	BoyerMooreBadSkipTableBase<utf32_t>,
	enc::Utf8,
	enc::Utf8::ReverseDecoder,
	true,
	sl::Nop<utf32_t>,
	false
> BoyerMooreHorspoolTextReverseDetails_utf8;

typedef BoyerMooreTextDetailsBase<
	BoyerMooreBadSkipTableBase<utf32_t>,
	enc::Utf8,
	enc::Utf8::Decoder,
	false,
	enc::ToCaseFolded,
	true
> BoyerMooreHorspoolCaseFoldedTextDetails_utf8;

typedef BoyerMooreTextDetailsBase<
	BoyerMooreBadSkipTableBase<utf32_t>,
	enc::Utf8,
	enc::Utf8::ReverseDecoder,
	true,
	enc::ToCaseFolded,
	true
> BoyerMooreHorspoolCaseFoldedTextReverseDetails_utf8;

typedef BoyerMooreTextDetailsBase<
	BoyerMooreSkipTablesBase<utf32_t>,
	enc::Utf8,
	enc::Utf8::Decoder,
	false,
	sl::Nop<utf32_t>,
	false
> BoyerMooreTextDetails_utf8;

typedef BoyerMooreTextDetailsBase<
	BoyerMooreSkipTablesBase<utf32_t>,
	enc::Utf8,
	enc::Utf8::ReverseDecoder,
	true,
	sl::Nop<utf32_t>,
	false
> BoyerMooreTextReverseDetails_utf8;

typedef BoyerMooreTextDetailsBase<
	BoyerMooreSkipTablesBase<utf32_t>,
	enc::Utf8,
	enc::Utf8::Decoder,
	false,
	enc::ToCaseFolded,
	true
> BoyerMooreCaseFoldedTextDetails_utf8;

typedef BoyerMooreTextDetailsBase<
	BoyerMooreSkipTablesBase<utf32_t>,
	enc::Utf8,
	enc::Utf8::ReverseDecoder,
	true,
	enc::ToCaseFolded,
	true
> BoyerMooreCaseFoldedTextReverseDetails_utf8;

//..............................................................................

} // namespace sl
} // namespace axl
