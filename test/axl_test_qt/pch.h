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

#define _CRT_SECURE_NO_WARNINGS // disable useless warnings about "unsafe" string functions
#define _SCL_SECURE_NO_WARNINGS // disable useless warnings about "unsafe" iterators

#define __STDC_LIMIT_MACROS
#define __STDC_CONSTANT_MACROS

#include "axl_g_Pch.h"

//..............................................................................

// QT

#include <QtGui>
#include <QAction>
#include <QApplication>
#include <QDockWidget>
#include <QFileDialog>
#include <QHeaderView>
#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QPlainTextEdit>
#include <QStatusBar>
#include <QSyntaxHighlighter>
#include <QTextBlock>
#include <QToolBar>
#include <QTreeWidget>
#include <QWidget>
#include <QScrollBar>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QListView>
#include <QTableView>
#include <QItemDelegate>
#include <QLineEdit>
#include <QSslSocket>
#include <QSslCertificate>
#include <QSslConfiguration>

#if (_AXL_OS_LINUX)
#	include <QtDBus>
#else
class QDBusVariant;
#endif

//..............................................................................

// AXL

#include "axl_io_ShmtReader.h"
#include "axl_io_ShmtWriter.h"
#include "axl_sys_Time.h"

using namespace axl;

//..............................................................................

#include <openssl/x509.h>

#include <map>
#include <unordered_map>
#include <iostream>
#include <string>
#include <locale>
#include <codecvt>

#if(_BOOST)
#   include <boost/unordered/unordered_flat_map.hpp>
#endif

#include "axl_sl_RbTree.h"
#include "axl_sl_RobinHashTable.h"
#include "axl_sl_QuadraticHashTable.h"
#include "axl_sys_DynamicLib.h"
