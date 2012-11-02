// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://msdn.microsoft.com/officeui.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#define _CRT_SECURE_NO_WARNINGS // disable useless warnings about "unsafe" string functions
#define _SCL_SECURE_NO_WARNINGS // disable useless warnings about "unsafe" iterators

#include <new>
#include <typeinfo>

// http://connect.microsoft.com/VisualStudio/feedback/details/621653/including-stdint-after-intsafe-generates-warnings
// warning C4005: 'INT8_MIN' : macro redefinition
#pragma warning (disable : 4005)
#include <stdint.h>
#include <intsafe.h>
#pragma warning (default : 4005)

inline
void 
clear_type_info_cache()
{
   __type_info_node* & node = __type_info_root_node._Next;
   while(node)
   {
      if (node->_MemPtr)
      {
         delete node->_MemPtr;
      }
      __type_info_node* tempNode = node;
      node = node->_Next;
      delete tempNode;
   }
}

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions


#include <afxdisp.h>        // MFC Automation classes



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC support for ribbons and control bars


#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


// LUA

extern "C" {

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

} // extern "C" {

#ifdef _DEBUG
#pragma comment (lib, "lua_d.lib")
#else
#pragma comment (lib, "lua.lib")
#endif

#define _CRT_SECURE_NO_WARNINGS // disable useless warnings about "unsafe" string functions

// AXL

#include "axl_io_MappedFile.h"
#include "axl_st_StringTemplate.h"
#include "axl_rtl_ArrayList.h"
#include "axl_rtl_BitMap.h"
#include "axl_rtl_StringHashTable.h"
#include "axl_rtl_EscapeEncoding.h"
#include "axl_io_FilePathUtils.h"
#include "axl_lex_RagelLexer.h"
#include "axl_err_ParseError.h"

#include "axl_rtl_HexEncoding.h"

using namespace axl;

// LLVM

// warning C4146: unary minus operator applied to unsigned type, result still unsigned
// warning C4355: 'this' : used in base member initializer list
// warning C4800: 'unsigned int' : forcing value to bool 'true' or 'false' (performance warning)

#pragma warning (disable: 4146)
#pragma warning (disable: 4355)
#pragma warning (disable: 4800) 

#undef min
#undef max

#include "llvm/DerivedTypes.h"
#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "llvm/PassManager.h"
#include "llvm/Support/IRBuilder.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/Analysis/Passes.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/ExecutionEngine/JIT.h"
#include "llvm/Target/TargetData.h"

#pragma comment (lib, "LLVMSupport.lib")
#pragma comment (lib, "LLVMCore.lib")
#pragma comment (lib, "LLVMAnalysis.lib")
#pragma comment (lib, "LLVMSelectionDAG.lib")
#pragma comment (lib, "LLVMTransformUtils.lib")
#pragma comment (lib, "LLVMScalarOpts.lib")
#pragma comment (lib, "LLVMInstCombine.lib")
#pragma comment (lib, "LLVMExecutionEngine.lib")
#pragma comment (lib, "LLVMJIT.lib")
#pragma comment (lib, "LLVMCodeGen.lib")
#pragma comment (lib, "LLVMTarget.lib")
#pragma comment (lib, "LLVMMC.lib")
#pragma comment (lib, "LLVMX86AsmPrinter.lib")
#pragma comment (lib, "LLVMX86CodeGen.lib")
#pragma comment (lib, "LLVMX86Desc.lib")
#pragma comment (lib, "LLVMX86Info.lib")
#pragma comment (lib, "LLVMX86Utils.lib")

#include <minmax.h>

#pragma warning (default: 4146)
#pragma warning (default: 4355)
#pragma warning (default: 4800)
