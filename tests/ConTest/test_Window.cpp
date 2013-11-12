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
		ClassStyle      = CS_DBLCLKS,
		ClassCursor     = (int) IDC_ARROW,
		ClassBackground = COLOR_MENUHILIGHT,
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
		AXL_G_WINDOW_MSG_HANDLER (WM_CREATE, OnCreate)
		AXL_G_WINDOW_MSG_HANDLER (WM_DESTROY, OnDestroy)
		AXL_G_WINDOW_MSG_HANDLER (WM_MOVE, OnMove)
		AXL_G_WINDOW_MSG_HANDLER (WM_SIZE, OnSize)
	AXL_G_WINDOW_MSG_MAP_END ()

	LRESULT OnCreate (UINT Msg, WPARAM wParam, LPARAM lParam, bool* pHandled)
	{
		printf ("WM_CREATE\n");
		*pHandled = false;
		return 0;
	}

	LRESULT OnDestroy (UINT Msg, WPARAM wParam, LPARAM lParam, bool* pHandled)
	{
		printf ("WM_DESTROY\n");
		PostQuitMessage (0);
		*pHandled = false;
		return 0;
	}

	LRESULT OnMove (UINT Msg, WPARAM wParam, LPARAM lParam, bool* pHandled)
	{
		printf ("WM_MOVE\n");
		*pHandled = false;
		return 0;
	}

	LRESULT OnSize (UINT Msg, WPARAM wParam, LPARAM lParam, bool* pHandled)
	{
		printf ("WM_SIZE\n");
		*pHandled = false;
		return 0;
	}
};

void
Test4_mt (int a)
{
	printf ("Test4_mt: TID: %x, i: %d\n", GetCurrentThreadId (), a);
}

void
Test4_wt (exe::IScheduler* pMtScheduler, int a)
{
	printf ("Test4_wt: TID: %x, i: %d\n", GetCurrentThreadId (), a);
	pMtScheduler->Schedule <exe::CArgT <int> > (Test4_mt, 600);
}

//.............................................................................

class CMyAtlWindow: public CWindowImpl <CMyAtlWindow>
{
public:
	DECLARE_WND_SUPERCLASS ("MyEdit", "EDIT")

	BEGIN_MSG_MAP (CMyAtlWindow)
		MESSAGE_HANDLER (WM_CREATE, OnCreate)
		MESSAGE_HANDLER (WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER (WM_MOVE, OnMove)
	END_MSG_MAP ()

	LRESULT OnCreate (UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		printf ("CMyAtlWindow::OnCreate\n");
		bHandled = false;
		return 0;
	}

	LRESULT OnDestroy (UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		printf ("CMyAtlWindow::OnDestroy\n");
		PostQuitMessage (0);
		bHandled = false;
		return 0;
	}

	LRESULT OnMove (UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		printf ("CMyAtlWindow::OnMove\n");
		bHandled = false;
		return 0;
	}
};

//.............................................................................

void 
Run ()
{
	printf ("Test4: TID: %x\n", GetCurrentThreadId ()); 

	CMyWindow::Register ();

	//CMyAtlWindow Window3;
	//Window3.Create (NULL, 0, "ATL:GOVNO", WS_OVERLAPPEDWINDOW | WS_VISIBLE);

	CMyWindow Window;
	Window.Create (NULL, NULL, "HHHUI!", WS_OVERLAPPEDWINDOW | WS_VISIBLE);

	exe::CWorkerWindow WorkerWindow;
	WorkerWindow.Create ();

	exe::CWorkerThread WorkerThread;
	WorkerThread.Start ();

	WorkerThread.Schedule <exe::CArgSeqT_2 < 
		exe::IScheduler*,
		int
		> > (Test4_wt, (exe::IScheduler*) &WorkerWindow, 300);

	MSG Msg;
	while (GetMessage (&Msg, NULL, 0, 0))
	{
		TranslateMessage (&Msg);
		DispatchMessage (&Msg);
	}
}

*/

//.............................................................................

} // namespace test_Window 
