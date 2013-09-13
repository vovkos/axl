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
	m_logWidget = new gui::qt::QtWidget <CMyLogWidget> (ui->centralWidget);

	bool Result = 
		m_logWidget->w ()->Create (&m_logServer, "d:/test_log.njidx") &&
		m_logServer.Create (m_logWidget->w (), &m_logRepresenter, "d:/test_log.njlog");

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
