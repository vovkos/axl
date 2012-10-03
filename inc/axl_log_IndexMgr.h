// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2010. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_log_Base.h"
#include "axl_log_MessageFile.h"
#include "axl_log_IndexFile.h"

AXL_EXTERN_C_BEGIN

//.............................................................................

typedef struct _axl_log_TIndexMgr axl_log_TIndexMgr;
typedef struct _axl_log_TLogCtrl      axl_log_TLogCtrl;

//.............................................................................

void 
AXL_API
axl_log_TIndexMgr_Construct(
	axl_log_TIndexMgr* pIndexMgr,
	axl_log_TLogCtrl* pLogCtrl
	);

void 
AXL_API
axl_log_TIndexMgr_Destruct(axl_log_TIndexMgr* pIndexMgr);

axl_log_TDataConfig* 
AXL_API
axl_log_TIndexMgr_GetPreferredDataConfig(axl_log_TIndexMgr* pIndexMgr);

void
AXL_API
axl_log_TIndexMgr_SetPreferredDataConfig(
	axl_log_TIndexMgr* pIndexMgr,
	const axl_log_TDataConfig* pConfig
	);

void
AXL_API
axl_log_TIndexMgr_ClearIndex(axl_log_TIndexMgr* pIndexMgr);

// returns anchor message offset 
// 0 -- full update (clear)
// offs == last_offs -- append
// offs < last_offs  -- rollback, then append

size_t
AXL_API
axl_log_TIndexMgr_UpdateIndex(axl_log_TIndexMgr* pIndexMgr);

size_t
AXL_API
axl_log_TIndexMgr_CalcMessageLineCount(
	axl_log_TIndexMgr* pIndexMgr,
	axl_log_TMessage* pMsg,
	axl_log_TDataConfig* pPreferredDataConfig,
	ulong_t VolatileFlags
	);

size_t
AXL_API
axl_log_TIndexMgr_ReFilterAll(axl_log_TIndexMgr* pIndexMgr);

typedef struct _axl_log_TIndexMgr
{
	axl_log_TDataConfig m_PreferredDataConfig;
	axl_log_TLogCtrl* m_pLogCtrl;
	ulong_t m_LineCountLimit;
	ulong_t m_VolatileMessageCountLimit;
	axl_rtl_TArray m_VolatileMessageDataBuffer;

#ifdef __cplusplus
	const axl_log_TDataConfig* GetPreferredDataConfig()
		{ return axl_log_TIndexMgr_GetPreferredDataConfig(this); }

	void SetPreferredDataConfig(const axl_log_TDataConfig* pConfig)
		{ axl_log_TIndexMgr_SetPreferredDataConfig(this, pConfig); }

	void ClearIndex()
		{ axl_log_TIndexMgr_ClearIndex(this); }

	size_t UpdateIndex()
		{ return axl_log_TIndexMgr_UpdateIndex(this); }

	size_t CalcMessageLineCount(axl_log_TMessage* pMsg, axl_log_TDataConfig* pPreferredDataConfig, ulong_t VolatileFlags)
		{ return axl_log_TIndexMgr_CalcMessageLineCount(this, pMsg, pPreferredDataConfig, VolatileFlags); }

	size_t ReFilterAll()
		{ return axl_log_TIndexMgr_ReFilterAll(this); }
#endif
} axl_log_TIndexMgr;

//.............................................................................

AXL_EXTERN_C_END

#ifdef __cplusplus

namespace axl {
namespace log {

typedef axl_log_TIndexMgr TIndexMgr;

} // namespace log {
} // namespace axl {

#endif

//.............................................................................
