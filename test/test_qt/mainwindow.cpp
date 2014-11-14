#include "pch.h"
#include "mainwindow.h"
#include "moc_mainwindow.cpp"

#include "axl_gui_TextPainter.h"
#include "axl_gui_HyperText.h"

//.............................................................................

MyWidget::MyWidget (QWidget* parent):
	QWidget (parent)
{	
}

void
MyWidget::paintEvent (QPaintEvent* e)
{
	char str [] = "abcdefghijklmnopqrstuvwxyz";
	
	gui::HyperText hyperText;
	hyperText.setHyperText ("abc \x1b^pizda\x1b[4mdef\x1b[m suka");
	
	const gui::TextAttrAnchorArray* attrArray = hyperText.getAttrArray ();
	rtl::String text = hyperText.getText ();

	const gui::HyperlinkAnchor* anchor = hyperText.findHyperlinkByOffset (1);

	for (size_t i = 0; i < text.getLength (); i++)
	{
		anchor = hyperText.findHyperlinkByOffset (i);
		if (anchor)
			printf ("%d -- %s\n", i, anchor->m_hyperlink);
	}

	gui::QtCanvas canvas;
	canvas.m_qtPainter.begin (this);
	canvas.m_font = gui::getQtEngine ()->getStdFont (gui::StdFontKind_Monospace);
	canvas.m_palette = gui::getQtEngine ()->getStdPalette ();

	gui::TextPainter painter (&canvas);

	QSize s = size ();	
	painter.m_bottom = s.height ();

	painter.drawHyperText (attrArray, text, text.getLength ());

	canvas.m_qtPainter.end ();
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
