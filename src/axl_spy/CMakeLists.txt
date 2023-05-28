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
# lib folder
#

set(
	LIB_H_LIST
	${AXL_INC_DIR}/axl_spy/axl_spy_ImportEnumerator.h
	${AXL_INC_DIR}/axl_spy/axl_spy_ImportWriteProtection.h
	${AXL_INC_DIR}/axl_spy/axl_spy_ModuleEnumerator.h
	${AXL_INC_DIR}/axl_spy/axl_spy_Hook.h
	axl_spy_ThreadState.h
	axl_spy_HookCommon.h
)

set(
	LIB_CPP_LIST
	axl_spy_ImportEnumerator.cpp
	axl_spy_ImportWriteProtection.cpp
	axl_spy_ModuleEnumerator.cpp
	axl_spy_ThreadState.cpp
	axl_spy_HookCommon.cpp
	axl_spy_Hook_amd64_msc.cpp
	axl_spy_Hook_amd64_gcc.cpp
	axl_spy_Hook_x86.cpp
)

set(
	LIB_ASM_LIST
	axl_spy_thunk_amd64_msc.asm
	axl_spy_thunk_amd64_gcc.asm
	axl_spy_thunk_x86.asm
)

source_group(
	lib
	FILES
	${LIB_H_LIST}
	${LIB_CPP_LIST}
	${LIB_ASM_LIST}
)

if(${TARGET_CPU} STREQUAL "x86")
	axl_exclude_from_build(axl_spy_Hook_amd64_msc.cpp)
	axl_exclude_from_build(axl_spy_Hook_amd64_gcc.cpp)
elseif(${TARGET_CPU} STREQUAL "amd64")
	axl_exclude_from_build(axl_spy_Hook_x86.cpp)
	if(NOT MSVC)
		axl_exclude_from_build(axl_spy_Hook_amd64_msc.cpp)
	elseif(NOT GCC)
		axl_exclude_from_build(axl_spy_Hook_amd64_gcc.cpp)
	endif()
endif()

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# pch folder
#

set(PCH_H   pch.h)

set(
	PCH_H_LIST
	${PCH_H}
	${AXL_INC_DIR}/axl_spy/axl_spy_Pch.h
)

source_group(
	pch
	FILES
	${PCH_H}
)

#...............................................................................
#
# axl_spy library (API spying/hooking engine)
#

include_directories(${AXL_INC_DIR})

add_library(
	axl_spy
	${PCH_H_LIST}
	${LIB_H_LIST}
	${LIB_CPP_LIST}
	${LIB_ASM_LIST}
)

target_precompile_headers(
	axl_spy
	PRIVATE
	${PCH_H}
)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

axl_install_redirected_includes(
	${AXL_INSTALL_INC_SUBDIR}
	${PCH_H_LIST}
	${LIB_H_LIST}
)

install(
	TARGETS axl_spy
	DESTINATION ${AXL_INSTALL_LIB_SUBDIR}
)

#...............................................................................
