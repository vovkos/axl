// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2010. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_log_Line.h"

AXL_EXTERN_C_BEGIN

//.............................................................................

enum
{
	axl_log_ERepresentMode_HyperText = 0,
	axl_log_ERepresentMode_PlainText,
	axl_log_ERepresentMode_Bin,
	axl_log_ERepresentMode_Delimiter,
};

enum
{
	axl_log_ELineDelimiter_None   = 0,
	axl_log_ELineDelimiter_Light  = 1,
	axl_log_ELineDelimiter_Normal = 2,
	axl_log_ELineDelimiter_Bold   = 3,
};

//.............................................................................

#ifndef __cplusplus

typedef
void 
(AXL_VFT* 
 axl_log_FRepresentSinkAddData)(
	void* pSink, 
	ulong_t Mode,
	const void* p,
	size_t Size
	);

typedef struct _axl_log_TRepresentSinkVTable axl_log_TRepresentSinkVTable;

struct _axl_log_TRepresentSinkVTable
{
	axl_log_FRepresentSinkAddData m_pfnAddData;
};

#endif

//.............................................................................

typedef struct _axl_log_TRepresentSink  axl_log_TRepresentSink;

void
AXL_API
axl_log_TRepresentSink_Construct(axl_log_TRepresentSink* pSink);

AXL_INLINE
void 
axl_log_TRepresentSink_Destruct(axl_log_TRepresentSink* pSink);

ulong_t 
AXL_API
axl_log_TRepresentSink_GetLineType(
	axl_log_TRepresentSink* pSink,
	ulong_t Mode
	);

AXL_INLINE
void
axl_log_TRepresentSink_AddData(
	axl_log_TRepresentSink* pSink, 
	ulong_t Mode,
	const void* p, 
	size_t Size
	);

AXL_INLINE
void
axl_log_TRepresentSink_AddDelimiter(
	axl_log_TRepresentSink* pSink, 
	long Delimiter
	);

void 
AXL_API
axl_log_TRepresentSink_AddDataFormatV (
	axl_log_TRepresentSink* pSink, 
	ulong_t Mode,
	const tchar_t* pFormat,
	va_list va
	);

AXL_INLINE
void 
axl_log_TRepresentSink_AddBin(
	axl_log_TRepresentSink* pSink, 
	const void* p,
	size_t Size
	);

AXL_INLINE
void
axl_log_TRepresentSink_AddPlainText(
	axl_log_TRepresentSink* pSink, 
	const tchar_t* pText,
	size_t Length
	);

AXL_INLINE
void 
axl_log_TRepresentSink_AddPlainTextFormatV (
	axl_log_TRepresentSink* pSink, 
	const tchar_t* pFormat,
	va_list va
	);

AXL_INLINE
void 
axl_log_TRepresentSink_AddPlainTextFormat(
	axl_log_TRepresentSink* pSink, 
	const tchar_t* pFormat,
	...
	);

AXL_INLINE
void
axl_log_TRepresentSink_AddHyperText(
	axl_log_TRepresentSink* pSink, 
	const tchar_t* pText,
	size_t Length
	);

AXL_INLINE
void 
axl_log_TRepresentSink_AddHyperTextFormatV (
	axl_log_TRepresentSink* pSink, 
	const tchar_t* pFormat,
	va_list va
	);

AXL_INLINE
void 
axl_log_TRepresentSink_AddHyperTextFormat(
	axl_log_TRepresentSink* pSink, 
	const tchar_t* pFormat,
	...
	);

AXL_INLINE
void
axl_log_TRepresentSink_BackBin(
	axl_log_TRepresentSink* pSink, 
	size_t Size
	);

AXL_INLINE
void
axl_log_TRepresentSink_BackText(
	axl_log_TRepresentSink* pSink, 
	size_t Length
	);

struct _axl_log_TRepresentSink
{
#ifndef __cplusplus
	void* m_pVTable;
#endif

	long m_Icon;
	bool_t m_TileIcon;
	axl_gr_TTextAttr m_Attr;

	ulong_t m_PartCode;
	ulong_t m_Merge;

	axl_log_TDataConfig m_DataConfig;
	
#ifdef __cplusplus
	virtual void AXL_VFT AddData(ulong_t Mode, const void* p, size_t Size)
		{ }

	void AddDelimiter(int Delimiter = axl_log_ELineDelimiter_Normal)
		{ axl_log_TRepresentSink_AddDelimiter(this, Delimiter); }

	void AddBin(const void* p, size_t Size)
		{ axl_log_TRepresentSink_AddBin(this, p, Size); }

	void AddPlainText(const tchar_t* pText, size_t Length = -1)
		{ axl_log_TRepresentSink_AddPlainText(this, pText, Length); }

	void AddPlainTextFormatV (const tchar_t* pFormat, va_list va)
		{ axl_log_TRepresentSink_AddPlainTextFormatV (this, pFormat, va); } 

	void AddPlainTextFormat(const tchar_t* pFormat, ...)
	{ 
		va_list va;
		va_start(va, pFormat);
		axl_log_TRepresentSink_AddPlainTextFormatV (this,	pFormat, va);
	}

	void AddHyperText(const tchar_t* pText, size_t Length = -1)
		{ axl_log_TRepresentSink_AddHyperText(this, pText, Length); }

	void AddHyperTextFormatV (const tchar_t* pFormat, va_list va)
		{ axl_log_TRepresentSink_AddHyperTextFormatV (this, pFormat, va); }

	void AddHyperTextFormat(const tchar_t* pFormat, ...)
	{ 
		va_list va;
		va_start(va, pFormat);
		axl_log_TRepresentSink_AddHyperTextFormatV (this,pFormat, va); 
	}

#endif
};

//.............................................................................

AXL_INLINE
void 
axl_log_TRepresentSink_Destruct(axl_log_TRepresentSink* pSink)
	{}

AXL_INLINE
void
axl_log_TRepresentSink_AddData(
	axl_log_TRepresentSink* pSink, 
	ulong_t Mode,
	const void* p, 
	size_t Size
	)
{ 
#ifdef __cplusplus
	pSink->AddData(Mode, p, Size); 
#else
	((axl_log_TRepresentSinkVTable*) pSink->m_pVTable)->m_pfnAddData(pSink, Mode, p, Size); 
#endif
}

AXL_INLINE
void
axl_log_TRepresentSink_AddDelimiter(
	axl_log_TRepresentSink* pSink, 
	long Delimiter
	) 
	{ axl_log_TRepresentSink_AddData(pSink, axl_log_ERepresentMode_Delimiter, &Delimiter, sizeof(Delimiter)); }

AXL_INLINE
void 
axl_log_TRepresentSink_AddBin(
	axl_log_TRepresentSink* pSink, 
	const void* p,
	size_t Size
	)
	{ axl_log_TRepresentSink_AddData(pSink, axl_log_ERepresentMode_Bin, p, Size); }

AXL_INLINE
void
axl_log_TRepresentSink_AddPlainText(
	axl_log_TRepresentSink* pSink, 
	const tchar_t* pText,
	size_t Length
	)
	{ axl_log_TRepresentSink_AddData(pSink, axl_log_ERepresentMode_PlainText, pText, (Length != -1 ? Length : _tcslen(pText)) * sizeof(tchar_t)); }

AXL_INLINE
void 
axl_log_TRepresentSink_AddPlainTextFormatV (
	axl_log_TRepresentSink* pSink, 
	const tchar_t* pFormat,
	va_list va
	)
	{ axl_log_TRepresentSink_AddDataFormatV (pSink, axl_log_ERepresentMode_PlainText, pFormat, va); }

AXL_INLINE
void 
axl_log_TRepresentSink_AddPlainTextFormat(
	axl_log_TRepresentSink* pSink, 
	const tchar_t* pFormat,
	...
	)
{
	va_list va;
	va_start(va, pFormat);
	axl_log_TRepresentSink_AddDataFormatV (pSink, axl_log_ERepresentMode_PlainText, pFormat, va);
}

AXL_INLINE
void
axl_log_TRepresentSink_AddHyperText(
	axl_log_TRepresentSink* pSink, 
	const tchar_t* pText,
	size_t Length
	)
	{ axl_log_TRepresentSink_AddData(pSink, axl_log_ERepresentMode_HyperText, pText, (Length != -1 ? Length : _tcslen(pText)) * sizeof(tchar_t)); }

AXL_INLINE
void 
axl_log_TRepresentSink_AddHyperTextFormatV (
	axl_log_TRepresentSink* pSink, 
	const tchar_t* pFormat,
	va_list va
	)
	{ axl_log_TRepresentSink_AddDataFormatV (pSink, axl_log_ERepresentMode_HyperText, pFormat, va); }

AXL_INLINE
void 
axl_log_TRepresentSink_AddHyperTextFormat(
	axl_log_TRepresentSink* pSink, 
	const tchar_t* pFormat,
	...
	)
	{ axl_log_TRepresentSink_AddDataFormatV (pSink, axl_log_ERepresentMode_HyperText, pFormat, va_start_e(pFormat)); }

//.............................................................................

AXL_EXTERN_C_END

#ifdef __cplusplus

namespace axl {
namespace log {

struct ERepresentMode { enum
{
	HyperText = axl_log_ERepresentMode_HyperText,
	PlainText = axl_log_ERepresentMode_PlainText,
	Bin       = axl_log_ERepresentMode_Bin,
};};

struct ELineDelimiter { enum
{
	None   = axl_log_ELineDelimiter_None,
	Light  = axl_log_ELineDelimiter_Light,
	Normal = axl_log_ELineDelimiter_Normal,
	Bold   = axl_log_ELineDelimiter_Bold,
};};

typedef axl_log_TRepresentSink TRepresentSink;

} // namespace log 
} // namespace axl

#endif // __cplusplus

//.............................................................................
