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

.. define-macro:: configure-intro project

	Alright, all the necessary tools and libraries are available on your machine. What's next? As with every CMake-based project, we now need to *configure* our build and create appropriate makefiles.

	However, there is an important difference from most CMake-based projects. $project build system does not rely on CMake ``find_package`` facility to find dependencies. Why not?

	First of all - ``find_package`` simply does not work reliably on Windows. Here you would need to set up an extra layer of path configuration variables and then provide these paths as *hints* to ``find_package``. This effectively **voids** the whole idea behind ``find_package`` -- it's you, not ``find_package``, who is doing the actual "finding"!

	Another important reason -- ``find_package`` is not convenient to use in situations when you need to **switch** between different **versions** of the same library, between **Debug/Release** builds, between **static/dynamic** libraries, between different **processor architectures** during cross-compilation and so on.

	In other words, ``find_package`` is really convenient on **Unix** systems when all you do is a **native** build with the **default** versions of installed tools and libraries. Need more flexibility? Then ``find_package`` alone is not the answer.

.. define-macro:: paths-cmake project

	$project build system uses ``paths.cmake`` file as the main reference when it needs to find a certain tool or library. When a path is not specified, a fallback attempt to find it using ``find_package`` will be made.

	This allows for out-of-the-box *default* build and at the same time provides a *fine-grained* control over locations of dependencies. Here in Tibbo, we have multiple versions of tools and libraries installed on the single build machine and at the same time, we are always in full control over which tool or library is going to be used when building a particular project.

	``paths.cmake`` files are **cascading**. It means, you can place one *anywhere* above the current directory and it will be found and used. From there you can chain-include the next ``paths.cmake`` and so on. This way you can specify some default locations for *all* your projects but still be able to override the paths for sub-projects.

	Being machine-specific ``paths.cmake`` files are added to ``.gitignore`` and are never tracked in Git. Therefore, you need to write ``paths.cmake`` file as the very first step of the configuration process. So, what should be inside?

	To answer this question, you need to check ``dependencies.cmake`` file. Inside this file, a variable called ``AXL_PATH_LIST`` contains all the paths that will be used during the build:

.. define-macro:: dependencies-cmake project

	Some dependencies may be auto-detected -- on Unix systems installed libraries and tools will likely be found automatically. On Windows $project build system will automatically find *tool executables* if they are added to ``PATH`` (via ``where`` command).

	But even when auto-detection works out-of-the-box, you can always use ``paths.cmake`` to *fine-tune* the location of a specific tool/library. I personally prefer to always specify all the paths explicitly.

.. define-macro:: cmake-configure dir

	Once ``paths.cmake`` is created, it's time for CMake configuration step.

	Create a build folder. I usually create two-level build paths: ``./build/<build-specific-folder>``, e.g. ``./build/msvc10-amd64``, ``./build/make-x86``, ``./build/netbeans`` etc. But to keep things shorter, I will use ``./build`` in the snippets below::

		cd $dir
		mkdir build
		cd build
		cmake ..

	If all the required paths have been configured properly, you should see something like this.

.. define-macro:: cmake-post-configure

	After that you can optionally run::

		cmake-gui .

	This will launch a GUI client for CMake and allow you to **fine-tune** CMake configuration variables. For example, you may want to turn **precompiled headers** ON or OFF, change C++ **RTTI** settings, for *make*-based builds change **configuration** from ``Debug`` to ``Release`` or vice versa and so on.

	You can also use ``cmake-gui`` all along for the whole CMake configuration process, without doing command line ``cmake`` step.
