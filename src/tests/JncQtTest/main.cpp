#include "pch.h"
#include "mainwindow.h"

int main (int argc, char* argv [])
{
#if (_AXL_ENV == AXL_ENV_WIN)
	_CrtSetDbgFlag(
		_CRTDBG_ALLOC_MEM_DF |
		_CRTDBG_LEAK_CHECK_DF
		); // Memory leak detection.

#endif

	atexit (llvm::llvm_shutdown);

	llvm::InitializeNativeTarget ();
	llvm::InitializeNativeTargetAsmParser ();
	llvm::InitializeNativeTargetAsmPrinter ();
	llvm::InitializeNativeTargetDisassembler ();

	err::CParseErrorProvider::Register ();
	srand ((int) axl::g::GetTimestamp ());

	QApplication app (argc, argv);
	QCoreApplication::setOrganizationName("Tibbo");
	QCoreApplication::setOrganizationDomain("tibbo.com");
	QCoreApplication::setApplicationName("JancyEdit");

	MainWindow mainWindow;
	mainWindow.showMaximized();
	
	return app.exec();
}
