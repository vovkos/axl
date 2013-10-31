// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

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

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Attributes.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/IRBuilder.h"

#include "llvm/Support/Dwarf.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/CallSite.h"
#include "llvm/Support/raw_ostream.h"

#include "llvm/PassManager.h"
#include "llvm/DIBuilder.h"
#include "llvm/DebugInfo.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/Analysis/Passes.h"
#include "llvm/Transforms/Scalar.h"

#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/ExecutionEngine/SectionMemoryManager.h"
#include "llvm/ExecutionEngine/ObjectImage.h"

// LLVM JIT forces linkage to LLVM libraries if JIT is merely included;
// we want to be able avoid that (i.e. if a libraries defines LLVM-dependant classes, but
// application does not use those classes -- then why link to LLVM?)

#ifndef _AXL_LLVM_NO_JIT
#	include "llvm/ExecutionEngine/JIT.h"
#	include "llvm/ExecutionEngine/JITEventListener.h"
#	include "llvm/ExecutionEngine/JITMemoryManager.h"
#	include "llvm/ExecutionEngine/JITEventListener.h"
#	include "llvm/ExecutionEngine/MCJIT.h"
#endif

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


