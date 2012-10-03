// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_WIN_WINDOW_H

#include "axl_exe_Thunk.h"
#include "axl_g_Module.h"
#include "axl_err_Error.h"

namespace axl {
namespace win {

//.............................................................................

class CWindow
{
public:
	// CWindowImplT <> overridables

	enum
	{
		ClassStyle      = CS_DBLCLKS,
		ClassCursor     = (int) IDC_ARROW,
		ClassBackground = COLOR_WINDOW,
	};

protected:
	HWND m_hWnd;

public:
	CWindow ()
	{
		m_hWnd = NULL;
	}

	operator HWND ()
	{ 
		return m_hWnd; 
	}

	void 
	Attach (HWND hWnd)
	{
		m_hWnd = hWnd;
	}

	HWND
	Detach ()
	{
		HWND hWnd = m_hWnd;
		m_hWnd = NULL;
		return hWnd;
	}

	// Win32 wrappers

	bool
	Create (
		const tchar_t* pClassName, 
		HWND hWndParent,
		const RECT* pRect = NULL,
		const tchar_t* pWindowName = NULL,
		ulong_t Style = 0,
		ulong_t StyleEx = 0,
		uint_t ID = 0,
		void* pParam = NULL
		)
	{
		static RECT _DefaultRect = { CW_USEDEFAULT, CW_USEDEFAULT, 0, 0 };

		if (!pRect)
			pRect = &_DefaultRect;

		m_hWnd = ::CreateWindowEx (
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

		return err::Complete (m_hWnd != NULL);
	}

	bool 
	DestroyWindow ()
	{
		bool_t Result = ::DestroyWindow (m_hWnd);
		return err::Complete (Result);
	}

	LONG_PTR
	GetClassLongPtr (int Index)
	{
		return ::GetClassLongPtr (m_hWnd, Index);
	}

	LONG_PTR
	SetClassLongPtr (
		int Index, 
		LONG_PTR Value
		)
	{
		return ::SetClassLongPtr (m_hWnd, Index, Value);
	}

	LONG_PTR
	GetWindowLongPtr (int Index)
	{
		return ::GetWindowLongPtr (m_hWnd, Index);
	}

	LONG_PTR
	SetWindowLongPtr (
		int Index, 
		LONG_PTR Value
		)
	{
		return ::SetWindowLongPtr (m_hWnd, Index, Value);
	}

	LRESULT
	SendMessage (
		UINT Msg, 
		WPARAM wParam = 0,
		LPARAM lParam = 0
		)
	{
		return ::SendMessage (m_hWnd, Msg, wParam, lParam);
	}

	LRESULT
	PostMessage (
		UINT Msg, 
		WPARAM wParam = 0,
		LPARAM lParam = 0
		)
	{
		return ::PostMessage (m_hWnd, Msg, wParam, lParam);
	}

	bool 
	ShowWindow (int Show = SW_SHOW)
	{
		bool_t Result = ::ShowWindow (m_hWnd, Show);
		return err::Complete (Result);
	}

	bool 
	SetWindowPos (
		HWND hWndInsertAfter,
		int x,
		int y,
		int Width,
		int Height,
		int Flags
		)
	{
		bool_t Result = ::SetWindowPos (m_hWnd, hWndInsertAfter, x, y, Width, Height, Flags);
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
		int Flags
		)
	{
		return ::DeferWindowPos (hDwp, m_hWnd, hWndInsertAfter, x, y, Width, Height, Flags);
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

extern const DWORD AXL_SELECT_ANY g_CreateWindowTls = TlsAlloc ();

template <
	class T, 
	class B = CWindow
	>
class CWindowImplT: public B
{
protected:
	enum EState
	{
		EState_Idle = 0,
		EState_Normal,
		EState_Internal,
		EState_Attached,
		EState_Subclassed
	};

	class CRegister
	{
	protected:
		WNDCLASS** m_ppWndClass;
		long* m_pLastError;

	public:
		CRegister (
			WNDCLASS** ppWndClass,
			long* pLastError
			)
		{
			m_ppWndClass = ppWndClass;
			m_pLastError = pLastError;
		}

		void 
		operator () ()
		{
			static WNDCLASS _WndClass = { 0 };
			_WndClass.style = T::ClassStyle;
			_WndClass.hCursor = ::LoadCursor (NULL, MAKEINTRESOURCE (T::ClassCursor));
			_WndClass.hbrBackground = (HBRUSH) (T::ClassBackground + 1);
			_WndClass.hInstance = g::GetModule ()->GetModuleHandle ();
			_WndClass.lpfnWndProc = &_WindowProc;
			_WndClass.cbWndExtra = sizeof (T*);				
			_WndClass.lpszClassName = T::GetClassName ();

			if (!_WndClass.lpszClassName)
			{
				static tchar_t _ClassName [64];
				_stprintf (_ClassName, _T("axl:%x"), &_WindowProc);
				_WndClass.lpszClassName = _ClassName;
			}; 

			bool_t Result = ::RegisterClass (&_WndClass);

			*m_ppWndClass = Result ? &_WndClass : NULL;
			*m_pLastError = Result ? ERROR_SUCCESS : ::GetLastError ();
		}
	};

protected:
	int m_State;

	// subclass

	exe::TThunk* m_pSubclassThunk;
	WNDPROC m_pfnOriginalWindowProc;

public:
	CWindowImplT ()
	{		
		m_State = EState_Idle;
		m_pSubclassThunk = NULL;
		m_pfnOriginalWindowProc = NULL;
	}

	~CWindowImplT ()
	{
#ifndef _AXL_DISABLE_PREMATURE_WINDOW_DESTRUCTION_CHECK
		ASSERT (!m_hWnd);
#endif
		Detach ();
	}

	// reset wndclass name every time

	static 
	const tchar_t*
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
		static WNDCLASS* _pWndClass = NULL;
		static long _LastError = ERROR_SUCCESS;
		
		exe::CallOnce (CRegister (&_pWndClass, &_LastError));
		
		if (!_pWndClass)
		{
			err::SetError (_LastError);
			return NULL;
		}

		return _pWndClass;
	}

	void
	Attach (HWND hWnd)
	{
		Detach ();

		m_hWnd = hWnd;
		m_State = EState_Attached;
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
		const tchar_t* pWindowName = NULL,
		ulong_t Style = 0,
		ulong_t StyleEx = 0,
		uint_t ID = 0,
		void* pParam = NULL
		)
	{ 
		Detach ();

		TlsSetValue (g_CreateWindowTls, (T*) this);

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

		m_pSubclassThunk = exe::CreateThunk (_SubclassWindowProc, (T*) this);
		if (!m_pSubclassThunk)
			return false;

		m_hWnd = hWnd;
		m_State = EState_Subclassed;
		m_pfnOriginalWindowProc = (WNDPROC) ::SetWindowLongPtr (hWnd, GWLP_WNDPROC, (LONG_PTR) m_pSubclassThunk);	
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
		return m_State == EState_Subclassed ? 
			::CallWindowProc (m_pfnOriginalWindowProc, m_hWnd, Msg, wParam, lParam) :
			::DefWindowProc (m_hWnd, Msg, wParam, lParam);
	}

protected:
	HWND 
	DetachEx (bool DoDrop)
	{
		HWND hWnd = m_hWnd;

		WNDPROC pfnPrev;

		switch (m_State)
		{
		case EState_Idle:
			return NULL;

		case EState_Attached:
			break;

		case EState_Internal:
			ASSERT (DoDrop); // should only be called from _WindowProc::WM_NCDESTROY
			break;

		case EState_Normal:
			if (!DoDrop)
			{
				pfnPrev = (WNDPROC) SetWindowLongPtr (GWLP_WNDPROC, (LONG_PTR) ::DefWindowProc);
				ASSERT (pfnPrev == (WNDPROC) &_WindowProc); // check if anybody has subclassed us
				SetWindowLongPtr (0, NULL);
			}

			break;

		case EState_Subclassed:
			ASSERT (m_pSubclassThunk);

			if (!DoDrop)
			{
				pfnPrev = (WNDPROC) SetWindowLongPtr (GWLP_WNDPROC, (LONG_PTR) m_pfnOriginalWindowProc);
				ASSERT (pfnPrev == (WNDPROC) m_pSubclassThunk); // check if anybody has over-subclassed us
			}

			AXL_MEM_FREE (m_pSubclassThunk);
			m_pSubclassThunk = NULL;
			m_pfnOriginalWindowProc = NULL;
			break;
		}

		m_hWnd = NULL;
		m_State = EState_Idle;
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
		LRESULT Result = ((T*) this)->WindowProc (Msg, wParam, lParam, &IsHandled);
		
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
				pThis->m_State = EState_Normal;
			}
			else
			{
				pThis = AXL_MEM_NEW (T);
				pThis->m_State = EState_Internal;
			}

			pThis->m_hWnd = hWnd;
			::SetWindowLongPtr (hWnd, 0, (LONG_PTR) pThis);

			return pThis->ProcessMessage (Msg, wParam, lParam);
		}

		T* pThis = (T*) ::GetWindowLongPtr (hWnd, 0);
		if (!pThis)
			return ::DefWindowProc (hWnd, Msg, wParam, lParam);

		ASSERT (pThis->m_State == EState_Normal || pThis->m_State == EState_Internal);

		LRESULT Result = pThis->ProcessMessage (Msg, wParam, lParam);

		if (Msg == WM_NCDESTROY)
		{
			bool DoDelete = pThis->m_State == EState_Internal;

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
		ASSERT (pThis->m_State == EState_Subclassed);

		LRESULT Result = pThis->ProcessMessage (Msg, wParam, lParam);

		if (Msg == WM_NCDESTROY)
			pThis->DetachEx (true);

		return Result;
	}
};

//.............................................................................

// convenient macros

#define AXL_WINDOW_CLASS(pName) \
static \
const tchar_t* \
GetClassName () \
{ \
	return pName; \
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_WINDOW_MESSAGE_MAP_BEGIN() \
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

#define AXL_WINDOW_MESSAGE_HANDLER(MsgCode, Handler) \
	case MsgCode: \
		Result = Handler (Msg, wParam, lParam, pIsHandled); \
		break; \

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_WINDOW_MESSAGE_MAP_END() \
	default: \
		*pIsHandled = false; \
	} \
	return Result; \
} 

//.............................................................................

} // namespace win
} // namespace axl

