.. .............................................................................
..
..  This file is part of the AXL library.
..
..  AXL is distributed under the MIT license.
..  For details see accompanying license.txt file,
..  the public copy of which is also available at:
..  http://tibbo.com/downloads/archive/axl/license.txt
..
.. .............................................................................

.. define-macro:: cpp-compilers project

	$project is written in C++, so obviously you will need a C++ compiler and standard C/C++ libraries. The latest version of $project was built and tested on:

	* Windows
		- Microsoft Visual Studio 2010 version 10.0.40219.1 SP1Rel
		- cl version 16.0.40219.1

		Here in Tibbo we still use Visual Studio 2010 for building the official Windows packages of our software (due to many issues with Visual Studio 2015: telemetry code injection scandal, noticeable slow-down of editor and debugger, largely increased size of generated temporaries etc).

		This being said, the newer versions of Visual Studio should work just as well.

	* Linux
		- GCC++ version 6.2.1
		- Clang++ version 3.8.1

	* Mac OS X
		- Clang++ version 8.0.0 (clang-800.0.42.1)

	The above version numbers are provided for reference only. If the version of C++ compiler on your machine is higher or not too much lower, don't worry -- it will most likely compile $project just fine.
