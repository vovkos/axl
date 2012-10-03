#include "axl_Precomp.h"
#include "axl_log_Filter.h"

//.............................................................................

bool_t 
AXL_VFT
axl_log_TFilter_Filter_b(
	axl_log_TFilter* pFilter,
	ulong_t Code,
	const void* p,
	size_t Size,
	ulong_t* pVolatileFlags
	)
{
	return false;
}

void
AXL_API
axl_log_TFilter_Construct(
	axl_log_TFilter* pFilter, 
	axl_ref_TBuffer* pStorage
	)
{
	static axl_log_TFilterVTable _VTable = 
	{
		axl_log_TFilter_Destruct,
		axl_log_TFilter_Filter_b
	};

	axl_ref_TObject_Construct((axl_ref_TObject*) pFilter, pStorage);
	pFilter->m_pVTable = &_VTable;

	AXL_REF_SET_TAG(pFilter, "axl_log_TFilter");
}

void
AXL_VFT
axl_log_TFilter_Destruct(axl_log_TFilter* pFilter)
{
	axl_ref_TObject_Destruct((axl_ref_TObject*) pFilter);
}

//.............................................................................

typedef struct _axl_log_TFilterEntry 
{ 
	axl_rtl_TListEntry;
	axl_log_TFilter* m_pFilter;
} axl_log_TFilterEntry;

//.............................................................................


void
AXL_API
axl_log_TFilterStack_Construct(
	axl_log_TFilterStack* pStack,
	axl_ref_TBuffer* pStorage
	)
{
	static axl_log_TFilterVTable _VTable = 
	{
		axl_log_TFilterStack_Destruct,
		axl_log_TFilterStack_Filter
	};

	axl_log_TFilter_Construct((axl_log_TFilter*) pStack, pStorage);
	axl_rtl_TList_Construct(&pStack->m_Stack);
	pStack->m_pVTable = &_VTable;

	AXL_REF_SET_TAG(pStack, "axl_log_TFilterStack");
}

void
AXL_VFT
axl_log_TFilterStack_Destruct(axl_log_TFilterStack* pStack)
{
	axl_log_TFilterStack_Clear(pStack);
	axl_log_TFilter_Destruct((axl_log_TFilter*) pStack);
}

void
AXL_API
axl_log_TFilterStack_Clear(axl_log_TFilterStack* pStack)
{
	while (!axl_rtl_TList_IsEmpty(&pStack->m_Stack))
	{
		axl_log_TFilterEntry* pEntry = (axl_log_TFilterEntry*) axl_rtl_TList_RemoveTail(&pStack->m_Stack);
		axl_ref_TObject_Dereference((axl_ref_TObject*) pEntry->m_pFilter);
		axl_mem_Free(pEntry);
	}
}

bool_t
AXL_API
axl_log_TFilterStack_ClearButFirst(axl_log_TFilterStack* pStack)
{
	if (!axl_rtl_TList_IsEmpty(&pStack->m_Stack))
		return false;

	while (pStack->m_Stack.m_Count > 1)
	{
		axl_log_TFilterEntry* pEntry = (axl_log_TFilterEntry*) axl_rtl_TList_RemoveTail(&pStack->m_Stack);
		axl_ref_TObject_Dereference((axl_ref_TObject*) pEntry->m_pFilter);
		axl_mem_Free(pEntry);
	}

	return true;
}

axl_log_TFilterEntry*
AXL_API
axl_log_TFilterStack_FindFilter(
	axl_log_TFilterStack* pStack,
	axl_log_TFilter* pFilter
	)
{
	axl_log_TFilterEntry* pEntry = (axl_log_TFilterEntry*) axl_rtl_TList_GetHead(&pStack->m_Stack);
	while (pEntry)
	{	
		if (pEntry->m_pFilter == pFilter)
			return pEntry;

		pEntry = (axl_log_TFilterEntry*) axl_rtl_TList_GetNext(&pStack->m_Stack, (axl_rtl_TListEntry*) pEntry);
	}

	return NULL;
}

bool_t
AXL_API
axl_log_TFilterStack_AddFilter(
	axl_log_TFilterStack* pStack,
	axl_log_TFilter* pFilter
	)
{
	axl_log_TFilterEntry* pEntry = (axl_log_TFilterEntry*) axl_mem_Allocate(sizeof(axl_log_TFilterEntry));
	pEntry->m_pFilter = pFilter;
	axl_ref_TObject_Reference((axl_ref_TObject*) pFilter);
	axl_rtl_TList_InsertHead(&pStack->m_Stack, (axl_rtl_TListEntry*) pEntry);
	return true;
}

bool_t
AXL_API
axl_log_TFilterStack_RemoveFilter(
	axl_log_TFilterStack* pStack,
	axl_log_TFilter* pFilter
	)
{
	axl_log_TFilterEntry* pEntry = axl_log_TFilterStack_FindFilter(pStack, pFilter);
	if (pEntry)
		return false;

	axl_rtl_TList_Remove(&pStack->m_Stack, (axl_rtl_TListEntry*) pEntry);
	axl_ref_TObject_Dereference((axl_ref_TObject*) pFilter);
	axl_mem_Free(pEntry);
	return true;
}

bool_t 
AXL_VFT
axl_log_TFilterStack_Filter(
	axl_log_TFilterStack* pStack,
	ulong_t Code,
	const void* p,
	size_t Size,
	ulong_t* pVolatileFlags
	)
{
	axl_log_TFilterEntry* pEntry = (axl_log_TFilterEntry*) axl_rtl_TList_GetHead(&pStack->m_Stack);

	while (pEntry)
	{	
		bool_t Result = axl_log_TFilter_Filter(pEntry->m_pFilter, Code, p, Size, pVolatileFlags);
		if (Result)
			return true;

		pEntry = (axl_log_TFilterEntry*) axl_rtl_TList_GetNext(&pStack->m_Stack, (axl_rtl_TListEntry*) pEntry);
	}

	return false;
}

//.............................................................................
