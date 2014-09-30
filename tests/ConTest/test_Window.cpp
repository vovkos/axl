#include "pch.h"

using namespace axl;

namespace test_Window { 

//.............................................................................

/*

class CMyWindow: public g::win::CWindowImplT <CMyWindow>
{
public:
	enum
	{
		classStyle      = CS_DBLCLKS,
		classCursor     = (int) IDC_ARROW,
		classBackground = COLOR_MENUHILIGHT,
	};

public:
	CMyWindow ()
	{
		printf ("CMyWindow::CMyWindow ()\n");
	}

	~CMyWindow ()
	{
		printf ("CMyWindow::~CMyWindow ()\n");
	}

//	AXL_G_WINDOW_CLASS ("CMyWindow")

	AXL_G_WINDOW_MSG_MAP_BEGIN ()
		AXL_G_WINDOW_MSG_HANDLER (WM_CREATE, onCreate)
		AXL_G_WINDOW_MSG_HANDLER (WM_DESTROY, onDestroy)
		AXL_G_WINDOW_MSG_HANDLER (WM_MOVE, onMove)
		AXL_G_WINDOW_MSG_HANDLER (WM_SIZE, onSize)
	AXL_G_WINDOW_MSG_MAP_END ()

	LRESULT onCreate (UINT msg, WPARAM wParam, LPARAM lParam, bool* handled)
	{
		printf ("WM_CREATE\n");
		*handled = false;
		return 0;
	}

	LRESULT onDestroy (UINT msg, WPARAM wParam, LPARAM lParam, bool* handled)
	{
		printf ("WM_DESTROY\n");
		postQuitMessage (0);
		*handled = false;
		return 0;
	}

	LRESULT onMove (UINT msg, WPARAM wParam, LPARAM lParam, bool* handled)
	{
		printf ("WM_MOVE\n");
		*handled = false;
		return 0;
	}

	LRESULT onSize (UINT msg, WPARAM wParam, LPARAM lParam, bool* handled)
	{
		printf ("WM_SIZE\n");
		*handled = false;
		return 0;
	}
};

void
test4_mt (int a)
{
	printf ("Test4_mt: TID: %x, i: %d\n", getCurrentThreadId (), a);
}

void
test4_wt (exe::IScheduler* mtScheduler, int a)
{
	printf ("Test4_wt: TID: %x, i: %d\n", getCurrentThreadId (), a);
	mtScheduler->schedule <exe::CArgT <int> > (test4_mt, 600);
}

//.............................................................................

class CMyAtlWindow: public CWindowImpl <CMyAtlWindow>
{
public:
	DECLARE_WND_SUPERCLASS ("MyEdit", "EDIT")

	BEGIN_MSG_MAP (CMyAtlWindow)
		MESSAGE_HANDLER (WM_CREATE, onCreate)
		MESSAGE_HANDLER (WM_DESTROY, onDestroy)
		MESSAGE_HANDLER (WM_MOVE, onMove)
	END_MSG_MAP ()

	LRESULT onCreate (UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		printf ("CMyAtlWindow::OnCreate\n");
		bHandled = false;
		return 0;
	}

	LRESULT onDestroy (UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		printf ("CMyAtlWindow::OnDestroy\n");
		postQuitMessage (0);
		bHandled = false;
		return 0;
	}

	LRESULT onMove (UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		printf ("CMyAtlWindow::OnMove\n");
		bHandled = false;
		return 0;
	}
};

//.............................................................................

void 
run ()
{
	printf ("Test4: TID: %x\n", getCurrentThreadId ()); 

	CMyWindow::register ();

	//CMyAtlWindow Window3;
	//Window3.Create (NULL, 0, "ATL:GOVNO", WS_OVERLAPPEDWINDOW | WS_VISIBLE);

	CMyWindow window;
	window.create (NULL, NULL, "HHHUI!", WS_OVERLAPPEDWINDOW | WS_VISIBLE);

	exe::CWorkerWindow workerWindow;
	workerWindow.create ();

	exe::CWorkerThread workerThread;
	workerThread.start ();

	workerThread.schedule <exe::CArgSeqT_2 < 
		exe::IScheduler*,
		int
		> > (test4_wt, (exe::IScheduler*) &workerWindow, 300);

	MSG msg;
	while (getMessage (&msg, NULL, 0, 0))
	{
		translateMessage (&msg);
		dispatchMessage (&msg);
	}
}

*/

//.............................................................................

} // namespace test_Window 
