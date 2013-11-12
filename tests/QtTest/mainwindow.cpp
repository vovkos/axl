#include "pch.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "moc_mainwindow.cpp"

//.............................................................................

#ifdef _JANCY_REPRESENTER

StdLib::StdLib ()
{
	m_FunctionMap ["printf"] = (void*) Printf;
	m_FunctionMap ["rand"]   = (void*) rand;
}

int
StdLib::Printf (
	const char* pFormat,
	...
	)
{
	AXL_VA_DECL (va, pFormat);
	return vprintf (pFormat, va.m_va);
}

#endif

//.............................................................................

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	m_logWidget = new gui::QtWidget <MyLogWidget> (ui->centralWidget);
}

MainWindow::~MainWindow()
{
	delete ui;
}

bool MainWindow::initialize ()
{
	bool result;

	// connect proxies

	result = 
		m_logServerProxy.listen (m_logWidget->w (), 1001) &&
		m_logWidgetProxy.connect (&m_logServer, 0x7f000001, 1001);
	
	ASSERT (result);

	// create widget first, then server (cause server will start populating index immediatly)
#if 0
	result = 
		m_logWidget->w ()->Create (
			(log::CServerPeer*) &m_logServerProxy, 
			"d:/test_log.njidx"
			) &&
		m_logServer.start (
			(log::CClientPeer*) &m_logWidgetProxy, 
			"d:/test_log.njlog",
			"d:/test_log.njmrg",
			"d:/test_log.njcol"
			);
#else
	result = m_logWidget->w ()->Create (
		&m_memoryServer, 
		"d:/test_log.njidx"
		);

	log::TBinDataConfig BinDataConfig;
	BinDataConfig.m_BinViewKind = log::EBinView_Text;

	static const char data [] = 
		"suka bla\n"
		"govno nahui\n"
		"huicov poesh\n"
		"pidaras\n";

	m_memoryServer.Create (
		(log::CClientPeer*) &m_logWidgetProxy, 
		&BinDataConfig,
		data, sizeof (data) - 1,
		g::GetTimestamp ()
		);

#endif

	ASSERT (result);
	
	return true;
}

void MainWindow::resizeEvent (QResizeEvent* e)
{
	m_logWidget->setGeometry (ui->centralWidget->geometry ());
}

//.............................................................................
