#include "pch.h"
#include "mylogrepresentor.h"

//.............................................................................

bool 
MyLogRepresenter::Represent (
	log::IRepresenterTarget* pTarget, 
	uint_t PacketCode, 
	const void* p, 
	size_t Size, 
	uint_t VolatileFlags
	)
{
	char Data [] = 
		"sdfjshdjfjhsdjkhfkjsdfsdfsdfsdhkfhkjsdhfkjhsdkjf hsdjkfh skdjhf kjshd kj "
		"sdfjshdjfjhsdjkhfkjsdfsdfsdfsdhkfhkjsdhfkjhsdkjf hsdjkfh skdjhf kjshd kj "
		"sdfjshdjfjhsdjkhfkjsdfsdfsdfsdhkfhkjsdhfkjhsdkjf hsdjkfh skdjhf kjshd kj "
		"sdfjshdjfjhsdjkhfkjsdfsdfsdfsdhkfhkjsdhfkjhsdkjf hsdjkfh skdjhf kjshd kj ";

	switch (PacketCode)
	{
	case EMyLogPacket_DeviceOpened:
/*		pTarget->m_LineAttr.m_Icon = 3;

		pTarget->AddHyperText(
			log::EMergeFlag_MergeableForward,
			"Device <#0000ff>COM3<> is..."
			);

		pTarget->AddHyperText(
			log::EMergeFlag_Mergeable,
			"<+b>opened<> <=!5,3>nahui!<>\n"
			);

		pTarget->AddHyperText(
			log::EMergeFlag_Mergeable,
			"Vot <+is>tak<> vot!!"
			);
			 */
		pTarget->AddBin("123456789abc", 12);
		break;

	case EMyLogPacket_DeviceClosed:
		pTarget->m_LineAttr.m_Icon = 2;

		if (!(VolatileFlags & 1))
			pTarget->AddHyperText("[<#0000ff=+1>+<>] Device is closed.");
		else
			pTarget->AddHyperText(
				"[<#0000ff=-1>-<>] Device is closed.\n"
				"  Some info...\n"
				"  Some more info which makes a very long line..\n"
				"  bla bla bla");
		break;

	case EMyLogPacket_Connect:
		pTarget->m_LineAttr.m_Icon = 4;
		pTarget->m_LineAttr.m_BackColor = gui::EStdColor_PastelGreen;

		pTarget->AddHyperText(
			log::EMergeFlag_MergeableForward,
			"Connecting..."
			);
		break;

	case EMyLogPacket_ConnectComplete:
		pTarget->m_LineAttr.m_Icon = 1;
		pTarget->m_LineAttr.m_BackColor = gui::EStdColor_PastelBlue;

		pTarget->AddHyperText(
			EMyLogPacket_Connect,
			log::EMergeFlag_MergeableBackward,
			"OK!"
			);
		break;

	case EMyLogPacket_ConnectError:
		pTarget->m_LineAttr.m_Icon = 0;
		pTarget->m_LineAttr.m_BackColor = gui::EStdColor_PastelRed;
		pTarget->AddHyperText(
			EMyLogPacket_Connect,
			log::EMergeFlag_MergeableBackward,
			"<\b*>FAIL!"
			);
		break;

	case EMyLogPacket_Bin:
		pTarget->m_LineAttr.m_Icon = 8;
		pTarget->m_LineAttr.m_Flags |= log::ELineAttrFlag_TileIcon;
		pTarget->AddBin(p, Size);
		break;

	default:
		return false;
	}

	return true;
}

//.............................................................................
