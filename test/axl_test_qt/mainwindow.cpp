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

enum UiColor {
	// QT palette

	UiColor_Text,
	UiColor_TextSelected,
	UiColor_TextDisabled,
	UiColor_Back,
	UiColor_BackAlt,
	UiColor_BackDisabled,
	UiColor_Window,
	UiColor_Button,
	UiColor_Tooltip,
	UiColor_Link,
	UiColor_LinkVisited,
	UiColor_Light,
	UiColor_Midlight,
	UiColor_Mid,
	UiColor_Dark,
	UiColor_Selection,
	UiColor_SelectionInactive,
	UiColor_SelectionAlt,
	UiColor_SelectionAltInactive,

	// njscroll colors

	UiColor_Timestamp,
	UiColor_Offset,
	UiColor_Highlight,
	UiColor_Delimiter,
	UiColor_DelimiterLight,

	// ANSI colors

	UiColor_Gray,
	UiColor_Red,
	UiColor_Green,
	UiColor_Orange,
	UiColor_Blue,
	UiColor_Purple,
	UiColor_Cyan,
	UiColor_Yellow,

	// log background colors

	UiColor_PastelGray,
	UiColor_PastelRed,
	UiColor_PastelGreen,
	UiColor_PastelOrange,
	UiColor_PastelBlue,
	UiColor_PastelPurple,
	UiColor_PastelCyan,
	UiColor_PastelYellow,

	// syntax highlight

	UiColor_Keyword,
	UiColor_Constant,
	UiColor_Comment,

	UiColor__Count
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

uint_t backColors[] = {
	UiColor_Back,
	UiColor_BackAlt,
	UiColor_BackDisabled,
	UiColor_Window,
	UiColor_Button,
	UiColor_Tooltip,
	UiColor_Selection,
	UiColor_SelectionInactive,
	UiColor_SelectionAlt,
	UiColor_SelectionAltInactive,
	UiColor_Highlight,
	UiColor_PastelGray,
	UiColor_PastelRed,
	UiColor_PastelGreen,
	UiColor_PastelOrange,
	UiColor_PastelBlue,
	UiColor_PastelPurple,
	UiColor_PastelCyan,
	UiColor_PastelYellow,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

uint_t textColors[] = {
	UiColor_Text,
	UiColor_TextDisabled,
	UiColor_Link,
	UiColor_LinkVisited,
	UiColor_Timestamp,
	UiColor_Offset,
	UiColor_Gray,
	UiColor_Red,
	UiColor_Green,
	UiColor_Orange,
	UiColor_Blue,
	UiColor_Purple,
	UiColor_Cyan,
	UiColor_Yellow,
	UiColor_Keyword,
	UiColor_Constant,
	UiColor_Comment,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const char* colorNames[UiColor__Count] = {
	"text",
	"text-selected",
	"text-disabled",
	"back",
	"back-alt",
	"back-disabled",
	"window",
	"button",
	"tooltip",
	"link",
	"link-visited",
	"light",
	"midlight",
	"mid",
	"dark",
	"selection",
	"selection-inactive",
	"selection-alt",
	"selection-alt-inactive",
	"timestamp",
	"offset",
	"highlight",
	"delimiter",
	"delimiter-light",
	"gray",
	"red",
	"green",
	"orange",
	"blue",
	"purple",
	"cyan",
	"yellow",
	"pastel-gray",
	"pastel-red",
	"pastel-green",
	"pastel-orange",
	"pastel-blue",
	"pastel-purple",
	"pastel-cyan",
	"pastel-yellow",
	"keyword",
	"constant",
	"comment",
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

uint_t lightTheme[UiColor__Count] = {
	0x000000, // text
	0xffffff, // text-selected
	0x787878, // text-disabled
	0xffffff, // back
	0xefefef, // back-alt
	0xefefef, // back-disabled
	0xf0f0f0, // window
	0xf0f0f0, // button
	0xffffdc, // tooltip
	0x0000ff, // link
	0xff00ff, // link-visited
	0xffffff, // light
	0xe3e3e3, // midlight
	0xa0a0a0, // mid
	0xa0a0a0, // dark
	0x0078d7, // selection
	0x8cb6d7, // selection-inactive
	0x99c9ef, // selection-alt
	0xe0e0e0, // selection-alt-inactive
	0x808080, // timestamp
	0x808080, // offset
	0xddccff, // highlight
	0xc0c0c0, // delimiter
	0xdcdcdc, // delimiter-light
	0x808080, // gray
	0x800000, // red
	0x008000, // green
	0xa06000, // orange
	0x000080, // blue
	0x800080, // purple
	0x008080, // cyan
	0x808000, // yellow
	0xefefef, // pastel-gray
	0xffe4e1, // pastel-red
	0xe1fada, // pastel-green
	0xfffacd, // pastel-orange
	0xe2edff, // pastel-blue
	0xe2dafd, // pastel-purple
	0xcdfaff, // pastel-cyan
	0xffffc0, // pastel-yellow
	0x0000ff, // keyword
	0xce7b00, // constant
	0x969696, // comment
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

uint_t darkTheme[UiColor__Count] = {
	0xfdf9f3, // text
	0xffffff, // text-selected
	0x808080, // text-disabled
	0x2c292d, // back
	0x3c393d, // back-alt
	0x3d3d3d, // back-disabled
	0x222020, // window
	0x424245, // button
	0x3f3f36, // tooltip
	0x0986d3, // link
	0xa70b06, // link-visited
	0x979797, // light
	0x5e5c5b, // midlight
	0x4a4947, // mid
	0x302f2e, // dark
	0x12608a, // selection
	0x607b8a, // selection-inactive
	0x405672, // selection-alt
	0x304050, // selection-alt-inactive
	0x7d7d7d, // timestamp
	0x7d7d7d, // offset
	0x835c42, // highlight
	0x404040, // delimiter
	0x202020, // delimiter-light
	0xa5acb8, // gray
	0xf56168, // red
	0xa9dc76, // green
	0xff7c5c, // orange
	0x77dce8, // blue
	0xca95c5, // purple
	0x77dce8, // cyan
	0xffaf5e, // yellow
	0x595959, // pastel-gray
	0x773f40, // pastel-red
	0x3d6640, // pastel-green
	0x835c42, // pastel-orange
	0x405672, // pastel-blue
	0x604266, // pastel-purple
	0x367370, // pastel-cyan
	0x747242, // pastel-yellow
	0xca95c5, // keyword
	0x94c796, // constant
	0xa5acb8, // comment
};

//..............................................................................

void
MyWidget::paintEvent(QPaintEvent* e) {
	enum {
		Margin = 4,
	};

	QPainter painter(this);

	static const QString text = "Abc";

	const uint_t* theme = darkTheme;
	QFontMetrics fm(font());
	QRect rect = fm.boundingRect(text);
	rect;
	int width = rect.width() + Margin * 2;
	int height = rect.height() + Margin * 2 + 1; // delimiter
	int fullWidth = width * countof(textColors);
	int halfWidth = fullWidth / 2;
	int fullHeight = height * countof(backColors);

	for (int i = 0; i < countof(backColors); i++) {
		int y = i * height;

		uint_t backColor = backColors[i];
		painter.fillRect(0, y, fullWidth, height, QColor(theme[backColor]));
		painter.fillRect(0, y + height - 1, halfWidth, 1, QColor(theme[UiColor_Delimiter]));
		painter.fillRect(halfWidth, y + height - 1, halfWidth, 1, QColor(theme[UiColor_DelimiterLight]));

		painter.setPen(Qt::darkGray);
		painter.drawText(fullWidth + Margin, y + height - Margin - 1, colorNames[backColor]);

		for (int j = 0; j < countof(textColors); j++) {
			uint_t textColor = textColors[j];
			if (backColor == UiColor_Selection || backColor == UiColor_SelectionInactive)
				textColor = UiColor_TextSelected; // override

			int x = j * width;
			painter.setPen(QColor(theme[textColor]));
			painter.drawText(x + Margin, y + height - Margin - 1, text);
		}
	}

	painter.setPen(Qt::darkGray);

	for (int j = 0; j < countof(textColors); j++) {
		uint_t textColor = textColors[j];
		QPoint origin(j * width + Margin, fullHeight + Margin);

		painter.translate(origin);
		painter.rotate(90);
		painter.drawText(0, 0, colorNames[textColor]);
		painter.rotate(-90);
		painter.translate(-origin);
	}
}

void
MyWidget::keyPressEvent(QKeyEvent *event) {
	printf(
		"MyWidget::keyPressEvent(mod: %x, key:%x, text: %s)\n",
		event->modifiers(),
		event->key(),
		event->text().toLatin1().constData()
	);

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
	vlayout->setMargin(0);
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

#if (_TEST_LAYOUT)
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

	combo1->setModel(new MyItemModel(qobject_cast<QStandardItemModel*>(combo1->model())));

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
#endif

#if (_TEST_PAINT)
	QWidget* widget = &m_myWidget;
	resize(700, 600);
#elif (_TEST_EDIT)
	QWidget* widget = &m_editWidget;
#elif (_TEST_TREE)
	QWidget* widget = &m_treeWidget;
	widget->setStyle(new StyleFix(widget->style()));
#endif

	setFocusProxy(widget);
	widget->setFocus();

#if (_AXL_OS_LINUX)
	connectDbus();
#endif
}

#if (_AXL_OS_LINUX)

static const QString DesktopService("org.freedesktop.portal.Desktop");
static const QString DesktopPath("/org/freedesktop/portal/desktop");
static const QString DesktopSettingsIface("org.freedesktop.portal.Settings");
static const QString SettingsChangedSignal("SettingChanged");
static const QString ReadOneMethod("ReadOne");
static const QString AppearanceNspace("org.freedesktop.appearance");
static const QString ColorSchemeKey("color-scheme");

bool MainWindow::connectDbus() {
	QDBusConnection dbus = QDBusConnection::sessionBus();
	if (!dbus.isConnected())
		return false;

	printf(
		"connecting %s %s %s %s\n",
		DesktopService.toLatin1().constData(),
		DesktopPath.toLatin1().constData(),
		DesktopSettingsIface.toLatin1().constData(),
		SettingsChangedSignal.toLatin1().constData()
	);

	QDBusInterface iface(DesktopService, DesktopPath, DesktopSettingsIface, dbus);
	QDBusMessage msg = iface.call(ReadOneMethod, AppearanceNspace, ColorSchemeKey);
	QList<QVariant> values = msg.arguments();
	if (!values.isEmpty()) {
		QVariant value = values.front().value<QDBusVariant>().variant();
		printf(
			"%s %s: %s %s\n",
			AppearanceNspace.toLatin1().constData(),
			ColorSchemeKey.toLatin1().constData(),
			value.typeName(),
			value.toString().toLatin1().constData()
		);
	}

	qRegisterMetaType<QDBusVariant>();
	bool result = dbus.connect(
		DesktopService,
		DesktopPath,
		DesktopSettingsIface,
		SettingsChangedSignal,
		this,
		SLOT(onSettingChanged(const QString&, const QString&, const QDBusVariant&))
	);

	printf("dbus.connect -> %d\n", result);
	return result;
}
#endif

void MainWindow::onSettingChanged(const QString& nspace, const QString& key, const QDBusVariant& value) {
#if (_AXL_OS_LINUX)
	printf(
		"onSettingChanged(namespace: %s, key: %s, value: %s)\n",
		nspace.toLatin1().constData(),
		key.toLatin1().constData(),
		value.variant().toString().toLatin1().constData()
	);

/*
 if (location == QLatin1StringView("org.kde.kdeglobals.KDE")
       && key == QLatin1StringView("widgetStyle"))
     return SettingType::KdeApplicationStyle;
 if (location == QLatin1StringView("org.kde.kdeglobals.General")
       && key == QLatin1StringView("ColorScheme"))
     return SettingType::KdeGlobalTheme;
 if (location == QLatin1StringView("org.gnome.desktop.interface")
       && key == QLatin1StringView("gtk-theme"))
     return SettingType::GtkTheme;
*/
#endif
}

//..............................................................................
