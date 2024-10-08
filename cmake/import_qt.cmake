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

unset(Qt5Core_DIR    CACHE)
unset(Qt5Gui_DIR     CACHE)
unset(Qt5Widgets_DIR CACHE)
unset(Qt5Network_DIR CACHE)

set(QT_FOUND FALSE)
set(QTCORE_FOUND FALSE)
set(QTGUI_FOUND FALSE)
set(QTWIDGETS_FOUND FALSE)
set(QTNETWORK_FOUND FALSE)

if(NOT QT_CMAKE_DIR)
	find_package(Qt5Core QUIET)
	if(Qt5Core_FOUND)
	 	set(QT_FOUND TRUE)
		set(QTCORE_FOUND TRUE)
		set(QTCORE_CMAKE_DIR ${Qt5Core_DIR})
	endif()

	find_package(Qt5Gui QUIET)
	if(Qt5Gui_FOUND)
		set(QTGUI_FOUND TRUE)
		set(QTGUI_CMAKE_DIR ${Qt5Gui_DIR})
	endif()

	find_package(Qt5Widgets QUIET)
	if(Qt5Widgets_FOUND)
		set(QTWIDGETS_FOUND TRUE)
		set(QTWIDGETS_CMAKE_DIR ${Qt5Widgets_DIR})
	endif()

	find_package(Qt5Network QUIET)
	if(Qt5Network_FOUND)
		set(QTNETWORK_FOUND TRUE)
		set(QTNETWORK_CMAKE_DIR ${Qt5Network_DIR})
	endif()

	if(UNIX AND NOT APPLE)
		find_package(Qt5DBus QUIET)
		if(Qt5DBus_FOUND)
		 	set(QT_FOUND TRUE)
			set(QTDBUS_FOUND TRUE)
			set(QTDBUS_CMAKE_DIR ${Qt5DBus_DIR})
		endif()
	endif()
else()
	if(EXISTS ${QT_CMAKE_DIR}/Qt5Core/Qt5CoreConfig.cmake)
		include("${QT_CMAKE_DIR}/Qt5Core/Qt5CoreConfig.cmake")
	 	set(QT_FOUND TRUE)
		set(QTCORE_FOUND TRUE)
		set(QTCORE_CMAKE_DIR ${QT_CMAKE_DIR}/Qt5Core)
	endif()

	if(EXISTS ${QT_CMAKE_DIR}/Qt5Gui/Qt5GuiConfig.cmake)
		include("${QT_CMAKE_DIR}/Qt5Gui/Qt5GuiConfig.cmake")
		set(QTGUI_FOUND TRUE)
		set(QTGUI_CMAKE_DIR ${QT_CMAKE_DIR}/Qt5Gui)
	endif()

	if(EXISTS ${QT_CMAKE_DIR}/Qt5Widgets/Qt5WidgetsConfig.cmake)
		include("${QT_CMAKE_DIR}/Qt5Widgets/Qt5WidgetsConfig.cmake")
		set(QTWIDGETS_FOUND TRUE)
		set(QTWIDGETS_CMAKE_DIR ${QT_CMAKE_DIR}/Qt5Widgets)
	endif()

	if(EXISTS ${QT_CMAKE_DIR}/Qt5Network/Qt5NetworkConfig.cmake)
		include("${QT_CMAKE_DIR}/Qt5Network/Qt5NetworkConfig.cmake")
		set(QTNETWORK_FOUND TRUE)
		set(QTNETWORK_CMAKE_DIR ${QT_CMAKE_DIR}/Qt5Network)
	endif()

	if(UNIX AND NOT APPLE AND EXISTS ${QT_CMAKE_DIR}/Qt5DBus/Qt5DBusConfig.cmake)
		include("${QT_CMAKE_DIR}/Qt5DBus/Qt5DBusConfig.cmake")
		set(QTDBUS_FOUND TRUE)
		set(QTDBUS_CMAKE_DIR ${QT_CMAKE_DIR}/Qt5DBus)
	endif()
endif()

if(QT_FOUND)
	axl_message("QT ${Qt5Core_VERSION_STRING} paths:")
	axl_message("    Core CMake files:" "${QTCORE_CMAKE_DIR}")

	if(QTGUI_FOUND)
		axl_message("    Gui CMake files:" "${QTGUI_CMAKE_DIR}")
	endif()

	if(QTWIDGETS_FOUND)
		axl_message("    Widgets CMake files:" "${QTWIDGETS_CMAKE_DIR}")
	endif()

	if(QTNETWORK_FOUND)
		axl_message("    Network CMake files:" "${QTNETWORK_CMAKE_DIR}")
	endif()

	if(QTDBUS_FOUND)
		axl_message("    DBus CMake files:" "${QTDBUS_CMAKE_DIR}")
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

# this macro is similar to qt5_wrap_cpp but does not generate those ridiculous output paths

macro(
qt5_wrap_cpp_alt
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

		qt5_generate_moc(${_H_PATH} ${_MOC_CPP_PATH})
		list(APPEND ${_MOC_CPP_LIST} ${_MOC_CPP_PATH})
	endforeach()
endmacro()

#...............................................................................

# this macro is similar to qt5_use_modules but doesn't touch the target's
# POSITION_INDEPENDENT_CODE property which MAY cause some QT statics to be
# instantiated and exported from the executable -- ultimately resulting in
# possible ABI versioning issues with newer QT versions (e.g. users start to
# experience crashes after upgrading to qt-5.15)
#
# crucial for qt-5.0.x through 5.3.x; starting with 5.4.x QT macros are fixed

macro(
qt5_use_modules_alt
	_TARGET
	# ...
)

	if(Qt5Core_VERSION_STRING VERSION_LESS 5.4.0)
		get_target_property(_PREV_PIC ${_TARGET} POSITION_INDEPENDENT_CODE)
		qt5_use_modules(${_TARGET} ${ARGN})
		set_target_properties(${_TARGET} PROPERTIES POSITION_INDEPENDENT_CODE ${_PREV_PIC})
	else() # on newer QTs, use imported targets
		set(_MODULES ${ARGN})
		set(_IMPORTS)
		foreach(_MODULE ${_MODULES})
			list(APPEND _IMPORTS "Qt5::${_MODULE}")
		endforeach()
		target_link_libraries(${_TARGET} ${_IMPORTS})
	endif()
endmacro()

macro(
qt5_create_qt_conf
	_DIR
)

	file(
		WRITE ${_DIR}/qt.conf
		"[Paths]\n"
		"Plugins = ${QT_PLUGIN_DIR}\n"
	)
endmacro()

#...............................................................................
