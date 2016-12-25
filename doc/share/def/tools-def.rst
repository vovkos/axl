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

.. define-macro:: tools-intro project

	$project build system relies on its own path configuration via the ``paths.cmake`` file, so you do not actually need to add any of these tools to ``PATH``. However, it is recommended to make CMake easily available (e.g. via ``PATH`` or a shortcut to ``cmake-gui``) -- because it is used to initiate the whole build process.

.. define-macro:: cmake-common-info project

	$project uses CMake-based build system.

	CMake is a cross-platform C/C++-centric build system which uses build scripts to generate platform-specific makefiles. CMake is quickly becoming a de-facto standard when it comes to building cross-platform C/C++ applications.

	CMake is available in official repositories of most Linux distributions. Source and pre-compiled binaries can also be directly downloaded from the official website: http://cmake.org

	Any version of CMake above 3.3 should work. Versions prior to CMake 3.3 improperly expand ``CMAKE_PARENT_LIST_FILE`` variable which $project build system relies on.

.. define-macro:: ragel-common-info

	Ragel is a cross-platform finite state machine compiler. I found Ragel to be perfect for lexer/scanner generation due to the convenience of its input language and great performance of the output code.

	Ragel is available in official repositories of most Linux distributions. Sources can be downloaded from the official website: http://www.colm.net/open-source/ragel

	Unfortunately, the official website does not distribute pre-compiled binaries anymore, so you have to either build Ragel yourself or choose one of the unofficial pre-compiled binaries available on the web.

.. define-macro:: 7zip-common-info

	7-Zip is available in official repositories of many Linux distributions as ``p7zip``. For Mac OS X you can either use Homebrew (http://brew.sh) to install ``p7zip`` or download an unofficial pre-compiled binary (such as ``7zx``). Pre-compiled binaries for Windows are available at the official website: http://www.7-zip.org

.. define-macro:: python-common-info

	Python is available out-of-the-box in most Linux distributions and on Mac OS X. On Window you can install the official binaries available at: https://www.python.org
	or use ActivePython: http://www.activestate.com/activepython

	Both 3.5.x and 2.7.x branches should be fine (Sphinx documentation states that the required version is at least 2.6).

.. define-macro:: perl-common-info

	Perl is available out-of-the-box in most Linux distributions and Mac OS X. On Windows you can use ActivePerl (http://www.activestate.com/activeperl) or StraberryPerl (http://strawberryperl.com).

	Both should work just fine.

.. define-macro:: python-sphinx-common-info project

	$project documentation uses Sphinx as a back-end, therefore, Python is also needed.

	.. expand-macro:: python-common-info

.. define-macro:: doxygen-common-info project

	$project documentation uses Doxygen to analyze API headers, extract declarations and comments and build XML database.

	Doxygen is available in official repositories of most Linux distributions. Source and pre-compiled binaries can also be directly downloaded from the official website: http://www.stack.nl/~dimitri/doxygen/

.. define-macro:: doxyrest-common-info

	Doxyrest is a tool based on Lua string templates; it's purpose is to convert Doxygen XML database to reStructured Text.

	Doxyrest must be built from sources available at: http://tibbo.com/downloads/archive/doxyrest

.. define-macro:: sphinx-common-info project

	$project documentation uses Sphinx for generating HTML or PDF output.

	Sphinx is available via ``pip`` Python package management infrastructure. It should also be available in many Linux distributions as a stand-alone binary package.

	The official website of Sphinx: http://www.sphinx-doc.org
