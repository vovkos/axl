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

AXL was born as a cross-platform C/C++ support library for `Tibbo <http://tibbo.com>`_ projects such as `TDST <http://tibbo.com/soi/software.html>`_, `TIDE <http://tibbo.com/programmable.html#tide>`_, `Jancy <http://tibbo.com/jancy>`__, `Graco <http://github.com/vovkos/graco>`_, `Doxyrest <http://github.com/vovkos/doxyrest>`_, `ECCKey <http://github.com/vovkos/ecckey>`_, `IO Ninja <http://tibbo.com/ninja>`_. It coveres our **specialized needs** for compiler/parser writing, asynchronous IO, device management, cryptography, and so on -- all that while providing **consistency** in naming conventions and error handling model.

Will it cover **everything** you may need in your own large project? Unlikely. Like I said, no framework could *single-handedly* provide all the needed functionality, and AXL is no exception. If you choose to use AXL you still likely will need to build your own support library on top of AXL -- e.g., use it as a foundation framework.

But why would you want to use AXL in the first place?

Why Use AXL?
------------

* Contribute to `Jancy <http://github.com/vovkos/jancy>`__/`Graco <http://github.com/vovkos/graco>`_/`Doxyrest <http://github.com/vovkos/doxyrest>`_/`ECCKey <http://github.com/vovkos/ecckey>`_

	If you are willing to **contribute** to any of the aforementioned open-source projects (e.g. apply a bugfix or add a feature), you simply have no other choise but to use AXL -- these projects **already employ AXL** as a support library.

* Java naming convention

	Whether this is a good thing or not is of course a matter of personal preference. It also may depend on what style the rest of your project is written in. However, if you **do prefer** to use **Java naming convention** over old-fashioned C-style which creeped its way into STL, boost and many other C++ libraries -- you might consider using AXL.

* Lightweight

	AXL is **extremely lightweight** compared to QT -- both in **code- and data-memory** footprint. Most modules of AXL are orthogonal and don't bring additional dependencies (of course, if it's not a C-library wrapper; if you link to ``axl_lua`` you surely will also need ``liblua``)

* Non-zero-terminated strings

	AXL is probably the only C++ framework which uses **non-zero-terminated UTF-8 string slices** as the default for string passing and returning. Zero-terminated strings is an ancient evil which follows C/C++ programmers everywhere. String slices is a **much more efficient** way of dealing with strings, and in AXL it's the default. Zero-terminated strings will only be allocated when (and if!) needed.

* Advanced reference counting model

	Allocate multiple ref-counted objects on the **same memory block**, store temporary ref-counted objects on **stack** buffers for efficiency, use **weak/strong** reference semantics and more. Since ref-counting is **unified across AXL**, you can also pass **strings as arrays**, **arrays as strings**, and share memory buffers between objects in **all kinds of ways**.

* TLS-based error handling model

	AXL purposely does not employ C++ exceptions as a standard mechanism for error handling (detailed discussion of C++ exception-related issues is beyond the scope of this introduction). Instead, a classic C **error-code model** is used. Unlike most C libraries, however, AXL does not limit your capabilities of describing the error with 32 (or 64) bits of an integer error code; neither does it require you to pass an error buffer to every function. With AXL you **store as much information** about the error as needed in **TLS-based** error buffer. Later the caller of your function inspects and gets human-readable description of it.

* Contained memory allocation

	AXL allocates all of its memory using a pair of pre-defined functions, making it easy to both **replace** the allocator and to **isolate memory leaks**. Default memory allocator **dumps** all AXL-related memory leaks in **debug build**.

* Configurable CMake-based build system

	AXL features a **highly configurable** and reusable CMake-based build infrastructure, with **cascading sheets** for external tool library/tool **path definitions** and adjustment of compiler/linker **switch settings**, easy configuration of **cross-compilation**, **precompiled headers** for the big three of C++ compilers (MSVC, GCC, Clang) and a lot more.

* Built-in wrappers for many popular libraries/tools/services

	- OpenSSL
	- LibSSH2
	- LibLua
	- LinPCap
	- LibUSB
	- LibExpat
	- MiniZ
	- Ragel
	- CoreFoundation
	- IOKit
	- LibUDev
	- Windows Service Control Manager
	- Windows Setup API

* Built-in special features

	- Lua string templates
	- Incremental Boyer-Moore
	- Incremental regular expressions
	- Shared memory transport
	- Map-on-demand memory mapped files
	- Widget creation abstraction layer
