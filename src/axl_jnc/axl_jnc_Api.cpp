#include "pch.h"
#include "axl_jnc_Api.h"

namespace axl {
namespace jnc {

//.............................................................................

void
Prime (
	TInterface* pThis,
	CClassType* pClassType	
	)
{
	pThis->m_pObject->m_pType = pClassType;
	pThis->m_pVTable = NULL; // <-- currently i dont have vtable-dependent API's
	
	// prime all the base types
	
	rtl::CArrayT <CBaseTypeSlot*> BaseTypePrimeArray = pClassType->GetBaseTypePrimeArray ();
	size_t Count = BaseTypePrimeArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CBaseTypeSlot* pSlot = BaseTypePrimeArray [i];
		ASSERT (pSlot->GetType ()->GetTypeKind () == EType_Class);

		jnc::TInterface* pInterface = (jnc::TInterface*) ((char*) pThis + pSlot->GetOffset ());
		pInterface->m_pObject = pThis->m_pObject;
		
		Prime (pInterface, (CClassType*) pSlot->GetType ());
	}
	
	// prime all the class fields

	rtl::CArrayT <CStructField*> FieldPrimeArray = pClassType->GetClassMemberFieldArray ();
	Count = FieldPrimeArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CStructField* pField = FieldPrimeArray [i];
		ASSERT (pField->GetType ()->GetTypeKind () == EType_Class);
		
		jnc::TObject* pObject = (jnc::TObject*) ((char*) pThis + pField->GetOffset ());
		jnc::TInterface* pInterface = (jnc::TInterface*) (pObject + 1);
		pInterface->m_pObject = pObject;
		
		Prime (pInterface, (CClassType*) pField->GetType ());
	}
}

//.............................................................................
	
} // namespace jnc {
} // namespace axl {
