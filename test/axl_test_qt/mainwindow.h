//..............................................................................
//
//  This file is part of the AXL library.
//
//  AXL is distributed under the MIT license.
//  For details see accompanying license.txt file,
//  the public copy of which is also available at:
//  http://tibbo.com/downloads/archive/axl/license.txt
//
//..............................................................................

#pragma once

#define MAINWINDOW_H

#define _TEST_PAINT 1
#define _TEST_EDIT  1

//..............................................................................

#if (_TEST_PAINT)

class MyWidget: public QWidget {
public:
	MyWidget(QWidget* parent = NULL);

	virtual
	void
	paintEvent(QPaintEvent* e);

	virtual
	void
	keyPressEvent(QKeyEvent *event);
};

#endif

//..............................................................................

class MainWindow: public QMainWindow {
	Q_OBJECT

public:
	MainWindow(QWidget* parent = NULL);

	virtual QSize sizeHint() const {
		return QSize(640, 480);
	}

private:
#if (_TEST_PAINT)
	MyWidget m_myWidget;
#elif (_TEST_EDIT)
	QTextEdit m_editWidget;
#endif
};

//..............................................................................
