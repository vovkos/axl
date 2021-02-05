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

Tools
=====

.. expand-macro:: tools-intro AXL

Required Tools
--------------

There is only one tool essential for building AXL:

* CMake 3.3 or above

	.. expand-macro:: cmake-common-info AXL

.. _optional-tools:

Optional Tools
--------------

These tools are **optional** and if your build machine does not have them installed, AXL will still build:

* Ragel

	AXL uses Ragel as a lexer/scanner generator for the tokenization stage in ``axl_ini`` (parser of ``.ini`` files) and ``axl_st`` (string template engine). If Ragel is missing, ``axl_ini`` and ``axl_st`` will not be built.

	.. expand-macro:: ragel-common-info

* 7-Zip

	AXL uses 7-Zip file archiver to conveniently build **source package** archives on Windows with a ``.bat`` shell script. If you don't plan to build AXL packages on Windows, you don't need 7-Zip.

	.. expand-macro:: 7zip-common-info

These tools are only needed if you plan to build AXL **documentation**:

* Python

	.. expand-macro:: python-sphinx-common-info AXL

* Doxygen

	.. expand-macro:: doxygen-common-info AXL

* Doxyrest

	AXL documentation uses Doxyrest as a middle-end in the ``doxygen-doxyrest-sphinx`` pipeline.

	.. expand-macro:: doxyrest-common-info

* Sphinx

	.. expand-macro:: sphinx-common-info AXL
