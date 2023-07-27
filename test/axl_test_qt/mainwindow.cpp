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

#include "pch.h"
#include "mainwindow.h"
#include "moc_mainwindow.cpp"
#include "QProxyStyle"

class StyleFix: public QProxyStyle {
public:
	StyleFix(QStyle* style):
		QProxyStyle(style) {}

	int styleHint(
		StyleHint hint,
		const QStyleOption *option,
		const QWidget *widget,
		QStyleHintReturn *returnData
	) const {
		return hint == SH_ItemView_ShowDecorationSelected ?
			1 :
			QProxyStyle::styleHint(hint, option, widget, returnData);
	}
};

#if (_TEST_PAINT)

//..............................................................................

void
MyWidget::paintEvent(QPaintEvent* e) {
	QPainter painter(this);
	painter.drawText(QPointF(100, 200), QString::fromUtf16((const ushort*)L"\x25cc\x0301."));

/*	char str[] = "abcdefghijklmnopqrstuvwxyz";

	gui::HyperText hyperText;
	hyperText.setHyperText("\xd0\x91\xd0\xbe " "abc \x1b^pizda\x1b[4mdef\x1b[m suka");

	const gui::TextAttrAnchorArray* attrArray = hyperText.getAttrArray();
	sl::String_utf32 text = hyperText.getText();

	const gui::HyperlinkAnchor* anchor = hyperText.findHyperlinkByOffset(1);

	for (size_t i = 0; i < text.getLength(); i++) {
		anchor = hyperText.findHyperlinkByOffset(i);
		if (anchor)
			printf("%d -- %s\n", i, anchor->m_hyperlink.sz());
	}

	gui::QtCanvas canvas;
	canvas.m_qtPainter.begin(this);
	canvas.m_font = gui::getQtEngine()->getStdFont(gui::StdFontKind_Monospace);
	canvas.m_palette = gui::getQtEngine()->getStdPalette();

	gui::TextPainter painter(&canvas);

	QSize s = size();
	painter.m_bottom = s.height();

	painter.drawHyperText_utf32(attrArray, text);

	canvas.m_qtPainter.end(); */
}

void
MyWidget::keyPressEvent(QKeyEvent *event) {
	printf("MyWidget::keyPressEvent(mod: %x, key:%x, text: %s)\n", event->modifiers(), event->key(), event->text().toLatin1().data());

	if (event->modifiers() & Qt::AltModifier)
		printf("ALT mod\n");

	if (event->key() == Qt::Key_I)
		printf("I key\n");
}

#elif (_TEST_TREE)

void
MyTreeWidget::drawRow(
	QPainter *painter,
	const QStyleOptionViewItem &option,
    const QModelIndex &index
) const {
	QStyleOptionViewItem option2 = option;
	// option2.showDecorationSelected = true;
	QTreeWidget::drawRow(painter, option2, index);
}

void
MyTreeWidget::drawBranches(
	QPainter *painter,
	const QRect &rect,
	const QModelIndex &index
) const {
	QTreeWidget::drawBranches(painter, rect, index);
}

#endif

//..............................................................................

class SizeHintCombo: public QComboBox {
public:
	SizeHintCombo(QWidget* parent):
		QComboBox(parent) {}

	void setSizeHint(const QSize& size) {
		m_sizeHint = size;
	}

	void setSizeHint(int width, int height) {
		setSizeHint(QSize(width, height));
	}

	virtual QSize sizeHint() const {
		return m_sizeHint.isValid() ? m_sizeHint : QComboBox::sizeHint();
	}

	void setMinimumSizeHint(const QSize& size) {
		m_minimumSizeHint = size;
	}

	void setMinimumSizeHint(int width, int height) {
		setMinimumSizeHint(QSize(width, height));
	}

	virtual QSize minimumSizeHint() const {
		return m_minimumSizeHint.isValid() ? m_minimumSizeHint : QComboBox::minimumSizeHint();
	}

private:
	QSize m_sizeHint;
	QSize m_minimumSizeHint;
};

int
cloneStandardItemModel(
	QStandardItemModel* dst,
	const QStandardItemModel* src
) {
	dst->clear();

	int count = src->rowCount();
	for (int i = 0 ; i < count; i++) {
		QStandardItem* item = src->item(i);
		dst->appendRow(item->clone());
	}

	return count;
}

class MyItemModel: public QStandardItemModel {
public:
	MyItemModel(QObject* parent = NULL):
		QStandardItemModel(parent) {}

	MyItemModel(
		const QStandardItemModel* model,
		QObject* parent = NULL
	):
		QStandardItemModel(parent) {
		cloneStandardItemModel(this, model);
	}

protected:
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const {
		if (role == Qt::DisplayRole) {
			QVariant userData = QStandardItemModel::data(index, Qt::UserRole);
			if (!userData.isNull())
				return userData;
		}

		return QStandardItemModel::data(index, role);
	}
};



MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent),
#if (_TEST_PAINT)
	m_myWidget(this)
#elif (_TEST_EDIT)
	m_editWidget(this)
#elif (_TEST_TREE)
	m_treeWidget(this)
#endif
{
	QFont f("Consolas", 10);
	f.setFixedPitch(true);
	f.setStyleHint(
		QFont::Monospace,
		(QFont::StyleStrategy)(QFont::NoFontMerging | QFont::ForceIntegerMetrics)
	);

	QWidget* client = new QWidget(this);
	setCentralWidget(client);

	QVBoxLayout* vlayout = new QVBoxLayout;
	client->setLayout(vlayout);

#if (_TEST_PAINT)
	m_myWidget.setFont(f);
	m_myWidget.setGeometry(0, 0, 400, 40);
	vlayout->addWidget(&m_myWidget);
#elif (_TEST_EDIT)
	m_editWidget.setFont(f);
	m_editWidget.setFrameStyle(0);
	m_editWidget.setGeometry(0, 40, 400, 100);
	m_editWidget.setContentsMargins(0, 0, 0, 0);
	m_editWidget.setText("abcdefghijklmnopqrstuvwxyz");
	vlayout->addWidget(&m_editWidget);
#elif (_TEST_TREE)
	m_treeWidget.setFont(f);
	m_treeWidget.setFrameStyle(0);
	m_treeWidget.setAlternatingRowColors(true);
	m_treeWidget.setHeaderHidden(true);
	m_treeWidget.setRootIsDecorated(true);

	for (int i = 0; i < 10; i++) {
		QTreeWidgetItem* item = new QTreeWidgetItem;
		item->setText(0, QString("Item #%1").arg(i));
		for (int j = 0; j < 3 + i % 2; j++) {
			QTreeWidgetItem* subItem = new QTreeWidgetItem;
			subItem->setText(0, QString("SubItem #%1").arg(j));
			item->addChild(subItem);
		}

		m_treeWidget.addTopLevelItem(item);
	}

	vlayout->addWidget(&m_treeWidget);
#endif

	QHBoxLayout* hlayout = new QHBoxLayout;
	vlayout->addLayout(hlayout);

	QLabel* label1 = new QLabel(this);
	label1->setText("Adapter:");
	hlayout->addWidget(label1);

	SizeHintCombo* combo1 = new SizeHintCombo(this);
	combo1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	combo1->setEditable(true);
	combo1->setSizeAdjustPolicy(QComboBox::AdjustToContents);
	combo1->setSizeHint(combo1->sizeHint());
	combo1->setMinimumSizeHint(combo1->minimumSizeHint());
	combo1->view()->setTextElideMode(Qt::ElideRight);

	combo1->addItem("hui", "HUI");
	combo1->addItem("hui govno", "HUI GOVNO");
	combo1->addItem("hui govno i", "HUI GOVNO I");
	combo1->addItem("hui govno i muravei", "HUI GOVNO I MURAVEI");
	combo1->addItem("hui govno i muravei (c) laertsky", "LAERTSKY");

	combo1->setItemData(0, "This is a tooltip for item[0]", Qt::ToolTipRole);
	combo1->setItemData(1, "This is a tooltip for item[1]", Qt::ToolTipRole);
	combo1->setItemData(2, "This is a tooltip for item[2]", Qt::ToolTipRole);
	combo1->setItemData(3, "This is a tooltip for item[3]", Qt::ToolTipRole);

	combo1->setModel(new MyItemModel(qobject_cast<QStandardItemModel*> (combo1->model())));

	combo1->view()->setMinimumWidth(combo1->QComboBox::sizeHint().width());
	hlayout->addWidget(combo1);

	QLabel* label2 = new QLabel(this);
	label2->setText("Port:");
	hlayout->addWidget(label2);

	SizeHintCombo* combo2 = new SizeHintCombo(this);
	combo2->setEditable(true);
	combo2->lineEdit()->setText("127.0.0.1:1001");
	combo2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	hlayout->addWidget(combo2);

#if (_TEST_PAINT)
	QWidget* widget = &m_myWidget;
#elif (_TEST_EDIT)
	QWidget* widget = &m_editWidget;
#elif (_TEST_TREE)
	QWidget* widget = &m_treeWidget;
	widget->setStyle(new StyleFix(widget->style()));
#endif

	setFocusProxy(widget);
	widget->setFocus();
}

//..............................................................................
