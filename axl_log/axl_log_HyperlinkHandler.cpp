#include "axl_Precomp.h"
#include "axl_log_HyperlinkHandler.h"

//.............................................................................

bool_t
AXL_VFT
axl_log_THyperlinkHandler_OnHyperlink_b(
	axl_log_THyperlinkHandler* pHandler,
	HWND hWndLogCtrl,
	axl_log_TLine* pLine,
	const tchar_t* pHyperlink
	)
{
	return false;
}

void
AXL_API
axl_log_THyperlinkHandler_Construct(
	axl_log_THyperlinkHandler* pHandler, 
	axl_ref_TBuffer* pStorage
	)
{
	static axl_log_THyperlinkHandlerVTable _VTable = 
	{
		axl_log_THyperlinkHandler_Destruct,
		axl_log_THyperlinkHandler_OnHyperlink_b
	};

	axl_ref_TObject_Construct((axl_ref_TObject*) pHandler, pStorage);
	pHandler->m_pVTable = &_VTable;

	AXL_REF_SET_TAG(pHandler, "axl_log_THyperlinkHandler");
}

void
AXL_VFT
axl_log_THyperlinkHandler_Destruct(axl_log_THyperlinkHandler* pHandler)
{
	axl_ref_TObject_Destruct((axl_ref_TObject*) pHandler);
}

//.............................................................................

typedef struct _axl_log_THyperlinkHandlerEntry 
{ 
	axl_rtl_TListEntry;
	axl_log_THyperlinkHandler* m_pHyperlinkHandler;
} axl_log_THyperlinkHandlerEntry;

//.............................................................................

void
AXL_API
axl_log_THyperlinkHandlerStack_Construct(
	axl_log_THyperlinkHandlerStack* pStack,
	axl_ref_TBuffer* pStorage
	)
{
	static axl_log_THyperlinkHandlerVTable _VTable = 
	{
		axl_log_THyperlinkHandlerStack_Destruct,
		axl_log_THyperlinkHandlerStack_OnHyperlink
	};

	axl_log_THyperlinkHandler_Construct((axl_log_THyperlinkHandler*) pStack, pStorage);
	axl_rtl_TList_Construct(&pStack->m_Stack);
	pStack->m_pVTable = &_VTable;

	AXL_REF_SET_TAG(pStack, "axl_log_THyperlinkHandlerStack");
}

void
AXL_VFT
axl_log_THyperlinkHandlerStack_Destruct(axl_log_THyperlinkHandlerStack* pStack)
{
	axl_log_THyperlinkHandlerStack_Clear(pStack);
	axl_log_THyperlinkHandler_Destruct((axl_log_THyperlinkHandler*) pStack);
}

void
AXL_API
axl_log_THyperlinkHandlerStack_Clear(axl_log_THyperlinkHandlerStack* pStack)
{
	while (!axl_rtl_TList_IsEmpty(&pStack->m_Stack))
	{
		axl_log_THyperlinkHandlerEntry* pEntry = (axl_log_THyperlinkHandlerEntry*) axl_rtl_TList_RemoveTail(&pStack->m_Stack);
		axl_ref_TObject_Dereference((axl_ref_TObject*) pEntry->m_pHyperlinkHandler);
		axl_mem_Free(pEntry);
	}
}

bool_t
AXL_API
axl_log_THyperlinkHandlerStack_ClearButFirst(axl_log_THyperlinkHandlerStack* pStack)
{
	if (!axl_rtl_TList_IsEmpty(&pStack->m_Stack))
		return false;

	while (pStack->m_Stack.m_Count > 1)
	{
		axl_log_THyperlinkHandlerEntry* pEntry = (axl_log_THyperlinkHandlerEntry*) axl_rtl_TList_RemoveTail(&pStack->m_Stack);
		axl_ref_TObject_Dereference((axl_ref_TObject*) pEntry->m_pHyperlinkHandler);
		axl_mem_Free(pEntry);
	}

	return true;
}

axl_log_THyperlinkHandlerEntry*
AXL_API
axl_log_THyperlinkHandlerStack_FindHyperlinkHandler(
	axl_log_THyperlinkHandlerStack* pStack,
	axl_log_THyperlinkHandler* pHyperlinkHandler
	)
{
	axl_log_THyperlinkHandlerEntry* pEntry = (axl_log_THyperlinkHandlerEntry*) axl_rtl_TList_GetHead(&pStack->m_Stack);
	while (pEntry)
	{	
		if (pEntry->m_pHyperlinkHandler == pHyperlinkHandler)
			return pEntry;

		pEntry = (axl_log_THyperlinkHandlerEntry*) axl_rtl_TList_GetNext(&pStack->m_Stack, (axl_rtl_TListEntry*) pEntry);
	}

	return NULL;
}

bool_t
AXL_API
axl_log_THyperlinkHandlerStack_AddHyperlinkHandler(
	axl_log_THyperlinkHandlerStack* pStack,
	axl_log_THyperlinkHandler* pHyperlinkHandler
	)
{
	axl_log_THyperlinkHandlerEntry* pEntry = (axl_log_THyperlinkHandlerEntry*) axl_mem_Allocate(sizeof(axl_log_THyperlinkHandlerEntry));
	pEntry->m_pHyperlinkHandler = pHyperlinkHandler;
	axl_ref_TObject_Reference((axl_ref_TObject*) pHyperlinkHandler);
	axl_rtl_TList_InsertHead(&pStack->m_Stack, (axl_rtl_TListEntry*) pEntry);
	return true;
}

bool_t
AXL_API
axl_log_THyperlinkHandlerStack_RemoveHyperlinkHandler(
	axl_log_THyperlinkHandlerStack* pStack,
	axl_log_THyperlinkHandler* pHyperlinkHandler
	)
{
	axl_log_THyperlinkHandlerEntry* pEntry = axl_log_THyperlinkHandlerStack_FindHyperlinkHandler(pStack, pHyperlinkHandler);
	if (pEntry)
		return false;

	axl_rtl_TList_Remove(&pStack->m_Stack, (axl_rtl_TListEntry*) pEntry);
	axl_ref_TObject_Dereference((axl_ref_TObject*) pHyperlinkHandler);
	axl_mem_Free(pEntry);
	return true;
}

bool_t 
AXL_VFT
axl_log_THyperlinkHandlerStack_OnHyperlink(
	axl_log_THyperlinkHandlerStack* pStack,
	HWND hWndLogCtrl,
	axl_log_TLine* pLine,
	const tchar_t* pHyperlink
	)
{
	axl_log_THyperlinkHandlerEntry* pEntry = (axl_log_THyperlinkHandlerEntry*) axl_rtl_TList_GetHead(&pStack->m_Stack);

	while (pEntry)
	{	
		bool_t Result = axl_log_THyperlinkHandler_OnHyperlink(pEntry->m_pHyperlinkHandler, hWndLogCtrl, pLine, pHyperlink);
		if (Result)
			return true;

		pEntry = (axl_log_THyperlinkHandlerEntry*) axl_rtl_TList_GetNext(&pStack->m_Stack, (axl_rtl_TListEntry*) pEntry);
	}

	return false;
}

//.............................................................................

