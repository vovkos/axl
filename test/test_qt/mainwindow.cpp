#include "pch.h"
#include "mainwindow.h"
#include "moc_mainwindow.cpp"

//.............................................................................

MyWidget::MyWidget (QWidget* parent):
	QWidget (parent)
{	
}

void
MyWidget::paintEvent (QPaintEvent* e)
{
	char str [] = "abcdefghijklmnopqrstuvwxyz";

	QPainter painter (this);

	QSize s = size ();

	painter.fillRect (0, 0, s.width (), s.height (), Qt::white);
	painter.drawRect (0, 0, s.width () - 1, s.height () -1);
	painter.drawText (0, 0, s.width (), s.height (), 0, str);

	QFontMetrics fm (painter.font ());

	int x = 0;

	for (size_t i = 0; i < lengthof (str); i++)
	{
		QChar c = str [i];
		int w = fm.width (c);
		int h = fm.height ();
	
		painter.setPen (Qt::red);
		painter.drawRect (x, 0, w, h);
		painter.drawRect (x, 20, w, h);
		painter.setPen (Qt::black);
		painter.drawText (x, 20, s.width (), s.height (), 0, c);
		x += w;
	}
}

//.............................................................................

MainWindow::MainWindow (QWidget* parent) :
	QMainWindow (parent),
	m_myWidget (this),
	m_editWidget (this)

{
	QFont f ("Monospace", 10);
	f.setFixedPitch (true);
	f.setStyleHint (
		QFont::Monospace,
		(QFont::StyleStrategy) (QFont::NoFontMerging | QFont::ForceIntegerMetrics)
		);

	m_myWidget.setFont (f);
	m_myWidget.setGeometry (0, 0, 400, 40);

	m_editWidget.setFont (f);
	m_editWidget.setFrameStyle (0);
	m_editWidget.setGeometry (0, 40, 400, 100);
	m_editWidget.setContentsMargins (0, 0, 0, 0);
	m_editWidget.setText ("abcdefghijklmnopqrstuvwxyz");
}

//.............................................................................
