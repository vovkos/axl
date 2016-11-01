#...............................................................................
#
#  This file is part of the AXL library.
#
#  AXL is distributed under the MIT license.
#  For details see accompanying license.txt file,
#  the public copy of which is also available at:
#  http://tibbo.com/downloads/archive/axl/license.txt
#
#...............................................................................

unset (Qt5Core_DIR    CACHE)
unset (Qt5Gui_DIR     CACHE)
unset (Qt5Widgets_DIR CACHE)
unset (Qt5Network_DIR CACHE)

set (QT_FOUND FALSE)
set (QTCORE_FOUND FALSE)
set (QTGUI_FOUND FALSE)
set (QTWIDGETS_FOUND FALSE)
set (QTNETWORK_FOUND FALSE)

if (NOT QT_CMAKE_DIR)
	find_package (Qt5Core)
	if (Qt5Core_FOUND)
	 	set (QT_FOUND TRUE)
		set (QTCORE_FOUND TRUE)
	endif ()

	find_package (Qt5Gui)
	if (Qt5Gui_FOUND)
		set (QTGUI_FOUND TRUE)
	endif ()

	find_package (Qt5Widgets)
	if (Qt5Widgets_FOUND)
		set (QTWIDGETS_FOUND TRUE)
	endif ()

	find_package (Qt5Network)
	if (Qt5Network_FOUND)
		set (QTNETWORK_FOUND TRUE)
	endif ()
else ()
	if (EXISTS ${QT_CMAKE_DIR}/Qt5Core/Qt5CoreConfig.cmake)
		include ("${QT_CMAKE_DIR}/Qt5Core/Qt5CoreConfig.cmake")
	 	set (QT_FOUND TRUE)
		set (QTCORE_FOUND TRUE)
	endif ()

	if (EXISTS ${QT_CMAKE_DIR}/Qt5Core/Qt5GuiConfig.cmake)
		include ("${QT_CMAKE_DIR}/Qt5Gui/Qt5GuiConfig.cmake")
		set (QTGUI_FOUND TRUE)
	endif ()

	if (EXISTS ${QT_CMAKE_DIR}/Qt5Core/Qt5WidgetsConfig.cmake)
		include ("${QT_CMAKE_DIR}/Qt5Widgets/Qt5WidgetsConfig.cmake")
		set (QTWIDGETS_FOUND TRUE)
	endif ()

	if (EXISTS ${QT_CMAKE_DIR}/Qt5Core/Qt5NetworkConfig.cmake)
		include ("${QT_CMAKE_DIR}/Qt5Network/Qt5NetworkConfig.cmake")
		set (QTNETWORK_FOUND TRUE)
	endif ()
endif ()

if (QT_FOUND)
	axl_message ("QT paths:")
	axl_message ("    Core:")
	axl_message ("        CMake files:" "${Qt5Core_DIR}")
	axl_message ("        Includes:"    "${Qt5Core_INCLUDE_DIRS}")

	if (QTGUI_FOUND)
		axl_message ("    Gui:")
		axl_message ("        CMake files:" "${Qt5Gui_DIR}")
		axl_message ("        Includes:"    "${Qt5Gui_INCLUDE_DIRS}")
	endif ()

	if (QTWIDGETS_FOUND)
		axl_message ("    Widgets:")
		axl_message ("        CMake files:" "${Qt5Widgets_DIR}")
		axl_message ("        Includes:"    "${Qt5Widgets_INCLUDE_DIRS}")
	endif ()

	if (QTNETWORK_FOUND)
		axl_message ("    Network:")
		axl_message ("        CMake files:" "${Qt5Network_DIR}")
		axl_message ("        Includes:"    "${Qt5Network_INCLUDE_DIRS}")
	endif ()
endif ()

#...............................................................................

# this macro is similar to qt5_wrap_cpp but does not generate those ridiculous output paths

macro (
qt5_wrap_cpp_alt
	_MOC_CPP_LIST
	# ...
	)

	set (${_MOC_CPP_LIST}) # clear list
	set (_H_LIST ${ARGN})

	foreach (_H_PATH ${_H_LIST})
		get_filename_component (_H_NAME ${_H_PATH} NAME_WE)
		get_filename_component (_H_EXT ${_H_PATH} EXT)

		string (COMPARE EQUAL ${_H_EXT} ".cpp" _IS_CPP)

		if (${_IS_CPP})
			set (_MOC_CPP_PATH ${CMAKE_CURRENT_BINARY_DIR}/${_H_NAME}.moc)
		else ()
			set (_MOC_CPP_PATH ${CMAKE_CURRENT_BINARY_DIR}/moc_${_H_NAME}.cpp)
		endif ()

		qt5_generate_moc (${_H_PATH} ${_MOC_CPP_PATH})
		list (APPEND ${_MOC_CPP_LIST} ${_MOC_CPP_PATH})
	endforeach ()
endmacro ()

#...............................................................................

macro (
copy_qt_dll_files
	_TARGET
	# ...
	)

	if (NOT QT_FOUND)
		message (FATAL_ERROR "QT is required for copy_qt_dll_files () macro")
	endif ()

	if (NOT WIN32)
		message (FATAL_ERROR "copy_qt_dll_files should only be used on Windows")
	endif ()

	set (_QT_MODULE_LIST ${ARGN})

	if (CMAKE_GENERATOR MATCHES "Visual Studio")
		foreach (_QT_MODULE ${_QT_MODULE_LIST})
			set (_QT_FILE_NAME_DEBUG Qt5${_QT_MODULE}d.dll)
			set (_QT_FILE_NAME_RELEASE Qt5${_QT_MODULE}.dll)

			axl_copy_file_if_different (
				${_TARGET}
				${QT_DLL_DIR}/${_QT_FILE_NAME_DEBUG}
				${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug/${_QT_FILE_NAME_DEBUG}
				)

			axl_copy_file_if_different (
				${_TARGET}
				${QT_DLL_DIR}/${_QT_FILE_NAME_RELEASE}
				${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release/${_QT_FILE_NAME_RELEASE}
				)
		endforeach ()
	else ()
		if ("${_BUILD_TYPE}" STREQUAL "Debug")
			set (_QT_DLL_SUFFIX "d.dll")
		else ()
			set (_QT_DLL_SUFFIX ".dll")
		endif ()

		foreach (_QT_MODULE ${_QT_MODULE_LIST})
			set (_QT_FILE_NAME Qt5${_QT_MODULE}${_QT_DLL_SUFFIX})

			axl_copy_file_if_different (
				${_TARGET}
				${QT_DLL_DIR}/${_QT_FILE_NAME}
				${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${_QT_FILE_NAME}
				)
		endforeach ()
	endif ()
endmacro ()

#...............................................................................

macro (
add_qt_rpath_link)
	if (NOT QT_FOUND)
		message (FATAL_ERROR "QT is required for add_qt_rpath_link () macro")
	endif ()

	if (NOT UNIX OR APPLE)
		message (FATAL_ERROR "add_qt_rpath_link should only be used on Unix")
	endif ()

	if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
		set (
			CMAKE_EXE_LINKER_FLAGS
			"${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath-link,'${QT_CMAKE_DIR}/..'"
			)
	endif ()
endmacro ()

#...............................................................................
