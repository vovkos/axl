#..............................................................................

if (EXISTS ${LLVM_CMAKE_DIR}/LLVMConfig.cmake)
	include (${LLVM_CMAKE_DIR}/LLVMConfig.cmake)

	if (NOT LLVM_INC_DIR)
		set (LLVM_INC_DIR ${LLVM_INCLUDE_DIRS})
	endif ()

	if (NOT LLVM_LIB_DIR)
		set (LLVM_LIB_DIR ${LLVM_LIBRARY_DIRS})
	endif ()

	message (STATUS "LLVM paths:")
	message (STATUS "    LLVM cmake files: ${LLVM_CMAKE_DIR}")
	message (STATUS "    LLVM includes:    ${LLVM_INC_DIR}")
	message (STATUS "    LLVM libraries:   ${LLVM_LIB_DIR}")

	set (LLVM_FOUND TRUE)
else ()
	set (LLVM_FOUND FALSE)
endif ()

#..............................................................................
