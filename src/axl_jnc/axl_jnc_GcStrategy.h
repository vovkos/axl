namespace axl {
namespace jnc {

//.............................................................................

/// @brief The map for a single function's stack frame.  One of these is
///        compiled as constant data into the executable for each function.
///
/// Storage of metadata values is elided if the %metadata parameter to
/// @llvm.gcroot is null.
struct FrameMap 
{
	int32_t NumRoots;     //< Number of roots in stack frame.
	int32_t NumMeta;      //< Number of metadata entries.  May be < NumRoots.
	const void *Meta [1]; //< Metadata for each root.
};

/// @brief A link in the dynamic shadow stack.  One of these is embedded in
///        the stack frame of each function on the call stack.
struct StackEntry 
{
	StackEntry* Next;     //< Link to next stack entry (the caller's).
	const FrameMap* Map;  //< Pointer to constant FrameMap.
	void* Roots [1];      //< Stack roots (in-place array).
};

//.............................................................................

class ShadowStackGC : public llvm::GCStrategy 
{
	/// RootChain - This is the global linked-list that contains the chain of GC
	/// roots.
	llvm::GlobalVariable* Head;

	/// StackEntryTy - Abstract type of a link in the shadow stack.
	///
	llvm::StructType* StackEntryTy;
	llvm::StructType* FrameMapTy;

	/// Roots - GC roots in the current function. Each is a pair of the
	/// intrinsic call and its corresponding alloca.
	std::vector <std::pair <llvm::CallInst*, llvm::AllocaInst*> > Roots;

public:
	ShadowStackGC();

	virtual
	bool 
	initializeCustomLowering (llvm::Module& M);
	
	virtual
	bool 
	performCustomLowering (llvm::Function& F);

private:
	bool 
	IsNullValue (llvm::Value* V);

	llvm::Constant*
	GetFrameMap (llvm::Function& F);

	llvm::Type* 
	GetConcreteStackEntryType (llvm::Function& F);

	void 
	CollectRoots (llvm::Function& F);

	static 
	llvm::GetElementPtrInst*
	CreateGEP (
		llvm::LLVMContext& Context, 
		llvm::IRBuilder<>& B, 
		llvm::Value* BasePtr,
		int Idx1, 
		const char *Name
		);

	static 
	llvm::GetElementPtrInst*
	CreateGEP (
		llvm::LLVMContext& Context,
		llvm::IRBuilder<>& B, 
		llvm::Value* BasePtr,
		int Idx1, 
		int Idx2, 
		const char *Name
		);
};

//.............................................................................

/// EscapeEnumerator - This is a little algorithm to find all escape points
/// from a function so that "finally"-style code can be inserted. In addition
/// to finding the existing return and unwind instructions, it also (if
/// necessary) transforms any call instructions into invokes and sends them to
/// a landing pad.
///
/// It's wrapped up in a state machine using the same transform C# uses for
/// 'yield return' enumerators, This transform allows it to be non-allocating.

class EscapeEnumerator 
{
	llvm::Function& F;
	const char* CleanupBBName;

	// State.
	int State;
	llvm::Function::iterator StateBB, StateE;
	llvm::IRBuilder<> Builder;

public:
	EscapeEnumerator (llvm::Function &F, const char* N = "cleanup");

	llvm::IRBuilder<>*
	Next ();
};

//.............................................................................

void
RegisterGcStrategy (int);

//.............................................................................

} // namespace axl 
} // namespace jnc 
