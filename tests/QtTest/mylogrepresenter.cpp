#include "pch.h"
#include "mylogrepresenter.h"

//.............................................................................

bool 
myLogRepresenter::represent (
	log::Representerarget* target, 
	uint_t packetCode, 
	const void* p, 
	size_t size, 
	uint64_t foldFlags
	)
{
	char data [] = "0123456789abcf01234";

	switch (packetCode)
	{
	case MyLogKind_DeviceOpened:
		target->m_lineAttr.m_iconIdx = 3;
		target->m_lineAttr.m_backColor = gui::StdColorKind_PastelYellow;
		target->m_lineAttr.m_flags = log::LineAttrFlagKind_Delimiter;			 

		target->addHyperText(
			"Device <#0000ff>COM3<> is...<+b>opened<> <=!5,3>nahui!<>\n"
			"Vot <+is>tak<> vot!!"
			);

		target->m_lineAttr.m_backColor = gui::ColorFlagKind_Transparent;
		target->addBin (data, sizeof (data));
		break;

	case MyLogKind_DeviceClosed:
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

	case MyLogKind_Connect:
		target->m_lineAttr.m_iconIdx = 4;
		target->m_lineAttr.m_backColor = gui::StdColorKind_PastelBlue;
		target->addHyperText(
			MyLogKind_Connect_m | log::PacketCodeFlagKind_MergeableForward,
			"Connecting..."
			);
		break;

	case MyLogKind_ConnectComplete:
		target->m_lineAttr.m_flags = log::LineAttrFlagKind_Override;
		target->m_lineAttr.m_iconIdx = 1;
		target->m_lineAttr.m_backColor = gui::StdColorKind_PastelBlue;

		target->addHyperText(
			MyLogKind_Connect_m | log::PacketCodeFlagKind_MergeableBackward,
			"OK!"
			);
		break;

	case MyLogKind_ConnectError:
		target->m_lineAttr.m_flags = log::LineAttrFlagKind_Override;
		target->m_lineAttr.m_iconIdx = 0;
		target->m_lineAttr.m_backColor = gui::StdColorKind_PastelRed;
		target->addHyperText(
			MyLogKind_Connect_m | log::PacketCodeFlagKind_MergeableBackward,
			"<\b*>FAIL!"
			);
		break;

	case MyLogKind_Bin:
		target->m_lineAttr.m_iconIdx = 8;
		target->m_lineAttr.m_flags |= log::LineAttrFlagKind_TileIcon;
		target->addBin(p, size);
		break;

	default:
		return false;
	}

	return true;
}

//.............................................................................
