#include "pch.h"

using namespace axl;

namespace test_Log { 

//.............................................................................

enum EMsg
{
	EMsg_DeviceOpened     = 1,
	EMsg_DeviceClosed     = 2 | log::EPacketCodeFlag_Foldable,
	EMsg_Connect          = 3,
	EMsg_ConnectComplete  = 4,
	EMsg_ConnectError     = 5,	
	EMsg_Bin              = 6 | log::EPacketCodeFlag_Mergeable,
};

class CLogWidget: public log::CWidget
{
public:
	CLogWidget (gui::IEngine* pEngine):
		log::CWidget (pEngine)
	{
	}

protected:
	AXL_GUI_WIDGET_MSG_MAP_BEGIN ()
		AXL_GUI_WIDGET_MSG_HANDLER (gui::EWidgetMsg_Close, OnClose)
		AXL_GUI_WIDGET_MSG_HANDLER (gui::EWidgetMsg_KeyDown, OnKeyDown)
	AXL_GUI_WIDGET_MSG_MAP_END_CHAIN (log::CWidget)

	void
	OnClose (
		gui::TWidgetMsg* pMsg,
		bool* pIsHandled
		)
	{
		PostQuitMessage (0);
	}

	void
	OnKeyDown (
		gui::TWidgetMsg* pMsg,
		bool* pIsHandled
		)
	{
		gui::TWidgetKeyMsg* pKeyMsg = (gui::TWidgetKeyMsg*) pMsg;

		switch (pKeyMsg->m_Key)
		{
		case '1':
			GetPacketFile ()->Write (EMsg_DeviceOpened);
			break;

		case '2':
			GetPacketFile ()->Write (EMsg_DeviceClosed);
			break;

		case '3':
			GetPacketFile ()->Write (EMsg_Connect);
			break;

		case '4':
			GetPacketFile ()->Write (EMsg_ConnectComplete);
			break;

		case '5':
			GetPacketFile ()->Write (EMsg_ConnectError);
			break;

		default:
			GetPacketFile ()->Write (EMsg_Bin, &pKeyMsg->m_Key, 1);
		}

		UpdateLog ();
	}
};

class CRepresentor: public log::IRepresentor
{
public:
	AXL_OBJ_CLASS_0 (CRepresentor, log::IRepresentor)

	virtual 
	bool 
	Represent (
		log::IRepresentorTarget* pTarget, 
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
		case EMsg_DeviceOpened:
			pTarget->m_LineAttr.m_Icon = 3;

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

			pTarget->AddBin(Data, sizeof(Data));
			break;

		case EMsg_DeviceClosed:
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

		case EMsg_Connect:
			pTarget->m_LineAttr.m_Icon = 4;
			pTarget->m_LineAttr.m_BackColor = gui::EStdColor_PastelGreen;

			pTarget->AddHyperText(
				log::EMergeFlag_MergeableForward,
				"Connecting..."
				);
			break;

		case EMsg_ConnectComplete:
			pTarget->m_LineAttr.m_Icon = 1;
			pTarget->m_LineAttr.m_BackColor = gui::EStdColor_PastelBlue;

			pTarget->AddHyperText(
				EMsg_Connect,
				log::EMergeFlag_MergeableBackward,
				"OK!"
				);
			break;

		case EMsg_ConnectError:
			pTarget->m_LineAttr.m_Icon = 0;
			pTarget->m_LineAttr.m_BackColor = gui::EStdColor_PastelRed;
			pTarget->AddHyperText(
				EMsg_Connect,
				log::EMergeFlag_MergeableBackward,
				"<\b*>FAIL!"
				);
			break;

		case EMsg_Bin:
			pTarget->m_LineAttr.m_Icon = 8;
			pTarget->m_LineAttr.m_Flags |= log::ELineAttrFlag_TileIcon;
			pTarget->AddBin(p, Size);
			break;

		default:
			return false;
		}

		return true;
	}
};


void
Run ()
{
/*
	io::CMappedFile f;
	f.Open ("d:\\c.txt");
	uchar_t* p = (uchar_t*) f.View ();
	uchar_t* pEnd = p + f.GetSize ();

	for (size_t i = 0; p < pEnd; p++, i++)
	{
		if (i >= 8)
		{
			printf ("\n");
			i = 0;
		}

		printf ("0x%02x, ", *p);
	}

	return;
*/
	printf ("hui govno i muravei\n");

	log::CPacketFile PacketFile;
	PacketFile.Open ("test_log.nlog");
	
	CRepresentor Representor;

	gui::gdi::CWidgetT <CLogWidget> Widget;
	Widget.Create (NULL, NULL, L"Test", WS_OVERLAPPEDWINDOW | WS_VISIBLE);
	Widget.SetPacketFile (&PacketFile, &Representor);

	MSG Msg;
	while (GetMessage(&Msg, NULL, 0, 0))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
}

//.............................................................................

} // namespace test_Log
