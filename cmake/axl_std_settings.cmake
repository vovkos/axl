#..............................................................................

macro (
axl_create_cpu_setting)

	axl_detect_cpu (_CPU)

	# Microsoft Visual C++ is a native-only generator, GCC can cross-compile

	if (MSVC)
		set (AXL_CPU ${_CPU})
	else ()
		axl_create_setting (
			AXL_CPU
			DESCRIPTION "Target CPU"
			DEFAULT ${_CPU}
			"x86" "amd64"
			)
	endif ()

	if ("${AXL_CPU}" STREQUAL "amd64")
		set (CMAKE_SIZEOF_VOID_P 8)

		set_property (GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS TRUE)
		set_property (GLOBAL PROPERTY FIND_LIBRARY_USE_LIB32_PATHS FALSE)

		if (GCC)
			set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64 -mcx16")
			set (CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -m64 -mcx16")
		endif ()
	elseif ("${AXL_CPU}" STREQUAL "x86")
		set (CMAKE_SIZEOF_VOID_P 4)

		set_property (GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS FALSE)
		set_property (GLOBAL PROPERTY FIND_LIBRARY_USE_LIB32_PATHS TRUE)

		if (GCC)
			set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
			set (CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -m32")
		endif ()
	endif ()
endmacro ()

macro (
axl_create_build_type_setting)

	set (
		CMAKE_CONFIGURATION_TYPES
		Debug
		Release
		CACHE STRING "List of supported configurations" FORCE
		)

	# Microsoft Visual C++ and Xcode are multi-configuration generator
	# otherwise, create a setting for choosing current configuration

	if (CMAKE_GENERATOR MATCHES "Visual Studio")
		set (CONFIGURATION_SUFFIX   "$(Configuration)")
		set (CONFIGURATION_SUFFIX_0)
		set (CMAKE_BUILD_TYPE)
	elseif (CMAKE_GENERATOR MATCHES "Xcode")
		set (CONFIGURATION_SUFFIX   "$(CONFIGURATION)")
		set (CONFIGURATION_SUFFIX_0)
		set (CMAKE_BUILD_TYPE)
	else ()
		axl_create_setting (
			CMAKE_BUILD_TYPE
			DESCRIPTION "Configuration type"
			DEFAULT Debug
			${CMAKE_CONFIGURATION_TYPES}
			)

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

macro (
axl_create_compiler_setting)

	axl_find_executable	(_CC      cc)
	axl_find_executable	(_CXX     c++)
	axl_find_executable	(_GCC     gcc)
	axl_find_executable	(_GXX     g++)
	axl_find_executable	(_CLANG   clang)
	axl_find_executable	(_CLANGXX clang++)

	axl_create_setting (
		AXL_C_COMPILER
		DESCRIPTION "C compiler"
		DEFAULT ${CMAKE_C_COMPILER}
		${_CC} ${_GCC} ${_CLANG}
		)

	axl_create_setting (
		AXL_CXX_COMPILER
		DESCRIPTION "C++ compiler"
		DEFAULT ${CMAKE_CXX_COMPILER}
		${_CXX} ${_GXX} ${_CLANGXX}
		)

	set (CMAKE_C_COMPILER ${AXL_C_COMPILER})
	set (CMAKE_CXX_COMPILER ${AXL_CXX_COMPILER})
endmacro ()

#..............................................................................

macro (
axl_create_msvc_settings)

	option (
		MSVC_USE_FOLDERS
		"Use solution folders in Microsoft Visual Studio"
		ON
		)

	set_property (
		GLOBAL PROPERTY
		USE_FOLDERS ${MSVC_USE_FOLDERS}
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

	if (${MSVC_FLAG_UNICODE})
		add_definitions (-DUNICODE -D_UNICODE)
	endif ()

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
endmacro ()

#..............................................................................

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
		GCC_FLAG_CPP_STANDARD
		DESCRIPTION "Specify the C++ standard to use"
		"-std=c++98" "-std=gnu++98" "-std=c++0x" "-std=gnu++0x"
		)

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
		GCC_FLAG_SANITIZER_DEBUG
		DESCRIPTION "Sanitizer to use in Debug build"
		DEFAULT "-fsanitize=address"
		"-fsanitize=address" "-fsanitize=memory" "-fsanitize=thread"
		)

	if (NOT "${GCC_FLAG_SANITIZER_DEBUG}" STREQUAL " " AND CMAKE_GENERATOR MATCHES "Xcode")

		# Xcode fires unresolved externals otherwise

		set (CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} ${GCC_FLAG_SANITIZER_DEBUG}")
		set (CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} ${GCC_FLAG_SANITIZER_DEBUG}")
	endif ()

	# linker flags

	axl_create_setting (
		GCC_FLAG_RDYNAMIC
		DESCRIPTION "Remove -rdynamic flag from default linker options"
		DEFAULT " "
		" " "-rdynamic"
		)

	axl_create_compiler_flag_regex (_REGEX "-rdynamic")
	axl_apply_compiler_flag (CMAKE_SHARED_LIBRARY_LINK_C_FLAGS ${_REGEX} ${GCC_FLAG_RDYNAMIC})
	axl_apply_compiler_flag (CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS ${_REGEX} ${GCC_FLAG_RDYNAMIC})

	# warnings -- certain warnings can only be disabled on command line

	axl_create_compiler_flag_setting (
		GCC_FLAG_WARNING_MULTICHAR
		DESCRIPTION "GNU C++ shows include stack during compilation"
		DEFAULT "-Wno-multichar"
		"-Wmultichar" "-Wno-multichar"
		)

endmacro ()

#..............................................................................

macro (
axl_create_std_settings)

	if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
		set (LINUX TRUE)
	endif ()

	if (CMAKE_COMPILER_IS_GNUCXX OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
		set (GCC TRUE)
	endif ()

	axl_create_cpu_setting ()
	axl_create_build_type_setting ()

	if (UNIX)
		axl_create_compiler_setting ()
	endif ()

	if (MSVC)
		axl_create_msvc_settings ()
	elseif (GCC)
		axl_create_gcc_settings ()
	endif ()
endmacro ()

macro (
axl_print_std_settings)

	message (STATUS "Target CPU:          ${AXL_CPU}")

	if (HAS_BUILD_TYPE_SETTING)
		message (STATUS "Build type:          ${CMAKE_BUILD_TYPE}")
	endif ()

	string (STRIP "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG}"   _CXX_FLAGS_DEBUG)
	string (STRIP "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE}" _CXX_FLAGS_RELEASE)

	message (STATUS "C Compiler:          ${CMAKE_C_COMPILER}")
	message (STATUS "C++ Compiler:        ${CMAKE_CXX_COMPILER}")
	message (STATUS "C++ flags (Debug):   ${_CXX_FLAGS_DEBUG}")
	message (STATUS "C++ flags (Release): ${_CXX_FLAGS_RELEASE}")

	get_directory_property (_CXX_DEFINITIONS COMPILE_DEFINITIONS)

	if (_CXX_DEFINITIONS)
		message (STATUS "C++ definitions:     ${_CXX_DEFINITIONS}")
	endif ()
endmacro ()

#..............................................................................
