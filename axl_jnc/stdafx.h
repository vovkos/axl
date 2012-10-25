// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define WIN32_LEAN_AND_MEAN      // Exclude rarely-used stuff from Windows headers

#ifndef _WIN32_WINNT            // Specifies that the minimum required platform is Windows Vista.
#define _WIN32_WINNT 0x0600     // Change this to the appropriate value to target other versions of Windows.
#endif

#define _CRT_SECURE_NO_WARNINGS // disable useless warnings about "unsafe" string functions
#define _SCL_SECURE_NO_WARNINGS // disable useless warnings about "unsafe" iterators

// CRT

#include <new>
#include <typeinfo>

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <crtdbg.h>

// SDK

#include <tchar.h>
#include <guiddef.h>
#include <cguid.h>
#include <windows.h>

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

// AXL

#include "axl_io_MappedFile.h"
#include "axl_rtl_ArrayList.h"
#include "axl_rtl_BitMap.h"
#include "axl_rtl_StringHashTable.h"
#include "axl_io_FilePathUtils.h"
#include "axl_lex_RagelLexer.h"
#include "axl_dbg_Trace.h"

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
