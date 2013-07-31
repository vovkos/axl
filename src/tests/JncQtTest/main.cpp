#include "pch.h"
#include "mainwindow.h"

#if (_AXL_ENV == AXL_ENV_WIN)

void 
clear_type_info_cache ()
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

#endif

int main (int argc, char* argv [])
{
#if (_AXL_ENV == AXL_ENV_WIN)
	atexit (clear_type_info_cache);

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

	QApplication app (argc, argv);
	QCoreApplication::setOrganizationName("Tibbo");
	QCoreApplication::setOrganizationDomain("tibbo.com");
	QCoreApplication::setApplicationName("JancyEdit");

	MainWindow mainWindow;
	mainWindow.showMaximized();
	
	return app.exec();
}
