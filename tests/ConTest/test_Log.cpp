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
	CLogWidget (gui::IEngine* engine):
		log::CWidget (engine)
	{
	}

protected:
	AXL_GUI_WIDGET_MSG_MAP_BEGIN ()
		AXL_GUI_WIDGET_MSG_HANDLER (gui::EWidgetMsg_Close, onClose)
		AXL_GUI_WIDGET_MSG_HANDLER (gui::EWidgetMsg_KeyDown, onKeyDown)
	AXL_GUI_WIDGET_MSG_MAP_END_CHAIN (log::CWidget)

	void
	onClose (
		gui::TWidgetMsg* msg,
		bool* isHandled
		)
	{
		postQuitMessage (0);
	}

	void
	onKeyDown (
		gui::TWidgetMsg* msg,
		bool* isHandled
		)
	{
		gui::TWidgetKeyMsg* keyMsg = (gui::TWidgetKeyMsg*) msg;

		switch (keyMsg->m_key)
		{
		case '1':
			getPacketFile ()->write (EMsg_DeviceOpened);
			break;

		case '2':
			getPacketFile ()->write (EMsg_DeviceClosed);
			break;

		case '3':
			getPacketFile ()->write (EMsg_Connect);
			break;

		case '4':
			getPacketFile ()->write (EMsg_ConnectComplete);
			break;

		case '5':
			getPacketFile ()->write (EMsg_ConnectError);
			break;

		default:
			getPacketFile ()->write (EMsg_Bin, &keyMsg->m_key, 1);
		}

		updateLog ();
	}
};

class CRepresentor: public log::IRepresentor
{
public:
	virtual 
	bool 
	represent (
		log::IRepresentorTarget* target, 
		uint_t packetCode, 
		const void* p, 
		size_t size, 
		uint_t volatileFlags
		)
	{
		char data [] = 
			"sdfjshdjfjhsdjkhfkjsdfsdfsdfsdhkfhkjsdhfkjhsdkjf hsdjkfh skdjhf kjshd kj "
			"sdfjshdjfjhsdjkhfkjsdfsdfsdfsdhkfhkjsdhfkjhsdkjf hsdjkfh skdjhf kjshd kj "
			"sdfjshdjfjhsdjkhfkjsdfsdfsdfsdhkfhkjsdhfkjhsdkjf hsdjkfh skdjhf kjshd kj "
			"sdfjshdjfjhsdjkhfkjsdfsdfsdfsdhkfhkjsdhfkjhsdkjf hsdjkfh skdjhf kjshd kj ";

		switch (packetCode)
		{
		case EMsg_DeviceOpened:
			target->m_lineAttr.m_icon = 3;

			target->addHyperText(
				log::EMergeFlag_MergeableForward,
				"Device <#0000ff>COM3<> is..."
				);

			target->addHyperText(
				log::EMergeFlag_Mergeable,
				"<+b>opened<> <=!5,3>nahui!<>\n"
				);

			target->addHyperText(
				log::EMergeFlag_Mergeable,
				"Vot <+is>tak<> vot!!"
				);

			target->addBin(data, sizeof(data));
			break;

		case EMsg_DeviceClosed:
			target->m_lineAttr.m_icon = 2;

			if (!(volatileFlags & 1))
				target->addHyperText("[<#0000ff=+1>+<>] Device is closed.");
			else
				target->addHyperText(
					"[<#0000ff=-1>-<>] Device is closed.\n"
					"  Some info...\n"
					"  Some more info which makes a very long line..\n"
					"  bla bla bla");
			break;

		case EMsg_Connect:
			target->m_lineAttr.m_icon = 4;
			target->m_lineAttr.m_backColor = gui::EStdColor_PastelGreen;

			target->addHyperText(
				log::EMergeFlag_MergeableForward,
				"Connecting..."
				);
			break;

		case EMsg_ConnectComplete:
			target->m_lineAttr.m_icon = 1;
			target->m_lineAttr.m_backColor = gui::EStdColor_PastelBlue;

			target->addHyperText(
				EMsg_Connect,
				log::EMergeFlag_MergeableBackward,
				"OK!"
				);
			break;

		case EMsg_ConnectError:
			target->m_lineAttr.m_icon = 0;
			target->m_lineAttr.m_backColor = gui::EStdColor_PastelRed;
			target->addHyperText(
				EMsg_Connect,
				log::EMergeFlag_MergeableBackward,
				"<\b*>FAIL!"
				);
			break;

		case EMsg_Bin:
			target->m_lineAttr.m_icon = 8;
			target->m_lineAttr.m_flags |= log::ELineAttrFlag_TileIcon;
			target->addBin(p, size);
			break;

		default:
			return false;
		}

		return true;
	}
};

void
run ()
{
/*
	io::CMappedFile f;
	f.open ("d:\\c.txt");
	uchar_t* p = (uchar_t*) f.view ();
	uchar_t* end = p + f.getSize ();

	for (size_t i = 0; p < end; p++, i++)
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

	log::CPacketFile packetFile;
	packetFile.open ("test_log.nlog");
	
	CRepresentor representor;

	gui::gdi::CWidgetT <CLogWidget> widget;
	widget.create (NULL, NULL, L"Test", WS_OVERLAPPEDWINDOW | WS_VISIBLE);
	widget.setPacketFile (&packetFile, &representor);

	MSG msg;
	while (getMessage(&msg, NULL, 0, 0))
	{
		translateMessage(&msg);
		dispatchMessage(&msg);
	}
}

//.............................................................................

} // namespace test_Log
