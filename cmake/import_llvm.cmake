#..............................................................................

if (EXISTS ${LLVM_CMAKE_DIR}/LLVMConfig.cmake)
	include (${LLVM_CMAKE_DIR}/LLVMConfig.cmake)

	message (STATUS "Path defintions for LLVM")
	message (STATUS "    LLVM cmake files: ${LLVM_CMAKE_DIR}")
	message (STATUS "    LLVM includes:    ${LLVM_INC_DIR}")
	message (STATUS "    LLVM libraries:   ${LLVM_LIB_DIR}")

	set (LLVM_FOUND TRUE)
else ()
	set (LLVM_FOUND FALSE)
endif ()

#..............................................................................
