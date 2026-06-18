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

#include "pch.h"
#include "test.h"
#include "axl_enc_Unicode.h"

namespace {

//..............................................................................

// bit per enc:: predicate; expected masks below are oracle values from the
// Unicode Character Database (general category / East_Asian_Width)

enum Prop {
	Pr   = 0x0001, // isPrintable
	PrNM = 0x0002, // isPrintableNonMark
	Mk   = 0x0004, // isMark
	Sp   = 0x0008, // isSpace
	Pu   = 0x0010, // isPunctuation
	Sy   = 0x0020, // isSymbol
	Le   = 0x0040, // isLetter
	Di   = 0x0080, // isDigit
	Nu   = 0x0100, // isNumber
	LD   = 0x0200, // isLetterOrDigit
	LN   = 0x0400, // isLetterOrNumber
	Wd   = 0x0800, // isWord
	Lo   = 0x1000, // isLowerCase
	Up   = 0x2000, // isUpperCase
	Dw   = 0x4000, // isDoubleWidth
};

uint_t
calcProps(utf32_t c) {
	return
		(enc::isPrintable(c)        ? Pr   : 0) |
		(enc::isPrintableNonMark(c) ? PrNM : 0) |
		(enc::isMark(c)             ? Mk   : 0) |
		(enc::isSpace(c)            ? Sp   : 0) |
		(enc::isPunctuation(c)      ? Pu   : 0) |
		(enc::isSymbol(c)           ? Sy   : 0) |
		(enc::isLetter(c)           ? Le   : 0) |
		(enc::isDigit(c)            ? Di   : 0) |
		(enc::isNumber(c)           ? Nu   : 0) |
		(enc::isLetterOrDigit(c)    ? LD   : 0) |
		(enc::isLetterOrNumber(c)   ? LN   : 0) |
		(enc::isWord(c)             ? Wd   : 0) |
		(enc::isLowerCase(c)        ? Lo   : 0) |
		(enc::isUpperCase(c)        ? Up   : 0) |
		(enc::isDoubleWidth(c)      ? Dw   : 0);
}

void testProps() {
	struct Row {
		utf32_t m_cp;
		uint_t m_expected;
		const char* m_desc;
	} table[] = {
		{ 0x0041, Pr|PrNM|Le|LD|LN|Wd|Up, "'A' Lu"                  },
		{ 0x0061, Pr|PrNM|Le|LD|LN|Wd|Lo, "'a' Ll"                  },
		{ 0x0037, Pr|PrNM|Di|Nu|LD|LN|Wd, "'7' Nd"                  },
		{ 0x0020, Pr|PrNM|Sp,             "space Zs"                },
		{ 0x0009, Sp,                     "TAB Cc (control-space)"  },
		{ 0x000a, Sp,                     "LF Cc (control-space)"   },
		{ 0x00a0, Pr|PrNM|Sp,             "NBSP Zs"                 },
		{ 0x005f, Pr|PrNM|Pu|Wd,          "'_' Pc (word)"           },
		{ 0x002e, Pr|PrNM|Pu,             "'.' Po"                  },
		{ 0x002b, Pr|PrNM|Sy,             "'+' Sm"                  },
		{ 0x0024, Pr|PrNM|Sy,             "'$' Sc"                  },
		{ 0x00e9, Pr|PrNM|Le|LD|LN|Wd|Lo, "e-acute Ll"             },
		{ 0x00c9, Pr|PrNM|Le|LD|LN|Wd|Up, "E-acute Lu"             },
		{ 0x00df, Pr|PrNM|Le|LD|LN|Wd|Lo, "sharp-s Ll"             },
		{ 0x1e9e, Pr|PrNM|Le|LD|LN|Wd|Up, "capital sharp-s Lu"     },
		{ 0x0130, Pr|PrNM|Le|LD|LN|Wd|Up, "I-with-dot Lu"          },
		{ 0x0131, Pr|PrNM|Le|LD|LN|Wd|Lo, "dotless-i Ll"           },
		{ 0x01c5, Pr|PrNM|Le|LD|LN|Wd,    "Dz-caron Lt (title)"    },
		{ 0x0410, Pr|PrNM|Le|LD|LN|Wd|Up, "Cyrillic A Lu"          },
		{ 0x0430, Pr|PrNM|Le|LD|LN|Wd|Lo, "Cyrillic a Ll"          },
		{ 0x00b5, Pr|PrNM|Le|LD|LN|Wd|Lo, "micro sign Ll"          },
		{ 0x212a, Pr|PrNM|Le|LD|LN|Wd|Up, "Kelvin sign Lu"         },
		{ 0x0301, Pr|Mk,                  "combining acute Mn"     },
		{ 0x4f60, Pr|PrNM|Le|LD|LN|Wd|Dw, "CJK U+4F60 Lo (wide)"   },
		{ 0x1100, Pr|PrNM|Le|LD|LN|Wd|Dw, "Hangul Jamo Lo (wide)"  },
		{ 0x3000, Pr|PrNM|Sp|Dw,          "ideographic space (wide)" },
		{ 0xff21, Pr|PrNM|Le|LD|LN|Wd|Up|Dw, "fullwidth A (wide)"  },
		{ 0x00a5, Pr|PrNM|Sy,             "yen sign Sc (narrow)"   },
		{ 0x1f600, Pr|PrNM|Sy|Dw,         "emoji grin (wide)"      },
		{ 0x1f300, Pr|PrNM|Sy|Dw,         "emoji cyclone (wide)"   },
		{ 0x1f680, Pr|PrNM|Sy|Dw,         "emoji rocket (wide)"    },
		{ 0x1f9c0, Pr|PrNM|Sy|Dw,         "emoji cheese (wide)"    },
		{ 0x10400, Pr|PrNM|Le|LD|LN|Wd|Up, "Deseret cap (SMP Lu)"  },
		{ 0x10428, Pr|PrNM|Le|LD|LN|Wd|Lo, "Deseret small (SMP Ll)" },
	};

	for (size_t i = 0; i < countof(table); i++) {
		uint_t actual = calcProps(table[i].m_cp);
		printf(
			"U+%05X props: 0x%04x expected: 0x%04x  (%s)\n",
			table[i].m_cp, actual, table[i].m_expected, table[i].m_desc
		);
		TEST_ASSERT(actual == table[i].m_expected);
	}
}

//..............................................................................

void testCaseMapping() {
	struct Row {
		utf32_t m_cp;
		utf32_t m_lower;
		utf32_t m_upper;
		utf32_t m_fold;
		const char* m_desc;
	} table[] = {
		//  code      lower     upper     fold
		{ 0x0041,   0x0061,   0x0041,   0x0061,   "'A'"                   },
		{ 0x0061,   0x0061,   0x0041,   0x0061,   "'a'"                   },
		{ 0x0037,   0x0037,   0x0037,   0x0037,   "'7' (no case)"         },
		{ 0x00e9,   0x00e9,   0x00c9,   0x00e9,   "e-acute"               },
		{ 0x00c9,   0x00e9,   0x00c9,   0x00e9,   "E-acute"               },
		{ 0x00df,   0x00df,   0x00df,   0x00df,   "sharp-s (simple=self)" },
		{ 0x1e9e,   0x00df,   0x1e9e,   0x00df,   "capital sharp-s"       },
		{ 0x0049,   0x0069,   0x0049,   0x0069,   "'I'"                   },
		{ 0x0130,   0x0069,   0x0130,   0x0130,   "I-with-dot (fold=self)" },
		{ 0x0131,   0x0131,   0x0049,   0x0131,   "dotless-i"             },
		{ 0x01c5,   0x01c6,   0x01c4,   0x01c6,   "Dz-caron titlecase"    },
		{ 0x0410,   0x0430,   0x0410,   0x0430,   "Cyrillic A"            },
		{ 0x0430,   0x0430,   0x0410,   0x0430,   "Cyrillic a"            },
		{ 0x00b5,   0x00b5,   0x039c,   0x03bc,   "micro -> MU / mu"      },
		{ 0x212a,   0x006b,   0x212a,   0x006b,   "Kelvin -> k"           },
		{ 0x4f60,   0x4f60,   0x4f60,   0x4f60,   "CJK (no case)"         },
		{ 0x1f600,  0x1f600,  0x1f600,  0x1f600,  "emoji (no case)"       },
		{ 0x10400,  0x10428,  0x10400,  0x10428,  "Deseret SMP cap"       },
		{ 0x10428,  0x10428,  0x10400,  0x10428,  "Deseret SMP small"     },
	};

	for (size_t i = 0; i < countof(table); i++) {
		utf32_t lower = enc::toLowerCase(table[i].m_cp);
		utf32_t upper = enc::toUpperCase(table[i].m_cp);
		utf32_t fold  = enc::toCaseFolded(table[i].m_cp);
		printf(
			"U+%05X lower:U+%05X upper:U+%05X fold:U+%05X  (%s)\n",
			table[i].m_cp, lower, upper, fold, table[i].m_desc
		);
		TEST_ASSERT(lower == table[i].m_lower);
		TEST_ASSERT(upper == table[i].m_upper);
		TEST_ASSERT(fold  == table[i].m_fold);
	}
}

//..............................................................................

void run() {
	testProps();
	testCaseMapping();
}

//..............................................................................

ADD_TEST_CASE("test_Unicode", run)

} // namespace
