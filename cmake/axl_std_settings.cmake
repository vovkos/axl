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
			${_CPU}
			STRING
			"Target CPU"
			"x86" "amd64"
			)
	endif ()

	if (GCC)
		if ("${AXL_CPU}" STREQUAL "amd64")
			set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64 -mcx16")
			set (CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -m64 -mcx16")
		elseif ("${AXL_CPU}" STREQUAL "x86")
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
		set (CONFIGURATION_SUFFIX_0 "")
	elseif (CMAKE_GENERATOR MATCHES "Xcode")
		set (CONFIGURATION_SUFFIX   "$(CONFIGURATION)")
		set (CONFIGURATION_SUFFIX_0 "")
	else ()
		axl_create_setting (
			CMAKE_BUILD_TYPE
			Debug
			STRING
			"Configuration type"
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

#..............................................................................

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

	set_property (
		GLOBAL PROPERTY
		USE_FOLDERS ${MSVC_USE_FOLDERS}
		)

	axl_create_c_cxx_flag_setting (
		MSVC_FLAG_EH
		"Microsoft Visual C++ EH (exception handling) model"
		"" # -- use default
		"/EHsc" "/EHs-c-"
		)

	axl_create_c_cxx_flag_setting (
		MSVC_FLAG_RTTI
		"Microsoft Visual C++ RTTI (run-time type information) support"
		"/GR-"
		"/GR" "/GR-"
		)

	axl_create_c_cxx_flag_setting (
		MSVC_FLAG_CRT_DEBUG
		"Microsoft Visual C++ CRT (Debug)"
		"/MTd"
		"/MT" "/MTd" "/MD" "/MDd"
		)

	axl_create_c_cxx_flag_setting (
		MSVC_FLAG_CRT_RELEASE
		"Microsoft Visual C++ CRT (Release)"
		"/MT"
		"/MT" "/MTd" "/MD" "/MDd"
		)

	axl_create_c_cxx_flag_setting (
		MSVC_FLAG_DEBUG_INFO_DEBUG
		"Microsoft Visual C++ debug information format (Debug)"
		"/Zi"
		"/Z7" "/Zi" "/ZI"
		)

	axl_create_c_cxx_flag_setting (
		MSVC_FLAG_DEBUG_INFO_RELEASE
		"Microsoft Visual C++ debug information format (Release)"
		"/Zi"
		"/Z7" "/Zi" "/ZI"
		)

	option (
		MSVC_FLAG_UNICODE
		"Compile for UNICODE rather than multibyte"
		ON
		)

	if (${MSVC_FLAG_UNICODE})
		add_definitions (-DUNICODE -D_UNICODE)
	endif ()
endmacro ()

#..............................................................................

macro (
axl_create_gcc_settings)

	option (
		GCC_SUPPRESS_RDYNAMIC
		"Remove -rdynamic flag from default linker options"
		ON
		)

	if (GCC_SUPPRESS_RDYNAMIC)
		string (
			REPLACE
			"-rdynamic" ""
			CMAKE_SHARED_LIBRARY_LINK_C_FLAGS
			"${CMAKE_SHARED_LIBRARY_LINK_C_FLAGS}"
			)

		string (
			REPLACE
			"-rdynamic" ""
			CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS
			"${CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS}"
			)
	endif ()

	if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
		unset (GCC_USE_PCH CACHE) # axl pch does not work on clang yet
	else ()
		option (
			GCC_USE_PCH
			"Use precompiled headers in GNU C/C++"
			ON
			)
	endif ()

	axl_create_c_cxx_flag_setting (
		GCC_FLAG_SHOW_INCLUDES
		"GNU C++ shows include stack during compilation"
		" "
		"-H"
		)

	axl_create_c_cxx_flag_setting (
		GCC_FLAG_CPP_RTTI
		"GNU C++ emits RTTI info"
		"-fno-rtti"
		"-frtti" "-fno-rtti"
		)

	axl_create_c_cxx_flag_setting (
		GCC_FLAG_PIC
		"GNU C++ generates position independent code"
		"-fPIC"
		"-fpic" "-fPIC" "-fpie" "-fPIE"
		)

	axl_create_c_cxx_flag_setting (
		GCC_FLAG_VISIBILITY
		"GNU C++ doesn't export all symbols by default (-fvisibility=hidden)"
		"-fvisibility=hidden"
		"-fvisibility=default" "-fvisibility=hidden"
		)

	axl_create_c_cxx_flag_setting (
		GCC_FLAG_WARNING_FORMAT
		"GNU C++ shows warnings if format specifier is incorrect"
		"-Wno-format"
		"-Wno-format" "-Wformat"
		)

	axl_create_c_cxx_flag_setting (
		GCC_FLAG_WARNING_MULTICHAR
		"GNU C++ shows warnings if multichar literal constant (e.g. 'ab12') is used"
		"-Wno-multichar"
		"-Wmultichar" "-Wno-multichar"
		)

	axl_create_c_cxx_flag_setting (
		GCC_FLAG_WARNING_DEPRECATED
		"GNU C++ shows warnings on depreceted declarations"
		"-Wno-deprecated-declarations"
		"-Wdeprecated-declarations" "-Wno-deprecated-declarations"
		)

	axl_create_c_cxx_flag_setting (
		GCC_FLAG_CPP_WARNING_INVALID_OFFSET
		"GNU C++ shows warnings on invalid-offset declarations"
		"-Wno-invalid-offsetof"
		"-Winvalid-offsetof" "-Wno-invalid-offsetof"
		)

	axl_create_c_cxx_flag_setting (
		GCC_FLAG_WARNING_UNUSED_RESULT
		"GNU C++ shows warnings on unused result"
		"-Wno-unused-result"
		"-Wunused-result" "-Wno-unused-result"
		)

	axl_create_c_cxx_flag_setting (
		GCC_FLAG_WARNING_UNUSED_VARIABLE
		"GNU C++ shows warnings on unused variables"
		"-Wno-unused-variable"
		"-Wunused-variable" "-Wno-unused-variable"
		)

	axl_create_c_cxx_flag_setting (
		GCC_FLAG_WARNING_SWITCH
		"GNU C++ shows warnings on missing enum member(s) in switch"
		"-Wno-switch"
		"-Wswitch" "-Wno-switch"
		)

	axl_create_c_cxx_flag_setting (
		GCC_FLAG_WARNING_MISSING_BRACES
		"GNU C++ shows warnings on missing braces in curly initializers"
		"-Wno-missing-braces"
		"-Wmissing-braces" "-Wno-missing-braces"
		)

	axl_create_c_cxx_flag_setting (
		GCC_FLAG_WARNING_NARROWING
		"GNU C++ produces warnings/errors on narrowing conversions"
		" "
		"-Wnarrowing" "-Wno-narrowing"
		)

	axl_create_c_cxx_flag_setting (
		GCC_FLAG_INCOMPATIBLE_MS_STRUCT
		"GNU C++ shows warnings on potentially MSVC-incompatible structs"
		"-Wno-incompatible-ms-struct"
		"-Wincompatible-ms-struct" "-Wno-incompatible-ms-struct"
		)

	axl_create_c_cxx_flag_setting (
		GCC_FLAG_CPP_STANDARD
		"Specify the C++ standard to use"
		"-std=gnu++98"
		"-std=c++98" "-std=gnu++98" "-std=c++0x" "-std=gnu++0x"
		)

	axl_create_setting (
		GCC_FLAG_SANITIZER
		" "
		STRING
		"Sanitizer to use in Debug build"
		" " "-fsanitize=address" "-fsanitize=memory" "-fsanitize=thread"
		)

	if (NOT "${GCC_FLAG_SANITIZER}" STREQUAL " ")
		if (CMAKE_GENERATOR MATCHES "Xcode")
			# Xcode fires unresolved externals otherwise
			set (CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} ${GCC_FLAG_SANITIZER}")
			set (CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} ${GCC_FLAG_SANITIZER}")
		endif ()

		set (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${GCC_FLAG_SANITIZER}")
		set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${GCC_FLAG_SANITIZER}")
	endif ()
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

	message (STATUS "C++ flags (Debug):   ${_CXX_FLAGS_DEBUG}")
	message (STATUS "C++ flags (Release): ${_CXX_FLAGS_RELEASE}")

	get_directory_property (_CXX_DEFINITIONS COMPILE_DEFINITIONS)

	if (_CXX_DEFINITIONS)
		message (STATUS "C++ definitions:     ${_CXX_DEFINITIONS}")
	endif ()
endmacro ()

#..............................................................................
