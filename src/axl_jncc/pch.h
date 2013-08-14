#pragma once

#include "axl_g_Pch.h"

#if (_AXL_ENV == AXL_ENV_WIN)
#	define getsockerror WSAGetLastError
#	define socklen_t    int

#elif (_AXL_ENV == AXL_ENV_POSIX)
#	include <sys/socket.h>
#	include <netinet/in.h>
#	include <netinet/ip.h>
#	include <arpa/inet.h>
#	define SOCKET         int
#	define INVALID_SOCKET (-1)
#	define closesocket    close

inline 
int
getsockerror ()
{
	return errno;
}

#endif

//.............................................................................

// LLVM

// warning C4800: 'unsigned int' : forcing value to bool 'true' or 'false' (performance warning)
// warning C4244: 'return' : conversion from 'uint64_t' to 'unsigned int', possible loss of data
// warning C4624: destructor could not be generated because a base class destructor is inaccessible

#pragma warning (disable: 4800) 
#pragma warning (disable: 4244) 
#pragma warning (disable: 4624) 

#undef min
#undef max

#include "llvm/DerivedTypes.h"
#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "llvm/Intrinsics.h"
#include "llvm/IntrinsicInst.h"
#include "llvm/PassManager.h"
#include "llvm/IRBuilder.h"
#include "llvm/DIBuilder.h"
#include "llvm/DebugInfo.h"
#include "llvm/Support/Dwarf.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/CallSite.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/Analysis/Passes.h"
#include "llvm/Transforms/Scalar.h"

#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/ExecutionEngine/JIT.h"
#include "llvm/ExecutionEngine/JITEventListener.h"
#include "llvm/ExecutionEngine/JITMemoryManager.h"
#include "llvm/ExecutionEngine/JITEventListener.h"
#include "llvm/ExecutionEngine/MCJIT.h"

#include "../lib/MC/MCDisassembler/EDDisassembler.h"
#include "../lib/MC/MCDisassembler/EDInst.h" 
#include "../lib/MC/MCDisassembler/EDOperand.h"
#include "../lib/MC/MCDisassembler/EDToken.h"

#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCDisassembler.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstPrinter.h"
#include "llvm/MC/MCSubtargetInfo.h"

#include "llvm/CodeGen/MachineCodeInfo.h"
#include "llvm/CodeGen/GCStrategy.h"
#include "llvm/CodeGen/GCs.h"

#pragma warning (default: 4800)
#pragma warning (default: 4244) 
#pragma warning (default: 4624) 

//.............................................................................

// AXL

#include "../axl_jnc/axl_jnc_Parser.llk.h"
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


