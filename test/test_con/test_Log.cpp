#include "pch.h"

namespace test_Log {

//..............................................................................

enum MsgKind
{
	MsgKind_DeviceOpened     = 1,
	MsgKind_DeviceClosed     = 2 | log::PacketCodeFlag_Foldable,
	MsgKind_Connect          = 3,
	MsgKind_ConnectComplete  = 4,
	MsgKind_ConnectError     = 5,
	MsgKind_Bin              = 6 | log::PacketCodeFlag_Mergeable,
};

class LogWidget: public log::Widget
{
public:
	LogWidget (gui::IEngine* engine):
		log::Widget (engine)
	{
	}

protected:
	AXL_GUI_WIDGET_MSG_MAP_BEGIN ()
		AXL_GUI_WIDGET_MSG_HANDLER (gui::WidgetMsgKind_Close, onClose)
		AXL_GUI_WIDGET_MSG_HANDLER (gui::WidgetMsgKind_KeyDown, onKeyDown)
	AXL_GUI_WIDGET_MSG_MAP_END_CHAIN (log::Widget)

	void
	onClose (
		gui::WidgetMsg* msg,
		bool* isHandled
		)
	{
		postQuitMessage (0);
	}

	void
	onKeyDown (
		gui::WidgetMsg* msg,
		bool* isHandled
		)
	{
		gui::WidgetKeyMsg* keyMsg = (gui::WidgetKeyMsg*) msg;

		switch (keyMsg->m_key)
		{
		case '1':
			getPacketFile ()->write (MsgKind_DeviceOpened);
			break;

		case '2':
			getPacketFile ()->write (MsgKind_DeviceClosed);
			break;

		case '3':
			getPacketFile ()->write (MsgKind_Connect);
			break;

		case '4':
			getPacketFile ()->write (MsgKind_ConnectComplete);
			break;

		case '5':
			getPacketFile ()->write (MsgKind_ConnectError);
			break;

		default:
			getPacketFile ()->write (MsgKind_Bin, &keyMsg->m_key, 1);
		}

		updateLog ();
	}
};

class Representor: public log::IRepresentor
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
		case MsgKind_DeviceOpened:
			target->m_lineAttr.m_icon = 3;

			target->addHyperText(
				log::MergeFlag_MergeableForward,
				"Device <#0000ff>COM3<> is..."
				);

			target->addHyperText(
				log::MergeFlag_Mergeable,
				"<+b>opened<> <=!5,3>nahui!<>\n"
				);

			target->addHyperText(
				log::MergeFlag_Mergeable,
				"Vot <+is>tak<> vot!!"
				);

			target->addBin(data, sizeof(data));
			break;

		case MsgKind_DeviceClosed:
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

		case MsgKind_Connect:
			target->m_lineAttr.m_icon = 4;
			target->m_lineAttr.m_backColor = gui::StdColorKind_PastelGreen;

			target->addHyperText(
				log::MergeFlag_MergeableForward,
				"Connecting..."
				);
			break;

		case MsgKind_ConnectComplete:
			target->m_lineAttr.m_icon = 1;
			target->m_lineAttr.m_backColor = gui::StdColorKind_PastelBlue;

			target->addHyperText(
				MsgKind_Connect,
				log::MergeFlag_MergeableBackward,
				"OK!"
				);
			break;

		case MsgKind_ConnectError:
			target->m_lineAttr.m_icon = 0;
			target->m_lineAttr.m_backColor = gui::StdColorKind_PastelRed;
			target->addHyperText(
				MsgKind_Connect,
				log::MergeFlag_MergeableBackward,
				"<\b*>FAIL!"
				);
			break;

		case MsgKind_Bin:
			target->m_lineAttr.m_icon = 8;
			target->m_lineAttr.m_flags |= log::LineAttrFlag_TileIcon;
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
	io::MappedFile f;
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

	log::PacketFile packetFile;
	packetFile.open ("test_log.nlog");

	Representor representor;

	gui::gdi::Widget <LogWidget> widget;
	widget.create (NULL, NULL, L"Test", WS_OVERLAPPEDWINDOW | WS_VISIBLE);
	widget.setPacketFile (&packetFile, &representor);

	MSG msg;
	while (getMessage(&msg, NULL, 0, 0))
	{
		translateMessage(&msg);
		dispatchMessage(&msg);
	}
}

//..............................................................................

} // namespace test_Log
