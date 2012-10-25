#pragma once

#include "LogCtrl.h"

//.............................................................................

class COutputPane : public CDockablePane
{
public:
	CLogCtrl m_LogCtrl;

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
};

//.............................................................................

