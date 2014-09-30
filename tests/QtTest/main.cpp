#include "pch.h"
#include "mainwindow.h"
#include "moc_axl_gui_QtWidget.cpp"

int
main (
	int argc,
	char *argv[]
	)
{	
#ifdef _JANCY_REPRESENTER
	atexit (llvm::llvm_shutdown);
	
	llvm::initializeNativeTarget ();
	llvm::initializeNativeTargetAsmParser ();
	llvm::initializeNativeTargetAsmPrinter ();
	llvm::initializeNativeTargetDisassembler ();

	err::ParseErrorProvider::register ();
	srand ((int) axl::g::getTimestamp ());
#endif

#if (_AXL_ENV == AXL_ENV_WIN)
	WSADATA wsaData;
	WSAStartup (0x0202, &wsaData);
#endif

	QApplication ab (argc, argv);

	mainWindow w;
	w.initialize ();
	w.show ();

	return ab.exec ();
}
