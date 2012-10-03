#include "axl_Precomp.h"
#include "axl_log_RepresentSink.h"

void 
AXL_VFT
axl_log_TRepresentSink_AddData_b(
	void* pSink, 
	ulong_t Mode,
	const void* p,
	size_t Size
	)
{
}

//.............................................................................

void
AXL_API
axl_log_TRepresentSink_Construct(axl_log_TRepresentSink* pSink)
{
	static axl_log_TRepresentSinkVTable _VTable = 
	{
		axl_log_TRepresentSink_AddData_b
	};

	pSink->m_pVTable = &_VTable;
	pSink->m_Merge = 0;
	pSink->m_PartCode = 0;
	pSink->m_DataConfig = g_axl_log_TDataConfig_Default;
	pSink->m_Icon = -1;
	pSink->m_TileIcon = false;
	axl_gr_TTextAttr_Clear(&pSink->m_Attr);
}

ulong_t 
AXL_API
axl_log_TRepresentSink_GetLineType(
	axl_log_TRepresentSink* pSink,
	ulong_t Mode
	)
{
	switch (Mode)
	{
	case axl_log_ERepresentMode_HyperText:
	case axl_log_ERepresentMode_PlainText:
		return axl_log_ELine_Text;

	case axl_log_ERepresentMode_Bin:
		return pSink->m_DataConfig.m_BinView == axl_log_EBinView_Text ? 
			axl_log_ELine_BinText : 
			axl_log_ELine_BinHex;
		
	default:
		return 0;
	}
}

void
AXL_API
axl_log_TRepresentSink_AddText(
	axl_log_TRepresentSink* pSink, 
	ulong_t Mode,
	const tchar_t* pText, 
	size_t Length
	)
{
	if (Length == -1)
		Length = _tcslen(pText);

	axl_log_TRepresentSink_AddData(pSink, Mode, pText, Length * sizeof(tchar_t));
}

void 
AXL_API
axl_log_TRepresentSink_AddDataFormatV (
	axl_log_TRepresentSink* pSink, 
	ulong_t Mode,
	const tchar_t* pFormat,
	va_list va
	)
{
	axl_rtl_TString Text;
	axl_rtl_TString_Construct(&Text);
	axl_rtl_TString_FormatV (&Text, pFormat, va);
	axl_log_TRepresentSink_AddText(pSink, Mode, Text.m_p, axl_rtl_TString_GetLength(&Text));
	axl_rtl_TString_Destruct(&Text);
}

//.............................................................................

