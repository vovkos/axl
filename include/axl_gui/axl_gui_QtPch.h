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

#include "axl_gui_Pch.h"

#include "axl_sl_StringHashTable.h"

// by default, QT headers force linkage to the qt_version_tag symbol
// this makes it impossible to use axl::gui members without linking to QT --
// even if QT is really not required!

#define QT_NO_VERSION_TAGGING 1

#include <QAbstractScrollArea>
#include <QApplication>
#include <QClipboard>
#include <QFont>
#include <QIcon>
#include <QImage>
#include <QMouseEvent>
#include <QMimeData>
#include <QPainter>
#include <QPixmap>
#include <QLinearGradient>
#include <QScrollBar>
#include <QWidget>
#include <QSyntaxHighlighter>
#include <QToolTip>
#include <QTimer>
