#pragma once

#define MAINWINDOW_H

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
};

//.............................................................................
