#include "pch.h"
#include "mylogrepresentor.h"

//.............................................................................

bool 
MyLogRepresenter::Represent (
	log::IRepresenterTarget* pTarget, 
	uint_t PacketCode, 
	const void* p, 
	size_t Size, 
	uint64_t FoldFlags
	)
{
	char Data [] = "0123456789abcf01234";

	switch (PacketCode)
	{
	case EMyLog_DeviceOpened:
		pTarget->m_LineAttr.m_IconIdx = 3;
		pTarget->m_LineAttr.m_BackColor = gui::EStdColor_PastelYellow;
		pTarget->m_LineAttr.m_Flags = log::ELineAttrFlag_Delimiter;			 

		pTarget->AddHyperText(
			"Device <#0000ff>COM3<> is...<+b>opened<> <=!5,3>nahui!<>\n"
			"Vot <+is>tak<> vot!!"
			);

		pTarget->m_LineAttr.m_BackColor = gui::EColorFlag_Transparent;
		pTarget->AddBin (Data, sizeof (Data));
		break;

	case EMyLog_DeviceClosed:
		pTarget->m_LineAttr.m_IconIdx = 2;

		if (!(FoldFlags & 1))
			pTarget->AddHyperText("[<#0000ff=+1>+<>] Device is closed.");
		else
			pTarget->AddHyperText(
				"[<#0000ff=-1>-<>] Device is closed.\n"
				"  Some info...\n"
				"  Some more info which makes a very long line..\n"
				"  bla bla bla");
		break;

	case EMyLog_Connect:
		pTarget->m_LineAttr.m_IconIdx = 4;
		pTarget->m_LineAttr.m_BackColor = gui::EStdColor_PastelGreen;
		pTarget->AddHyperText(
			EMyLog_Connect_m | log::EPacketCodeFlag_MergeableForward,
			"Connecting..."
			);
		break;

	case EMyLog_ConnectComplete:
		pTarget->m_LineAttr.m_Flags = log::ELineAttrFlag_Override;
		pTarget->m_LineAttr.m_IconIdx = 1;
		pTarget->m_LineAttr.m_BackColor = gui::EStdColor_PastelBlue;

		pTarget->AddHyperText(
			EMyLog_Connect_m | log::EPacketCodeFlag_MergeableBackward,
			"OK!"
			);
		break;

	case EMyLog_ConnectError:
		pTarget->m_LineAttr.m_Flags = log::ELineAttrFlag_Override;
		pTarget->m_LineAttr.m_IconIdx = 0;
		pTarget->m_LineAttr.m_BackColor = gui::EStdColor_PastelRed;
		pTarget->AddHyperText(
			EMyLog_Connect_m | log::EPacketCodeFlag_MergeableBackward,
			"<\b*>FAIL!"
			);
		break;

	case EMyLog_Bin:
		pTarget->m_LineAttr.m_IconIdx = 8;
		pTarget->m_LineAttr.m_Flags |= log::ELineAttrFlag_TileIcon;
		pTarget->AddBin(p, Size);
		break;

	default:
		return false;
	}

	return true;
}

//.............................................................................
