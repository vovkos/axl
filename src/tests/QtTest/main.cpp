#include "pch.h"
#include "mainwindow.h"

int
main (
	int argc,
	char *argv[]
	)
{	
	QApplication ab (argc, argv);

	MainWindow w;
	w.show ();

	return ab.exec ();
}
