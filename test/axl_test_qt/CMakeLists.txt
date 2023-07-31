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

#
# src folder
#

set(
	SRC_H_LIST
	mainwindow.h
)

set(
	SRC_CPP_LIST
	main.cpp
	mainwindow.cpp
)

source_group(
	src
	FILES
	${SRC_H_LIST}
	${SRC_CPP_LIST}
	${SRC_RL_LIST}
)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# gen folder
#

set(GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/gen)
file(MAKE_DIRECTORY ${GEN_DIR})

axl_push_and_set(CMAKE_CURRENT_BINARY_DIR ${GEN_DIR})

set(
	MOC_H_LIST
	mainwindow.h
)

qt5_wrap_cpp_alt(
	GEN_MOC_CPP_LIST
	${MOC_H_LIST}
)

axl_pop(CMAKE_CURRENT_BINARY_DIR)

axl_exclude_from_build(${GEN_MOC_CPP_LIST}) # include "moc_*.cpp" manually

source_group(
	gen
	FILES
	${GEN_MOC_CPP_LIST}
)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# pch folder
#

set(PCH_H   pch.h)

source_group(
	pch
	FILES
	${PCH_H}
	REGULAR_EXPRESSION cmake_pch
)

#...............................................................................
#
# axl_test_qt compiler test
#

include_directories(
	${AXL_INC_DIR}
	${GEN_DIR}
)

link_directories(${AXL_LIB_DIR})

add_definitions(
	-DQT_COMPILING_QSTRING_COMPAT_CPP
	-DQT_COMPILING_QIMAGE_COMPAT_CPP
)

add_executable(
	axl_test_qt
	${PCH_H}
	${SRC_H_LIST}
	${SRC_CPP_LIST}
	${GEN_MOC_CPP_LIST}
)

qt5_use_modules_alt(axl_test_qt Widgets)

target_precompile_headers(
	axl_test_qt
	PRIVATE
	${PCH_H}
)

set_target_properties(
	axl_test_qt
	PROPERTIES
	FOLDER test
)

target_link_libraries(
	axl_test_qt
	axl_io
	axl_mem_new
	axl_core
)

if(UNIX)
	target_link_libraries(
		axl_test_qt
		pthread
	)

	if(NOT APPLE)
		target_link_libraries(
			axl_test_qt
			dl
			rt
		)
	endif()
endif()

if(WIN32 AND QT_DLL_DIR)
	set(_DEBUG_SUFFIX $<$<CONFIG:Debug>:d>)

	add_custom_command(
		TARGET axl_test_qt
		POST_BUILD
		COMMAND
			echo Copying DLL files for axl_test_qt...
		COMMAND
			${CMAKE_COMMAND} -E copy
			${QT_DLL_DIR}/Qt5Core${_DEBUG_SUFFIX}.dll
			${QT_DLL_DIR}/Qt5Gui${_DEBUG_SUFFIX}.dll
			${QT_DLL_DIR}/Qt5Widgets${_DEBUG_SUFFIX}.dll
			${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CONFIGURATION}
		)

	qt5_create_qt_conf(${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug)
	qt5_create_qt_conf(${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release)
endif()

#...............................................................................
