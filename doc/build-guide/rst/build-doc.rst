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

Building Documentation
======================

AXL contains two documentation packages:

* Build guide
* Library reference

If you have required :ref:`prerequisites <optional-tools>` for building documentation, CMake should have created all the necessary shell scripts under: ``./build/doc/<doc-package>``

``sphinx-build`` is always needed; ``pdflatex`` is also needed to build PDF.

Resulting HTML pages will be placed at: ``./build/doc/<doc-package>/html``

Resulting PDF will be placed at: ``./build/doc/<doc-package>/pdf``

Build Guide
-----------

This is what you are reading right now.

Located at: ``./doc/build-guide``

Build steps:

.. code-block:: bash

	cd ./build/doc/build-guide
	./build-html
	./build-pdf

AXL Reference & Manual
----------------------

Located at: ``./doc/manual``

``doxygen`` is additionally needed to analyze API headers and extract documentation comments.

``doxyrest`` is also needed to convert Doxygen XML database to Re-Structured Text (which will be further passed to ``sphinx-build``).

Once ``doxygen`` and ``doxyrest`` are ready, you can run the scripts (replace ``Debug`` with ``Release`` if you have built ``doxyrest`` under ``Release`` configuration).

Build steps:

.. code-block:: bash

	cd ./build/doc/api
	./build-xml
	./build-rst Debug # replace with Release if doxyrest was built for Release
	./build-html
	./build-pdf
