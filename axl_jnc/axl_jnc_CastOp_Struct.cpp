#include "stdafx.h"
#include "axl_jnc_CastOp_Struct.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

ECast
CCast_Struct::GetCastKind (
	const CValue& OpValue,
	CType* pType
	)
{
	ASSERT (OpValue.GetType ()->GetTypeKind () == EType_Struct);
	CStructType* pStructType = (CStructType*) OpValue.GetType ();
	return pStructType->FindBaseType (pType) ? ECast_Implicit : ECast_None;
}

bool
CCast_Struct::ConstCast (
	const CValue& OpValue,
	CType* pType,
	void* pDst
	)
{
	ASSERT (OpValue.GetType ()->GetTypeKind () == EType_Struct);
	CStructType* pStructType = (CStructType*) OpValue.GetType ();

	CBaseTypeCoord Coord;
	bool Result = pStructType->FindBaseType (pType, &Coord);
	if (!Result)
	{
		SetCastError (OpValue, pType);
		return false;
	}
	
	memcpy (pDst, (char*) OpValue.GetConstData () + Coord.m_Offset, pType->GetSize ());
	return true;
}

bool
CCast_Struct::LlvmCast (
	EAlloc AllocKind,
	const CValue& OpValue,
	CType* pType,
	CValue* pResultValue
	)
{
	ASSERT (OpValue.GetType ()->GetTypeKind () == EType_Struct);
	CStructType* pStructType = (CStructType*) OpValue.GetType ();

	CBaseTypeCoord Coord;
	bool Result = pStructType->FindBaseType (pType, &Coord);
	if (!Result)
	{
		SetCastError (OpValue, pType);
		return false;
	}

	m_pModule->m_LlvmBuilder.CreateExtractValue (
		OpValue, 
		Coord.m_LlvmIndexArray, 
		Coord.m_LlvmIndexArray.GetCount (), 
		pType, 
		pResultValue
		);

	return true;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
