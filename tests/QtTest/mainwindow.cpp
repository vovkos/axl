#include "pch.h"
#include "mainwindow.h"
#include "moc_mainwindow.cpp"

//.............................................................................

MainWindow::MainWindow (QWidget* parent) :
	QMainWindow (parent)
{
	setCentralWidget (&m_editWidget);
}

//.............................................................................
