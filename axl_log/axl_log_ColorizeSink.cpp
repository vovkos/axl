#include "axl_Precomp.h"
#include "axl_log_ColorizeSink.h"
#include "axl_log_ColorizeMgr.h"
#include "axl_log_CacheMgr.h"
#include "axl_log_Line.h"

//.............................................................................

void
AXL_API
axl_log_TColorizeSink_Construct(axl_log_TColorizeSink* pSink)
{
	axl_gr_TTextAttrAnchorArray_Construct(&pSink->m_AttrArray);
	pSink->m_ColorizerState = 0;
	pSink->m_pColorizerStateEx = NULL;
	pSink->m_ColorizerMetric = 0;
}

void
AXL_API
axl_log_TColorizeSink_Destruct(axl_log_TColorizeSink* pSink)
{
	axl_ref_TObject__ClearPtr(&pSink->m_pColorizerStateEx);
	axl_gr_TTextAttrAnchorArray_Destruct(&pSink->m_AttrArray);
}

//.............................................................................
