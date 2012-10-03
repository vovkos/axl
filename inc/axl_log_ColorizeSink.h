// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2010. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_log_Base.h"

AXL_EXTERN_C_BEGIN

//.............................................................................

typedef struct _axl_log_TColorizeSink  axl_log_TColorizeSink;

void
AXL_API
axl_log_TColorizeSink_Construct(axl_log_TColorizeSink* pSink);

void
AXL_API
axl_log_TColorizeSink_Destruct(axl_log_TColorizeSink* pSink);

AXL_INLINE
void
axl_log_TColorizeSink_SetAttr(
	axl_log_TColorizeSink* pSink, 
	axl_gr_TTextAttr* pAttr,
	size_t Begin, 
	size_t End
	);

AXL_INLINE
void
axl_log_TColorizeSink_SetColorizerState(
	axl_log_TColorizeSink* pSink, 
	intptr_t State,
	axl_ref_TObject* pStateEx
	);

typedef struct _axl_log_TColorizeSink
{
	axl_gr_TTextAttrAnchorArray m_AttrArray;
	ulong_t m_ColorizerMetric;
	intptr_t m_ColorizerState;
	axl_ref_TObject* m_pColorizerStateEx;

#ifdef __cplusplus
	void SetAttr(axl_gr_TTextAttr* pAttr, size_t Begin, size_t End)
		{ axl_log_TColorizeSink_SetAttr(this, pAttr, Begin, End); }

	void SetColorizerState(intptr_t State, axl_ref_TObject* pStateEx)
		{ axl_log_TColorizeSink_SetColorizerState(this, State, pStateEx); }
#endif
} axl_log_TColorizeSink;

//.............................................................................

AXL_INLINE
void
axl_log_TColorizeSink_SetAttr(
	axl_log_TColorizeSink* pSink, 
	axl_gr_TTextAttr* pAttr,
	size_t Begin, 
	size_t End
	)
{
	axl_gr_TTextAttrAnchorArray_SetAttr(&pSink->m_AttrArray, pAttr, Begin, End, pSink->m_ColorizerMetric);
}

AXL_INLINE
void
axl_log_TColorizeSink_SetColorizerState(
	axl_log_TColorizeSink* pSink, 
	intptr_t State,
	axl_ref_TObject* pStateEx
	)
{
	pSink->m_ColorizerState = State;
	axl_ref_TObject__CopyPtr(&pSink->m_pColorizerStateEx, pStateEx);
}

//.............................................................................

AXL_EXTERN_C_END

#ifdef __cplusplus

namespace axl {
namespace log {

typedef axl_log_TColorizeSink   TColorizeSink;

} // namespace log {
} // namespace axl {

#endif

//.............................................................................
