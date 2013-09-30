#pragma once

#define MAINWINDOW_H

#include "mylogwidget.h"
#include "mylogrepresentor.h"
#include "jancycolorizer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:	
	gui::qt::QtWidget <MyLogWidget>* m_logWidget;
	
	MyLogRepresenter m_logRepresenter;
	JancyColorizer m_logColorizer;
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

