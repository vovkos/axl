#include "axl_Precomp.h"
#include "axl_log_Colorizer.h"

//.............................................................................

void 
AXL_VFT
axl_log_TColorizer_Colorize_b(
	void* pColorizer,
	axl_log_TColorizeSink* pSink,
	axl_log_TColorizeBlock* pBlock,
	bool_t SaveStateHint
	)
{
}

void
AXL_API
axl_log_TColorizer_Construct(
	axl_log_TColorizer* pColorizer, 
	axl_ref_TBuffer* pStorage
	)
{
	static axl_log_TColorizerVTable _VTable = 
	{
		axl_log_TColorizer_Destruct,
		axl_log_TColorizer_Colorize_b
	};

	axl_ref_TObject_Construct((axl_ref_TObject*) pColorizer, pStorage);
	pColorizer->m_pVTable = &_VTable;

	AXL_REF_SET_TAG(pColorizer, "axl_log_TColorizer");
}

void
AXL_VFT
axl_log_TColorizer_Destruct(axl_log_TColorizer* pColorizer)
{
	axl_ref_TObject_Destruct((axl_ref_TObject*) pColorizer);
}

//.............................................................................

// colorizer stack state is simply a list of states for individual colorizers

typedef struct _axl_log_TColorizerStateEntry
{ 
	axl_rtl_TListEntry;
	intptr_t m_State;
	axl_ref_TObject* m_pStateEx;
} axl_log_TColorizerStateEntry;

typedef struct _axl_log_TColorizerStackState
{
	axl_ref_TObject;
	axl_rtl_TListEx m_Stack;
} axl_log_TColorizerStackState;

void
AXL_VFT
axl_log_TColorizerStackState_Destruct(axl_log_TColorizerStackState* pState)
{
	while (!axl_rtl_TListEx_IsEmpty(&pState->m_Stack))
	{
		axl_log_TColorizerStateEntry* pEntry = (axl_log_TColorizerStateEntry*) axl_rtl_TListEx_RemoveHead(&pState->m_Stack);
		axl_ref_TObject__ClearPtr(&pEntry->m_pStateEx);
		axl_mem_Free(pEntry);
	}

	axl_ref_TObject_Destruct((axl_ref_TObject*) pState);
}

void
AXL_API
axl_log_TColorizerStackState_Construct(
	axl_log_TColorizerStackState* pState,
	axl_ref_TBuffer* pStorage	
	)
{
	static axl_ref_TObjectVTable _VTable = 
	{
		axl_log_TColorizerStackState_Destruct
	};

	axl_ref_TObject_Construct((axl_ref_TObject*) pState, pStorage);
	axl_rtl_TListEx_Construct(&pState->m_Stack);
	pState->m_pVTable = &_VTable;

	AXL_REF_SET_TAG(pState, "axl_log_TColorizerStackState");
}

//.............................................................................

typedef struct _axl_log_TColorizerEntry 
{ 
	axl_rtl_TListEntry;
	axl_log_TColorizer* m_pColorizer;
} axl_log_TColorizerEntry;

void
AXL_API
axl_log_TColorizerStack_Construct(
	axl_log_TColorizerStack* pStack,
	axl_ref_TBuffer* pStorage
	)
{
	static axl_log_TColorizerVTable _VTable = 
	{
		axl_log_TColorizerStack_Destruct,
		axl_log_TColorizerStack_Colorize
	};

	axl_log_TColorizer_Construct((axl_log_TColorizer*) pStack, pStorage);
	axl_rtl_TListEx_Construct(&pStack->m_Stack);

	pStack->m_pVTable = &_VTable;

	AXL_REF_SET_TAG(pStack, "axl_log_TColorizerStack");
}

void
AXL_VFT
axl_log_TColorizerStack_Destruct(axl_log_TColorizerStack* pStack)
{
	axl_log_TColorizerStack_Clear(pStack);
	axl_log_TColorizer_Destruct((axl_log_TColorizer*) pStack);
}

void
AXL_API
axl_log_TColorizerStack_Clear(axl_log_TColorizerStack* pStack)
{
	while (!axl_rtl_TListEx_IsEmpty(&pStack->m_Stack))
	{
		axl_log_TColorizerEntry* pEntry = (axl_log_TColorizerEntry*) axl_rtl_TListEx_RemoveTail(&pStack->m_Stack);
		axl_ref_TObject_Dereference((axl_ref_TObject*) pEntry->m_pColorizer);
		axl_mem_Free(pEntry);
	}
}

bool_t
AXL_API
axl_log_TColorizerStack_ClearButFirst(axl_log_TColorizerStack* pStack)
{
	if (!axl_rtl_TListEx_IsEmpty(&pStack->m_Stack))
		return false;

	while (pStack->m_Stack.m_Count > 1)
	{
		axl_log_TColorizerEntry* pEntry = (axl_log_TColorizerEntry*) axl_rtl_TListEx_RemoveTail(&pStack->m_Stack);
		axl_ref_TObject_Dereference((axl_ref_TObject*) pEntry->m_pColorizer);
		axl_mem_Free(pEntry);
	}

	return true;
}

axl_log_TColorizerEntry*
AXL_API
axl_log_TColorizerStack_FindColorizer(
	axl_log_TColorizerStack* pStack,
	axl_log_TColorizer* pColorizer
	)
{
	axl_log_TColorizerEntry* pEntry = (axl_log_TColorizerEntry*) axl_rtl_TListEx_GetHead(&pStack->m_Stack);
	while (pEntry)
	{	
		if (pEntry->m_pColorizer == pColorizer)
			return pEntry;

		pEntry = (axl_log_TColorizerEntry*) pEntry->m_pNext;
	}

	return NULL;
}

bool_t
AXL_API
axl_log_TColorizerStack_AddColorizer(
	axl_log_TColorizerStack* pStack,
	axl_log_TColorizer* pColorizer
	)
{
	axl_log_TColorizerEntry* pEntry = (axl_log_TColorizerEntry*) axl_mem_Allocate(sizeof(axl_log_TColorizerEntry));
	pEntry->m_pColorizer = pColorizer;
	axl_ref_TObject_Reference((axl_ref_TObject*) pColorizer);
	axl_rtl_TListEx_InsertHead(&pStack->m_Stack, (axl_rtl_TListEntry*) pEntry);
	return true;
}

bool_t
AXL_API
axl_log_TColorizerStack_RemoveColorizer(
	axl_log_TColorizerStack* pStack,
	axl_log_TColorizer* pColorizer
	)
{
	axl_log_TColorizerEntry* pEntry = axl_log_TColorizerStack_FindColorizer(pStack, pColorizer);
	if (pEntry)
		return false;

	axl_rtl_TListEx_Remove(&pStack->m_Stack, (axl_rtl_TListEntry*) pEntry);
	axl_ref_TObject_Dereference((axl_ref_TObject*) pColorizer);
	axl_mem_Free(pEntry);
	return true;
}

void
AXL_VFT
axl_log_TColorizerStack_Colorize(
	axl_log_TColorizerStack* pStack,
	axl_log_TColorizeSink* pSink,
	axl_log_TColorizeBlock* pBlock,
	bool_t SaveStateHint
	)
{
	axl_log_TColorizer* pColorizer;
	axl_log_TColorizerEntry* pStackEntry = (axl_log_TColorizerEntry*) axl_rtl_TListEx_GetHead(&pStack->m_Stack);
	axl_log_TColorizerStackState* pState = (axl_log_TColorizerStackState*) pSink->m_pColorizerStateEx;
	axl_log_TColorizerStateEntry* pStateEntry;

	pSink->m_ColorizerMetric = 0;

	if (pState)
	{
		axl_ref_TObject_Reference((axl_ref_TObject*) pState);

		pStateEntry = (axl_log_TColorizerStateEntry*) axl_rtl_TListEx_GetHead(&pState->m_Stack);
		
		ASSERT(pState->m_Stack.m_Count == pStack->m_Stack.m_Count);
		while (pStackEntry && pStateEntry)
		{	
			pColorizer = pStackEntry->m_pColorizer;

			axl_log_TColorizeSink_SetColorizerState(pSink, pStateEntry->m_State, pStateEntry->m_pStateEx);
			axl_log_TColorizer_Colorize(pColorizer, pSink, pBlock, SaveStateHint);

			pStateEntry->m_State = pSink->m_ColorizerState;
			axl_ref_TObject__CopyPtr(&pStateEntry->m_pStateEx, pSink->m_pColorizerStateEx);

			pStackEntry = (axl_log_TColorizerEntry*) pStackEntry->m_pNext;
			pStateEntry = (axl_log_TColorizerStateEntry*) pStateEntry->m_pNext;
			pSink->m_ColorizerMetric++;
		}

		axl_log_TColorizeSink_SetColorizerState(pSink, 0, (axl_ref_TObject*) pState);
		axl_ref_TObject_Dereference((axl_ref_TObject*) pState);
	}
	else if (SaveStateHint)
	{
		axl_ref_TObject__New(
			&pState, 
			sizeof(axl_log_TColorizerStackState),
			axl_log_TColorizerStackState_Construct
			);

		while (pStackEntry)
		{	
			pColorizer = pStackEntry->m_pColorizer;

			pStateEntry = axl_mem_Allocate(sizeof(axl_log_TColorizerStateEntry));
			pStateEntry->m_State = 0; 
			pStateEntry->m_pStateEx = NULL;
			axl_rtl_TListEx_InsertTail(&pState->m_Stack, (axl_rtl_TListEntry*) pStateEntry);

			axl_log_TColorizeSink_SetColorizerState(pSink, 0, NULL);
			axl_log_TColorizer_Colorize(pColorizer, pSink, pBlock, true);

			pStateEntry->m_State = pSink->m_ColorizerState;
			axl_ref_TObject__CopyPtr(&pStateEntry->m_pStateEx, pSink->m_pColorizerStateEx);

			pStackEntry = (axl_log_TColorizerEntry*) pStackEntry->m_pNext;
			pSink->m_ColorizerMetric++;
		}

		axl_log_TColorizeSink_SetColorizerState(pSink, 0, (axl_ref_TObject*) pState);
		axl_ref_TObject_Dereference((axl_ref_TObject*) pState);
	}
	else
	{
		while (pStackEntry)
		{	
			pColorizer = pStackEntry->m_pColorizer;

			axl_log_TColorizeSink_SetColorizerState(pSink, 0, NULL);
			axl_log_TColorizer_Colorize(pColorizer, pSink, pBlock, false);

			pStackEntry = (axl_log_TColorizerEntry*) pStackEntry->m_pNext;
			pSink->m_ColorizerMetric++;
		}
	}
}

//.............................................................................
