#include "pch.h"
#include "axl_jnc_Api.h"

namespace axl {
namespace jnc {

//.............................................................................

void
Prime (
	TIfaceHdr* pThis,
	CClassType* pType,
	void* pVTable,
	TObjHdr* pRoot
	)
{
	ASSERT (pThis->m_pObject);

	if (!pThis->m_pObject->m_pType)
		pThis->m_pObject->m_pType = pType;

	if (!pRoot)
		pRoot = pThis->m_pObject;

	pThis->m_pVTable = pVTable;

	// prime all the base types

	rtl::CArrayT <CBaseTypeSlot*> BaseTypePrimeArray = pType->GetBaseTypePrimeArray ();
	size_t Count = BaseTypePrimeArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CBaseTypeSlot* pSlot = BaseTypePrimeArray [i];
		ASSERT (pSlot->GetType ()->GetTypeKind () == EType_Class);

		jnc::TIfaceHdr* pInterface = (jnc::TIfaceHdr*) ((char*) pThis + pSlot->GetOffset ());
		pInterface->m_pObject = pThis->m_pObject;

		Prime (
			pInterface,
			(CClassType*) pSlot->GetType (),
			(void**) pVTable + pSlot->GetVTableIndex (),
			pRoot
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

		jnc::TObjHdr* pObject = (jnc::TObjHdr*) ((char*) pThis + pField->GetOffset ());
		jnc::TIfaceHdr* pInterface = (jnc::TIfaceHdr*) (pObject + 1);
		pInterface->m_pObject = pObject;

		Prime (pInterface, pFieldType, NULL, pRoot);
	}
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
