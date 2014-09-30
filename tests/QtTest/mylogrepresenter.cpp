#include "pch.h"
#include "mylogrepresenter.h"

//.............................................................................

bool 
myLogRepresenter::represent (
	log::CRepresenterTarget* target, 
	uint_t packetCode, 
	const void* p, 
	size_t size, 
	uint64_t foldFlags
	)
{
	char data [] = "0123456789abcf01234";

	switch (packetCode)
	{
	case EMyLog_DeviceOpened:
		target->m_lineAttr.m_iconIdx = 3;
		target->m_lineAttr.m_backColor = gui::EStdColor_PastelYellow;
		target->m_lineAttr.m_flags = log::ELineAttrFlag_Delimiter;			 

		target->addHyperText(
			"Device <#0000ff>COM3<> is...<+b>opened<> <=!5,3>nahui!<>\n"
			"Vot <+is>tak<> vot!!"
			);

		target->m_lineAttr.m_backColor = gui::EColorFlag_Transparent;
		target->addBin (data, sizeof (data));
		break;

	case EMyLog_DeviceClosed:
		target->m_lineAttr.m_iconIdx = 2;

		if (!(foldFlags & 1))
			target->addHyperText("[<#0000ff=+1>+<>] Device is closed.");
		else
			target->addHyperText(
				"[<#0000ff=-1>-<>] Device is closed.\n"
				"  Some info...\n"
				"  Some more info which makes a very long line..\n"
				"  bla bla bla");
		break;

	case EMyLog_Connect:
		target->m_lineAttr.m_iconIdx = 4;
		target->m_lineAttr.m_backColor = gui::EStdColor_PastelBlue;
		target->addHyperText(
			EMyLog_Connect_m | log::EPacketCodeFlag_MergeableForward,
			"Connecting..."
			);
		break;

	case EMyLog_ConnectComplete:
		target->m_lineAttr.m_flags = log::ELineAttrFlag_Override;
		target->m_lineAttr.m_iconIdx = 1;
		target->m_lineAttr.m_backColor = gui::EStdColor_PastelBlue;

		target->addHyperText(
			EMyLog_Connect_m | log::EPacketCodeFlag_MergeableBackward,
			"OK!"
			);
		break;

	case EMyLog_ConnectError:
		target->m_lineAttr.m_flags = log::ELineAttrFlag_Override;
		target->m_lineAttr.m_iconIdx = 0;
		target->m_lineAttr.m_backColor = gui::EStdColor_PastelRed;
		target->addHyperText(
			EMyLog_Connect_m | log::EPacketCodeFlag_MergeableBackward,
			"<\b*>FAIL!"
			);
		break;

	case EMyLog_Bin:
		target->m_lineAttr.m_iconIdx = 8;
		target->m_lineAttr.m_flags |= log::ELineAttrFlag_TileIcon;
		target->addBin(p, size);
		break;

	default:
		return false;
	}

	return true;
}

//.............................................................................
