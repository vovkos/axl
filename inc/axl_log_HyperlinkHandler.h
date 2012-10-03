// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2010. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_log_Line.h"

AXL_EXTERN_C_BEGIN

typedef struct _axl_log_TLogCtrl axl_log_TLogCtrl;

//.............................................................................

#ifndef __cplusplus

typedef
bool_t 
(AXL_VFT* 
 axl_log_FHyperlinkHandlerOnHyperlink)(
	void* pHandler,
	HWND hWndLogCtrl,
	axl_log_TLine* pLine,
	const tchar_t* pHyperlink
	);

typedef struct _axl_log_THyperlinkHandlerVTable axl_log_THyperlinkHandlerVTable;

struct _axl_log_THyperlinkHandlerVTable
{
	axl_ref_TObjectVTable;
	axl_log_FHyperlinkHandlerOnHyperlink m_pfnOnHyperlink;
};

#endif

//.............................................................................

typedef struct _axl_log_THyperlinkHandler axl_log_THyperlinkHandler;

void
AXL_API
axl_log_THyperlinkHandler_Construct(
	axl_log_THyperlinkHandler* pHandler, 
	axl_ref_TBuffer* pStorage
	);

void
AXL_VFT
axl_log_THyperlinkHandler_Destruct(axl_log_THyperlinkHandler* pHandler);

AXL_INLINE
bool_t 
axl_log_THyperlinkHandler_OnHyperlink(
	axl_log_THyperlinkHandler* pHandler,
	HWND hWndLogCtrl,
	axl_log_TLine* pLine,
	const tchar_t* pHyperlink
	);

#ifdef __cplusplus
typedef struct AXL_NO_VTABLE _axl_log_THyperlinkHandler: axl_ref_TObject 
{
	virtual bool_t AXL_VFT OnHyperlink(HWND hWndLogCtrl, axl_log_TLine* pLine, const tchar_t* pHyperlink)
		{ return false; }
#else
typedef struct _axl_log_THyperlinkHandler { axl_ref_TObject;
#endif
} axl_log_THyperlinkHandler;

//.............................................................................

typedef struct _axl_log_THyperlinkHandlerStack axl_log_THyperlinkHandlerStack;

void
AXL_API
axl_log_THyperlinkHandlerStack_Construct(
	axl_log_THyperlinkHandlerStack* pStack,
	axl_ref_TBuffer* pStorage
	);

void
AXL_VFT
axl_log_THyperlinkHandlerStack_Destruct(axl_log_THyperlinkHandlerStack* pStack);

void
AXL_API
axl_log_THyperlinkHandlerStack_Clear(axl_log_THyperlinkHandlerStack* pStack);

bool_t
AXL_API
axl_log_THyperlinkHandlerStack_ClearButFirst(axl_log_THyperlinkHandlerStack* pStack);

bool_t
AXL_API
axl_log_THyperlinkHandlerStack_AddHyperlinkHandler(
	axl_log_THyperlinkHandlerStack* pStack,
	axl_log_THyperlinkHandler* pHyperlinkHandler
	);

bool_t
AXL_API
axl_log_THyperlinkHandlerStack_RemoveHyperlinkHandler(
	axl_log_THyperlinkHandlerStack* pStack,
	axl_log_THyperlinkHandler* pHyperlinkHandler
	);

bool_t 
AXL_VFT
axl_log_THyperlinkHandlerStack_OnHyperlink(
	axl_log_THyperlinkHandlerStack* pStack,
	HWND hWndLogCtrl,
	axl_log_TLine* pLine,
	const tchar_t* pHyperlink
	);

#ifdef __cplusplus
typedef struct _axl_log_THyperlinkHandlerStack: axl_log_THyperlinkHandler {
#else
typedef struct _axl_log_THyperlinkHandlerStack { axl_log_THyperlinkHandler;
#endif
	axl_rtl_TList m_Stack;

#ifdef __cplusplus
	void Clear()
		{ axl_log_THyperlinkHandlerStack_Clear(this); }

	bool ClearButFirst()
		{ return axl_log_THyperlinkHandlerStack_ClearButFirst(this) != 0; }

	bool AddHyperlinkHandler(axl_log_THyperlinkHandler* pHyperlinkHandler)
		{ return axl_log_THyperlinkHandlerStack_AddHyperlinkHandler(this, pHyperlinkHandler) != 0; }

	bool RemoveHyperlinkHandler(axl_log_THyperlinkHandler* pHyperlinkHandler)
		{ return axl_log_THyperlinkHandlerStack_RemoveHyperlinkHandler(this, pHyperlinkHandler) != 0; }
#endif
} axl_log_THyperlinkHandlerStack;

//.............................................................................

// inlines

AXL_INLINE
bool_t 
axl_log_THyperlinkHandler_OnHyperlink(
	axl_log_THyperlinkHandler* pHandler,
	HWND hWndLogCtrl,
	axl_log_TLine* pLine,
	const tchar_t* pHyperlink
	)
{
#ifdef __cplusplus
	return pHandler->OnHyperlink(hWndLogCtrl, pLine, pHyperlink);
#else
	return ((axl_log_THyperlinkHandlerVTable*) pHandler->m_pVTable)->m_pfnOnHyperlink(pHandler, hWndLogCtrl, pLine, pHyperlink);
#endif
}

//.............................................................................

AXL_EXTERN_C_END

#ifdef __cplusplus

namespace axl {
namespace log {

typedef axl_log_THyperlinkHandler      THyperlinkHandler;
typedef axl_log_THyperlinkHandlerStack THyperlinkHandlerStack;

class AXL_NO_VTABLE CHyperlinkHandler: public THyperlinkHandler
{
public:
	CHyperlinkHandler(axl::ref::TBuffer* pStorage)
		{ axl_log_THyperlinkHandler_Construct(this, pStorage); }

	~CHyperlinkHandler()
		{ axl_log_THyperlinkHandler_Destruct(this); }
};

class AXL_NO_VTABLE CHyperlinkHandlerStack: public THyperlinkHandlerStack
{
public:
	CHyperlinkHandlerStack(axl::ref::TBuffer* pStorage)
		{ axl_log_THyperlinkHandlerStack_Construct(this, pStorage); }

	~CHyperlinkHandlerStack()
		{ axl_log_THyperlinkHandlerStack_Destruct(this); }
};


} // namespace log 
} // namespace axl

#endif // __cplusplus

//.............................................................................
