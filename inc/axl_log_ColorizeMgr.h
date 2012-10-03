// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2010. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_log_Colorizer.h"
#include "axl_rtl_BinTree.h"

typedef struct _axl_log_TLogCtrl    axl_log_TLogCtrl;
typedef struct _axl_log_TCachePage  axl_log_TCachePage;
typedef struct _axl_log_TBinLine    axl_log_TBinLine;

AXL_EXTERN_C_BEGIN

//.............................................................................

typedef struct _axl_log_TColorizeMgr axl_log_TColorizeMgr;

void
AXL_API
axl_log_TColorizeMgr_Construct(
	axl_log_TColorizeMgr* pCacheMgr,
	axl_log_TLogCtrl* pLogCtrl
	);

void
AXL_API
axl_log_TColorizeMgr_Destruct(axl_log_TColorizeMgr* pCacheMgr);

bool_t
AXL_API
axl_log_TColorizeMgr_EnsureColorized(
	axl_log_TColorizeMgr* pCacheMgr,
	axl_log_TBinLine* pLine
	);

bool_t
AXL_API
axl_log_TColorizeMgr_ReColorizeLineRange(
	axl_log_TColorizeMgr* pColorizeMgr,
	axl_log_TCachePage* pPage,
	size_t FirstLine,
	size_t LineCount
	);

typedef struct _axl_log_TColorizeMgr
{ 
	axl_log_TLogCtrl* m_pLogCtrl;
	axl_rtl_TByteArray m_DataBuffer;	
	axl_rtl_TArray m_PartBuffer;	
	size_t m_CrossoverLimit;
	bool_t m_IsInvalidateRequired;

#ifdef __cplusplus
	bool EnsureColorized(axl_log_TBinLine* pLine)
		{ return axl_log_TColorizeMgr_EnsureColorized(this, pLine) != 0; }
#endif

} axl_log_TColorizeMgr;

//.............................................................................

AXL_EXTERN_C_END

#ifdef __cplusplus

namespace axl {
namespace log {

typedef axl_log_TColorizeMgr TColorizeMgr;

} // namespace log {
} // namespace axl {

#endif

//.............................................................................
