#pragma once

//.............................................................................

class COutputPane : public CDockablePane
{
protected:
	CEdit m_EditCtrl;

public:
	void
	Trace0 (const tchar_t* pText);

	void
	Trace (
		const tchar_t* pFormat, 
		...
		)
	{
		TraceV (pFormat, va_start_e (pFormat));
	}
		
	void
	TraceV (
		const tchar_t* pFormat, 
		va_list va
		);

	void
	Clear ()
	{
		m_EditCtrl.SetWindowText (NULL);
	}

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
};

//.............................................................................

