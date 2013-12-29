#include "pch.h"
#include "axl_jnc_Object.h"
#include "axl_jnc_ClosureClassType.h"
#include "axl_jnc_Runtime.h"

namespace axl {
namespace jnc {

//.............................................................................

void 
TObject::GcMarkData (CRuntime* pRuntime)
{
	m_pRoot->m_Flags |= EObjectFlag_GcWeakMark;

	if (m_Flags & EObjectFlag_GcRootsAdded)
		return;

	m_Flags |= EObjectFlag_GcRootsAdded;

	if (!(m_pType->GetFlags () & ETypeFlag_GcRoot))
		return;

	if (!(m_Flags & EObjectFlag_DynamicArray))
	{
		if (m_pType->GetTypeKind () == EType_Class)
			pRuntime->AddGcRoot (this, m_pType);
		else
			pRuntime->AddGcRoot (this + 1, m_pType);
	}
	else
	{
		ASSERT (m_pType->GetTypeKind () != EType_Class);

		TDynamicArray* pDynamicArray = (TDynamicArray*) this;
		char* p = (char*) (pDynamicArray + 1);		
		for (size_t i = 0; i < pDynamicArray->m_Count; i++)
		{
			pRuntime->AddGcRoot (p, m_pType);
			p += m_pType->GetSize  ();
		}
	}
}

void 
TObject::GcMarkObject (CRuntime* pRuntime)
{
	m_pRoot->m_Flags |= EObjectFlag_GcWeakMark;
	m_Flags |= EObjectFlag_GcMark;

	if (m_Flags & EObjectFlag_GcRootsAdded)
		return;

	m_Flags |= EObjectFlag_GcRootsAdded;

	if (!(m_pType->GetFlags () & ETypeFlag_GcRoot))
		return;

	pRuntime->AddGcRoot (this, m_pType);
}

void
TObject::GcWeakMarkClosureObject (CRuntime* pRuntime)
{
	m_pRoot->m_Flags |= EObjectFlag_GcWeakMark;
	m_Flags |= EObjectFlag_GcMark;

	if (m_Flags & (EObjectFlag_GcWeakMark_c | EObjectFlag_GcRootsAdded))
		return;

	m_Flags |= EObjectFlag_GcWeakMark_c;

	CClosureClassType* pClosureClassType = (CClosureClassType*) m_pClassType;
	if (!pClosureClassType->GetWeakMask ())
	{
		GcMarkObject (pRuntime);
		return;
	}

	char* p = (char*) (this + 1);

	rtl::CArrayT <CStructField*> GcRootMemberFieldArray = pClosureClassType->GetGcRootMemberFieldArray ();
	size_t Count = GcRootMemberFieldArray.GetCount ();

	for (size_t i = 0; i < Count; i++)
	{
		CStructField* pField = GcRootMemberFieldArray [i];
		CType* pType = pField->GetType ();
		ASSERT (pType->GetFlags () & ETypeFlag_GcRoot);		

		if (pField->GetFlags () & EStructFieldFlag_WeakMasked)
			pType = GetWeakPtrType (pType);

		pType->GcMark (pRuntime, p + pField->GetOffset ());
	}
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
