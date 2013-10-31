# This file is part of AXL (R) Library
# Tibbo Technology Inc (C) 2004-2013. All rights reserved
# Author: Vladimir Gladkov

project_virtual_folder (axl_jnc)

#..............................................................................
#
# callconv folder
#

set (
	CALLCONV_H_LIST
	${AXL_INC_DIR}/axl_jnc_CallConv.h
	${AXL_INC_DIR}/axl_jnc_CallConv_gcc32.h
	${AXL_INC_DIR}/axl_jnc_CallConv_msc.h
	${AXL_INC_DIR}/axl_jnc_CdeclCallConv_gcc64.h
	${AXL_INC_DIR}/axl_jnc_JnccallCallConv.h
	)

set (
	CALLCONV_CPP_LIST
	axl_jnc_CallConv.cpp
	axl_jnc_CallConv_gcc32.cpp
	axl_jnc_CallConv_msc.cpp
	axl_jnc_CdeclCallConv_gcc64.cpp
	axl_jnc_JnccallCallConv.cpp
	)

source_virtual_folder (
	callconv
	${CALLCONV_H_LIST}
	${CALLCONV_CPP_LIST}
	)

#..............................................................................
#
# controlflow folder
#

set (
	CONTROLFLOW_H_LIST
	${AXL_INC_DIR}/axl_jnc_BasicBlock.h
	${AXL_INC_DIR}/axl_jnc_ControlFlowMgr.h
	)

set (
	CONTROLFLOW_CPP_LIST
	axl_jnc_BasicBlock.cpp
	axl_jnc_ControlFlowMgr.cpp
	axl_jnc_ControlFlowMgr_Stmt.cpp
	)

source_virtual_folder (
	controlflow
	${CONTROLFLOW_H_LIST}
	${CONTROLFLOW_CPP_LIST}
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# function folder
#

set (
	FUNCTION_H_LIST
	${AXL_INC_DIR}/axl_jnc_Disassembler.h
	${AXL_INC_DIR}/axl_jnc_Function.h
	${AXL_INC_DIR}/axl_jnc_FunctionMgr.h
	${AXL_INC_DIR}/axl_jnc_Property.h
	${AXL_INC_DIR}/axl_jnc_PropertyTemplate.h
	${AXL_INC_DIR}/axl_jnc_PropertyVerifier.h
	${AXL_INC_DIR}/axl_jnc_ScheduleLauncherFunction.h
	${AXL_INC_DIR}/axl_jnc_ThunkFunction.h
	${AXL_INC_DIR}/axl_jnc_ThunkProperty.h
	axl_jnc_GcShadowStack.h
	)

set (
	FUNCTION_CPP_LIST
	axl_jnc_Disassembler.cpp
	axl_jnc_Function.cpp
	axl_jnc_FunctionMgr.cpp
	axl_jnc_Property.cpp
	axl_jnc_PropertyTemplate.cpp
	axl_jnc_PropertyVerifier.cpp
	axl_jnc_ScheduleLauncherFunction.cpp
	axl_jnc_ThunkFunction.cpp
	axl_jnc_ThunkProperty.cpp
	axl_jnc_GcShadowStack.cpp
	)

source_virtual_folder (
	function
	${FUNCTION_H_LIST}
	${FUNCTION_CPP_LIST}
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# module folder
#

set (
	MODULE_H_LIST
	${AXL_INC_DIR}/axl_jnc_AttributeBlock.h
	${AXL_INC_DIR}/axl_jnc_AttributeMgr.h
	${AXL_INC_DIR}/axl_jnc_LlvmDiBuilder.h
	${AXL_INC_DIR}/axl_jnc_LlvmIrBuilder.h
	${AXL_INC_DIR}/axl_jnc_Module.h
	${AXL_INC_DIR}/axl_jnc_ModuleItem.h
	${AXL_INC_DIR}/axl_jnc_Pch.h
	)

set (
	MODULE_CPP_LIST
	axl_jnc_AttributeBlock.cpp
	axl_jnc_AttributeMgr.cpp
	axl_jnc_LlvmDiBuilder.cpp
	axl_jnc_LlvmIrBuilder.cpp
	axl_jnc_Module.cpp
	axl_jnc_ModuleItem.cpp
	)

source_virtual_folder (
	module
	${MODULE_H_LIST}
	${MODULE_CPP_LIST}
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# namespace folder
#

set (
	NAMESPACE_H_LIST
	${AXL_INC_DIR}/axl_jnc_Namespace.h
	${AXL_INC_DIR}/axl_jnc_NamespaceMgr.h
	${AXL_INC_DIR}/axl_jnc_QualifiedName.h
	${AXL_INC_DIR}/axl_jnc_Scope.h
	)

set (
	NAMESPACE_CPP_LIST
	axl_jnc_Namespace.cpp
	axl_jnc_NamespaceMgr.cpp
	axl_jnc_QualifiedName.cpp
	axl_jnc_Scope.cpp
	)

source_virtual_folder (
	namespace
	${NAMESPACE_H_LIST}
	${NAMESPACE_CPP_LIST}
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# operator folder
#

set (
	OPERATOR_H_LIST
	${AXL_INC_DIR}/axl_jnc_BinOp.h
	${AXL_INC_DIR}/axl_jnc_BinOp_Arithmetic.h
	${AXL_INC_DIR}/axl_jnc_BinOp_Assign.h
	${AXL_INC_DIR}/axl_jnc_BinOp_At.h
	${AXL_INC_DIR}/axl_jnc_BinOp_Cmp.h
	${AXL_INC_DIR}/axl_jnc_BinOp_Idx.h
	${AXL_INC_DIR}/axl_jnc_BinOp_Logic.h
	${AXL_INC_DIR}/axl_jnc_CastOp.h
	${AXL_INC_DIR}/axl_jnc_CastOp_Array.h
	${AXL_INC_DIR}/axl_jnc_CastOp_Bool.h
	${AXL_INC_DIR}/axl_jnc_CastOp_ClassPtr.h
	${AXL_INC_DIR}/axl_jnc_CastOp_DataPtr.h
	${AXL_INC_DIR}/axl_jnc_CastOp_Fp.h
	${AXL_INC_DIR}/axl_jnc_CastOp_FunctionPtr.h
	${AXL_INC_DIR}/axl_jnc_CastOp_Int.h
	${AXL_INC_DIR}/axl_jnc_CastOp_PropertyPtr.h
	${AXL_INC_DIR}/axl_jnc_CastOp_Struct.h
	${AXL_INC_DIR}/axl_jnc_OperatorMgr.h
	${AXL_INC_DIR}/axl_jnc_UnOp.h
	${AXL_INC_DIR}/axl_jnc_UnOp_Arithmetic.h
	${AXL_INC_DIR}/axl_jnc_UnOp_Inc.h
	${AXL_INC_DIR}/axl_jnc_UnOp_LogNot.h
	${AXL_INC_DIR}/axl_jnc_UnOp_Ptr.h
	)

set (
	OPERATOR_CPP_LIST
	axl_jnc_BinOp.cpp
	axl_jnc_BinOp_Arithmetic.cpp
	axl_jnc_BinOp_Assign.cpp
	axl_jnc_BinOp_At.cpp
	axl_jnc_BinOp_Cmp.cpp
	axl_jnc_BinOp_Idx.cpp
	axl_jnc_BinOp_Logic.cpp
	axl_jnc_CastOp.cpp
	axl_jnc_CastOp_Array.cpp
	axl_jnc_CastOp_Bool.cpp
	axl_jnc_CastOp_ClassPtr.cpp
	axl_jnc_CastOp_DataPtr.cpp
	axl_jnc_CastOp_Fp.cpp
	axl_jnc_CastOp_FunctionPtr.cpp
	axl_jnc_CastOp_Int.cpp
	axl_jnc_CastOp_PropertyPtr.cpp
	axl_jnc_CastOp_Struct.cpp
	axl_jnc_OperatorMgr.cpp
	axl_jnc_OperatorMgr_Call.cpp
	axl_jnc_OperatorMgr_CheckPtr.cpp
	axl_jnc_OperatorMgr_Closure.cpp
	axl_jnc_OperatorMgr_DataRef.cpp
	axl_jnc_OperatorMgr_Field.cpp
	axl_jnc_OperatorMgr_Member.cpp
	axl_jnc_OperatorMgr_New.cpp
	axl_jnc_OperatorMgr_Property.cpp
	axl_jnc_OperatorMgr_Virtual.cpp
	axl_jnc_UnOp.cpp
	axl_jnc_UnOp_Arithmetic.cpp
	axl_jnc_UnOp_Inc.cpp
	axl_jnc_UnOp_LogNot.cpp
	axl_jnc_UnOp_Ptr.cpp
	)

source_virtual_folder (
	operator
	${OPERATOR_H_LIST}
	${OPERATOR_CPP_LIST}
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# parser folder
#

set (
	PARSER_H_LIST
	${AXL_INC_DIR}/axl_jnc_Lexer.h
	axl_jnc_Decl.h
	axl_jnc_DeclTypeCalc.h
	axl_jnc_Parser.llk.h
	)

set (
	PARSER_CPP_LIST
	axl_jnc_Decl.cpp
	axl_jnc_DeclTypeCalc.cpp
	axl_jnc_Lexer.rl.cpp
	axl_jnc_Parser.cpp
	)

set (
	PARSER_RL_LIST
	axl_jnc_Lexer.rl
	)

set (
	PARSER_LLK_LIST
	axl_jnc_Decl.llk
	axl_jnc_DeclarationSpecifier.llk
	axl_jnc_Declarator.llk
	axl_jnc_Expr.llk
	axl_jnc_Expr_s.llk
	axl_jnc_NamedTypeSpecifier.llk
	axl_jnc_Parser.llk
	axl_jnc_Stmt.llk
	axl_jnc_Stmt_0.llk
	)

push_and_set (CMAKE_CURRENT_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_ragel_step (
	axl_jnc_Lexer.rl.cpp
	axl_jnc_Lexer.rl
	)

add_till_step (
	axl_jnc_Parser.llk.h
	axl_jnc_Parser.llk
	"${AXL_PG_FRAME_DIR}/axl_llk_ParserFrame.h.luaf"

	# dependencies:

	axl_jnc_Decl.llk
	axl_jnc_DeclarationSpecifier.llk
	axl_jnc_Declarator.llk
	axl_jnc_Expr.llk
	axl_jnc_Expr_s.llk
	axl_jnc_NamedTypeSpecifier.llk
	axl_jnc_Stmt.llk
	axl_jnc_Stmt_0.llk
	)

pop (CMAKE_CURRENT_BINARY_DIR)

source_virtual_folder (
	parser
	${PARSER_H_LIST}
	${PARSER_CPP_LIST}
	${PARSER_RL_LIST}
	${PARSER_LLK_LIST}
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# type folder
#

set (
	TYPE_H_LIST
	${AXL_INC_DIR}/axl_jnc_ArrayType.h
	${AXL_INC_DIR}/axl_jnc_BitFieldType.h
	${AXL_INC_DIR}/axl_jnc_ClassPtrType.h
	${AXL_INC_DIR}/axl_jnc_ClassType.h
	${AXL_INC_DIR}/axl_jnc_ClosureClassType.h
	${AXL_INC_DIR}/axl_jnc_DataPtrType.h
	${AXL_INC_DIR}/axl_jnc_DerivableType.h
	${AXL_INC_DIR}/axl_jnc_EnumType.h
	${AXL_INC_DIR}/axl_jnc_FunctionArg.h
	${AXL_INC_DIR}/axl_jnc_FunctionPtrType.h
	${AXL_INC_DIR}/axl_jnc_FunctionType.h
	${AXL_INC_DIR}/axl_jnc_FunctionTypeOverload.h
	${AXL_INC_DIR}/axl_jnc_ImportType.h
	${AXL_INC_DIR}/axl_jnc_McSnapshotClassType.h
	${AXL_INC_DIR}/axl_jnc_MulticastClassType.h
	${AXL_INC_DIR}/axl_jnc_NamedType.h
	${AXL_INC_DIR}/axl_jnc_PropertyPtrType.h
	${AXL_INC_DIR}/axl_jnc_PropertyType.h
	${AXL_INC_DIR}/axl_jnc_ReactorClassType.h
	${AXL_INC_DIR}/axl_jnc_StructType.h
	${AXL_INC_DIR}/axl_jnc_Type.h
	${AXL_INC_DIR}/axl_jnc_TypeMgr.h
	${AXL_INC_DIR}/axl_jnc_UnionType.h
	)

set (
	TYPE_CPP_LIST
	axl_jnc_ArrayType.cpp
	axl_jnc_BitFieldType.cpp
	axl_jnc_ClassPtrType.cpp
	axl_jnc_ClassType.cpp
	axl_jnc_ClosureClassType.cpp
	axl_jnc_DataPtrType.cpp
	axl_jnc_DerivableType.cpp
	axl_jnc_EnumType.cpp
	axl_jnc_FunctionArg.cpp
	axl_jnc_FunctionPtrType.cpp
	axl_jnc_FunctionType.cpp
	axl_jnc_FunctionTypeOverload.cpp
	axl_jnc_ImportType.cpp
	axl_jnc_McSnapshotClassType.cpp
	axl_jnc_MulticastClassType.cpp
	axl_jnc_NamedType.cpp
	axl_jnc_PropertyPtrType.cpp
	axl_jnc_PropertyType.cpp
	axl_jnc_ReactorClassType.cpp
	axl_jnc_StructType.cpp
	axl_jnc_Type.cpp
	axl_jnc_TypeMgr.cpp
	axl_jnc_UnionType.cpp
	)

source_virtual_folder (
	type
	${TYPE_H_LIST}
	${TYPE_CPP_LIST}
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# value folder
#

set (
	VALUE_H_LIST
	${AXL_INC_DIR}/axl_jnc_Closure.h
	${AXL_INC_DIR}/axl_jnc_ConstMgr.h
	${AXL_INC_DIR}/axl_jnc_ThinDataPtrValidator.h
	${AXL_INC_DIR}/axl_jnc_Value.h
	)

set (
	VALUE_CPP_LIST
	axl_jnc_Closure.cpp
	axl_jnc_ConstMgr.cpp
	axl_jnc_ThinDataPtrValidator.cpp
	axl_jnc_Value.cpp
	)

source_virtual_folder (
	value
	${VALUE_H_LIST}
	${VALUE_CPP_LIST}
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# variable folder
#

set (
	VARIABLE_H_LIST
	${AXL_INC_DIR}/axl_jnc_Alias.h
	${AXL_INC_DIR}/axl_jnc_DestructList.h
	${AXL_INC_DIR}/axl_jnc_Variable.h
	${AXL_INC_DIR}/axl_jnc_VariableMgr.h
	)

set (
	VARIABLE_CPP_LIST
	axl_jnc_Alias.cpp
	axl_jnc_DestructList.cpp
	axl_jnc_Variable.cpp
	axl_jnc_VariableMgr.cpp
	)

source_virtual_folder (
	variable
	${VARIABLE_H_LIST}
	${VARIABLE_CPP_LIST}
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# runtime folder
#

set (
	RUNTIME_H_LIST
	${AXL_INC_DIR}/axl_jnc_Runtime.h
	${AXL_INC_DIR}/axl_jnc_StdLib.h
	${AXL_INC_DIR}/axl_jnc_TlsMgr.h
	axl_jnc_Multicast.h
	)

set (
	RUNTIME_CPP_LIST
	axl_jnc_Runtime.cpp
	axl_jnc_StdLib.cpp
	axl_jnc_TlsMgr.cpp
	axl_jnc_Multicast.cpp
	)

source_virtual_folder (
	runtime
	${RUNTIME_H_LIST}
	${RUNTIME_CPP_LIST}
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# pch folder
#

set (PCH_H   pch.h)
set (PCH_CPP pch.cpp)

source_virtual_folder (
	pch
	${PCH_H}
	${PCH_CPP}
	)

#..............................................................................
#
# axl_jnc library
#

include_directories (
	${PATH_LLVM_INC_DIR}
	${PATH_LLVM_INC_DIR_2}
	)

add_library (
	axl_jnc
	${CALLCONV_H_LIST}
	${CALLCONV_CPP_LIST}
	${CONTROLFLOW_H_LIST}
	${CONTROLFLOW_CPP_LIST}
	${FUNCTION_H_LIST}
	${FUNCTION_CPP_LIST}
	${MODULE_H_LIST}
	${MODULE_CPP_LIST}
	${NAMESPACE_H_LIST}
	${NAMESPACE_CPP_LIST}
	${OPERATOR_H_LIST}
	${OPERATOR_CPP_LIST}
	${PARSER_H_LIST}
	${PARSER_CPP_LIST}
	${PARSER_RL_LIST}
	${PARSER_LLK_LIST}
	${TYPE_H_LIST}
	${TYPE_CPP_LIST}
	${VALUE_H_LIST}
	${VALUE_CPP_LIST}
	${VARIABLE_H_LIST}
	${VARIABLE_CPP_LIST}
	${RUNTIME_H_LIST}
	${RUNTIME_CPP_LIST}
	${PCH_H}
	${PCH_CPP}
	)

if (AXL_USE_PCH)
	set_pch (
		axl_jnc
		${PCH_H}
		${PCH_CPP}
		)
endif ()

#..............................................................................

