#pragma once

#define MAINWINDOW_H

//.............................................................................

class MyWidget: public QWidget
{
public:
	MyWidget (QWidget* parent = NULL);
	
	virtual
	void
	paintEvent (QPaintEvent* e);
};

//.............................................................................

class MainWindow: public QMainWindow
{
	Q_OBJECT

public:
	MainWindow (QWidget* parent = NULL);

	virtual QSize sizeHint () const
	{
		return QSize (640, 480);
	}

private:
	QTextEdit m_editWidget;
	MyWidget m_myWidget;
};

//.............................................................................
