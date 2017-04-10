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

.. define-macro:: build-intro project

	Once CMake configuration step completes, makefiles/project files are generated. You are now ready to build $project.

.. define-macro:: build-windows project solution

	To build $project on Windows you can start Visual Studio by double-clicking the newly generated solution file ``$solution`` and build it from the IDE.

	If you prefer building from the command line, run in ``./build`` folder::

		cmake --build .

	By default, ``msbuild.exe`` will build the ``Debug`` configuration of the project. To override that and build for ``Release``, use::

		cmake --build . --config Release
		
	You can pass extra command-line arguments to ``msbuild.exe`` using ``--`` switch. For example, if you want to have a multi-core build of ``Release`` configuration, run::

		cmake --build . --config Release -- /maxcpucount

	Please refer to ``msbuild`` documentation for more details on command-line parameters: https://msdn.microsoft.com/en-us/library/ms164311.aspx

.. define-macro:: build-unix project

	To build $project on Unix based systems, simply run the following in ``./build`` folder::

		make

	You might also want to add ``-j <n>`` to make use of multiple CPU cores and speed up build process, like::

		make -j 4

	Note that with ``make``-based build changing configuration from ``Debug`` to ``Release`` should be done at CMake configuration step (Xcode and Visual Studio are multi-configuration build systems).

.. define-macro:: build-doc-intro build_dir

	If you have required :ref:`prerequisites <optional-tools>` for building documentation, CMake should have created all the necessary shell scripts under: ``$build_dir/doc/<doc-package>``

	``sphinx-build`` is always needed; ``pdflatex`` is also needed to build PDF.

	Resulting HTML pages will be placed at: ``$build_dir/doc/<doc-package>/html``

	Resulting PDF will be placed at: ``$build_dir/doc/<doc-package>/pdf``

.. define-macro:: build-doc-build-guide build_dir

	This is what you are reading right now.

	Documentation sources located at: ``./doc/build-guide``

	Build steps:

	.. code-block:: bash

		cd $build_dir/doc/build-guide
		./build-html
		./build-pdf

.. define-macro:: build-doc-doxygen build_dir sub_dir

	Documentation source files are located at: ``./doc/$sub_dir``

	``doxygen`` is additionally needed to analyze API headers and extract documentation comments.

	``doxyrest`` is also needed to convert Doxygen XML database to reStructured Text (which will be further passed to ``sphinx-build``).

	Once ``doxygen`` and ``doxyrest`` are ready, you can run the scripts (replace ``Debug`` with ``Release`` if you have built ``doxyrest`` under ``Release`` configuration).

	Build steps:

	.. code-block:: bash

		cd $build_dir/doc/$sub_dir
		./build-xml
		./build-rst Debug # replace with Release if doxyrest was built for Release
		./build-html
		./build-pdf
