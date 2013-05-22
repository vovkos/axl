#include "pch.h"
#include "axl_log_Line.h"
#include "axl_log_CacheMgr.h"

namespace axl {
namespace log {

//.............................................................................

TLineAttr::TLineAttr ()
{
	m_Flags = 0;
	m_Icon = -1;

	m_UpperDelimiter = ELineDelimiter_Normal;
	m_LowerDelimiter = ELineDelimiter_Normal;
}

//.............................................................................

CLine::CLine ()
{
	m_LineKind = ELine_Undefined;
	m_pVolatilePacket = NULL;
	m_IsFirstLineOfPacket = false;
	m_FirstPacketOffset = 0;
	m_FirstTimestamp = 0;
	m_LastTimestamp = 0;
	m_pPage = NULL;
	m_PageIndex = 0;
	m_UserParam = 0;
	m_PartCode = 0;
	m_PartIndex = 0;
	m_MergeId = 0;
}

CWidget*
CLine::GetWidget ()
{
	return m_pPage->GetCacheMgr ()->GetWidget ();
}

//.............................................................................

} // namespace log {
} // namespace axl {
