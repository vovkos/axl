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

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
// from umachine.h
//

/*==========================================================================*/
/* Boolean data type                                                        */
/*==========================================================================*/

/** The ICU boolean type @stable ICU 2.0 */
typedef int8_t UBool;

#ifndef TRUE
/** The TRUE value of a UBool @stable ICU 2.0 */
#   define TRUE  1
#endif
#ifndef FALSE
/** The FALSE value of a UBool @stable ICU 2.0 */
#   define FALSE 0
#endif

/*==========================================================================*/
/* Unicode data types                                                       */
/*==========================================================================*/

/* UChar and UChar32 definitions -------------------------------------------- */

/** Number of bytes in a UChar. @stable ICU 2.0 */
#define U_SIZEOF_UCHAR 2

/**
 * \var UChar
 * Define UChar to be UCHAR_TYPE, if that is #defined (for example, to char16_t),
 * or wchar_t if that is 16 bits wide; always assumed to be unsigned.
 * If neither is available, then define UChar to be uint16_t.
 *
 * This makes the definition of UChar platform-dependent
 * but allows direct string type compatibility with platforms with
 * 16-bit wchar_t types.
 *
 * @stable ICU 4.4
 */
#if defined(UCHAR_TYPE)
	typedef UCHAR_TYPE UChar;
/* Not #elif U_HAVE_CHAR16_T -- because that is type-incompatible with pre-C++11 callers
	typedef char16_t UChar;  */
#elif U_SIZEOF_WCHAR_T==2
	typedef wchar_t UChar;
#elif defined(__CHAR16_TYPE__)
	typedef __CHAR16_TYPE__ UChar;
#else
	typedef uint16_t UChar;
#endif

/**
 * Define UChar32 as a type for single Unicode code points.
 * UChar32 is a signed 32-bit integer (same as int32_t).
 *
 * The Unicode code point range is 0..0x10ffff.
 * All other values (negative or >=0x110000) are illegal as Unicode code points.
 * They may be used as sentinel values to indicate "done", "error"
 * or similar non-code point conditions.
 *
 * Before ICU 2.4 (Jitterbug 2146), UChar32 was defined
 * to be wchar_t if that is 32 bits wide (wchar_t may be signed or unsigned)
 * or else to be uint32_t.
 * That is, the definition of UChar32 was platform-dependent.
 *
 * @see U_SENTINEL
 * @stable ICU 2.4
 */
typedef int32_t UChar32;

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
// from putilimp.h
//

/**
 * \def U_SIGNED_RIGHT_SHIFT_IS_ARITHMETIC
 * Nearly all CPUs and compilers implement a right-shift of a signed integer
 * as an Arithmetic Shift Right which copies the sign bit (the Most Significant Bit (MSB))
 * into the vacated bits (sign extension).
 * For example, (int32_t)0xfff5fff3>>4 becomes 0xffff5fff and -1>>1=-1.
 *
 * This can be useful for storing a signed value in the upper bits
 * and another bit field in the lower bits.
 * The signed value can be retrieved by simple right-shifting.
 *
 * This is consistent with the Java language.
 *
 * However, the C standard allows compilers to implement a right-shift of a signed integer
 * as a Logical Shift Right which copies a 0 into the vacated bits.
 * For example, (int32_t)0xfff5fff3>>4 becomes 0x0fff5fff and -1>>1=0x7fffffff.
 *
 * Code that depends on the natural behavior should be guarded with this macro,
 * with an alternate path for unusual platforms.
 * @internal
 */
#ifdef U_SIGNED_RIGHT_SHIFT_IS_ARITHMETIC
	/* Use the predefined value. */
#else
	/*
	 * Nearly all CPUs & compilers implement a right-shift of a signed integer
	 * as an Arithmetic Shift Right (with sign extension).
	 */
#   define U_SIGNED_RIGHT_SHIFT_IS_ARITHMETIC 1
#endif

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
// from platform.h
//

/*===========================================================================*/
/** @{ Symbol import-export control                                          */
/*===========================================================================*/

#ifdef U_EXPORT
	/* Use the predefined value. */
#elif defined(U_STATIC_IMPLEMENTATION)
#   define U_EXPORT
#elif defined(__GNUC__)
#   define U_EXPORT __attribute__((visibility("default")))
#elif (defined(__SUNPRO_CC) && __SUNPRO_CC >= 0x550) \
   || (defined(__SUNPRO_C) && __SUNPRO_C >= 0x550)
#   define U_EXPORT __global
/*#elif defined(__HP_aCC) || defined(__HP_cc)
#   define U_EXPORT __declspec(dllexport)*/
#elif defined(_MSC_VER)
#   define U_EXPORT __declspec(dllexport)
#else
#   define U_EXPORT
#endif

/* U_CALLCONV is releated to U_EXPORT2 */
#ifdef U_EXPORT2
	/* Use the predefined value. */
#elif defined(_MSC_VER)
#   define U_EXPORT2 __cdecl
#else
#   define U_EXPORT2
#endif

#ifdef U_IMPORT
	/* Use the predefined value. */
#elif defined(_MSC_VER)
	/* Windows needs to export/import data. */
#   define U_IMPORT __declspec(dllimport)
#else
#   define U_IMPORT
#endif

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
// from uchar.h
//

/**
 * Get a single-bit bit set (a flag) from a bit number 0..31.
 * @stable ICU 2.1
 */
#define U_MASK(x) ((uint32_t)1<<(x))

/**
 * Data for enumerated Unicode general category types.
 * See http://www.unicode.org/Public/UNIDATA/UnicodeData.html .
 * @stable ICU 2.0
 */
typedef enum UCharCategory
{
	/*
	 * Note: UCharCategory constants and their API comments are parsed by preparseucd.py.
	 * It matches pairs of lines like
	 *     / ** <Unicode 2-letter General_Category value> comment... * /
	 *     U_<[A-Z_]+> = <integer>,
	 */

	/** Non-category for unassigned and non-character code points. @stable ICU 2.0 */
	U_UNASSIGNED              = 0,
	/** Cn "Other, Not Assigned (no characters in [UnicodeData.txt] have this property)" (same as U_UNASSIGNED!) @stable ICU 2.0 */
	U_GENERAL_OTHER_TYPES     = 0,
	/** Lu @stable ICU 2.0 */
	U_UPPERCASE_LETTER        = 1,
	/** Ll @stable ICU 2.0 */
	U_LOWERCASE_LETTER        = 2,
	/** Lt @stable ICU 2.0 */
	U_TITLECASE_LETTER        = 3,
	/** Lm @stable ICU 2.0 */
	U_MODIFIER_LETTER         = 4,
	/** Lo @stable ICU 2.0 */
	U_OTHER_LETTER            = 5,
	/** Mn @stable ICU 2.0 */
	U_NON_SPACING_MARK        = 6,
	/** Me @stable ICU 2.0 */
	U_ENCLOSING_MARK          = 7,
	/** Mc @stable ICU 2.0 */
	U_COMBINING_SPACING_MARK  = 8,
	/** Nd @stable ICU 2.0 */
	U_DECIMAL_DIGIT_NUMBER    = 9,
	/** Nl @stable ICU 2.0 */
	U_LETTER_NUMBER           = 10,
	/** No @stable ICU 2.0 */
	U_OTHER_NUMBER            = 11,
	/** Zs @stable ICU 2.0 */
	U_SPACE_SEPARATOR         = 12,
	/** Zl @stable ICU 2.0 */
	U_LINE_SEPARATOR          = 13,
	/** Zp @stable ICU 2.0 */
	U_PARAGRAPH_SEPARATOR     = 14,
	/** Cc @stable ICU 2.0 */
	U_CONTROL_CHAR            = 15,
	/** Cf @stable ICU 2.0 */
	U_FORMAT_CHAR             = 16,
	/** Co @stable ICU 2.0 */
	U_PRIVATE_USE_CHAR        = 17,
	/** Cs @stable ICU 2.0 */
	U_SURROGATE               = 18,
	/** Pd @stable ICU 2.0 */
	U_DASH_PUNCTUATION        = 19,
	/** Ps @stable ICU 2.0 */
	U_START_PUNCTUATION       = 20,
	/** Pe @stable ICU 2.0 */
	U_END_PUNCTUATION         = 21,
	/** Pc @stable ICU 2.0 */
	U_CONNECTOR_PUNCTUATION   = 22,
	/** Po @stable ICU 2.0 */
	U_OTHER_PUNCTUATION       = 23,
	/** Sm @stable ICU 2.0 */
	U_MATH_SYMBOL             = 24,
	/** Sc @stable ICU 2.0 */
	U_CURRENCY_SYMBOL         = 25,
	/** Sk @stable ICU 2.0 */
	U_MODIFIER_SYMBOL         = 26,
	/** So @stable ICU 2.0 */
	U_OTHER_SYMBOL            = 27,
	/** Pi @stable ICU 2.0 */
	U_INITIAL_PUNCTUATION     = 28,
	/** Pf @stable ICU 2.0 */
	U_FINAL_PUNCTUATION       = 29,
	/** One higher than the last enum UCharCategory constant. @stable ICU 2.0 */
	U_CHAR_CATEGORY_COUNT
} UCharCategory;

/**
 * U_GC_XX_MASK constants are bit flags corresponding to Unicode
 * general category values.
 * For each category, the nth bit is set if the numeric value of the
 * corresponding UCharCategory constant is n.
 *
 * There are also some U_GC_Y_MASK constants for groups of general categories
 * like L for all letter categories.
 *
 * @see u_charType
 * @see U_GET_GC_MASK
 * @see UCharCategory
 * @stable ICU 2.1
 */
#define U_GC_CN_MASK    U_MASK(U_GENERAL_OTHER_TYPES)

/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_LU_MASK    U_MASK(U_UPPERCASE_LETTER)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_LL_MASK    U_MASK(U_LOWERCASE_LETTER)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_LT_MASK    U_MASK(U_TITLECASE_LETTER)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_LM_MASK    U_MASK(U_MODIFIER_LETTER)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_LO_MASK    U_MASK(U_OTHER_LETTER)

/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_MN_MASK    U_MASK(U_NON_SPACING_MARK)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_ME_MASK    U_MASK(U_ENCLOSING_MARK)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_MC_MASK    U_MASK(U_COMBINING_SPACING_MARK)

/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_ND_MASK    U_MASK(U_DECIMAL_DIGIT_NUMBER)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_NL_MASK    U_MASK(U_LETTER_NUMBER)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_NO_MASK    U_MASK(U_OTHER_NUMBER)

/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_ZS_MASK    U_MASK(U_SPACE_SEPARATOR)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_ZL_MASK    U_MASK(U_LINE_SEPARATOR)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_ZP_MASK    U_MASK(U_PARAGRAPH_SEPARATOR)

/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_CC_MASK    U_MASK(U_CONTROL_CHAR)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_CF_MASK    U_MASK(U_FORMAT_CHAR)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_CO_MASK    U_MASK(U_PRIVATE_USE_CHAR)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_CS_MASK    U_MASK(U_SURROGATE)

/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_PD_MASK    U_MASK(U_DASH_PUNCTUATION)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_PS_MASK    U_MASK(U_START_PUNCTUATION)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_PE_MASK    U_MASK(U_END_PUNCTUATION)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_PC_MASK    U_MASK(U_CONNECTOR_PUNCTUATION)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_PO_MASK    U_MASK(U_OTHER_PUNCTUATION)

/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_SM_MASK    U_MASK(U_MATH_SYMBOL)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_SC_MASK    U_MASK(U_CURRENCY_SYMBOL)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_SK_MASK    U_MASK(U_MODIFIER_SYMBOL)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_SO_MASK    U_MASK(U_OTHER_SYMBOL)

/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_PI_MASK    U_MASK(U_INITIAL_PUNCTUATION)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_PF_MASK    U_MASK(U_FINAL_PUNCTUATION)


/** Mask constant for multiple UCharCategory bits (L Letters). @stable ICU 2.1 */
#define U_GC_L_MASK \
			(U_GC_LU_MASK|U_GC_LL_MASK|U_GC_LT_MASK|U_GC_LM_MASK|U_GC_LO_MASK)

/** Mask constant for multiple UCharCategory bits (LC Cased Letters). @stable ICU 2.1 */
#define U_GC_LC_MASK \
			(U_GC_LU_MASK|U_GC_LL_MASK|U_GC_LT_MASK)

/** Mask constant for multiple UCharCategory bits (M Marks). @stable ICU 2.1 */
#define U_GC_M_MASK (U_GC_MN_MASK|U_GC_ME_MASK|U_GC_MC_MASK)

/** Mask constant for multiple UCharCategory bits (N Numbers). @stable ICU 2.1 */
#define U_GC_N_MASK (U_GC_ND_MASK|U_GC_NL_MASK|U_GC_NO_MASK)

/** Mask constant for multiple UCharCategory bits (Z Separators). @stable ICU 2.1 */
#define U_GC_Z_MASK (U_GC_ZS_MASK|U_GC_ZL_MASK|U_GC_ZP_MASK)

/** Mask constant for multiple UCharCategory bits (C Others). @stable ICU 2.1 */
#define U_GC_C_MASK \
			(U_GC_CN_MASK|U_GC_CC_MASK|U_GC_CF_MASK|U_GC_CO_MASK|U_GC_CS_MASK)

/** Mask constant for multiple UCharCategory bits (P Punctuation). @stable ICU 2.1 */
#define U_GC_P_MASK \
			(U_GC_PD_MASK|U_GC_PS_MASK|U_GC_PE_MASK|U_GC_PC_MASK|U_GC_PO_MASK| \
			 U_GC_PI_MASK|U_GC_PF_MASK)

/** Mask constant for multiple UCharCategory bits (S Symbols). @stable ICU 2.1 */
#define U_GC_S_MASK (U_GC_SM_MASK|U_GC_SC_MASK|U_GC_SK_MASK|U_GC_SO_MASK)

/**
 * This specifies the language directional property of a character set.
 * @stable ICU 2.0
 */
typedef enum UCharDirection {
	/*
	 * Note: UCharDirection constants and their API comments are parsed by preparseucd.py.
	 * It matches pairs of lines like
	 *     / ** <Unicode 1..3-letter Bidi_Class value> comment... * /
	 *     U_<[A-Z_]+> = <integer>,
	 */

	/** L @stable ICU 2.0 */
	U_LEFT_TO_RIGHT               = 0,
	/** R @stable ICU 2.0 */
	U_RIGHT_TO_LEFT               = 1,
	/** EN @stable ICU 2.0 */
	U_EUROPEAN_NUMBER             = 2,
	/** ES @stable ICU 2.0 */
	U_EUROPEAN_NUMBER_SEPARATOR   = 3,
	/** ET @stable ICU 2.0 */
	U_EUROPEAN_NUMBER_TERMINATOR  = 4,
	/** AN @stable ICU 2.0 */
	U_ARABIC_NUMBER               = 5,
	/** CS @stable ICU 2.0 */
	U_COMMON_NUMBER_SEPARATOR     = 6,
	/** B @stable ICU 2.0 */
	U_BLOCK_SEPARATOR             = 7,
	/** S @stable ICU 2.0 */
	U_SEGMENT_SEPARATOR           = 8,
	/** WS @stable ICU 2.0 */
	U_WHITE_SPACE_NEUTRAL         = 9,
	/** ON @stable ICU 2.0 */
	U_OTHER_NEUTRAL               = 10,
	/** LRE @stable ICU 2.0 */
	U_LEFT_TO_RIGHT_EMBEDDING     = 11,
	/** LRO @stable ICU 2.0 */
	U_LEFT_TO_RIGHT_OVERRIDE      = 12,
	/** AL @stable ICU 2.0 */
	U_RIGHT_TO_LEFT_ARABIC        = 13,
	/** RLE @stable ICU 2.0 */
	U_RIGHT_TO_LEFT_EMBEDDING     = 14,
	/** RLO @stable ICU 2.0 */
	U_RIGHT_TO_LEFT_OVERRIDE      = 15,
	/** PDF @stable ICU 2.0 */
	U_POP_DIRECTIONAL_FORMAT      = 16,
	/** NSM @stable ICU 2.0 */
	U_DIR_NON_SPACING_MARK        = 17,
	/** BN @stable ICU 2.0 */
	U_BOUNDARY_NEUTRAL            = 18,
	/** FSI @stable ICU 52 */
	U_FIRST_STRONG_ISOLATE        = 19,
	/** LRI @stable ICU 52 */
	U_LEFT_TO_RIGHT_ISOLATE       = 20,
	/** RLI @stable ICU 52 */
	U_RIGHT_TO_LEFT_ISOLATE       = 21,
	/** PDI @stable ICU 52 */
	U_POP_DIRECTIONAL_ISOLATE     = 22,
	/** @stable ICU 2.0 */
	U_CHAR_DIRECTION_COUNT
} UCharDirection;

/** Option value for case folding: use default mappings defined in CaseFolding.txt. @stable ICU 2.0 */
#define U_FOLD_CASE_DEFAULT 0

/**
 * Option value for case folding:
 *
 * Use the modified set of mappings provided in CaseFolding.txt to handle dotted I
 * and dotless i appropriately for Turkic languages (tr, az).
 *
 * Before Unicode 3.2, CaseFolding.txt contains mappings marked with 'I' that
 * are to be included for default mappings and
 * excluded for the Turkic-specific mappings.
 *
 * Unicode 3.2 CaseFolding.txt instead contains mappings marked with 'T' that
 * are to be excluded for default mappings and
 * included for the Turkic-specific mappings.
 *
 * @stable ICU 2.0
 */
#define U_FOLD_CASE_EXCLUDE_SPECIAL_I 1

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
// from uchar.c
//

/* Some control characters that are used as space. */
#define IS_THAT_CONTROL_SPACE(c) \
	(c<=0x9f && ((c>=TAB && c<=CR) || (c>=0x1c && c <=0x1f) || c==NL))

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
// from uprop.h
//

/* indexes[] entries */
enum {
	UPROPS_PROPS32_INDEX,
	UPROPS_EXCEPTIONS_INDEX,
	UPROPS_EXCEPTIONS_TOP_INDEX,

	UPROPS_ADDITIONAL_TRIE_INDEX,
	UPROPS_ADDITIONAL_VECTORS_INDEX,
	UPROPS_ADDITIONAL_VECTORS_COLUMNS_INDEX,

	UPROPS_SCRIPT_EXTENSIONS_INDEX,

	UPROPS_RESERVED_INDEX_7,
	UPROPS_RESERVED_INDEX_8,

	/* size of the data file (number of 32-bit units after the header) */
	UPROPS_DATA_TOP_INDEX,

	/* maximum values for code values in vector word 0 */
	UPROPS_MAX_VALUES_INDEX=10,
	/* maximum values for code values in vector word 2 */
	UPROPS_MAX_VALUES_2_INDEX,

	UPROPS_INDEX_COUNT=16
};

/* definitions for the main properties words */
enum {
	/* general category shift==0                                0 (5 bits) */
	/* reserved                                                 5 (1 bit) */
	UPROPS_NUMERIC_TYPE_VALUE_SHIFT=6                       /*  6 (10 bits) */
};

#define GET_CATEGORY(props) ((props)&0x1f)
#define CAT_MASK(props) U_MASK(GET_CATEGORY(props))

#define GET_NUMERIC_TYPE_VALUE(props) ((props)>>UPROPS_NUMERIC_TYPE_VALUE_SHIFT)

/** Some code points. @internal */
enum {
	TAB     =0x0009,
	LF      =0x000a,
	FF      =0x000c,
	CR      =0x000d,
	U_A     =0x0041,
	U_F     =0x0046,
	U_Z     =0x005a,
	U_a     =0x0061,
	U_f     =0x0066,
	U_z     =0x007a,
	DEL     =0x007f,
	NL      =0x0085,
	NBSP    =0x00a0,
	CGJ     =0x034f,
	FIGURESP=0x2007,
	HAIRSP  =0x200a,
	ZWNJ    =0x200c,
	ZWJ     =0x200d,
	RLM     =0x200f,
	NNBSP   =0x202f,
	WJ      =0x2060,
	INHSWAP =0x206a,
	NOMDIG  =0x206f,
	U_FW_A  =0xff21,
	U_FW_F  =0xff26,
	U_FW_Z  =0xff3a,
	U_FW_a  =0xff41,
	U_FW_f  =0xff46,
	U_FW_z  =0xff5a,
	ZWNBSP  =0xfeff
};

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
// from utrie2.h
//

/** Build-time trie structure. */
struct UNewTrie2;
typedef struct UNewTrie2 UNewTrie2;

/*
 * Trie structure definition.
 *
 * Either the data table is 16 bits wide and accessed via the index
 * pointer, with each index item increased by indexLength;
 * in this case, data32==NULL, and data16 is used for direct ASCII access.
 *
 * Or the data table is 32 bits wide and accessed via the data32 pointer.
 */
struct UTrie2 {
	/* protected: used by macros and functions for reading values */
	const uint16_t *index;
	const uint16_t *data16;     /* for fast UTF-8 ASCII access, if 16b data */
	const uint32_t *data32;     /* NULL if 16b data is used via index */

	int32_t indexLength, dataLength;
	uint16_t index2NullOffset;  /* 0xffff if there is no dedicated index-2 null block */
	uint16_t dataNullOffset;
	uint32_t initialValue;
	/** Value returned for out-of-range code points and illegal UTF-8. */
	uint32_t errorValue;

	/* Start of the last range which ends at U+10ffff, and its value. */
	UChar32 highStart;
	int32_t highValueIndex;

	/* private: used by builder and unserialization functions */
	void *memory;           /* serialized bytes; NULL if not frozen yet */
	int32_t length;         /* number of serialized bytes at memory; 0 if not frozen yet */
	UBool isMemoryOwned;    /* TRUE if the trie owns the memory */
	UBool padding1;
	int16_t padding2;
	UNewTrie2 *newTrie;     /* builder object; NULL when frozen */
};

/**
 * Trie constants, defining shift widths, index array lengths, etc.
 *
 * These are needed for the runtime macros but users can treat these as
 * implementation details and skip to the actual public API further below.
 */
enum {
	/** Shift size for getting the index-1 table offset. */
	UTRIE2_SHIFT_1=6+5,

	/** Shift size for getting the index-2 table offset. */
	UTRIE2_SHIFT_2=5,

	/**
	 * Difference between the two shift sizes,
	 * for getting an index-1 offset from an index-2 offset. 6=11-5
	 */
	UTRIE2_SHIFT_1_2=UTRIE2_SHIFT_1-UTRIE2_SHIFT_2,

	/**
	 * Number of index-1 entries for the BMP. 32=0x20
	 * This part of the index-1 table is omitted from the serialized form.
	 */
	UTRIE2_OMITTED_BMP_INDEX_1_LENGTH=0x10000>>UTRIE2_SHIFT_1,

	/** Number of code points per index-1 table entry. 2048=0x800 */
	UTRIE2_CP_PER_INDEX_1_ENTRY=1<<UTRIE2_SHIFT_1,

	/** Number of entries in an index-2 block. 64=0x40 */
	UTRIE2_INDEX_2_BLOCK_LENGTH=1<<UTRIE2_SHIFT_1_2,

	/** Mask for getting the lower bits for the in-index-2-block offset. */
	UTRIE2_INDEX_2_MASK=UTRIE2_INDEX_2_BLOCK_LENGTH-1,

	/** Number of entries in a data block. 32=0x20 */
	UTRIE2_DATA_BLOCK_LENGTH=1<<UTRIE2_SHIFT_2,

	/** Mask for getting the lower bits for the in-data-block offset. */
	UTRIE2_DATA_MASK=UTRIE2_DATA_BLOCK_LENGTH-1,

	/**
	 * Shift size for shifting left the index array values.
	 * Increases possible data size with 16-bit index values at the cost
	 * of compactability.
	 * This requires data blocks to be aligned by UTRIE2_DATA_GRANULARITY.
	 */
	UTRIE2_INDEX_SHIFT=2,

	/** The alignment size of a data block. Also the granularity for compaction. */
	UTRIE2_DATA_GRANULARITY=1<<UTRIE2_INDEX_SHIFT,

	/* Fixed layout of the first part of the index array. ------------------- */

	/**
	 * The BMP part of the index-2 table is fixed and linear and starts at offset 0.
	 * Length=2048=0x800=0x10000>>UTRIE2_SHIFT_2.
	 */
	UTRIE2_INDEX_2_OFFSET=0,

	/**
	 * The part of the index-2 table for U+D800..U+DBFF stores values for
	 * lead surrogate code _units_ not code _points_.
	 * Values for lead surrogate code _points_ are indexed with this portion of the table.
	 * Length=32=0x20=0x400>>UTRIE2_SHIFT_2. (There are 1024=0x400 lead surrogates.)
	 */
	UTRIE2_LSCP_INDEX_2_OFFSET=0x10000>>UTRIE2_SHIFT_2,
	UTRIE2_LSCP_INDEX_2_LENGTH=0x400>>UTRIE2_SHIFT_2,

	/** Count the lengths of both BMP pieces. 2080=0x820 */
	UTRIE2_INDEX_2_BMP_LENGTH=UTRIE2_LSCP_INDEX_2_OFFSET+UTRIE2_LSCP_INDEX_2_LENGTH,

	/**
	 * The 2-byte UTF-8 version of the index-2 table follows at offset 2080=0x820.
	 * Length 32=0x20 for lead bytes C0..DF, regardless of UTRIE2_SHIFT_2.
	 */
	UTRIE2_UTF8_2B_INDEX_2_OFFSET=UTRIE2_INDEX_2_BMP_LENGTH,
	UTRIE2_UTF8_2B_INDEX_2_LENGTH=0x800>>6,  /* U+0800 is the first code point after 2-byte UTF-8 */

	/**
	 * The index-1 table, only used for supplementary code points, at offset 2112=0x840.
	 * Variable length, for code points up to highStart, where the last single-value range starts.
	 * Maximum length 512=0x200=0x100000>>UTRIE2_SHIFT_1.
	 * (For 0x100000 supplementary code points U+10000..U+10ffff.)
	 *
	 * The part of the index-2 table for supplementary code points starts
	 * after this index-1 table.
	 *
	 * Both the index-1 table and the following part of the index-2 table
	 * are omitted completely if there is only BMP data.
	 */
	UTRIE2_INDEX_1_OFFSET=UTRIE2_UTF8_2B_INDEX_2_OFFSET+UTRIE2_UTF8_2B_INDEX_2_LENGTH,
	UTRIE2_MAX_INDEX_1_LENGTH=0x100000>>UTRIE2_SHIFT_1,

	/*
	 * Fixed layout of the first part of the data array. -----------------------
	 * Starts with 4 blocks (128=0x80 entries) for ASCII.
	 */

	/**
	 * The illegal-UTF-8 data block follows the ASCII block, at offset 128=0x80.
	 * Used with linear access for single bytes 0..0xbf for simple error handling.
	 * Length 64=0x40, not UTRIE2_DATA_BLOCK_LENGTH.
	 */
	UTRIE2_BAD_UTF8_DATA_OFFSET=0x80,

	/** The start of non-linear-ASCII data blocks, at offset 192=0xc0. */
	UTRIE2_DATA_START_OFFSET=0xc0
};

/* Public UTrie2 API macros ------------------------------------------------- */

/*
 * These macros provide fast data lookup from a frozen trie.
 * They will crash when used on an unfrozen trie.
 */

/**
 * Return a 16-bit trie value from a code point, with range checking.
 * Returns trie->errorValue if c is not in the range 0..U+10ffff.
 *
 * @param trie (const UTrie2 *, in) a frozen trie
 * @param c (UChar32, in) the input code point
 * @return (uint16_t) The code point's trie value.
 */
#define UTRIE2_GET16(trie, c) _UTRIE2_GET((trie), index, (trie)->indexLength, (c))

/**
 * Return a 32-bit trie value from a code point, with range checking.
 * Returns trie->errorValue if c is not in the range 0..U+10ffff.
 *
 * @param trie (const UTrie2 *, in) a frozen trie
 * @param c (UChar32, in) the input code point
 * @return (uint32_t) The code point's trie value.
 */
#define UTRIE2_GET32(trie, c) _UTRIE2_GET((trie), data32, 0, (c))


/* Internal functions and macros -------------------------------------------- */

/** Internal low-level trie getter. Returns a data index. */
#define _UTRIE2_INDEX_RAW(offset, trieIndex, c) \
	(((int32_t)((trieIndex)[(offset)+((c)>>UTRIE2_SHIFT_2)]) \
	<<UTRIE2_INDEX_SHIFT)+ \
	((c)&UTRIE2_DATA_MASK))

/** Internal trie getter from a UTF-16 single/lead code unit. Returns the data index. */
#define _UTRIE2_INDEX_FROM_U16_SINGLE_LEAD(trieIndex, c) _UTRIE2_INDEX_RAW(0, trieIndex, c)

/** Internal trie getter from a lead surrogate code point (D800..DBFF). Returns the data index. */
#define _UTRIE2_INDEX_FROM_LSCP(trieIndex, c) \
	_UTRIE2_INDEX_RAW(UTRIE2_LSCP_INDEX_2_OFFSET-(0xd800>>UTRIE2_SHIFT_2), trieIndex, c)

/** Internal trie getter from a BMP code point. Returns the data index. */
#define _UTRIE2_INDEX_FROM_BMP(trieIndex, c) \
	_UTRIE2_INDEX_RAW(U_IS_LEAD(c) ? UTRIE2_LSCP_INDEX_2_OFFSET-(0xd800>>UTRIE2_SHIFT_2) : 0, \
					  trieIndex, c)

/** Internal trie getter from a supplementary code point below highStart. Returns the data index. */
#define _UTRIE2_INDEX_FROM_SUPP(trieIndex, c) \
	(((int32_t)((trieIndex)[ \
		(trieIndex)[(UTRIE2_INDEX_1_OFFSET-UTRIE2_OMITTED_BMP_INDEX_1_LENGTH)+ \
					  ((c)>>UTRIE2_SHIFT_1)]+ \
		(((c)>>UTRIE2_SHIFT_2)&UTRIE2_INDEX_2_MASK)]) \
	<<UTRIE2_INDEX_SHIFT)+ \
	((c)&UTRIE2_DATA_MASK))

/**
 * Internal trie getter from a code point, with checking that c is in 0..10FFFF.
 * Returns the data index.
 */
#define _UTRIE2_INDEX_FROM_CP(trie, asciiOffset, c) \
	((uint32_t)(c)<0xd800 ? \
		_UTRIE2_INDEX_RAW(0, (trie)->index, c) : \
		(uint32_t)(c)<=0xffff ? \
			_UTRIE2_INDEX_RAW( \
				(c)<=0xdbff ? UTRIE2_LSCP_INDEX_2_OFFSET-(0xd800>>UTRIE2_SHIFT_2) : 0, \
				(trie)->index, c) : \
			(uint32_t)(c)>0x10ffff ? \
				(asciiOffset)+UTRIE2_BAD_UTF8_DATA_OFFSET : \
				(c)>=(trie)->highStart ? \
					(trie)->highValueIndex : \
					_UTRIE2_INDEX_FROM_SUPP((trie)->index, c))

/** Internal trie getter from a UTF-16 single/lead code unit. Returns the data. */
#define _UTRIE2_GET_FROM_U16_SINGLE_LEAD(trie, data, c) \
	(trie)->data[_UTRIE2_INDEX_FROM_U16_SINGLE_LEAD((trie)->index, c)]

/** Internal trie getter from a supplementary code point. Returns the data. */
#define _UTRIE2_GET_FROM_SUPP(trie, data, c) \
	(trie)->data[(c)>=(trie)->highStart ? (trie)->highValueIndex : \
				 _UTRIE2_INDEX_FROM_SUPP((trie)->index, c)]

/**
 * Internal trie getter from a code point, with checking that c is in 0..10FFFF.
 * Returns the data.
 */
#define _UTRIE2_GET(trie, data, asciiOffset, c) \
	(trie)->data[_UTRIE2_INDEX_FROM_CP(trie, asciiOffset, c)]

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
// from ucase.h
//

/**
 * Bit mask for getting just the options from a string compare options word
 * that are relevant for case folding (of a single string or code point).
 * See uchar.h.
 * @internal
 */
#define _FOLD_CASE_OPTIONS_MASK 0xff

/* indexes into indexes[] */
enum {
	UCASE_IX_INDEX_TOP,
	UCASE_IX_LENGTH,
	UCASE_IX_TRIE_SIZE,
	UCASE_IX_EXC_LENGTH,
	UCASE_IX_UNFOLD_LENGTH,

	UCASE_IX_MAX_FULL_LENGTH=15,
	UCASE_IX_TOP=16
};

/* 2-bit constants for types of cased characters */
#define UCASE_TYPE_MASK     3
enum {
	UCASE_NONE,
	UCASE_LOWER,
	UCASE_UPPER,
	UCASE_TITLE
};

#define UCASE_GET_TYPE(props) ((props)&UCASE_TYPE_MASK)
#define UCASE_GET_TYPE_AND_IGNORABLE(props) ((props)&7)

#define UCASE_IGNORABLE         4
#define UCASE_SENSITIVE         8
#define UCASE_EXCEPTION         0x10

#define UCASE_DOT_MASK      0x60
enum {
	UCASE_NO_DOT=0,         /* normal characters with cc=0 */
	UCASE_SOFT_DOTTED=0x20, /* soft-dotted characters with cc=0 */
	UCASE_ABOVE=0x40,       /* "above" accents with cc=230 */
	UCASE_OTHER_ACCENT=0x60 /* other accent character (0<cc!=230) */
};

/* no exception: bits 15..7 are a 9-bit signed case mapping delta */
#define UCASE_DELTA_SHIFT   7
#define UCASE_DELTA_MASK    0xff80
#define UCASE_MAX_DELTA     0xff
#define UCASE_MIN_DELTA     (-UCASE_MAX_DELTA-1)

#if U_SIGNED_RIGHT_SHIFT_IS_ARITHMETIC
#   define UCASE_GET_DELTA(props) ((int16_t)(props)>>UCASE_DELTA_SHIFT)
#else
#   define UCASE_GET_DELTA(props) (int16_t)(((props)&0x8000) ? (((props)>>UCASE_DELTA_SHIFT)|0xfe00) : ((uint16_t)(props)>>UCASE_DELTA_SHIFT))
#endif

/* exception: bits 15..5 are an unsigned 11-bit index into the exceptions array */
#define UCASE_EXC_SHIFT     5
#define UCASE_EXC_MASK      0xffe0
#define UCASE_MAX_EXCEPTIONS ((UCASE_EXC_MASK>>UCASE_EXC_SHIFT)+1)

/* definitions for 16-bit main exceptions word ------------------------------ */

/* first 8 bits indicate values in optional slots */
enum {
	UCASE_EXC_LOWER,
	UCASE_EXC_FOLD,
	UCASE_EXC_UPPER,
	UCASE_EXC_TITLE,
	UCASE_EXC_4,            /* reserved */
	UCASE_EXC_5,            /* reserved */
	UCASE_EXC_CLOSURE,
	UCASE_EXC_FULL_MAPPINGS,
	UCASE_EXC_ALL_SLOTS     /* one past the last slot */
};

/* each slot is 2 uint16_t instead of 1 */
#define UCASE_EXC_DOUBLE_SLOTS      0x100

/* reserved: exception bits 11..9 */

/* UCASE_EXC_DOT_MASK=UCASE_DOT_MASK<<UCASE_EXC_DOT_SHIFT */
#define UCASE_EXC_DOT_SHIFT     7

/* normally stored in the main word, but pushed out for larger exception indexes */
#define UCASE_EXC_DOT_MASK      0x3000
enum {
	UCASE_EXC_NO_DOT=0,
	UCASE_EXC_SOFT_DOTTED=0x1000,
	UCASE_EXC_ABOVE=0x2000,         /* "above" accents with cc=230 */
	UCASE_EXC_OTHER_ACCENT=0x3000   /* other character (0<cc!=230) */
};

/* complex/conditional mappings */
#define UCASE_EXC_CONDITIONAL_SPECIAL   0x4000
#define UCASE_EXC_CONDITIONAL_FOLD      0x8000

/* definitions for lengths word for full case mappings */
#define UCASE_FULL_LOWER    0xf
#define UCASE_FULL_FOLDING  0xf0
#define UCASE_FULL_UPPER    0xf00
#define UCASE_FULL_TITLE    0xf000

/* maximum lengths */
#define UCASE_FULL_MAPPINGS_MAX_LENGTH (4*0xf)
#define UCASE_CLOSURE_MAX_LENGTH 0xf

/* constants for reverse case folding ("unfold") data */
enum {
	UCASE_UNFOLD_ROWS,
	UCASE_UNFOLD_ROW_WIDTH,
	UCASE_UNFOLD_STRING_WIDTH
};

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
// from udata.h
//

typedef struct UDataMemory UDataMemory;

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
// from ucase.cpp
//

struct UCaseProps {
	UDataMemory *mem;
	const int32_t *indexes;
	const uint16_t *exceptions;
	const uint16_t *unfold;

	UTrie2 trie;
	uint8_t formatVersion[4];
};

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
// from uversion.h
//

/** Maximum length of the copyright string.
 *  @stable ICU 2.4
 */
#define U_COPYRIGHT_STRING_LENGTH  128

/** An ICU version consists of up to 4 numbers from 0..255.
 *  @stable ICU 2.4
 */
#define U_MAX_VERSION_LENGTH 4

/** In a string, ICU version fields are delimited by dots.
 *  @stable ICU 2.4
 */
#define U_VERSION_DELIMITER '.'

/** The maximum length of an ICU version string.
 *  @stable ICU 2.4
 */
#define U_MAX_VERSION_STRING_LENGTH 20

/** The binary form of a version on ICU APIs is an array of 4 uint8_t.
 *  To compare two versions, use memcmp(v1,v2,sizeof(UVersionInfo)).
 *  @stable ICU 2.4
 */
typedef uint8_t UVersionInfo[U_MAX_VERSION_LENGTH];

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
