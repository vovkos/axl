#define _CRT_SECURE_NO_WARNINGS // disable useless warnings about "unsafe" string functions
#define _SCL_SECURE_NO_WARNINGS // disable useless warnings about "unsafe" iterators

#define __STDC_LIMIT_MACROS 
#define __STDC_CONSTANT_MACROS

//.............................................................................

#include "axl_g_Pch.h"
#include "axl_jnc_Pch.h"
#include "axl_gui_qt_Pch.h"

#undef min
#undef max

//.............................................................................

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

//.............................................................................

// AXL

#include "axl_log_Server.h"
#include "axl_log_Widget.h"
#include "axl_log_RagelColorizer.h"
#include "axl_log_JancyRepresenter.h"
#include "axl_gui_qt_Engine.h"

#include "../../axl_jnc/axl_jnc_Parser.llk.h"
#include "axl_jnc_StdLib.h"
#include "axl_jnc_Disassembler.h"
#include "axl_jnc_Module.h"
#include "axl_jnc_Runtime.h"
#include "axl_io_FilePathUtils.h"
#include "axl_io_File.h"
#include "axl_io_MappedFile.h"
#include "axl_err_Errno.h"
#include "axl_mt_Thread.h"
#include "axl_rtl_String.h"
#include "axl_g_Time.h"

using namespace axl;

//.............................................................................
