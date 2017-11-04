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

macro (
axl_create_target_cpu_setting)

	axl_detect_target_cpu (_CPU)

	# Microsoft Visual C++ is a native-only generator, x86/amd64 GCC can cross-compile

	if (NOT GCC OR NOT ("${_CPU}" STREQUAL "x86" OR "${_CPU}" STREQUAL "amd64"))
		set (TARGET_CPU ${_CPU})
	else ()
		axl_create_setting (
			TARGET_CPU
			DESCRIPTION "Target CPU"
			DEFAULT ${_CPU}
			"x86"
			"amd64"
			)
	endif ()
endmacro ()

macro (
axl_apply_target_cpu_setting)

	if ("${TARGET_CPU}" STREQUAL "amd64")
		set (CMAKE_SIZEOF_VOID_P 8)

		set_property (GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS TRUE)
		set_property (GLOBAL PROPERTY FIND_LIBRARY_USE_LIB32_PATHS FALSE)

		if (GCC)
			set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64 -mcx16")
			set (CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -m64 -mcx16")
		endif ()
	elseif ("${TARGET_CPU}" STREQUAL "x86")
		set (CMAKE_SIZEOF_VOID_P 4)

		set_property (GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS FALSE)
		set_property (GLOBAL PROPERTY FIND_LIBRARY_USE_LIB32_PATHS TRUE)

		if (GCC)
			set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
			set (CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -m32")
		endif ()
	elseif (NOT "${TARGET_CPU}" MATCHES "arm")
		message (FATAL_ERROR "Unsupported CPU: ${TARGET_CPU}")
	endif ()

	set (AXL_CPU ${TARGET_CPU}) # for backward compatibility (in paths.cmake)
endmacro ()

#...............................................................................

macro (
axl_create_build_type_setting)

	set (
		CMAKE_CONFIGURATION_TYPES
		"Debug"
		"Release"
		CACHE STRING "List of supported configurations" FORCE
		)

	# Microsoft Visual C++ and Xcode are multi-configuration generators

	if (NOT CMAKE_GENERATOR MATCHES "Visual Studio" AND
		NOT CMAKE_GENERATOR MATCHES "Xcode")
		axl_create_setting (
			CMAKE_BUILD_TYPE
			DESCRIPTION "Build configuration"
			DEFAULT Release
			${CMAKE_CONFIGURATION_TYPES}
			)
	endif ()
endmacro ()

macro (
axl_apply_build_type_setting)

	# Microsoft Visual C++ and Xcode are multi-configuration generators

	if (CMAKE_GENERATOR MATCHES "Visual Studio")
		set (CONFIGURATION_SUFFIX   "$(Configuration)")
		set (CONFIGURATION_SUFFIX_0)
	elseif (CMAKE_GENERATOR MATCHES "Xcode")
		set (CONFIGURATION_SUFFIX   "$(CONFIGURATION)")
		set (CONFIGURATION_SUFFIX_0)
	else ()
		set (CONFIGURATION_SUFFIX   "${CMAKE_BUILD_TYPE}")
		set (CONFIGURATION_SUFFIX_0 "${CMAKE_BUILD_TYPE}")

		if (UNIX)
			if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
				set (CMAKE_SKIP_RPATH TRUE)

				if (APPLE)
					set (
						CMAKE_EXE_LINKER_FLAGS
						"${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath,'@loader_path/../Frameworks'"
						)
				endif ()
			endif ()
		endif ()
	endif ()
endmacro ()

#...............................................................................

macro (
axl_create_msvc_settings)

	option (
		MSVC_USE_FOLDERS
		"Use solution folders in Microsoft Visual Studio"
		ON
		)

	option (
		MSVC_USE_PCH
		"Use precompiled headers in Microsoft Visual C/C++"
		ON
		)

	option (
		MSVC_USE_UNICODE
		"Compile for UNICODE rather than multibyte"
		ON
		)

	axl_create_compiler_flag_setting (
		MSVC_FLAG_SHOW_INCLUDES
		DESCRIPTION "Microsoft Visual C++ shows includes stack during compilation"
		"/showIncludes"
		)

	axl_create_compiler_flag_setting (
		MSVC_FLAG_EH
		DESCRIPTION "Microsoft Visual C++ EH (exception handling) model"
		"/EHsc" "/EHs-c-"
		)

	axl_create_compiler_flag_setting (
		MSVC_FLAG_RTTI
		DESCRIPTION "Microsoft Visual C++ RTTI (run-time type information) support"
		"/GR" "/GR-"
		)

	axl_create_compiler_flag_setting (
		MSVC_FLAG_CRT_DEBUG
		DESCRIPTION "Microsoft Visual C++ CRT (Debug)"
		DEFAULT "/MTd"
		"/MT" "/MTd" "/MD" "/MDd"
		)

	axl_create_compiler_flag_setting (
		MSVC_FLAG_CRT_RELEASE
		DESCRIPTION "Microsoft Visual C++ CRT (Release)"
		DEFAULT "/MT"
		"/MT" "/MTd" "/MD" "/MDd"
		)

	axl_create_compiler_flag_setting (
		MSVC_FLAG_DEBUG_INFO_DEBUG
		DESCRIPTION "Microsoft Visual C++ debug information format (Debug)"
		DEFAULT "/Zi"
		"/Z7" "/Zi" "/ZI"
		)

	axl_create_compiler_flag_setting (
		MSVC_FLAG_DEBUG_INFO_RELEASE
		DESCRIPTION "Microsoft Visual C++ debug information format (Release)"
		DEFAULT "/Zi"
		"/Z7" "/Zi" "/ZI"
		)

	axl_create_setting (
		MSVC_LINK_FLAG_IGNORE4221
		DESCRIPTION "Ignore linker warning LNK4221 (object file doesn't define new symbols)"
		DEFAULT "/IGNORE:4221"
		" " "/IGNORE:4221"
		)
endmacro ()

macro (
axl_apply_msvc_settings)

	set_property (
		GLOBAL PROPERTY
		USE_FOLDERS ${MSVC_USE_FOLDERS}
		)

	if (${MSVC_USE_UNICODE})
		add_definitions (-DUNICODE -D_UNICODE)
	endif ()

	axl_create_compiler_flag_setting_regex (_REGEX MSVC_LINK_FLAG_IGNORE4221)
	axl_apply_compiler_flag (CMAKE_EXE_LINKER_FLAGS ${_REGEX} ${MSVC_LINK_FLAG_IGNORE4221})
	axl_apply_compiler_flag (CMAKE_SHARED_LINKER_FLAGS ${_REGEX} ${MSVC_LINK_FLAG_IGNORE4221})
	axl_apply_compiler_flag (CMAKE_STATIC_LINKER_FLAGS ${_REGEX} ${MSVC_LINK_FLAG_IGNORE4221})
endmacro ()

#...............................................................................

macro (
axl_create_gcc_settings)

	option (
		GCC_USE_PCH
		"Use precompiled headers in GNU C/C++"
		ON
		)

	axl_create_compiler_flag_setting (
		GCC_FLAG_SHOW_INCLUDES
		DESCRIPTION "GNU C++ shows include stack during compilation"
		"-H"
		)

	axl_create_compiler_flag_setting (
		GCC_FLAG_C_STANDARD
		DESCRIPTION "Specify the C standard to use"
		"-std=c90" "-std=c99" "-std=c11" "-std=gnu90" "-std=gnu99" "-std=gnu11"
		)

	axl_create_compiler_flag_setting (
		GCC_FLAG_CPP_STANDARD
		DESCRIPTION "Specify the C++ standard to use"
		"-std=c++98" "-std=c++11" "-std=c++14" "-std=gnu++98" "-std=gnu++11" "-std=gnu++14"
		)

	if (NOT CLANG)
		axl_create_compiler_flag_setting (
			GCC_FLAG_STATIC_LIBGCC
			DESCRIPTION "Use static linkage to libgcc"
			DEFAULT "-static-libgcc"
			"-static-libgcc"
			)

		axl_create_compiler_flag_setting (
			GCC_FLAG_CPP_STATIC_LIBSTDCPP
			DESCRIPTION "Use static linkage to libstdc++"
			DEFAULT "-static-libstdc++"
			"-static-libstdc++"
			)
	endif ()

	axl_create_compiler_flag_setting (
		GCC_FLAG_CPP_RTTI
		DESCRIPTION "GNU C++ emits RTTI info"
		"-frtti" "-fno-rtti"
		)

	axl_create_compiler_flag_setting (
		GCC_FLAG_PIC
		DESCRIPTION "GNU C++ generates position independent code"
		DEFAULT "-fPIC"
		"-fpic" "-fPIC" "-fpie" "-fPIE"
		)

	axl_create_compiler_flag_setting (
		GCC_FLAG_VISIBILITY
		DESCRIPTION "GNU C++ doesn't export all symbols by default (-fvisibility=hidden)"
		DEFAULT "-fvisibility=hidden"
		"-fvisibility=default" "-fvisibility=hidden"
		)

	axl_create_compiler_flag_setting (
		GCC_FLAG_SANITIZE
		DESCRIPTION "Compile with a specific sanitizer instrumentation"
		"-fsanitize=address" "-fsanitize=memory" "-fsanitize=thread"
		)

	axl_create_compiler_flag_setting (
		GCC_FLAG_COVERAGE
		DESCRIPTION "Compile with code coverage instrumentation"
		"-coverage"
		)

	axl_create_setting (
		GCC_LINK_FLAG_RDYNAMIC
		DESCRIPTION "Remove -rdynamic flag from default linker options"
		DEFAULT " "
		" " "-rdynamic"
		)

	axl_create_compiler_flag_setting (
		GCC_FLAG_WARNING_MULTICHAR
		DESCRIPTION "GNU C++ shows warnings if multicharacter constants are used, e.g. 'ABCD'"
		DEFAULT "-Wno-multichar"
		"-Wmultichar" "-Wno-multichar"
		)

	axl_create_compiler_flag_setting (
		GCC_FLAG_WARNING_FORMAT
		DESCRIPTION "GNU C++ shows warnings if printf format specifiers mismatch arguments"
		DEFAULT "-Wno-format"
		"-Wformat" "-Wno-format"
		)
endmacro ()

macro (
axl_apply_gcc_settings)

	if (NOT "${GCC_FLAG_SANITIZER_DEBUG}" STREQUAL " " AND CMAKE_GENERATOR MATCHES "Xcode")

		# Xcode fires unresolved externals otherwise

		set (CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} ${GCC_FLAG_SANITIZER_DEBUG}")
		set (CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} ${GCC_FLAG_SANITIZER_DEBUG}")
	endif ()

	axl_create_compiler_flag_setting_regex (_REGEX GCC_LINK_FLAG_RDYNAMIC)
	axl_apply_compiler_flag (CMAKE_SHARED_LIBRARY_LINK_C_FLAGS ${_REGEX} ${GCC_LINK_FLAG_RDYNAMIC})
	axl_apply_compiler_flag (CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS ${_REGEX} ${GCC_LINK_FLAG_RDYNAMIC})
endmacro ()

#...............................................................................

macro (
axl_create_std_settings)

	if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
		set (LINUX TRUE)
	endif ()

	if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
		set (CLANG TRUE)
		set (GCC TRUE)
	elseif (CMAKE_COMPILER_IS_GNUCXX)
		set (GCC TRUE)
	endif ()

	axl_create_target_cpu_setting ()
	axl_create_build_type_setting ()

	if (MSVC)
		axl_create_msvc_settings ()
	elseif (GCC)
		axl_create_gcc_settings ()
	endif ()
endmacro ()

macro (
axl_apply_std_settings)

	axl_apply_target_cpu_setting ()
	axl_apply_build_type_setting ()

	if (MSVC)
		axl_apply_msvc_settings ()
	elseif (GCC)
		axl_apply_gcc_settings ()
	endif ()

	axl_apply_all_compiler_flag_settings ()
endmacro ()

macro (
axl_print_std_settings)

	string (LENGTH ".......................:" AXL_G_MESSAGE_ALIGN)

	message (STATUS "AXL CMake:")
	axl_message ("    Invoked from:" ${AXL_CMAKE_ORIGIN_FILE})
	axl_message ("    dependencies.cmake:" ${AXL_CMAKE_ORIGIN_DIR}/dependencies.cmake)

	if (AXL_SETTINGS_CMAKE)
		axl_message ("    settings.cmake:" ${AXL_SETTINGS_CMAKE})
	endif ()

	if (AXL_PATHS_CMAKE)
		axl_message ("    paths.cmake:" ${AXL_PATHS_CMAKE})
	endif ()

	axl_message ("    Target CPU:" ${TARGET_CPU})

	if (NOT "${CMAKE_BUILD_TYPE}" STREQUAL "")
		axl_message ("    Build configuration:" ${CMAKE_BUILD_TYPE})
	endif ()

	string (STRIP "${CMAKE_C_COMPILER_ARG1} ${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_DEBUG}" _C_FLAGS_DEBUG)
	string (STRIP "${CMAKE_C_COMPILER_ARG1} ${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_RELEASE}" _C_FLAGS_RELEASE)
	string (STRIP "${CMAKE_CXX_COMPILER_ARG1} ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG}" _CXX_FLAGS_DEBUG)
	string (STRIP "${CMAKE_CXX_COMPILER_ARG1} ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE}" _CXX_FLAGS_RELEASE)

	message (STATUS "C/C++:")
	axl_message ("    C Compiler:"          ${CMAKE_C_COMPILER})
	axl_message ("    C flags (Debug):"     ${_C_FLAGS_DEBUG})
	axl_message ("    C flags (Release):"   ${_C_FLAGS_RELEASE})
	axl_message ("    C++ Compiler:"        ${CMAKE_CXX_COMPILER})
	axl_message ("    C++ flags (Debug):"   ${_CXX_FLAGS_DEBUG})
	axl_message ("    C++ flags (Release):" ${_CXX_FLAGS_RELEASE})

	get_directory_property (_CXX_DEFINITIONS COMPILE_DEFINITIONS)

	if (_CXX_DEFINITIONS)
		list (REMOVE_DUPLICATES _CXX_DEFINITIONS)
		axl_message ("    C/C++ definitions:" "${_CXX_DEFINITIONS}")
	endif ()
endmacro ()

#...............................................................................
