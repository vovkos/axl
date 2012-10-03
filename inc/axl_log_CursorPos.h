// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2010. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_base_Decl.h"

AXL_EXTERN_C_BEGIN

//.............................................................................

typedef struct _axl_log_TCursorPos axl_log_TCursorPos;

AXL_INLINE
void
axl_log_TCursorPos_ConstructEx(
	axl_log_TCursorPos* pPos,
	long Line,
	long Col
	);

AXL_INLINE
void
axl_log_TCursorPos_Construct(axl_log_TCursorPos* pPos);

AXL_INLINE
int
axl_log_TCursorPos_Cmp(
	const axl_log_TCursorPos* pPos1,
	const axl_log_TCursorPos* pPos2
	);

struct _axl_log_TCursorPos
{
	long m_Line;
	long m_Col;

#ifdef __cplusplus
	int Cmp(const axl_log_TCursorPos* pPos2)
		{ return axl_log_TCursorPos_Cmp(this, pPos2); }
#endif
};

//.............................................................................

// inlines

AXL_INLINE
void
axl_log_TCursorPos_ConstructEx(
	axl_log_TCursorPos* pPos,
	long Line,
	long Col
	)
{
	pPos->m_Line = Line, pPos->m_Col = Col;
}

AXL_INLINE
void
axl_log_TCursorPos_Construct(axl_log_TCursorPos* pPos)
{
	axl_log_TCursorPos_ConstructEx(pPos, 0, 0);
}

AXL_INLINE
int
axl_log_TCursorPos_Cmp(
	const axl_log_TCursorPos* pPos1,
	const axl_log_TCursorPos* pPos2
	)
{
	return 
		pPos1->m_Line < pPos2->m_Line ? -1 : 
		pPos1->m_Line > pPos2->m_Line ?  1 : 
		pPos1->m_Col  < pPos2->m_Col  ? -1 : 
		pPos1->m_Col  > pPos2->m_Col  ?  1 : 0;
}

//.............................................................................

AXL_EXTERN_C_END

#ifdef __cplusplus

namespace axl {
namespace log {

typedef axl_log_TCursorPos TCursorPos;

class CCursorPos: public TCursorPos
{
public:
	CCursorPos(long Line = 0, long Col = 0)
		{ axl_log_TCursorPos_ConstructEx(this, Line, Col); }
};

} // namespace log 
} // namespace axl

#endif // __cplusplus

//.............................................................................
