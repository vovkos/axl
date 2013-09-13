#include "pch.h"
#include "axl_log_BinDataConfig.h"

namespace axl {
namespace log {

//.............................................................................

void
TBinDataConfig::SetDefaults ()
{
	m_BinViewKind       = EBinView_Hex;
	m_BinTextEnc        = EBinTextEnc_Ascii;
	m_BinTextTabSize    = 4;
	m_BinTextLineSize   = 80;
	m_BinHexLineSize    = 16;
	m_BinMergeThreshold = -1;
}

//.............................................................................

} // namespace log
} // namespace axl
