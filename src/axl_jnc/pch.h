#pragma once

#include "axl_g_Pch.h"

//.............................................................................

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
#include "llvm/Intrinsics.h"
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
 
#pragma warning (default: 4146)
#pragma warning (default: 4355) 
#pragma warning (default: 4800)

//.............................................................................
