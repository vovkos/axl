#include "pch.h"
#include "mainwindow.h"

int
main (
	int argc,
	char *argv[]
	)
{	
#ifdef _JANCY_REPRESENTER
	atexit (llvm::llvm_shutdown);
	
	llvm::InitializeNativeTarget ();
	llvm::InitializeNativeTargetAsmParser ();
	llvm::InitializeNativeTargetAsmPrinter ();
	llvm::InitializeNativeTargetDisassembler ();

	err::CParseErrorProvider::Register ();
	srand ((int) axl::g::GetTimestamp ());
#endif

#if (_AXL_ENV == AXL_ENV_WIN)
	WSADATA WsaData;
	WSAStartup (0x0202, &WsaData);
#endif

	QApplication ab (argc, argv);

	MainWindow w;
	w.initialize ();
	w.show ();

	return ab.exec ();
}
