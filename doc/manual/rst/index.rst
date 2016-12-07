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

AXL C++ Library Reference
=========================

Every large C/C++ project needs a **support library**. Period. If you're able to get by with just libc/STL/boost/QT -- your project is not large enough.

Thing is, no matter how comprehensive a framework of your choise is (both QT and Boost are **extremely** comprehensive), when you are working on a large project with multiple sub-modules, sooner or later you inevitably will face a situation when desired functionality is **missing**. So you may need to bring in additional C library and write C++ wrappers consistent with the rest of your project; maybe you will find it's more appropriate to implement an additional layer of functionality yourself; most likely, you will end up doing a little bit of both.

AXL was born as a cross-platform C/C++ support library for `Tibbo <http://tibbo.com>`_ projects such as `TDST <http://tibbo.com/soi/software.html>`_, `TIDE <http://tibbo.com/programmable.html#tide>`_, `Jancy <http://tibbo.com/jancy>`_, `Graco <http://github.com/vovkos/graco>`_, `Doxyrest <http://github.com/vovkos/doxyrest>`_, `ECCKey <http://github.com/vovkos/ecckey>`_, `IO Ninja <http://tibbo.com/ninja>`_. It coveres our **specialized needs** for compiler/parser writing, asynchronous IO, device management, cryptography, and so on -- all that while providing **consistency** in naming conventions and error handling model.

Will it cover **everything** you may need in your own large project? Unlikely. Like I said, no framework could *single-handedly* provide all the needed functionality, and AXL is no exception. If you choose to use AXL you still likely will need to build your own support library on top of AXL -- e.g., use it as a foundation framework.

But why would you want to use AXL in the first place?

.. toctree::
	:maxdepth: 2

	why.rst
	global.rst
