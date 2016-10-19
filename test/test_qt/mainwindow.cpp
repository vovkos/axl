#include "pch.h"
#include "mainwindow.h"
#include "moc_mainwindow.cpp"

#include "axl_g_WarningSuppression.h"
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
	sl::String text = hyperText.getText ();

	const gui::HyperlinkAnchor* anchor = hyperText.findHyperlinkByOffset (1);

	for (size_t i = 0; i < text.getLength (); i++)
	{
		anchor = hyperText.findHyperlinkByOffset (i);
		if (anchor)
			printf ("%d -- %s\n", i, anchor->m_hyperlink.sz ());
	}

	gui::QtCanvas canvas;
	canvas.m_qtPainter.begin (this);
	canvas.m_font = gui::getQtEngine ()->getStdFont (gui::StdFontKind_Monospace);
	canvas.m_palette = gui::getQtEngine ()->getStdPalette ();

	gui::TextPainter painter (&canvas);

	QSize s = size ();	
	painter.m_bottom = s.height ();

	painter.drawHyperText (attrArray, text);

	canvas.m_qtPainter.end ();
}

//.............................................................................

class SizeHintCombo: public QComboBox
{
public:
	SizeHintCombo (QWidget* parent):
		QComboBox (parent)
	{
	}

	void setSizeHint (const QSize& size)
	{
		m_sizeHint = size;
	}

	void setSizeHint (int width, int height)
	{
		setSizeHint (QSize (width, height));
	}

	virtual QSize sizeHint() const
	{
		return m_sizeHint.isValid () ? m_sizeHint : QComboBox::sizeHint ();
	}

	void setMinimumSizeHint (const QSize& size)
	{
		m_minimumSizeHint = size;
	}

	void setMinimumSizeHint (int width, int height)
	{
		setMinimumSizeHint (QSize (width, height));
	}

	virtual QSize minimumSizeHint() const
	{
		return m_minimumSizeHint.isValid () ? m_minimumSizeHint : QComboBox::minimumSizeHint ();
	}
	
private:
	QSize m_sizeHint;
	QSize m_minimumSizeHint;
};

MainWindow::MainWindow (QWidget* parent) :
	QMainWindow (parent),
//	m_myWidget (this),
	m_editWidget (this)

{
	QFont f ("Monospace", 10);
	f.setFixedPitch (true);
	f.setStyleHint (
		QFont::Monospace,
		(QFont::StyleStrategy) (QFont::NoFontMerging | QFont::ForceIntegerMetrics)
		);

	QWidget* client = new QWidget (this);
	setCentralWidget (client);

	QVBoxLayout* vlayout = new QVBoxLayout;
	client->setLayout (vlayout);

//	m_myWidget.setFont (f);
//	m_myWidget.setGeometry (0, 0, 400, 40);

	m_editWidget.setFont (f);
	m_editWidget.setFrameStyle (0);
	m_editWidget.setGeometry (0, 40, 400, 100);
	m_editWidget.setContentsMargins (0, 0, 0, 0);
	m_editWidget.setText ("abcdefghijklmnopqrstuvwxyz");

	vlayout->addWidget (&m_editWidget);

	QHBoxLayout* hlayout = new QHBoxLayout;
	vlayout->addLayout (hlayout);

	QLabel* label1 = new QLabel (this);
	label1->setText ("Adapter:");
	hlayout->addWidget (label1);

	SizeHintCombo* combo1 = new SizeHintCombo (this);
	combo1->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Fixed);
	combo1->setEditable (false);
	combo1->setSizeAdjustPolicy (QComboBox::AdjustToContents);
	combo1->setSizeHint (combo1->sizeHint ());
	combo1->setMinimumSizeHint (combo1->minimumSizeHint ());
	combo1->view ()->setTextElideMode (Qt::ElideRight);

	combo1->addItem ("hui");
	combo1->addItem ("hui govno");
	combo1->addItem ("hui govno i");
	combo1->addItem ("hui govno i muravei");
	combo1->addItem ("hui govno i muravei (c) laertsky");
	combo1->view ()->setMinimumWidth (combo1->QComboBox::sizeHint ().width ());
	hlayout->addWidget (combo1);

	QLabel* label2 = new QLabel (this);
	label2->setText ("Port:");
	hlayout->addWidget (label2);

	SizeHintCombo* combo2 = new SizeHintCombo (this);
	combo2->setEditable (true);
	combo2->lineEdit ()->setText ("127.0.0.1:1001");
	combo2->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Fixed);
	hlayout->addWidget (combo2);
}

//.............................................................................
