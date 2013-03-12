#include "stdafx.h"
#include "axl_jnc_CastOp_Bool.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

bool
CCast_BoolFromZeroCmp::ConstCast (
	const CValue& OpValue,
	CType* pType,
	void* pDst
	)
{
	const char* p = (const char*) OpValue.GetConstData ();
	const char* pEnd = p + OpValue.GetType ()->GetSize ();
	
	bool Bool = false;

	for (; p < pEnd; p++)
	{
		if (*p)
		{
			Bool = true;
			break;
		}
	}

	*(bool*) pDst = Bool;
	return true;
}

bool
CCast_BoolFromZeroCmp::LlvmCast (
	EStorage StorageKind,
	const CValue& OpValue,
	CType* pType,
	CValue* pResultValue
	)
{
	CValue ZeroValue = OpValue.GetType ()->GetZeroValue ();
	return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_Ne, OpValue, ZeroValue, pResultValue);
}

//.............................................................................

bool
CCast_BoolFromPtr::LlvmCast (
	EStorage StorageKind,
	const CValue& OpValue,
	CType* pType,
	CValue* pResultValue
	)
{
	if (OpValue.GetType ()->GetSize () == sizeof (void*))
		return CCast_BoolFromZeroCmp::LlvmCast (StorageKind, OpValue, pType, pResultValue);

	CValue PtrValue;
	m_pModule->m_LlvmBuilder.CreateExtractValue (OpValue, 0, m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr), &PtrValue);
	return CCast_BoolFromZeroCmp::LlvmCast (StorageKind, PtrValue, pType, pResultValue);
}

//.............................................................................

ICastOperator*
CCast_Bool::GetCastOperator (
	const CValue& OpValue,
	CType* pType
	)
{
	EType SrcTypeKind = OpValue.GetType ()->GetTypeKind ();
	switch (SrcTypeKind)
	{
	case EType_Bool:
	case EType_Int8:
	case EType_Int8_u:
	case EType_Int16:
	case EType_Int16_u:
	case EType_Int32:
	case EType_Int32_u:
	case EType_Int64:
	case EType_Int64_u:
	case EType_Int16_be:
	case EType_Int16_beu:
	case EType_Int32_be:
	case EType_Int32_beu:
	case EType_Int64_be:
	case EType_Int64_beu:
	case EType_Float:
	case EType_Double:
	case EType_BitField:
	case EType_Enum:
		return &m_FromZeroCmp;

	case EType_DataPtr:
	case EType_ClassPtr:
	case EType_FunctionPtr:
	case EType_PropertyPtr:
		return &m_FromPtr;

	default:
		return NULL;
	}
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
