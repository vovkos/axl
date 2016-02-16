#..............................................................................

if ("${QT_CMAKE_DIR}" STREQUAL "")
	set (QT_FOUND FALSE)
	message (STATUS "QT:                         <not-found>")
else ()
	set (QT_FOUND TRUE)
	message (STATUS "Path to QT cmake files:     ${QT_CMAKE_DIR}")
endif ()

unset (Qt5Core_DIR    CACHE)
unset (Qt5Gui_DIR     CACHE)
unset (Qt5Widgets_DIR CACHE)
unset (Qt5Network_DIR CACHE)

if (QT_FOUND)
	include ("${QT_CMAKE_DIR}/Qt5Core/Qt5CoreConfig.cmake")
	include ("${QT_CMAKE_DIR}/Qt5Gui/Qt5GuiConfig.cmake")
	include ("${QT_CMAKE_DIR}/Qt5Widgets/Qt5WidgetsConfig.cmake")
	include ("${QT_CMAKE_DIR}/Qt5Network/Qt5NetworkConfig.cmake")
endif ()

#..............................................................................

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

#..............................................................................

macro (
copy_qt_dll_files
	_TARGET
	# ...
	)

	if (NOT WIN32)
		message (FATAL_ERROR "copy_qt_dll_files should only be used on Windows")
	endif ()

	set (_QT_MODULE_LIST ${ARGN})

	if (CMAKE_GENERATOR MATCHES "Visual Studio")
		foreach (_QT_MODULE ${_QT_MODULE_LIST})
			axl_copy_file_if_different (
				${_TARGET}
				${QT_DLL_DIR}
				${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug
				Qt5${_QT_MODULE}d.dll
				)

			axl_copy_file_if_different (
				${_TARGET}
				${QT_DLL_DIR}
				${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release
				Qt5${_QT_MODULE}.dll
				)
		endforeach ()
	else ()
		if ("${_BUILD_TYPE}" STREQUAL "Debug")
			set (_QT_DLL_SUFFIX "d.dll")
		else ()
			set (_QT_DLL_SUFFIX ".dll")
		endif ()

		foreach (_QT_MODULE ${_QT_MODULE_LIST})
			axl_copy_file_if_different (
				${_TARGET}
				${QT_DLL_DIR}
				${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
				Qt5${_QT_MODULE}${_QT_DLL_SUFFIX}
				)
		endforeach ()
	endif ()
endmacro ()

#..............................................................................

macro (
add_qt_rpath_link)
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

#..............................................................................
