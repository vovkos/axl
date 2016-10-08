Introduction
============

What Is AXL?
------------
Every large C/C++ project needs a **support library**. Period. If you're able to get by with just libc/STL/boost/QT -- your project is not large enough. Thing is, no matter how comprehensive a framework of your choise is (both QT and boost are **extremely** comprehensive), when you are working on a large project with multiple sub-modules, sooner or later you inevitably will face a situation when desired functionality is **missing**. So you may need to bring in additional C library and write C++ wrappers consistent with the rest of your project; maybe you will find it's more appropriate to implement an additional layer of functionality yourself; most likely, you will end up doing a little bit of both.

AXL was born as a cross-platform C/C++ support library for Tibbo projects such as TDST, TIDE, Jancy, Graco, Doxyrest, IO Ninja. It coveres our **specialized needs** for compiler/parser writing, asynchronous IO, device management, cryptography, and so on -- all that while providing **consistency** in naming conventions and error handling model.

Will it cover **everything** you may need in your own large project? Unlikely. Like I said, no framework could single-handedly provide all the needed functionality, and AXL is no exception. If you choose to use AXL you still likely will need to build your own support library on top of AXL -- e.g., use it as a foundation framework. But why would you want to use AXL in the first place?

Why Use AXL?
------------

* Contribute to Jancy/Graco/Doxyrest

	If you are willing to contribute to any of the aforementioned open-source projects (e.g. apply a bugfix or add a feature), you simply have no other choise but to use AXL -- these projects already employ AXL as a support library.

* Java naming convention

	Whether this is a good thing or not is a matter of personal preference; it also depends on what style the rest of your project is written in. However, if you prefer to use Java naming convention over C-style which creeped its way into C++ STL and boost -- you might want to consider using AXL.

* Lightweight

	AXL is extremely lightweight compared to QT -- both in code- and data-memory footprint. Most modules of AXL are orthogonal and don't bring additional dependencies (of course, if it's not a C-library wrapper; if you link to axl_lua you surely will also need liblua)

* Contained memory allocation

	AXL allocates all of its memory using a pair of pre-defined functions, making it easy to both replace allocator and to isolate memory leaks. Default memory allocator dumps all memory leaks in debug build.

* Non-zero-terminated strings

	AXL is probably the only C++ framework which uses non-zero-terminated UTF-8 string slices as a default for string passing and returning. Zero-terminated strings is an ancient evil which follows C/C++ programmers everywhere. String slices is a much more efficient way of dealing with strings, and in AXL it's a default. Zero-terminated strings will only be allocated when (and if!) needed.

* Advanced reference counting model

	Allocate multiple ref-counted objects on the same memory block, store temporary ref-counted on stack buffers for efficiency, use weak and strong reference semantics and more. Since ref-counting is unified across AXL, you can also pass strings as arrays, arrays as strings, and share memory buffers between objects in all kinds of ways.

* TLS-based error handling model

* Built-in wrappers for many popular libraries/services

	- OpenSSL
	- LibSSH2
	- LibLua
	- LinPCap
	- LibUSB
	- LibExpat
	- Ragel
	- CoreFoundation
	- IOKit
	- LibUDev
	- Windows Service Control Manager
	- Windows Setup API

* Built-in special features
	- Lua string templates	-
	- Incremental Boyer-Moore
	- Incremental regular expressions

