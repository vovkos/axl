#include "stdafx.h"
#include "axl_jnc_UnOp_Indir.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

bool
CUnOp_Indir::Operator (
	const CValue& OpValue,
	CValue* pResultValue
	)
{
	CType* pResultType;

	CType* pOpType = OpValue.GetType ();
	EType OpTypeKind = pOpType->GetTypeKind ();
	switch (OpTypeKind)
	{
	case EType_DataPtr:
		pResultType = ((CDataPtrType*) pOpType)->GetDataType ()->GetDataPtrType (
			EType_DataRef, 
			((CDataPtrType*) pOpType)->GetPtrTypeKind (),
			pOpType->GetFlags ()
			);
		break;

	case EType_FunctionPtr:
		pResultType = ((CFunctionPtrType*) pOpType)->GetFunctionType ()->GetFunctionPtrType (
			EType_FunctionRef, 
			((CFunctionPtrType*) pOpType)->GetPtrTypeKind (),
			pOpType->GetFlags ()
			);
		break;

	case EType_PropertyPtr:
		pResultType = ((CPropertyPtrType*) pOpType)->GetPropertyType ()->GetPropertyPtrType (
			EType_PropertyRef, 
			((CPropertyPtrType*) pOpType)->GetPtrTypeKind (),
			pOpType->GetFlags ()
			);
		break;

	default:
		err::SetFormatStringError (_T("can only apply unary '*' to a pointer"));
		return false;
	}

	pResultValue->OverrideType (OpValue, pResultType);
	return true;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
