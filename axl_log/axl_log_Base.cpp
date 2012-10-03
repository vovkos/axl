#include "axl_Precomp.h"
#include "axl_log_Base.h"
#include "axl_log_Line.h"

//.............................................................................

axl_log_TDataConfig g_axl_log_TDataConfig_Default = 
{	
	axl_log_EBinView_Hex,           // ushort_t m_BinView:1;
	axl_log_EDef_BinTextTabSize,    // ushort_t m_BinTextTabSize:15;
	axl_log_EDef_BinHexLineSize,    // ushort_t m_BinHexLineSize;
	axl_log_EDef_BinTextLineSize,   // ushort_t m_BinTextLineSize;
	axl_log_EMergeThreshold_Always, // ulonglong_t m_TextMergeThreshold;
	axl_log_EMergeThreshold_Always, // ulonglong_t m_BinMergeThreshold;
};

//.............................................................................
 
bool_t
AXL_API
axl_log_TMergeCriteria_ShouldMerge(
	axl_log_TMergeCriteria* pCriteria,
	axl_log_TMergeCriteria* pCriteria2
	)
{
	ulonglong_t TimeSpan;

	if (pCriteria->m_PartCode != pCriteria2->m_PartCode ||
		pCriteria->m_LineType != pCriteria2->m_LineType ||
		!(pCriteria->m_Merge & axl_log_EMerge_Forward) ||
		!(pCriteria2->m_Merge & axl_log_EMerge_Backward))
		return false;

	TimeSpan = pCriteria2->m_Timestamp - pCriteria->m_Timestamp;

	if (pCriteria->m_LineType == axl_log_ELine_Text)
		return TimeSpan <= pCriteria2->m_DataConfig.m_TextMergeThreshold;

	return 
		axl_log_TDataConfig_Compare(&pCriteria->m_DataConfig, &pCriteria2->m_DataConfig) == 0 &&
		TimeSpan <= pCriteria2->m_DataConfig.m_BinMergeThreshold;
}

//.............................................................................
