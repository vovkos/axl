#pragma once

#define _CRT_SECURE_NO_WARNINGS // disable useless warnings about "unsafe" string functions
#define _SCL_SECURE_NO_WARNINGS // disable useless warnings about "unsafe" iterators

#define __STDC_LIMIT_MACROS
#define __STDC_CONSTANT_MACROS

#include "axl_g_Pch.h"
#include "axl_jnc_Pch.h"

#undef min
#undef max

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

// AXL

#include "axl_io_MappedFile.h"
#include "axl_rtl_ArrayList.h"
#include "axl_rtl_BitMap.h"
#include "axl_rtl_StringHashTable.h"
#include "axl_rtl_EscapeEncoding.h"
#include "axl_rtl_HandleTable.h"
#include "axl_io_FilePathUtils.h"
#include "axl_lex_RagelLexer.h"
#include "axl_err_ParseError.h"
#include "axl_rtl_HexEncoding.h"
#include "axl_jnc_Module.h"
#include "axl_jnc_Runtime.h"
#include "axl_jnc_StdLib.h"
#include "axl_jnc_Disassembler.h"
#include "axl_g_Time.h"
#include "../src/axl_jnc/axl_jnc_Parser.llk.h"

using namespace axl;

#if (_AXL_ENV == AXL_ENV_WIN)

// Memory Leak Detection

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif

#endif