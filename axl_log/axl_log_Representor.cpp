#include "axl_Precomp.h"
#include "axl_log_Representor.h"

//.............................................................................

bool_t 
AXL_VFT
axl_log_TRepresentor_Represent_b(
	axl_log_TRepresentor* pRepresentor,
	axl_log_TRepresentSink* pSink,
	ulong_t Code,
	const void* p,
	size_t Size,
	ulong_t VolatileFlags
	)
{
	return false;
}

void
AXL_API
axl_log_TRepresentor_Construct(
	axl_log_TRepresentor* pRepresentor, 
	axl_ref_TBuffer* pStorage
	)
{
	static axl_log_TRepresentorVTable _VTable = 
	{
		axl_log_TRepresentor_Destruct,
		axl_log_TRepresentor_Represent_b
	};

	axl_ref_TObject_Construct((axl_ref_TObject*) pRepresentor, pStorage);
	pRepresentor->m_pVTable = &_VTable;

	AXL_REF_SET_TAG(pRepresentor, "axl_log_TRepresentor");
}

void
AXL_VFT
axl_log_TRepresentor_Destruct(axl_log_TRepresentor* pRepresentor)
{
	axl_ref_TObject_Destruct((axl_ref_TObject*) pRepresentor);
}

//.............................................................................

typedef struct _axl_log_TRepresentorEntry 
{ 
	axl_rtl_TListEntry;
	axl_log_TRepresentor* m_pRepresentor;
} axl_log_TRepresentorEntry;

//.............................................................................

void
AXL_API
axl_log_TRepresentorStack_Construct(
	axl_log_TRepresentorStack* pStack,
	axl_ref_TBuffer* pStorage
	)
{
	static axl_log_TRepresentorVTable _VTable = 
	{
		axl_log_TRepresentorStack_Destruct,
		axl_log_TRepresentorStack_Represent
	};

	axl_log_TRepresentor_Construct((axl_log_TRepresentor*) pStack, pStorage);
	axl_rtl_TListEx_Construct(&pStack->m_Stack);
	pStack->m_pVTable = &_VTable;

	AXL_REF_SET_TAG(pStack, "axl_log_TRepresentorStack");
}

void
AXL_VFT
axl_log_TRepresentorStack_Destruct(axl_log_TRepresentorStack* pStack)
{
	axl_log_TRepresentorStack_Clear(pStack);
	axl_log_TRepresentor_Destruct((axl_log_TRepresentor*) pStack);
}

void
AXL_API
axl_log_TRepresentorStack_Clear(axl_log_TRepresentorStack* pStack)
{
	while (!axl_rtl_TListEx_IsEmpty(&pStack->m_Stack))
	{
		axl_log_TRepresentorEntry* pEntry = (axl_log_TRepresentorEntry*) axl_rtl_TListEx_RemoveTail(&pStack->m_Stack);
		axl_ref_TObject_Dereference((axl_ref_TObject*) pEntry->m_pRepresentor);
		axl_mem_Free(pEntry);
	}
}

bool_t
AXL_API
axl_log_TRepresentorStack_ClearButFirst(axl_log_TRepresentorStack* pStack)
{
	if (!axl_rtl_TListEx_IsEmpty(&pStack->m_Stack))
		return false;

	while (pStack->m_Stack.m_Count > 1)
	{
		axl_log_TRepresentorEntry* pEntry = (axl_log_TRepresentorEntry*) axl_rtl_TListEx_RemoveTail(&pStack->m_Stack);
		axl_ref_TObject_Dereference((axl_ref_TObject*) pEntry->m_pRepresentor);
		axl_mem_Free(pEntry);
	}

	return true;
}

axl_log_TRepresentorEntry*
AXL_API
axl_log_TRepresentorStack_FindRepresentor(
	axl_log_TRepresentorStack* pStack,
	axl_log_TRepresentor* pRepresentor
	)
{
	axl_log_TRepresentorEntry* pEntry = (axl_log_TRepresentorEntry*) axl_rtl_TListEx_GetHead(&pStack->m_Stack);
	while (pEntry)
	{	
		if (pEntry->m_pRepresentor == pRepresentor)
			return pEntry;

		pEntry = (axl_log_TRepresentorEntry*) pEntry->m_pNext;
	}

	return NULL;
}

bool_t
AXL_API
axl_log_TRepresentorStack_AddRepresentor(
	axl_log_TRepresentorStack* pStack,
	axl_log_TRepresentor* pRepresentor
	)
{
	axl_log_TRepresentorEntry* pEntry = (axl_log_TRepresentorEntry*) axl_mem_Allocate(sizeof(axl_log_TRepresentorEntry));
	pEntry->m_pRepresentor = pRepresentor;
	axl_ref_TObject_Reference((axl_ref_TObject*) pRepresentor);
	axl_rtl_TListEx_InsertHead(&pStack->m_Stack, (axl_rtl_TListEntry*) pEntry);
	return true;
}

bool_t
AXL_API
axl_log_TRepresentorStack_RemoveRepresentor(
	axl_log_TRepresentorStack* pStack,
	axl_log_TRepresentor* pRepresentor
	)
{
	axl_log_TRepresentorEntry* pEntry = axl_log_TRepresentorStack_FindRepresentor(pStack, pRepresentor);
	if (pEntry)
		return false;

	axl_rtl_TListEx_Remove(&pStack->m_Stack, (axl_rtl_TListEntry*) pEntry);
	axl_ref_TObject_Dereference((axl_ref_TObject*) pRepresentor);
	axl_mem_Free(pEntry);
	return true;
}

bool_t 
AXL_VFT
axl_log_TRepresentorStack_Represent(
	axl_log_TRepresentorStack* pStack,
	axl_log_TRepresentSink* pSink,
	ulong_t Code,
	const void* p,
	size_t Size,
	ulong_t VolatileFlags
	)
{
	axl_log_TRepresentorEntry* pEntry = (axl_log_TRepresentorEntry*) axl_rtl_TListEx_GetHead(&pStack->m_Stack);
	while (pEntry)
	{	
		bool_t Result = axl_log_TRepresentor_Represent(pEntry->m_pRepresentor, pSink, Code, p, Size, VolatileFlags);
		if (Result)
			return true;

		pEntry = (axl_log_TRepresentorEntry*) pEntry->m_pNext;
	}

	return false;
}

//.............................................................................
