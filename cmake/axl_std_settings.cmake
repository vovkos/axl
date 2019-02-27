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

# CPU setting

macro(
axl_create_target_cpu_setting)

	axl_detect_target_cpu(_CPU)

	# Microsoft Visual C++ is a native-only generator, x86/amd64 GCC can cross-compile

	if(NOT GCC OR NOT("${_CPU}" STREQUAL "x86" OR "${_CPU}" STREQUAL "amd64"))
		set(TARGET_CPU ${_CPU})
	else()
		axl_create_setting(
			TARGET_CPU
			DESCRIPTION "Target CPU"
			DEFAULT ${_CPU}
			"x86"
			"amd64"
			)
	endif()
endmacro()

macro(
axl_apply_target_cpu_setting)

	if("${TARGET_CPU}" STREQUAL "amd64")
		set(CMAKE_SIZEOF_VOID_P 8)

		set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS TRUE)
		set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB32_PATHS FALSE)

		if(GCC)
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64 -mcx16")
			set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -m64 -mcx16")
		endif()
	elseif("${TARGET_CPU}" STREQUAL "x86")
		set(CMAKE_SIZEOF_VOID_P 4)

		set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS FALSE)
		set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB32_PATHS TRUE)

		if(GCC)
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
			set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -m32")
		endif()
	elseif(NOT "${TARGET_CPU}" MATCHES "arm")
		message(FATAL_ERROR "Unsupported CPU: ${TARGET_CPU}")
	endif()

	# for backward compatibility (in paths.cmake, settings.cmake, etc)

	set(AXL_CPU ${TARGET_CPU})
endmacro()

#...............................................................................

macro(
axl_create_build_type_setting)

	set(
		CMAKE_CONFIGURATION_TYPES
		"Debug"
		"Release"
		CACHE STRING "List of supported configurations" FORCE
		)

	# Microsoft Visual C++ and Xcode are multi-configuration generators

	if(CMAKE_GENERATOR MATCHES "Visual Studio" OR
		CMAKE_GENERATOR MATCHES "Xcode")
		set(IS_MULTI_CONFIGURATION TRUE)
	else()
		set(IS_MULTI_CONFIGURATION FALSE)

		axl_create_setting(
			CMAKE_BUILD_TYPE
			DESCRIPTION "Build configuration"
			DEFAULT Release
			${CMAKE_CONFIGURATION_TYPES}
			)
	endif()
endmacro()

macro(
axl_apply_build_type_setting)

	# Microsoft Visual C++ and Xcode are multi-configuration generators

	if(CMAKE_GENERATOR MATCHES "Visual Studio")
		set(CONFIGURATION     "$(Configuration)")
		set(CONFIGURATION_MCG "$(Configuration)")
		set(CONFIGURATION_SCG)
	elseif(CMAKE_GENERATOR MATCHES "Xcode")
		set(CONFIGURATION     "$(CONFIGURATION)")
		set(CONFIGURATION_MCG "$(CONFIGURATION)")
		set(CONFIGURATION_SCG)
	else()
		set(CONFIGURATION     "${CMAKE_BUILD_TYPE}")
		set(CONFIGURATION_MCG)
		set(CONFIGURATION_SCG "${CMAKE_BUILD_TYPE}")
	endif()

	# for backward compatibility (in paths.cmake, settings.cmake, etc)

	set(CONFIGURATION_SUFFIX   "${CONFIGURATION}")
	set(CONFIGURATION_SUFFIX_0 "${CONFIGURATION_SCG}")
endmacro()

#...............................................................................

macro(
axl_create_msvc_settings)

	option(
		MSVC_USE_FOLDERS
		"Use solution folders in Microsoft Visual Studio"
		ON
		)

	option(
		MSVC_USE_PCH
		"Use precompiled headers in Microsoft Visual C/C++"
		ON
		)

	option(
		MSVC_USE_UNICODE
		"Compile for UNICODE rather than multibyte"
		ON
		)

	axl_create_compiler_flag_setting(
		MSVC_FLAG_SHOW_INCLUDES
		DESCRIPTION "Microsoft Visual C++ shows includes stack during compilation"
		"/showIncludes"
		)

	axl_create_compiler_flag_setting(
		MSVC_FLAG_EH
		DESCRIPTION "Microsoft Visual C++ EH(exception handling) model"
		"/EHsc" "/EHs-c-"
		)

	axl_create_compiler_flag_setting(
		MSVC_FLAG_RTTI
		DESCRIPTION "Microsoft Visual C++ RTTI(run-time type information) support"
		"/GR" "/GR-"
		)

	axl_create_compiler_flag_setting(
		MSVC_FLAG_CRT_DEBUG
		DESCRIPTION "Microsoft Visual C++ CRT(Debug)"
		DEFAULT "/MDd"
		"/MT" "/MTd" "/MD" "/MDd"
		)

	axl_create_compiler_flag_setting(
		MSVC_FLAG_CRT_RELEASE
		DESCRIPTION "Microsoft Visual C++ CRT(Release)"
		DEFAULT "/MD"
		"/MT" "/MTd" "/MD" "/MDd"
		)

	axl_create_compiler_flag_setting(
		MSVC_FLAG_DEBUG_INFO_DEBUG
		DESCRIPTION "Microsoft Visual C++ debug information format(Debug)"
		DEFAULT "/Zi"
		"/Z7" "/Zi" "/ZI"
		)

	axl_create_compiler_flag_setting(
		MSVC_FLAG_DEBUG_INFO_RELEASE
		DESCRIPTION "Microsoft Visual C++ debug information format(Release)"
		DEFAULT "/Zi"
		"/Z7" "/Zi" "/ZI"
		)

	axl_create_setting(
		MSVC_LINK_FLAG_IGNORE4221
		DESCRIPTION "Ignore linker warning LNK4221(object file doesn't define new symbols)"
		DEFAULT "/IGNORE:4221"
		" " "/IGNORE:4221"
		)

	option(
		MSVC_LINK_DEBUG_INFO_RELEASE
		"Generate linker debug information in Release builds"
		ON
		)
endmacro()

macro(
axl_apply_msvc_settings)

	set_property(
		GLOBAL PROPERTY
		USE_FOLDERS ${MSVC_USE_FOLDERS}
		)

	if(MSVC_USE_UNICODE)
		add_definitions(-DUNICODE -D_UNICODE)
	endif()

	if(MSVC_LINK_DEBUG_INFO_RELEASE)
		# /DEBUG implies linking unreferenced symbols by default; undo that with /OPT:REF

		set(
			_RELEASE_DEBUG_FLAGS
			"/DEBUG /OPT:REF"
			)

		set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} ${_RELEASE_DEBUG_FLAGS}")
		set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} ${_RELEASE_DEBUG_FLAGS}")
	endif()

	axl_apply_compiler_flag_setting_to_list(
		MSVC_LINK_FLAG_IGNORE4221
		CMAKE_EXE_LINKER_FLAGS
		CMAKE_MODULE_LINKER_FLAGS
		CMAKE_SHARED_LINKER_FLAGS
		CMAKE_STATIC_LINKER_FLAGS
		)
endmacro()

#...............................................................................

macro(
axl_create_gcc_settings)

	option(
		GCC_USE_PCH
		"Use precompiled headers in GNU C/C++"
		ON
		)

	axl_create_compiler_flag_setting(
		GCC_FLAG_SHOW_INCLUDES
		DESCRIPTION "GNU C++ shows include stack during compilation"
		"-H"
		)

	axl_create_compiler_flag_setting(
		GCC_FLAG_C_STANDARD
		DESCRIPTION "Specify the C standard to use"
		"-std=c90" "-std=c99" "-std=c11" "-std=gnu90" "-std=gnu99" "-std=gnu11"
		)

	axl_create_compiler_flag_setting(
		GCC_FLAG_CPP_STANDARD
		DESCRIPTION "Specify the C++ standard to use"
		"-std=c++98" "-std=c++11" "-std=c++14" "-std=gnu++98" "-std=gnu++11" "-std=gnu++14"
		)

	if(NOT CLANG)
		axl_create_compiler_flag_setting(
			GCC_FLAG_STATIC_LIBGCC
			DESCRIPTION "Use static linkage to libgcc"
			DEFAULT "-static-libgcc"
			"-static-libgcc"
			)

		axl_create_compiler_flag_setting(
			GCC_FLAG_CPP_STATIC_LIBSTDCPP
			DESCRIPTION "Use static linkage to libstdc++"
			DEFAULT "-static-libstdc++"
			"-static-libstdc++"
			)
	endif()

	axl_create_compiler_flag_setting(
		GCC_FLAG_CPP_RTTI
		DESCRIPTION "GNU C++ emits RTTI info"
		"-frtti" "-fno-rtti"
		)

	axl_create_compiler_flag_setting(
		GCC_FLAG_PIC
		DESCRIPTION "GNU C++ generates position independent code"
		DEFAULT "-fPIC"
		"-fpic" "-fPIC" "-fpie" "-fPIE"
		)

	axl_create_compiler_flag_setting(
		GCC_FLAG_VISIBILITY
		DESCRIPTION "GNU C++ doesn't export all symbols by default(-fvisibility=hidden)"
		DEFAULT "-fvisibility=hidden"
		"-fvisibility=default" "-fvisibility=hidden"
		)

	axl_create_compiler_flag_setting(
		GCC_FLAG_SANITIZE
		DESCRIPTION "Compile with a specific sanitizer instrumentation"
		"-fsanitize=address" "-fsanitize=memory" "-fsanitize=thread"
		"-fsanitize=fuzzer" "-fsanitize=fuzzer,address" "-fsanitize=fuzzer,memory"
		)

	axl_create_compiler_flag_setting(
		GCC_FLAG_COVERAGE
		DESCRIPTION "Compile with code coverage instrumentation"
		"-coverage"
		)

	axl_create_setting(
		GCC_LINK_FLAG_RDYNAMIC
		DESCRIPTION "Remove -rdynamic flag from default linker options"
		DEFAULT " "
		" " "-rdynamic"
		)

	if(NOT APPLE)
		option(
			GCC_LINK_FLAG_EXPORTLESS_EXE
			"Don't export ANYTHING from executables"
			ON
			)
	endif()

	# alas, warning suppressions must be passed in command line, not pragma-ed
	# that is because GCC diagnostic pragmas get lost if defined in precompiled headers

	axl_create_compiler_flag_setting(
		GCC_FLAG_WARNING_UNKNOWN_WARNING_OPTION
		DESCRIPTION "Warn about unknown warning command-line options"
		DEFAULT "-Wno-unknown-warning-option"
		"-Wunknown-warning-option" "-Wno-unknown-warning-option"
		)

	axl_create_compiler_flag_setting(
		GCC_FLAG_WARNING_DEPRECATED_DECLARATIONS
		DESCRIPTION "Warn about uses of functions/variables/types marked as deprecated"
		DEFAULT "-Wno-deprecated-declarations"
		"-Wdeprecated-declarations" "-Wno-deprecated-declarations"
		)

	axl_create_compiler_flag_setting(
		GCC_FLAG_WARNING_MULTICHAR
		DESCRIPTION "Warn if a multicharacter constant(‘'FOOF'’) is used"
		DEFAULT "-Wno-multichar"
		"-Wmultichar" "-Wno-multichar"
		)

	axl_create_compiler_flag_setting(
		GCC_FLAG_WARNING_FORMAT
		DESCRIPTION "Check calls to printf/scanf/etc to ensure that the arguments are appropriate to the format string"
		DEFAULT "-Wno-format"
		"-Wformat" "-Wno-format"
		)

	axl_create_compiler_flag_setting(
		GCC_FLAG_WARNING_INCOMPATIBLE_MS_STRUCT
		DESCRIPTION "Warn about possible layout incompatibilities with MS compilers"
		DEFAULT "-Wno-incompatible-ms-struct"
		"-Wincompatible-ms-struct" "-Wno-incompatible-ms-struct"
		)

	axl_create_compiler_flag_setting(
		GCC_FLAG_WARNING_DANGLING_ELSE
		DESCRIPTION "Warn about dangling else without explicit braces"
		DEFAULT "-Wno-dangling-else"
		"-Wdangling-else" "-Wno-dangling-else"
		)

	axl_create_compiler_flag_setting(
		GCC_FLAG_WARNING_LOGICAL_OP_PARENTHESES
		DESCRIPTION "Warn about '&&' within '||'"
		DEFAULT "-Wno-logical-op-parentheses"
		"-Wlogical-op-parentheses" "-Wno-logical-op-parentheses"
		)

	axl_create_compiler_flag_setting(
		GCC_FLAG_WARNING_SWITCH
		DESCRIPTION "Warn about missing case values in a switch statement"
		DEFAULT "-Wno-switch"
		"-Wswitch" "-Wno-switch"
		)

	axl_create_compiler_flag_setting(
		GCC_FLAG_WARNING_STRINGOP_OVERFLOW
		DESCRIPTION "Warn for calls to memcpy/strcpy which(GCC thinks) might overflow the buffer"
		DEFAULT "-Wno-stringop-overflow"
		"-Wstringop-overflow"
		"-Wstringop-overflow=1"
		"-Wstringop-overflow=2"
		"-Wstringop-overflow=3"
		"-Wstringop-overflow=4"
		"-Wno-stringop-overflow"
		)

	axl_create_compiler_flag_setting(
		GCC_FLAG_WARNING_MISSING_BRACES
		DESCRIPTION "Warn about missing braces in aggregate initializers"
		"-Wmissing-braces" "-Wno-missing-braces"
		)

	axl_create_compiler_flag_setting(
		GCC_FLAG_WARNING_UNUSED_VARIABLE
		DESCRIPTION "Warn about unused variables"
		"-Wunused-variable" "-Wno-unused-variable"
		)

	axl_create_compiler_flag_setting(
		GCC_FLAG_WARNING_UNUSED_VALUE
		DESCRIPTION "Warn about unused expressions"
		"-Wunused-value" "-Wno-unused-value"
		)

	# C++ specific warnings

	axl_create_compiler_flag_setting(
		GCC_FLAG_CPP_WARNING_INVALID_OFFSETOF
		DESCRIPTION "Warn about applying the offsetof macro to a non-POD type"
		DEFAULT "-Wno-invalid-offsetof"
		"-Winvalid-offsetof" "-Wno-invalid-offsetof"
		)

	axl_create_compiler_flag_setting(
		GCC_FLAG_CPP_WARNING_NARROWING
		DESCRIPTION "Warn about narrowing conversions"
		DEFAULT "-Wno-narrowing"
		"-Wnarrowing" "-Wno-narrowing"
		)

	axl_create_compiler_flag_setting(
		GCC_FLAG_CPP_WARNING_DELETE_NON_VIRTUAL_DTOR
		DESCRIPTION "Warn about calling non-virtual dtor of a class with virtual funtions"
		"-Wdelete-non-virtual-dtor" "-Wno-delete-non-virtual-dtor"
		)

	# the cppcheck static analyzer (introduced in CMake 3.10)

	if(NOT ${CMAKE_VERSION} VERSION_LESS 3.10)
		if(NOT CPPCHECK_EXE)
			find_program(CPPCHECK_EXE cppcheck)
		endif()

		if(CPPCHECK_EXE)
			option(
				GCC_USE_CPPCHECK
				"Use cppcheck for static analysis during build"
				OFF
				)
		endif()
	endif()
endmacro()

macro(
axl_apply_gcc_settings)

	if(NOT "${GCC_FLAG_SANITIZER_DEBUG}" STREQUAL " " AND CMAKE_GENERATOR MATCHES "Xcode")

		# Xcode fires unresolved externals otherwise

		set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} ${GCC_FLAG_SANITIZER_DEBUG}")
		set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} ${GCC_FLAG_SANITIZER_DEBUG}")
	endif()

	axl_create_compiler_flag_setting_regex(_REGEX GCC_LINK_FLAG_RDYNAMIC)
	axl_apply_compiler_flag(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS ${_REGEX} ${GCC_LINK_FLAG_RDYNAMIC})
	axl_apply_compiler_flag(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS ${_REGEX} ${GCC_LINK_FLAG_RDYNAMIC})

	if(GCC_LINK_FLAG_EXPORTLESS_EXE AND NOT APPLE)
		set(_VERSION_SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/exportless-exe.version")
		file(WRITE ${_VERSION_SCRIPT} "{ local: *; };")
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--version-script='${_VERSION_SCRIPT}'")
	endif()

	# cppcheck

	if(NOT GCC_USE_CPPCHECK)
		unset(CMAKE_C_CPPCHECK)
		unset(CMAKE_CXX_CPPCHECK)
	else()
		set(_EMPTY_C_FILE ${CMAKE_CURRENT_BINARY_DIR}/empty.c)
		set(_EMPTY_CPP_FILE ${CMAKE_CURRENT_BINARY_DIR}/empty.cpp)
		set(_CPPCHECK_C_PREAMBLE_FILE ${CMAKE_CURRENT_BINARY_DIR}/preamble_c.h)
		set(_CPPCHECK_CPP_PREAMBLE_FILE ${CMAKE_CURRENT_BINARY_DIR}/preamble_cpp.h)

		if(NOT EXISTS ${_CPPCHECK_C_PREAMBLE_FILE})
			axl_create_predefined_macro_file_gcc(
				${CMAKE_C_COMPILER}
				${_EMPTY_C_FILE}
				${_CPPCHECK_C_PREAMBLE_FILE}
				)
		endif()

		if(NOT EXISTS ${_CPPCHECK_CPP_PREAMBLE_FILE})
			axl_create_predefined_macro_file_gcc(
				${CMAKE_CXX_COMPILER}
				${_EMPTY_CPP_FILE}
				${_CPPCHECK_CPP_PREAMBLE_FILE}
				)
		endif()

		set(
			CMAKE_C_CPPCHECK
			${CPPCHECK_EXE}
			--quiet
			--include=${_CPPCHECK_C_PREAMBLE_FILE}
			-D_AXL_CPPCHECK=1
			)

		set(
			CMAKE_CXX_CPPCHECK
			${CPPCHECK_EXE}
			--quiet
			--include=${_CPPCHECK_CPP_PREAMBLE_FILE}
			-D_AXL_CPPCHECK=1
			)
	endif()
endmacro()

#...............................................................................

macro(
axl_create_std_settings)

	if("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
		set(LINUX TRUE)
	endif()

	if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
		set(CLANG TRUE)
		set(GCC TRUE)
	elseif(CMAKE_COMPILER_IS_GNUCXX)
		set(GCC TRUE)
	endif()

	axl_create_target_cpu_setting()
	axl_create_build_type_setting()

	if(MSVC)
		axl_create_msvc_settings()
	elseif(GCC)
		axl_create_gcc_settings()
	endif()
endmacro()

macro(
axl_apply_std_settings)

	axl_apply_target_cpu_setting()
	axl_apply_build_type_setting()

	if(MSVC)
		axl_apply_msvc_settings()
	elseif(GCC)
		axl_apply_gcc_settings()
	endif()

	axl_apply_all_compiler_flag_settings()
endmacro()

macro(
axl_cleanup_compiler_flags
	_FLAGS
	_WARNING_FLAG_LIST
	)

	string(REPLACE " " ";" _FLAG_LIST "${${_FLAGS}}")
	list(REMOVE_DUPLICATES _FLAG_LIST)

	foreach(_FLAG ${_FLAG_LIST})
		if(_FLAG MATCHES "-W[a-z-]+")
			list(APPEND ${_WARNING_FLAG_LIST} ${_FLAG})
			list(REMOVE_ITEM _FLAG_LIST ${_FLAG})
		endif()
	endforeach()

	string(REPLACE ";" " " ${_FLAGS} "${_FLAG_LIST}")
	string(STRIP "${${_FLAGS}}" ${_FLAGS})
endmacro()

macro(
axl_print_std_settings)

	string(LENGTH "........................:" AXL_G_MESSAGE_ALIGN)

	message(STATUS "AXL CMake:")
	axl_message("    Invoked from:" ${AXL_CMAKE_ORIGIN_FILE})
	axl_message("    dependencies.cmake:" ${AXL_CMAKE_ORIGIN_DIR}/dependencies.cmake)

	if(AXL_SETTINGS_CMAKE)
		axl_message("    settings.cmake:" ${AXL_SETTINGS_CMAKE})
	endif()

	if(AXL_PATHS_CMAKE)
		axl_message("    paths.cmake:" ${AXL_PATHS_CMAKE})
	endif()

	axl_message("    Target CPU:" ${TARGET_CPU})

	if(NOT "${CMAKE_BUILD_TYPE}" STREQUAL "")
		axl_message("    Build configuration:" ${CMAKE_BUILD_TYPE})
	endif()

	set(_C_FLAGS_DEBUG "${CMAKE_C_COMPILER_ARG1} ${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_DEBUG}")
	set(_C_FLAGS_RELEASE "${CMAKE_C_COMPILER_ARG1} ${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_RELEASE}")
	set(_CXX_FLAGS_DEBUG "${CMAKE_CXX_COMPILER_ARG1} ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG}")
	set(_CXX_FLAGS_RELEASE "${CMAKE_CXX_COMPILER_ARG1} ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE}")
	set(_WARNING_FLAG_LIST)

	axl_cleanup_compiler_flags(_C_FLAGS_DEBUG _WARNING_FLAG_LIST)
	axl_cleanup_compiler_flags(_C_FLAGS_RELEASE _WARNING_FLAG_LIST)
	axl_cleanup_compiler_flags(_CXX_FLAGS_DEBUG _WARNING_FLAG_LIST)
	axl_cleanup_compiler_flags(_CXX_FLAGS_RELEASE _WARNING_FLAG_LIST)

	message(STATUS "C/C++:")
	axl_message("    C Compiler:"          ${CMAKE_C_COMPILER})
	axl_message("    C flags(Debug):"     ${_C_FLAGS_DEBUG})
	axl_message("    C flags(Release):"   ${_C_FLAGS_RELEASE})
	axl_message("    C++ Compiler:"        ${CMAKE_CXX_COMPILER})
	axl_message("    C++ flags(Debug):"   ${_CXX_FLAGS_DEBUG})
	axl_message("    C++ flags(Release):" ${_CXX_FLAGS_RELEASE})

	if(_WARNING_FLAG_LIST)
		message(STATUS "    C/C++ warning flags:")

		list(REMOVE_DUPLICATES _WARNING_FLAG_LIST)
		list(SORT _WARNING_FLAG_LIST)

		foreach(_FLAG ${_WARNING_FLAG_LIST})
			axl_message("        ${_FLAG}")
		endforeach()
	endif()

	get_directory_property(_CXX_DEFINITIONS COMPILE_DEFINITIONS)

	if(_CXX_DEFINITIONS)
		list(REMOVE_DUPLICATES _CXX_DEFINITIONS)
		axl_message("    C/C++ definitions:" "${_CXX_DEFINITIONS}")
	endif()
endmacro()

#...............................................................................
