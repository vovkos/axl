#pragma once

#define MAINWINDOW_H

#include "mylogwidget.h"
#include "mylogrepresenter.h"
#include "jancycolorizer.h"

namespace Ui {
class MainWindow;
}

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
	gui::qt::QtWidget <MyLogWidget>* m_logWidget;
	
	jnc::CModule m_module;
	jnc::CRuntime m_runtime;
	StdLib m_stdlib;

	log::CJancyRepresenter m_logRepresenter;
	// MyLogRepresenter m_logRepresenter;
	JancyColorizer m_logColorizer;
	log::CServer m_logServer;

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	bool initialize ();
	bool compile (
		const char* representerClassName,
		size_t FileCount,
		...
		);

	virtual void resizeEvent (QResizeEvent* e);
	
private:
	Ui::MainWindow *ui;
};

