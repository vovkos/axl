#pragma once

#define MAINWINDOW_H

#include "mylogwidget.h"
#include "mylogserver.h"
#include "mylogproxy.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:	
	gui::QtWidget <MyLogWidget>* m_logWidget;
	MyLogServer m_logServer;
	
	log::CMemoryServer m_memoryServer;

	MyLogProxy m_logServerProxy;
	MyLogProxy m_logWidgetProxy;

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	bool initialize ();
	virtual void resizeEvent (QResizeEvent* e);
	
private:
	Ui::MainWindow *ui;
};

