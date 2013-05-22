// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_G_WIN_WINDOW_H

#include "axl_exe_Thunk.h"
#include "axl_g_Module.h"
#include "axl_err_Error.h"
#include "axl_rtl_Handle.h"

namespace axl {
namespace g {
namespace win {

//.............................................................................

class CWindow: public rtl::CHandleT <HWND>
{
public:
	// CWindowImplT <> overridables

	enum
	{
		ClassStyle      = CS_DBLCLKS,
		ClassCursor     = (int) IDC_ARROW,
		ClassBackground = COLOR_WINDOW,
	};

public:
	// Win32 wrappers

	bool
	Create (
		const wchar_t* pClassName, 
		HWND hWndParent,
		const RECT* pRect = NULL,
		const wchar_t* pWindowName = NULL,
		uint_t Style = 0,
		uint_t StyleEx = 0,
		uint_t ID = 0,
		void* pParam = NULL
		)
	{
		static RECT _DefaultRect = { CW_USEDEFAULT, CW_USEDEFAULT, 0, 0 };

		if (!pRect)
			pRect = &_DefaultRect;

		m_h = ::CreateWindowExW (
			StyleEx, 
			pClassName,
			pWindowName, 
			Style,
			pRect->left,
			pRect->top,
			pRect->right - pRect->left,
			pRect->bottom - pRect->top,
			hWndParent,
			(HMENU) ID,
			g::GetModule ()->GetModuleHandle (),
			pParam
			);

		return err::Complete (m_h != NULL);
	}

	bool 
	DestroyWindow ()
	{
		bool_t Result = ::DestroyWindow (m_h);
		return err::Complete (Result);
	}

	LONG_PTR
	GetClassLongPtr (int Index)
	{
		return ::GetClassLongPtr (m_h, Index);
	}

	LONG_PTR
	SetClassLongPtr (
		int Index, 
		LONG_PTR Value
		)
	{
		return ::SetClassLongPtr (m_h, Index, Value);
	}

	LONG_PTR
	GetWindowLongPtr (int Index)
	{
		return ::GetWindowLongPtr (m_h, Index);
	}

	LONG_PTR
	SetWindowLongPtr (
		int Index, 
		LONG_PTR Value
		)
	{
		return ::SetWindowLongPtr (m_h, Index, Value);
	}

	LRESULT
	SendMessage (
		UINT Msg, 
		WPARAM wParam = 0,
		LPARAM lParam = 0
		)
	{
		return ::SendMessage (m_h, Msg, wParam, lParam);
	}

	LRESULT
	PostMessage (
		UINT Msg, 
		WPARAM wParam = 0,
		LPARAM lParam = 0
		)
	{
		return ::PostMessage (m_h, Msg, wParam, lParam);
	}

	bool 
	ShowWindow (int Show = SW_SHOW)
	{
		bool_t Result = ::ShowWindow (m_h, Show);
		return err::Complete (Result);
	}

	bool 
	SetWindowPos (
		HWND hWndInsertAfter,
		int x,
		int y,
		int Width,
		int Height,
		uint_t Flags
		)
	{
		bool_t Result = ::SetWindowPos (m_h, hWndInsertAfter, x, y, Width, Height, Flags);
		return err::Complete (Result);
	}

	HDWP 
	DeferWindowPos (
		HDWP hDwp,
		HWND hWndInsertAfter,
		int x,
		int y,
		int Width,
		int Height,
		uint_t Flags
		)
	{
		return ::DeferWindowPos (hDwp, m_h, hWndInsertAfter, x, y, Width, Height, Flags);
	}

	// CWindowImplT <> overridable WindowProc

	LRESULT 
	WindowProc (
		UINT Msg, 
		WPARAM wParam, 
		LPARAM lParam, 
		bool* pIsHandled
		) 
	{
		*pIsHandled = false;
		return 0;
	} 
};

//.............................................................................

// lightweight window implementation class
// keeps object pointer in WNDLONGPTR::0 and not in Thunk (like ATL)
// therefore, creating windows based on existing class (e.g. EDIT-based class) is not possible
// implementation on thunks would interfere with ATL and therefore it would be not possible to use axl window as a 
// superclass for ATL CWindowImpl <>. unlike ATL doesnt require supporting C++ object 
// so it's possible to register a class and then use it with ::CreateWindow or as a superclass for ATL::CWindowImpl
// note that unlike deriving from existing classes subclassing of existing windows IS supported.

extern
AXL_SELECT_ANY
const uint_t g_CreateWindowTls = TlsAlloc ();

template <
	class T, 
	class B = CWindow
	>
class CWindowImplT: public B
{
protected:
	enum EWindowState
	{
		EWindowState_Idle = 0,
		EWindowState_Normal,
		EWindowState_Internal,
		EWindowState_Attached,
		EWindowState_Subclassed
	};

	class CRegisterWindowClass
	{
	public:
		void 
		operator () (WNDCLASSW** ppWndClass)
		{
			static WNDCLASSW WndClass = { 0 };
			WndClass.style = T::ClassStyle;
			WndClass.hCursor = ::LoadCursor (NULL, MAKEINTRESOURCE (T::ClassCursor));
			WndClass.hbrBackground = (HBRUSH) (T::ClassBackground + 1);
			WndClass.hInstance = g::GetModule ()->GetModuleHandle ();
			WndClass.lpfnWndProc = &_WindowProc;
			WndClass.cbWndExtra = sizeof (T*);				
			WndClass.lpszClassName = T::GetClassName ();

			if (!WndClass.lpszClassName)
			{
				static wchar_t ClassName [64] = { 0 };
				_snwprintf (ClassName, countof (ClassName) - 1, L"axl:%x", &_WindowProc);
				WndClass.lpszClassName = ClassName;
			}; 

			bool_t Result = ::RegisterClass (&WndClass);
			if (!Result)
			{
				err::SetLastSystemError ();
				return;
			}

			*ppWndClass = &WndClass;
		}
	};

protected:
	int m_WindowState;

	// subclass

	exe::TThunk* m_pSubclassWindowThunk;
	WNDPROC m_pfOriginalWindowProc;

public:
	CWindowImplT ()
	{		
		m_WindowState = EWindowState_Idle;
		m_pSubclassWindowThunk = NULL;
		m_pfOriginalWindowProc = NULL;
	}

	~CWindowImplT ()
	{
#ifndef _AXL_DISABLE_PREMATURE_WINDOW_DESTRUCTION_CHECK
		ASSERT (!m_h);
#endif
		Detach ();
	}

	// reset wndclass name every time

	static 
	const wchar_t*
	GetClassName ()
	{ 
		return NULL;
	}

	static 
	bool
	Register ()
	{
		return GetWndClass () != NULL;
	}

	static
	const WNDCLASS*
	GetWndClass ()
	{
		static WNDCLASS* pWndClass = NULL;
		mt::CallOnce (CRegisterWindowClass (), &pWndClass);
		return pWndClass;
	}

	void
	Attach (HWND hWnd)
	{
		Detach ();

		m_h = hWnd;
		m_WindowState = EWindowState_Attached;
	}

	HWND 
	Detach ()
	{
		return DetachEx (false);
	}

	bool 
	Create (
		HWND hWndParent,
		const RECT* pRect = NULL,
		const wchar_t* pWindowName = NULL,
		uint_t Style = 0,
		uint_t StyleEx = 0,
		uint_t ID = 0,
		void* pParam = NULL
		)
	{ 
		Detach ();

		TlsSetValue (g_CreateWindowTls, static_cast <T*> (this));

		const WNDCLASS* pWndClass = GetWndClass ();
		if (!pWndClass)
			return false;

		bool Result = CWindow::Create (
			pWndClass->lpszClassName, 
			hWndParent, 
			pRect, 
			pWindowName, 
			Style, 
			StyleEx, 
			ID, 
			pParam
			); 

		TlsSetValue (g_CreateWindowTls, NULL);

		return Result;
	}

	bool 
	Subclass (HWND hWnd)
	{
		Detach ();

		m_pSubclassWindowThunk = exe::CreateThunk (_SubclassWindowProc, static_cast <T*> (this));
		if (!m_pSubclassWindowThunk)
			return false;

		m_h = hWnd;
		m_WindowState = EWindowState_Subclassed;
		m_pfOriginalWindowProc = (WNDPROC) ::SetWindowLongPtr (hWnd, GWLP_WNDPROC, (LONG_PTR) m_pSubclassWindowThunk);	
		return true;
	}
	
	static 
	T* 
	FromHandle (HWND hWnd)
	{
		return (T*) (CWindow*) ::GetWindowLongPtr (hWnd, 0);
	}

	LRESULT 
	DefWindowProc (
		UINT Msg, 
		WPARAM wParam, 
		LPARAM lParam
		)
	{
		return m_WindowState == EWindowState_Subclassed ? 
			::CallWindowProc (m_pfOriginalWindowProc, m_h, Msg, wParam, lParam) :
			::DefWindowProc (m_h, Msg, wParam, lParam);
	}

protected:	
	HWND 
	DetachEx (bool DoDrop)
	{
		HWND hWnd = m_h;

		WNDPROC pfPrev;

		switch (m_WindowState)
		{
		case EWindowState_Idle:
			return NULL;

		case EWindowState_Attached:
			break;

		case EWindowState_Internal:
			ASSERT (DoDrop); // should only be called from _WindowProc::WM_NCDESTROY
			break;

		case EWindowState_Normal:
			if (!DoDrop)
			{
				pfPrev = (WNDPROC) SetWindowLongPtr (GWLP_WNDPROC, (LONG_PTR) ::DefWindowProc);
				ASSERT (pfPrev == (WNDPROC) &_WindowProc); // check if anybody has subclassed us
				SetWindowLongPtr (0, NULL);
			}

			break;

		case EWindowState_Subclassed:
			ASSERT (m_pSubclassWindowThunk);

			if (!DoDrop)
			{
				pfPrev = (WNDPROC) SetWindowLongPtr (GWLP_WNDPROC, (LONG_PTR) m_pfOriginalWindowProc);
				ASSERT (pfPrev == (WNDPROC) m_pSubclassWindowThunk); // check if anybody has over-subclassed us
			}

			AXL_MEM_FREE (m_pSubclassWindowThunk);
			m_pSubclassWindowThunk = NULL;
			m_pfOriginalWindowProc = NULL;
			break;
		}

		m_h = NULL;
		m_WindowState = EWindowState_Idle;
		return hWnd;
	}

	LRESULT 
	ProcessMessage (
		UINT Msg, 
		WPARAM wParam, 
		LPARAM lParam
		)
	{
		bool IsHandled = true;
		LRESULT Result = static_cast <T*> (this)->WindowProc (Msg, wParam, lParam, &IsHandled);
		
		if (!IsHandled)
			Result = DefWindowProc (Msg, wParam, lParam);

		return Result;
	}

	// actual WNDPROC procedures

	static 
	LRESULT 
	WINAPI
	_WindowProc (
		HWND hWnd,
		UINT Msg, 
		WPARAM wParam, 
		LPARAM lParam
		)
	{
		if (Msg == WM_NCCREATE)
		{
			T* pThis = (T*) TlsGetValue (g_CreateWindowTls);
			if (pThis)
			{
				pThis->m_WindowState = EWindowState_Normal;
			}
			else
			{
				pThis = AXL_MEM_NEW (T);
				pThis->m_WindowState = EWindowState_Internal;
			}

			pThis->m_h = hWnd;
			::SetWindowLongPtr (hWnd, 0, (LONG_PTR) pThis);

			return pThis->ProcessMessage (Msg, wParam, lParam);
		}

		T* pThis = (T*) ::GetWindowLongPtr (hWnd, 0);
		if (!pThis)
			return ::DefWindowProc (hWnd, Msg, wParam, lParam);

		ASSERT (pThis->m_WindowState == EWindowState_Normal || pThis->m_WindowState == EWindowState_Internal);

		LRESULT Result = pThis->ProcessMessage (Msg, wParam, lParam);

		if (Msg == WM_NCDESTROY)
		{
			bool DoDelete = pThis->m_WindowState == EWindowState_Internal;

			pThis->DetachEx (true);

			if (DoDelete)
				AXL_MEM_DELETE (pThis);
		}

		return Result;
	}

	static 
	LRESULT 
	WINAPI
	_SubclassWindowProc (
		T* pThis, // HWND gonna be replaced with T* by thunk proc
		UINT Msg, 
		WPARAM wParam, 
		LPARAM lParam
		)
	{
		ASSERT (pThis->m_WindowState == EWindowState_Subclassed);

		LRESULT Result = pThis->ProcessMessage (Msg, wParam, lParam);

		if (Msg == WM_NCDESTROY)
			pThis->DetachEx (true);

		return Result;
	}
};

//.............................................................................

// convenient macros

#define AXL_G_WINDOW_CLASS(pName) \
static \
const wchar_t* \
GetClassName () \
{ \
	return pName; \
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_G_WINDOW_MSG_MAP_BEGIN() \
LRESULT \
WindowProc ( \
	UINT Msg, \
	WPARAM wParam, \
	LPARAM lParam, \
	bool* pIsHandled \
	) \
{ \
	LRESULT Result = 0; \
	switch (Msg) \
	{

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_G_WINDOW_MSG_HANDLER(MsgCode, Handler) \
	case MsgCode: \
		Result = Handler (Msg, wParam, lParam, pIsHandled); \
		break; \

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_G_WINDOW_MSG_MAP_END() \
	default: \
		*pIsHandled = false; \
	} \
	return Result; \
} 

//.............................................................................

} // namespace win
} // namespace g
} // namespace axl

