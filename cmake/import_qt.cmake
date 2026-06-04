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

unset(Qt5Core_DIR        CACHE)
unset(Qt5Gui_DIR         CACHE)
unset(Qt5Widgets_DIR     CACHE)
unset(Qt5Network_DIR     CACHE)
unset(Qt5DBus_DIR        CACHE)

unset(Qt6Core_DIR        CACHE)
unset(Qt6Gui_DIR         CACHE)
unset(Qt6Widgets_DIR     CACHE)
unset(Qt6Network_DIR     CACHE)
unset(Qt6DBus_DIR        CACHE)
unset(Qt6Core5Compat_DIR CACHE)

unset(Qt6CoreTools_DIR    CACHE)
unset(Qt6GuiTools_DIR     CACHE)
unset(Qt6WidgetsTools_DIR CACHE)
unset(Qt6NetworkTools_DIR CACHE)
unset(Qt6DBusTools_DIR    CACHE)

set(QT_FOUND FALSE)
set(QTCORE_FOUND FALSE)
set(QTGUI_FOUND FALSE)
set(QTWIDGETS_FOUND FALSE)
set(QTNETWORK_FOUND FALSE)
set(QTDBUS_FOUND FALSE)

if(QT_CMAKE_DIR)
	set(
		_FIND_PACKAGE_FLAGS
		NO_DEFAULT_PATH
		NO_CMAKE_FIND_ROOT_PATH
		PATHS ${QT_CMAKE_DIR}
	)
endif()

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

macro(
	find_qt_paths
	_PREFIX
)
	find_package(${_PREFIX}Core QUIET ${_FIND_PACKAGE_FLAGS})
	if(${_PREFIX}Core_FOUND)
		set(QT_FOUND TRUE)
		set(QTCORE_FOUND TRUE)
		set(QTCORE_CMAKE_DIR ${${_PREFIX}Core_DIR})

		set(QT_VERSION_MAJOR ${${_PREFIX}Core_VERSION_MAJOR})
		set(QT_VERSION_MINOR ${${_PREFIX}Core_VERSION_MINOR})
		set(QT_VERSION_PATCH ${${_PREFIX}Core_VERSION_PATCH})
	endif()

	find_package(${_PREFIX}Gui QUIET ${_FIND_PACKAGE_FLAGS})
	if(${_PREFIX}Gui_FOUND)
		set(QTGUI_FOUND TRUE)
		set(QTGUI_CMAKE_DIR ${${_PREFIX}Gui_DIR})
	endif()

	find_package(${_PREFIX}Widgets QUIET ${_FIND_PACKAGE_FLAGS})
	if(${_PREFIX}Widgets_FOUND)
		set(QTWIDGETS_FOUND TRUE)
		set(QTWIDGETS_CMAKE_DIR ${${_PREFIX}Widgets_DIR})
	endif()

	find_package(${_PREFIX}Network QUIET ${_FIND_PACKAGE_FLAGS})
	if(${_PREFIX}Network_FOUND)
		set(QTNETWORK_FOUND TRUE)
		set(QTNETWORK_CMAKE_DIR ${${_PREFIX}Network_DIR})
	endif()

	if(UNIX AND NOT APPLE)
		find_package(${_PREFIX}DBus QUIET ${_FIND_PACKAGE_FLAGS})
		if(${_PREFIX}DBus_FOUND)
			set(QTDBUS_FOUND TRUE)
			set(QTDBUS_CMAKE_DIR ${${_PREFIX}DBus_DIR})
		endif()
	endif()

	if(QT_VERSION_MAJOR EQUAL 6)
		find_package(Qt6Core5Compat QUIET ${_FIND_PACKAGE_FLAGS})
		if(Qt6Core5Compat_FOUND)
			set(QTCORE5COMPAT_FOUND TRUE)
			set(QTCORE5COMPAT_CMAKE_DIR ${Qt6Core5Compat_DIR})
		endif()
	endif()
endmacro()

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

if(QT_CMAKE_DIR)
	# QT6's _qt_internal_find_tool_dependencies calls find_package() prepending
	# QT_HOST_PATH and QT_HOST_PATH_CMAKE_DIR to CMake find roots -- that's the 
	# only way to reliably beat system-installed QT6 tools, which could be 
	# incompatible  with the exact version of QT6 QT_CMAKE_DIR points to

	set(QT_HOST_PATH "${QT_CMAKE_DIR}/../.." CACHE PATH "" FORCE)
	set(QT_HOST_PATH_CMAKE_DIR "${QT_CMAKE_DIR}" CACHE PATH "" FORCE)
endif()

if(QT_VERSION_MAJOR)
	find_qt_paths(Qt${QT_VERSION_MAJOR})
else()
	find_qt_paths(Qt6)
	if(NOT QT_FOUND)
		find_qt_paths(Qt5)
	endif()
endif()

if(QT_CMAKE_DIR)
	unset(QT_HOST_PATH CACHE)
	unset(QT_HOST_PATH_CMAKE_DIR CACHE)
endif()

if(QT_FOUND)
	axl_message("QT ${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH} paths:")
	axl_message("    Core:" "${QTCORE_CMAKE_DIR}")

	if(QTGUI_FOUND)
		axl_message("    Gui:" "${QTGUI_CMAKE_DIR}")
	endif()

	if(QTWIDGETS_FOUND)
		axl_message("    Widgets:" "${QTWIDGETS_CMAKE_DIR}")
	endif()

	if(QTNETWORK_FOUND)
		axl_message("    Network:" "${QTNETWORK_CMAKE_DIR}")
	endif()

	if(QTDBUS_FOUND)
		axl_message("    DBus:" "${QTDBUS_CMAKE_DIR}")
	endif()

	if(QTCORE5COMPAT_FOUND)
		axl_message("    Core5Compat:" "${QTCORE5COMPAT_CMAKE_DIR}")
	endif()

	if(QT_DLL_DIR)
		axl_message("    DLLs:" "${QT_DLL_DIR}")
		if(NOT QT_PLUGIN_DIR)
			get_filename_component(QT_PLUGIN_DIR "${QT_DLL_DIR}/../plugins" ABSOLUTE)
		endif()
	endif()

	if(QT_PLUGIN_DIR)
		axl_message("    Plugins:" "${QT_PLUGIN_DIR}")
	endif()
endif()

#...............................................................................

# this macro is similar to qt5_wrap_cpp but guarantees to output directly to
# ${CMAKE_CURRENT_BINARY_DIR} without creating any subdirs
#
# crucial if you include generated moc_xxx.cpp files manually and thus need to
# add know exact include path

macro(
qt_wrap_cpp_alt
	_MOC_CPP_LIST
	# ...
)

	set(${_MOC_CPP_LIST}) # clear list
	set(_H_LIST ${ARGN})

	foreach(_H_PATH ${_H_LIST})
		get_filename_component(_H_NAME ${_H_PATH} NAME_WE)
		get_filename_component(_H_EXT ${_H_PATH} EXT)

		string(COMPARE EQUAL "${_H_EXT}" ".cpp" _IS_CPP)

		if(${_IS_CPP})
			set(_MOC_CPP_PATH ${CMAKE_CURRENT_BINARY_DIR}/${_H_NAME}.moc)
		else()
			set(_MOC_CPP_PATH ${CMAKE_CURRENT_BINARY_DIR}/moc_${_H_NAME}.cpp)
		endif()

		if(QT_VERSION_MAJOR EQUAL 6)
			qt6_generate_moc(${_H_PATH} ${_MOC_CPP_PATH})
		else()
			qt5_generate_moc(${_H_PATH} ${_MOC_CPP_PATH})
		endif()

		list(APPEND ${_MOC_CPP_LIST} ${_MOC_CPP_PATH})
	endforeach()
endmacro()

# this macro is similar to qt5_use_modules but doesn't touch the target's
# POSITION_INDEPENDENT_CODE property which MAY cause some QT statics to be
# instantiated and exported from the executable -- ultimately resulting in
# possible ABI versioning issues with newer QT versions (e.g. users start to
# experience crashes after upgrading to qt-5.15)
#
# crucial for qt-5.0.x through 5.3.x; starting with 5.4.x QT macros are fixed

macro(
qt_use_modules_alt
	_TARGET
	# ...
)
	set(_MODULES ${ARGN})

	if(QT_VERSION_MAJOR EQUAL 5 AND Qt5Core_VERSION_STRING VERSION_LESS 5.4.0)
		if(TARGET Qt5::Core)
			set_property(TARGET Qt5::Core PROPERTY INTERFACE_POSITION_INDEPENDENT_CODE)
		endif()

		get_target_property(_PREV_PIC ${_TARGET} POSITION_INDEPENDENT_CODE)
		qt5_use_modules(${_TARGET} ${_MODULES})

		if("${_PREV_PIC}" MATCHES "-NOTFOUND")
			unset(_PREV_PIC)
		endif()

		set_property(TARGET ${_TARGET} PROPERTY POSITION_INDEPENDENT_CODE ${_PREV_PIC})
	else() # on newer QTs, use imported targets
		set(_IMPORTS)
		foreach(_MODULE ${_MODULES})
			list(APPEND _IMPORTS "Qt${QT_VERSION_MAJOR}::${_MODULE}")
		endforeach()
		target_link_libraries(${_TARGET} ${_IMPORTS})
	endif()
endmacro()

macro(
qt_create_qt_conf
	_DIR
)

	file(
		WRITE ${_DIR}/qt.conf
		"[Paths]\n"
		"Plugins = ${QT_PLUGIN_DIR}\n"
	)
endmacro()

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

# versionless shims for QT 5 — QT 6 defines these natively

if(QT_VERSION_MAJOR EQUAL 5)
    macro(qt_wrap_cpp)
        qt5_wrap_cpp(${ARGN})
    endmacro()

    macro(qt_wrap_ui)
        qt5_wrap_ui(${ARGN})
    endmacro()

    macro(qt_add_resources)
        qt5_add_resources(${ARGN})
    endmacro()
endif()

#...............................................................................
