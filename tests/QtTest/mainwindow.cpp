#include "pch.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "moc_mainwindow.cpp"

//.............................................................................

#ifdef _JANCY_REPRESENTER

stdLib::stdLib ()
{
	m_functionMap ["printf"] = (void*) printf;
	m_functionMap ["rand"]   = (void*) rand;
}

int
stdLib::printf (
	const char* format,
	...
	)
{
	AXL_VA_DECL (va, format);
	return vprintf (format, va.m_va);
}

#endif

//.............................................................................

mainWindow::mainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new ui::mainWindow)
{
	ui->setupUi(this);
	m_logWidget = new gui::qtWidget <myLogWidget> (ui->centralWidget);
}

mainWindow::~mainWindow()
{
	delete ui;
}

bool mainWindow::initialize ()
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
		m_logWidget->w ()->create (
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
	result = m_logWidget->w ()->create (
		&m_memoryServer, 
		"d:/test_log.njidx"
		);

	log::TBinDataConfig binDataConfig;
	binDataConfig.m_binViewKind = log::EBinView_Text;

	static const char data [] = 
		"suka bla\n"
		"govno nahui\n"
		"huicov poesh\n"
		"pidaras\n";

	m_memoryServer.create (
		(log::CClientPeer*) &m_logWidgetProxy, 
		&binDataConfig,
		data, sizeof (data) - 1,
		g::getTimestamp ()
		);

#endif

	ASSERT (result);
	
	return true;
}

void mainWindow::resizeEvent (QResizeEvent* e)
{
	m_logWidget->setGeometry (ui->centralWidget->geometry ());
}

//.............................................................................
