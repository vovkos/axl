#pragma once

#define MAINWINDOW_H

#include "mylogwidget.h"
#include "mylogrepresentor.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:	
	gui::qt::QtWidget <CMyLogWidget>* m_logWidget;
	
	MyLogRepresenter m_logRepresenter;
	log::CServer m_logServer;
	
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	virtual
	void 
	resizeEvent (QResizeEvent* pEvent);
	
private:
	Ui::MainWindow *ui;
};

