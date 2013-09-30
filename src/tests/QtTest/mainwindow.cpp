#include "pch.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "moc_mainwindow.cpp"

#include "moc_axl_gui_qt_Widget.cpp"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	m_logWidget = new gui::qt::QtWidget <MyLogWidget> (ui->centralWidget);

	bool Result = 
		m_logWidget->w ()->Create (
			&m_logServer, 
			"d:/test_log.njidx",
			"d:/test_log.njcol"
			) &&
		m_logServer.Create (
			m_logWidget->w (), 
			&m_logRepresenter, 
			&m_logColorizer,
			"d:/test_log.njlog",
			"d:/test_log.njmrg",
			"d:/test_log.njcol"
			);

	ASSERT (Result);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void 
MainWindow::resizeEvent (QResizeEvent* pEvent)
{
	m_logWidget->setGeometry (ui->centralWidget->geometry ());
}
