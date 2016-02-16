#..............................................................................

if ("${LLVM_INC_DIR}" STREQUAL "")
	set (LLVM_FOUND FALSE)
	message (STATUS "LLVM:                       <not-found>")
else ()
	include (${LLVM_CMAKE_DIR}/LLVMConfig.cmake)

	set (LLVM_FOUND TRUE)
	message (STATUS "Path to LLVM cmake modules: ${LLVM_CMAKE_DIR}")
	message (STATUS "Path to LLVM includes:      ${LLVM_INC_DIR}")
	message (STATUS "Path to LLVM includes (2):  ${LLVM_INC_DIR_2}")
	message (STATUS "Path to LLVM libraries:     ${LLVM_LIB_DIR}")
endif ()

#..............................................................................
