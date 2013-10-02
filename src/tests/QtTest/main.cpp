#include "pch.h"
#include "mainwindow.h"

int
main (
	int argc,
	char *argv[]
	)
{	
	atexit (llvm::llvm_shutdown);
	
	llvm::InitializeNativeTarget ();
	llvm::InitializeNativeTargetAsmParser ();
	llvm::InitializeNativeTargetAsmPrinter ();
	llvm::InitializeNativeTargetDisassembler ();

	err::CParseErrorProvider::Register ();
	srand ((int) axl::g::GetTimestamp ());

	QApplication ab (argc, argv);

	MainWindow w;
	w.initialize ();
	w.show ();

	return ab.exec ();
}
