#pragma once

#define MAINWINDOW_H

#include "mylogwidget.h"
#include "mylogserver.h"
#include "mylogproxy.h"

namespace ui {
class mainWindow;
}

class mainWindow : public QMainWindow
{
	Q_OBJECT

public:	
	gui::qtWidget <myLogWidget>* m_logWidget;
	myLogServer m_logServer;
	
	log::MemoryServer m_memoryServer;

	myLogProxy m_logServerProxy;
	myLogProxy m_logWidgetProxy;

public:
	explicit mainWindow(QWidget *parent = 0);
	~mainWindow();

	bool initialize ();
	virtual void resizeEvent (QResizeEvent* e);
	
private:
	ui::mainWindow *ui;
};

