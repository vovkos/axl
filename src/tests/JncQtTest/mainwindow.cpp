#include "pch.h"
#include "mainwindow.h"
#include "mdichild.h"
#include "output.h"
#include "modulepane.h"
#include "llvmir.h"
#include "disassembly.h"
#include "moc_mainwindow.cpp"
#include "qrc_jancyedit.cpp"

int
StdLib_printf (
	const char* pFormat,
	...
	)
{
	AXL_VA_DECL (va, pFormat);

	rtl::CString Text;
	size_t Length = Text.Format_va (pFormat, va);

	WriteOutput (Text, Length);

	return Length;
}

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags)
	: QMainWindow(parent, flags)
{	
	llvmExecutionEngine = 0;

	createMdiArea();
	setCentralWidget(mdiArea);

	createActions();
	createMenu();
	createToolBars();
	createPanes();
	createStatusBar();

	readSettings();
}

void MainWindow::closeEvent(QCloseEvent *e)
{
	writeSettings();

	mdiArea->closeAllSubWindows();
	if (mdiArea->currentSubWindow()) {
		e->ignore();
	} else {
		e->accept();
	}
}

void MainWindow::newFile()
{
	MdiChild *child = createMdiChild();
	child->newFile();
	child->showMaximized();
}

void MainWindow::openFile(QString filePath)
{
	if (filePath.isEmpty())
	{
		filePath = QFileDialog::getOpenFileName(this, "Open File", "",
							"Jancy Files (*.jnc);;All Files (*.*)");
	}

	if (filePath.isEmpty())
		return;

	QMdiSubWindow *subWindow = findMdiSubWindow(filePath);
	if(subWindow) {
		mdiArea->setActiveSubWindow(subWindow);
	} else {
		MdiChild *child = createMdiChild();
		if (child->loadFile(filePath)) {
			writeStatus("File loaded", 2000);
			child->showMaximized();
		} else {
			child->close();
		}
	}
}

void MainWindow::saveFile()
{
	 if (MdiChild* mdiChild = activeMdiChild())
		 if (mdiChild->save())
			 writeStatus("File saved", 2000);
} 

void MainWindow::saveAs()
{
	if (MdiChild* mdiChild = activeMdiChild())
		 if (mdiChild->saveAs())
			 writeStatus("File saved", 2000);
}

void MainWindow::createActions()
{
	newFileAction = new QAction(QIcon(":/Images/New"), "&New", this);
	newFileAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
	QObject::connect(newFileAction, SIGNAL(triggered()),
		this, SLOT(newFile()));

	openFileAction = new QAction(QIcon(":/Images/Open"), "&Open", this);
	QObject::connect(openFileAction, SIGNAL(triggered()),
		this, SLOT(openFile()));

	saveFileAction = new QAction(QIcon(":/Images/Save"), "&Save", this);
	saveFileAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
	QObject::connect(saveFileAction, SIGNAL(triggered()),
		this, SLOT(saveFile()));

	saveAsAction = new QAction("S&ave as...", this);
	QObject::connect(saveAsAction, SIGNAL(triggered()),
		this, SLOT(saveAs()));
	
	quitAction = new QAction("&Exit", this);
	QObject::connect(quitAction, SIGNAL(triggered()),
		qApp, SLOT(quit()));

	clearOutputAction = new QAction("&Clear Output", this);
	QObject::connect(clearOutputAction, SIGNAL(triggered()),
		this, SLOT(clearOutput()));

	compileAction = new QAction(QIcon(":/Images/Compile"), "C&ompile", this);
	compileAction->setShortcut(QKeySequence(Qt::Key_F7));
	QObject::connect(compileAction, SIGNAL(triggered()),
		this, SLOT(compile()));

	runAction = new QAction(QIcon(":/Images/Run"), "&Run", this);
	runAction->setShortcut(QKeySequence(Qt::Key_F5));
	QObject::connect(runAction, SIGNAL(triggered()),
		this, SLOT(run()));
}

void MainWindow::createMenu()
{
	fileMenu = menuBar()->addMenu("&File");
	fileMenu->addAction(newFileAction);
	fileMenu->addAction(openFileAction);
	fileMenu->addSeparator();
	fileMenu->addAction(saveFileAction);
	fileMenu->addAction(saveAsAction);
	fileMenu->addSeparator();
	fileMenu->addAction(quitAction);

	editMenu = menuBar()->addMenu("&Edit");
	editMenu->addAction(clearOutputAction);

	debugMenu = menuBar()->addMenu("&Debug");
	debugMenu->addAction(compileAction);
	debugMenu->addAction(runAction);

	viewMenu = menuBar()->addMenu("&View");
}

void MainWindow::createToolBars()
{
	mainToolBar = addToolBar("Main Toolbar");
	mainToolBar->addAction(newFileAction);
	mainToolBar->addAction(openFileAction);
	mainToolBar->addAction(saveFileAction);
	mainToolBar->addSeparator();
	mainToolBar->addAction(compileAction);
	mainToolBar->addAction(runAction);

	viewMenu->addAction(mainToolBar->toggleViewAction());
}

void MainWindow::createStatusBar()
{
	writeStatus("Ready");
}

void MainWindow::createMdiArea() 
{
	mdiArea = new QMdiArea(this);
	mdiArea->setViewMode(QMdiArea::TabbedView);
	mdiArea->setTabShape(QTabWidget::Triangular);
	mdiArea->setTabsClosable(true);
	mdiArea->setTabsMovable(true);
	
	QTabBar *tabBar = mdiArea->findChild<QTabBar *>();
	if (tabBar)
		tabBar->setExpanding(false);
}

void MainWindow::createPanes()
{
	output = new Output(this);
	modulePane = new ModulePane(this);
	llvmIr = new LlvmIr(this);
	disassembly = new Disassembly(this);

	addPane(output, "Output", Qt::BottomDockWidgetArea);
	addPane(modulePane, "Module", Qt::RightDockWidgetArea);

	QDockWidget *llvmIrDock = addPane(llvmIr, "LLVM IR",
		Qt::RightDockWidgetArea);

	QDockWidget *disassemblyDock = addPane(disassembly, "Disassembly",
		Qt::RightDockWidgetArea);

	tabifyDockWidget(llvmIrDock, disassemblyDock);
	llvmIrDock->raise();
}

QDockWidget *MainWindow::addPane(QWidget *widget, const QString &title,
	Qt::DockWidgetArea dockArea)
{
	QDockWidget *dockWidget = new QDockWidget(title, this);
	dockWidget->setWidget(widget);

	addDockWidget(dockArea, dockWidget);

	viewMenu->addAction(dockWidget->toggleViewAction());

	return dockWidget;
}

void MainWindow::writeStatus(const QString &text, int timeout)
{
	statusBar()->showMessage(text, timeout);
}

void MainWindow::writeOutput_va(const char* format, va_list va)
{
	output->appendFormat_va (format, va);
	output->repaint ();
}

void MainWindow::writeOutput(const char* format, ...)
{
	va_list va;
	va_start (va, format);
	writeOutput_va (format, va);
}

MdiChild *MainWindow::findMdiChild(const QString &filePath)
{
	MdiChild *child = 0;

	QMdiSubWindow *subWindow = findMdiSubWindow(filePath);
	if (subWindow)
		child = qobject_cast<MdiChild *>(subWindow->widget());

	return child;
}

void MainWindow::readSettings()
{
	QSettings s;

	QStringList files = s.value("filesOpened").toStringList();
	
	foreach (QString file, files)
		openFile(file);
}

void MainWindow::writeSettings()
{
	QSettings s;

	QStringList files;
	foreach (QMdiSubWindow *subWindow, mdiArea->subWindowList())
		if(MdiChild *child = qobject_cast<MdiChild *>(subWindow->widget()))
			files.append(child->file());

	if(!files.isEmpty())
		s.setValue("filesOpened", files);
}

jnc::CFunction *MainWindow::findGlobalFunction(const QString &name)
{
	QByteArray nameBytes = name.toLocal8Bit();
	jnc::CModuleItem *item =
		module.m_NamespaceMgr.GetGlobalNamespace()->FindItem(nameBytes.data());

	if(!item)
		return NULL;

	if(item->GetItemKind() != jnc::EModuleItem_Function)
		return NULL;

	return (jnc::CFunction *)item;
}

void MainWindow::clearOutput()
{
	output->clear();
}

bool MainWindow::compile ()
{
	qApp->setCursorFlashTime (0);

	bool Result;

	MdiChild *child = activeMdiChild();
	if (!child)
		return false;

	if(!child->save())
		return false;

	if (llvmExecutionEngine)
	{
		delete llvmExecutionEngine;
		llvmExecutionEngine = NULL;
	}	

	QByteArray filePathBytes = child->file().toUtf8 ();
	module.Create (filePathBytes.data());
	
	llvm::Module* pLlvmModule = new llvm::Module (filePathBytes.data(), llvm::getGlobalContext ());
	module.m_pLlvmModule = pLlvmModule;

	jnc::CScopeThreadModule ScopeModule (&module);

	writeOutput("Parsing...\n");

	modulePane->clear ();
	llvmIr->clear ();
	disassembly->clear ();

	QByteArray sourceBytes = child->toPlainText().toUtf8();

	jnc::CLexer Lexer;
	Lexer.Create (
		filePathBytes.data (), 
		sourceBytes.data (), 
		sourceBytes.size ()
		);

	jnc::CParser Parser;
	Parser.Create (jnc::CParser::StartSymbol, true);

	for (;;)
	{
		const jnc::CToken* pToken = Lexer.GetToken ();
		if (pToken->m_Token == jnc::EToken_Eof)
			break;

		Result = Parser.ParseToken (pToken);
		if (!Result)
		{
			rtl::CString Text = err::GetError ()->GetDescription ();

			writeOutput(
				"%s(%d,%d): %s\n", 
				filePathBytes.data (), 
				pToken->m_Pos.m_Line + 1, 
				pToken->m_Pos.m_Col + 1, 
				Text.cc ()
				);
			return false;
		}

		Lexer.NextToken ();
	}

	writeOutput("Compiling...\n");
	Result = module.Compile ();

	// show module contents nevetheless

	modulePane->build (&module, child);
	llvmIr->build (&module);

	if (!Result)
	{
		writeOutput("%s\n", err::GetError ()->GetDescription ().cc ());
		return false;
	}

	writeOutput("JITting...\n");

	llvm::EngineBuilder EngineBuilder (pLlvmModule);	
	std::string errorString;
	EngineBuilder.setErrorStr (&errorString);
	EngineBuilder.setUseMCJIT(true);

	llvmExecutionEngine = EngineBuilder.create ();
	if (!llvmExecutionEngine)
	{
		writeOutput ("Error creating a JITter (%s)\n", errorString.c_str());
		return false;
	}

	jnc::CStdLib::Export (&module, llvmExecutionEngine);
	module.SetFunctionPointer (llvmExecutionEngine, "printf", (void*) StdLib_printf);

	Result = module.m_FunctionMgr.JitFunctions (llvmExecutionEngine);	
	if (!Result)
	{
		writeOutput("%s\n", err::GetError ()->GetDescription ().cc ());
		return false;
	}

	disassembly->build (&module);
	writeOutput ("Done.\n");
	child->setCompilationNeeded (false);
	return true;
}

bool MainWindow::runFunction (jnc::CFunction* pFunction, int* pReturnValue)
{
	typedef int (*FFunction) ();
	FFunction pf = (FFunction) pFunction->GetMachineCode ();
	ASSERT (pf);

	bool Result = true;

	try
	{
		int ReturnValue = pf ();
		if (pReturnValue)
			*pReturnValue = ReturnValue;
	}
	catch (err::CError Error)
	{
		writeOutput ("ERROR: %s\n", Error.GetDescription ().cc ());
		Result = false;
	}
	catch (...)
	{
		writeOutput ("UNKNOWN EXCEPTION\n");
		Result = false;
	}

	return Result;
}	

bool 
MainWindow::run ()
{
	bool Result;

	 MdiChild* mdiChild = activeMdiChild();
	 if (!mdiChild)
		 return true;

	if (mdiChild->isCompilationNeeded ())
	{
		Result = compile ();
		if (!Result)
			return false;
	}

	jnc::CFunction* pMainFunction = findGlobalFunction ("main");
	if (!pMainFunction)
	{
		writeOutput ("'main' is not found or not a function\n");
		return false;
	}

	writeOutput ("Running...\n");

	jnc::CScopeThreadModule ScopeModule (&module);

	// create a new heap (actually, can re-use the old one)

	module.m_GcHeap.CreateHeap (16, 1, 4);
	module.m_GcHeap.InitializeRoots (llvmExecutionEngine);

	// constructor

	jnc::CFunction* pConstructor = module.GetConstructor ();
	if (pConstructor)
	{
		Result = runFunction (pConstructor);
		if (!Result)
			return false;
	}

	// main

	int ReturnValue;
	Result = runFunction (pMainFunction, &ReturnValue);
	if (!Result)
		return false;

	// destructor

	jnc::CFunction* pDestructor = module.GetDestructor ();
	if (pDestructor)
	{
		Result = runFunction (pDestructor);
		if (!Result)
			return false;
	}

	// final gc run

	module.m_GcHeap.DropGlobalRoots ();
	module.m_GcHeap.RunGc ();

	writeOutput ("Done (retval = %d).\n", ReturnValue);
	return true;
}

MdiChild *MainWindow::createMdiChild()
{
	MdiChild *child = new MdiChild(this);
	child->setAttribute(Qt::WA_DeleteOnClose);
	mdiArea->addSubWindow(child);

	return child;
}

MdiChild *MainWindow::activeMdiChild()
 {
	 QMdiSubWindow *activeSubWindow = mdiArea->activeSubWindow();
		 
	 if (!activeSubWindow && !mdiArea->subWindowList().empty())
		 activeSubWindow = mdiArea->subWindowList().at(0);

	 if (!activeSubWindow)
		 return 0;

	 return qobject_cast<MdiChild *>(activeSubWindow->widget());
 }

QMdiSubWindow *MainWindow::findMdiSubWindow(const QString &filePath)
{
	QString canonicalFilePath = QFileInfo(filePath).canonicalFilePath();

	foreach (QMdiSubWindow *subWindow, mdiArea->subWindowList()) {
		MdiChild *child = qobject_cast<MdiChild *>(subWindow->widget());
		if(child && child->file() == canonicalFilePath)
			return subWindow;
	}

	return 0;
}