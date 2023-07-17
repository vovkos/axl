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
#include "axl_enc_Ascii.h"

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
> BoyerMooreHorspoolReverseBinDetails;

typedef BoyerMooreDetailsBase<
	char,
	BoyerMooreSkipTablesBase<char>,
	false
> BoyerMooreBinDetails;

typedef BoyerMooreDetailsBase<
	char,
	BoyerMooreSkipTablesBase<char>,
	true
> BoyerMooreReverseBinDetails;

//..............................................................................

template <
	typename SkipTables0,
	typename Encoding0,
	typename Decoder0,
	bool IsReverse0,
	typename CaseOp0,
	bool IsCaseFolded0,
	bool IsWholeWord0,
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
		IsWholeWord         = IsWholeWord0,
		DecoderBufferLength = DecoderBufferLength0
	};
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename SkipTables,
	typename Encoding
>
struct BoyerMooreTextDetailsImpl {
	typedef BoyerMooreTextDetailsBase<
		SkipTables,
		Encoding,
		typename Encoding::Decoder,
		false,
		sl::Nop<utf32_t>,
		false,
		false
	> Details;

	typedef BoyerMooreTextDetailsBase<
		SkipTables,
		Encoding,
		typename Encoding::ReverseDecoder,
		true,
		sl::Nop<utf32_t>,
		false,
		false
	> ReverseDetails;

	typedef BoyerMooreTextDetailsBase<
		SkipTables,
		Encoding,
		typename Encoding::Decoder,
		false,
		sl::Nop<utf32_t>,
		false,
		true
	> WholeWordDetails;

	typedef BoyerMooreTextDetailsBase<
		SkipTables,
		Encoding,
		typename Encoding::ReverseDecoder,
		true,
		sl::Nop<utf32_t>,
		false,
		true
	> WholeWordReverseDetails;

	typedef BoyerMooreTextDetailsBase<
		SkipTables,
		Encoding,
		typename Encoding::Decoder,
		false,
		enc::ToCaseFolded,
		true,
		false
	> CaseFoldedDetails;

	typedef BoyerMooreTextDetailsBase<
		SkipTables,
		Encoding,
		typename Encoding::ReverseDecoder,
		true,
		enc::ToCaseFolded,
		true,
		false
	> CaseFoldedReverseDetails;

	typedef BoyerMooreTextDetailsBase<
		SkipTables,
		Encoding,
		typename Encoding::Decoder,
		false,
		enc::ToCaseFolded,
		true,
		true
	> CaseFoldedWholeWordDetails;

	typedef BoyerMooreTextDetailsBase<
		SkipTables,
		Encoding,
		typename Encoding::ReverseDecoder,
		true,
		enc::ToCaseFolded,
		true,
		true
	> CaseFoldedWholeWordReverseDetails;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef BoyerMooreTextDetailsImpl<BoyerMooreBadSkipTableBase<utf32_t>, enc::Utf8>  BoyerMooreHorspoolTextDetails_utf8;
typedef BoyerMooreTextDetailsImpl<BoyerMooreBadSkipTableBase<utf32_t>, enc::Utf16> BoyerMooreHorspoolTextDetails_utf16;
typedef BoyerMooreTextDetailsImpl<BoyerMooreBadSkipTableBase<utf32_t>, enc::Utf32> BoyerMooreHorspoolTextDetails_utf32;
typedef BoyerMooreTextDetailsImpl<BoyerMooreBadSkipTableBase<utf32_t>, enc::Ascii> BoyerMooreHorspoolTextDetails_ascii;

typedef BoyerMooreTextDetailsImpl<BoyerMooreSkipTablesBase<utf32_t>, enc::Utf8>  BoyerMooreTextDetails_utf8;
typedef BoyerMooreTextDetailsImpl<BoyerMooreSkipTablesBase<utf32_t>, enc::Utf16> BoyerMooreTextDetails_utf16;
typedef BoyerMooreTextDetailsImpl<BoyerMooreSkipTablesBase<utf32_t>, enc::Utf32> BoyerMooreTextDetails_utf32;
typedef BoyerMooreTextDetailsImpl<BoyerMooreSkipTablesBase<utf32_t>, enc::Ascii> BoyerMooreTextDetails_ascii;

//..............................................................................

} // namespace sl
} // namespace axl
