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
#include "axl_enc_Unicode.h"

#define INCLUDED_FROM_UCHAR_C
#define INCLUDED_FROM_UCASE_CPP

#include "icu_def.h"
#include "icu_uchar_props_data.h"
#include "icu_ucase_props_data.h"

namespace axl {
namespace enc {

//
// from uchar.c & ucase.cpp
//

//..............................................................................

/* getting a uint32_t properties word from the data */
#define GET_PROPS(c, result) ((result)=UTRIE2_GET16(&propsTrie, c));

#define GET_CASE_PROPS() &ucase_props_singleton

#define GET_EXCEPTIONS(csp, props) ((csp)->exceptions+((props)>>UCASE_EXC_SHIFT))

#define PROPS_HAS_EXCEPTION(props) ((props)&UCASE_EXCEPTION)

/* number of bits in an 8-bit integer value */
static const uint8_t flagsOffset[256]={
	0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
	1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
	1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
	1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
	3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
	1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
	3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
	3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
	3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
	4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8
};

#define HAS_SLOT(flags, idx) ((flags)&(1<<(idx)))
#define SLOT_OFFSET(flags, idx) flagsOffset[(flags)&((1<<(idx))-1)]

/*
 * Get the value of an optional-value slot where HAS_SLOT(excWord, idx).
 *
 * @param excWord(in)initial exceptions word
 * @param idx(in)desired slot index
 * @param pExc16(in/out) const uint16_t * after excWord=*pExc16++;
 *               moved to the last uint16_t of the value, use +1 for beginning of next slot
 * @param value(out)int32_t or uint32_t output if hasSlot, otherwise not modified
 */
#define GET_SLOT_VALUE(excWord, idx, pExc16, value) \
	if(((excWord)&UCASE_EXC_DOUBLE_SLOTS)==0) { \
		(pExc16)+=SLOT_OFFSET(excWord, idx); \
		(value)=*pExc16; \
	} else { \
		(pExc16)+=2*SLOT_OFFSET(excWord, idx); \
		(value)=*pExc16++; \
		(value)=((value)<<16)|*pExc16; \
	}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

bool
isPrintable(utf32_t c) {
	uint32_t props;
	GET_PROPS(c, props);
	return (UBool)((CAT_MASK(props)&U_GC_C_MASK)==0);
}

bool
isPrintableNonMark(utf32_t c) {
	uint32_t props;
	GET_PROPS(c, props);
	/* comparing ==0 returns FALSE for the categories mentioned */
	return (UBool)((CAT_MASK(props)&(U_GC_C_MASK|U_GC_M_MASK))==0);
}

bool
isSpace(utf32_t c) {
	uint32_t props;
	GET_PROPS(c, props);
	return (UBool)((CAT_MASK(props)&U_GC_Z_MASK)!=0 || IS_THAT_CONTROL_SPACE(c));
}

bool
isPunctuation(utf32_t c) {
	uint32_t props;
	GET_PROPS(c, props);
	return (UBool)((CAT_MASK(props)&U_GC_P_MASK)!=0);
}

bool
isLetter(utf32_t c) {
	uint32_t props;
	GET_PROPS(c, props);
	return (UBool)((CAT_MASK(props)&U_GC_L_MASK)!=0);
}

bool
isDigit(utf32_t c) {
	uint32_t props;
	GET_PROPS(c, props);
	return (UBool)(GET_CATEGORY(props)==U_DECIMAL_DIGIT_NUMBER);
}

bool
isNumber(utf32_t c) {
	uint32_t props;
	GET_PROPS(c, props);
	return (UBool)((CAT_MASK(props)&U_GC_N_MASK)!=0);
}

bool
isLetterOrDigit(utf32_t c) {
	uint32_t props;
	GET_PROPS(c, props);
	return (UBool)((CAT_MASK(props)&(U_GC_L_MASK|U_GC_ND_MASK))!=0);
}

bool
isLetterOrNumber(utf32_t c) {
	uint32_t props;
	GET_PROPS(c, props);
	return (UBool)((CAT_MASK(props)&(U_GC_L_MASK|U_GC_N_MASK))!=0);
}

bool
isLowerCase(utf32_t c) {
	uint32_t props;
	GET_PROPS(c, props);
	return (UBool)(GET_CATEGORY(props)==U_LOWERCASE_LETTER);
}

bool
isUpperCase(utf32_t c) {
	uint32_t props;
	GET_PROPS(c, props);
	return (UBool)(GET_CATEGORY(props)==U_UPPERCASE_LETTER);
}

utf32_t
toLowerCase(utf32_t c) {
	const UCaseProps *csp = GET_CASE_PROPS();
	uint16_t props=UTRIE2_GET16(&csp->trie, c);
	if(!PROPS_HAS_EXCEPTION(props)) {
		if(UCASE_GET_TYPE(props)>=UCASE_UPPER) {
			c+=UCASE_GET_DELTA(props);
		}
	} else {
		const uint16_t *pe=GET_EXCEPTIONS(csp, props);
		uint16_t excWord=*pe++;
		if(HAS_SLOT(excWord, UCASE_EXC_LOWER)) {
			GET_SLOT_VALUE(excWord, UCASE_EXC_LOWER, pe, c);
		}
	}
	return c;
}

utf32_t
toUpperCase(utf32_t c) {
	const UCaseProps *csp = GET_CASE_PROPS();
	uint16_t props=UTRIE2_GET16(&csp->trie, c);
	if(!PROPS_HAS_EXCEPTION(props)) {
		if(UCASE_GET_TYPE(props)==UCASE_LOWER) {
			c+=UCASE_GET_DELTA(props);
		}
	} else {
		const uint16_t *pe=GET_EXCEPTIONS(csp, props);
		uint16_t excWord=*pe++;
		if(HAS_SLOT(excWord, UCASE_EXC_UPPER)) {
			GET_SLOT_VALUE(excWord, UCASE_EXC_UPPER, pe, c);
		}
	}
	return c;
}

utf32_t
toCaseFolded(utf32_t c) {
	uint32_t options = U_FOLD_CASE_DEFAULT;

	const UCaseProps *csp = GET_CASE_PROPS();
	uint16_t props=UTRIE2_GET16(&csp->trie, c);
	if(!PROPS_HAS_EXCEPTION(props)) {
		if(UCASE_GET_TYPE(props)>=UCASE_UPPER) {
			c+=UCASE_GET_DELTA(props);
		}
	} else {
		const uint16_t *pe=GET_EXCEPTIONS(csp, props);
		uint16_t excWord=*pe++;
		int32_t idx;
		if(excWord&UCASE_EXC_CONDITIONAL_FOLD) {
			/* special case folding mappings, hardcoded */
			if((options&_FOLD_CASE_OPTIONS_MASK)==U_FOLD_CASE_DEFAULT) {
				/* default mappings */
				if(c==0x49) {
					/* 0049; C; 0069; # LATIN CAPITAL LETTER I */
					return 0x69;
				} else if(c==0x130) {
					/* no simple case folding for U+0130 */
					return c;
				}
			} else {
				/* Turkic mappings */
				if(c==0x49) {
					/* 0049; T; 0131; # LATIN CAPITAL LETTER I */
					return 0x131;
				} else if(c==0x130) {
					/* 0130; T; 0069; # LATIN CAPITAL LETTER I WITH DOT ABOVE */
					return 0x69;
				}
			}
		}
		if(HAS_SLOT(excWord, UCASE_EXC_FOLD)) {
			idx=UCASE_EXC_FOLD;
		} else if(HAS_SLOT(excWord, UCASE_EXC_LOWER)) {
			idx=UCASE_EXC_LOWER;
		} else {
			return c;
		}
		GET_SLOT_VALUE(excWord, idx, pe, c);
	}
	return c;
}

//..............................................................................

} // namespace enc
} // namespace axl
