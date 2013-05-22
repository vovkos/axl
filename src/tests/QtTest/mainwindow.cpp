#include "pch.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "moc_mainwindow.cpp"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	
	logWidget = new gui::qt::CQtWidgetT <CMyLogWidget> (ui->centralWidget);
	
	logPacketFile.Open ("test_log.nlog");
	logWidget->w ()->SetPacketFile (&logPacketFile, &logRepresentor);
	
}

MainWindow::~MainWindow()
{
	delete ui;
}

void 
MainWindow::resizeEvent (QResizeEvent* pEvent)
{
	logWidget->setGeometry (ui->centralWidget->geometry ());
}
