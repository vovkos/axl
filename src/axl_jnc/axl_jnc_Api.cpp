#include "pch.h"
#include "axl_jnc_Api.h"

namespace axl {
namespace jnc {

//.............................................................................

void
Prime (
	TInterface* pThis,
	CClassType* pType,
	void* pVTable
	)
{
	ASSERT (pThis->m_pObject);

	pThis->m_pObject->m_pType = pType;
	pThis->m_pVTable = pVTable;

	// prime all the base types

	rtl::CArrayT <CBaseTypeSlot*> BaseTypePrimeArray = pType->GetBaseTypePrimeArray ();
	size_t Count = BaseTypePrimeArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CBaseTypeSlot* pSlot = BaseTypePrimeArray [i];
		ASSERT (pSlot->GetType ()->GetTypeKind () == EType_Class);

		jnc::TInterface* pInterface = (jnc::TInterface*) ((char*) pThis + pSlot->GetOffset ());
		pInterface->m_pObject = pThis->m_pObject;

		Prime (
			pInterface,
			(CClassType*) pSlot->GetType (),
			(void**) pVTable + pSlot->GetVTableIndex ()
			);
	}

	// prime all the class fields

	rtl::CArrayT <CStructField*> FieldPrimeArray = pType->GetClassMemberFieldArray ();
	Count = FieldPrimeArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CStructField* pField = FieldPrimeArray [i];
		ASSERT (pField->GetType ()->GetTypeKind () == EType_Class);
		CClassType* pFieldType = (CClassType*) pField->GetType ();

		jnc::TObject* pObject = (jnc::TObject*) ((char*) pThis + pField->GetOffset ());
		jnc::TInterface* pInterface = (jnc::TInterface*) (pObject + 1);
		pInterface->m_pObject = pObject;

		Prime (pInterface, pFieldType, NULL);
	}
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
