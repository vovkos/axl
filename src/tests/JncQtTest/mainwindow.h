#ifndef MAINWINDOW_H
#define MAINWINDOW_H

class Disassembly;
class LlvmIr;
class ModulePane;
class Output;
class MdiChild;

class StdLib: public jnc::CStdLib
{
public:
	StdLib ();
	
	static
	int
	Printf (
		const char* pFormat,
		...
		);	
};

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0);

	QSize sizeHint() const { return QSize(800, 600); }

	void writeStatus(const QString &text, int timeout = 0);
	void writeOutput_va(const char* format, va_list va);
	void writeOutput(const char* format, ...);
	MdiChild *findMdiChild(const QString &filePath);
	
protected:
	void closeEvent(QCloseEvent *e);

private slots:
	void newFile();
	void openFile(QString filePath = "");
	void saveFile();
	void saveAs();
	void clearOutput();
	bool compile();
	bool run();
	void outputSlot ();

signals:
	void outputSignal ();

private:
	void createActions();
	void createMenu();
	void createToolBars();
	void createStatusBar();
	void createMdiArea();

	bool runFunction (jnc::CFunction* pFunction, int* pReturnValue = NULL);

	void createPanes();
	QDockWidget *addPane(QWidget *widget, const QString &title, Qt::DockWidgetArea dockArea);

	MdiChild *createMdiChild();
	MdiChild *activeMdiChild();
	QMdiSubWindow *findMdiSubWindow(const QString &filePath);
	void readSettings();
	void writeSettings();

	jnc::CFunction *findGlobalFunction(const QString &name);

	QMdiArea *mdiArea;

	Output *output;
	ModulePane *modulePane;
	LlvmIr *llvmIr;
	Disassembly *disassembly;

	QMutex outputMutex;
	QStringList outputQueue;

	QMenu *fileMenu;
	QMenu *editMenu;
	QMenu *debugMenu;
	QMenu *viewMenu;

	QToolBar *mainToolBar;

	QAction *quitAction;
	QAction *newFileAction;
	QAction *openFileAction;
	QAction *saveFileAction;
	QAction *saveAsAction;
	QAction *clearOutputAction;
	QAction *compileAction;
	QAction *runAction;

	jnc::CModule module;
	jnc::CRuntime runtime;
	StdLib stdlib;
};

inline MainWindow *GetMainWindow()
{
	foreach (QWidget *widget, QApplication::topLevelWidgets())
	{
		MainWindow * mainWindow = qobject_cast<MainWindow *>(widget);
		if(mainWindow)
			return mainWindow;
	}

	return NULL;
}

inline void WriteOutput_va(const char* format, va_list va)
{
	GetMainWindow()->writeOutput_va(format, va);
}

inline void WriteOutput(const char* format, ...)
{
	va_list va;
	va_start (va, format);
	GetMainWindow()->writeOutput_va(format, va);
}

#endif
