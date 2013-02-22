// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2010. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_RTL_HANDLETABLE_H

#include "axl_g_Def.h"

namespace axl {
namespace rtl {

//.............................................................................

typedef struct _axl_rtl_THandleTablePage axl_rtl_THandleTablePage;

struct _axl_rtl_THandleTablePage: TListLink
{
	ulong_t m_Map;
	ulong_t m_ShadowMap; // shadow map forces allocated handle to increment
	ulong_t m_HandleBase;
	void* m_Handles[32];
};

//.............................................................................

typedef struct _axl_rtl_THandleTable axl_rtl_THandleTable;

void 
axl_rtl_THandleTable_Construct(axl_rtl_THandleTable* pTable);

void 
axl_rtl_THandleTable_Destruct(axl_rtl_THandleTable* pTable);

ulong_t 
axl_rtl_THandleTable_AddHandle(
	axl_rtl_THandleTable* pTable,
	void* pValue
	);

bool_t
axl_rtl_THandleTable_FindHandleEx(
	axl_rtl_THandleTable* pTable,
	ulong_t Handle, 
	void** ppValue
	);

void*
axl_rtl_THandleTable_FindHandle(
	axl_rtl_THandleTable* pTable,
	ulong_t Handle
	);

bool_t
axl_rtl_THandleTable_RemoveHandle(
	axl_rtl_THandleTable* pTable,
	ulong_t Handle
	);

void 
axl_rtl_THandleTable_RemoveAllHandles(axl_rtl_THandleTable* pTable);

struct _axl_rtl_THandleTable
{
	axl_rtl_TListEx m_AvailablePageList;
	axl_rtl_TListEx m_ExhaustedPageList;
	ulong_t m_PageCount;
	ulong_t m_HandleCount;

	axl_rtl_THandleTablePage** m_ppDirectory;
	ulong_t m_DirectorySize;

	axl_rtl_THandleTablePage* m_pBuiltInDirectory;
	axl_rtl_THandleTablePage m_BuiltInPage;

	typedef axl_rtl_THandleTablePage TPage;

	ulong_t AddHandle(void* pValue)
		{ return axl_rtl_THandleTable_AddHandle(this, pValue); }
	
	bool FindHandleEx(ulong_t Handle, void** ppValue)
		{ return axl_rtl_THandleTable_FindHandleEx(this, Handle, ppValue) != 0; }

	void* FindHandle(ulong_t Handle)
		{ return axl_rtl_THandleTable_FindHandle(this, Handle); }

	bool RemoveHandle(ulong_t Handle)
		{ return axl_rtl_THandleTable_RemoveHandle(this, Handle) != 0; }

	void RemoveAllHandles()
		{ axl_rtl_THandleTable_RemoveAllHandles(this); }
};

enum
{
	axl_rtl_EHandleTableDef_DirectoryGrow  = 8 // add 8 pages at once to directory 
};

//.............................................................................

void 
axl_rtl_THandleTable_Construct(axl_rtl_THandleTable* pTable)
{ 
	pTable->m_HandleCount = 0;
	pTable->m_PageCount = 1;

	pTable->m_BuiltInPage.m_HandleBase = 1;
	pTable->m_BuiltInPage.m_Map = 0;
	pTable->m_BuiltInPage.m_ShadowMap = 0;

	axl_rtl_TListEx_Construct(&pTable->m_AvailablePageList);
	axl_rtl_TListEx_Construct(&pTable->m_ExhaustedPageList);
	axl_rtl_TListEx_InsertTail(&pTable->m_AvailablePageList, (axl_rtl_TListEntry*) &pTable->m_BuiltInPage);

	pTable->m_pBuiltInDirectory = &pTable->m_BuiltInPage;
	pTable->m_ppDirectory = &pTable->m_pBuiltInDirectory;
	pTable->m_DirectorySize = 1;
}

void 
axl_rtl_THandleTable_Destruct(axl_rtl_THandleTable* pTable)
{ 
	if (pTable->m_ppDirectory != &pTable->m_pBuiltInDirectory)
		axl_mem_Free(pTable->m_ppDirectory);

	if (pTable->m_BuiltInPage.m_Map == -1)
		axl_rtl_TListEx_Remove(&pTable->m_ExhaustedPageList, (axl_rtl_TListEntry*) &pTable->m_BuiltInPage);
	else
		axl_rtl_TListEx_Remove(&pTable->m_AvailablePageList, (axl_rtl_TListEntry*) &pTable->m_BuiltInPage);

	while (!axl_rtl_TListEx_IsEmpty(&pTable->m_AvailablePageList))
	{
		axl_rtl_THandleTablePage* pPage = (axl_rtl_THandleTablePage*) axl_rtl_TListEx_RemoveHead(&pTable->m_AvailablePageList);
		axl_mem_Free(pPage);
	}

	while (!axl_rtl_TListEx_IsEmpty(&pTable->m_ExhaustedPageList))
	{
		axl_rtl_THandleTablePage* pPage = (axl_rtl_THandleTablePage*) axl_rtl_TListEx_RemoveHead(&pTable->m_ExhaustedPageList);
		axl_mem_Free(pPage);
	}
}

static
axl_rtl_THandleTablePage* 
axl_rtl_THandleTable_AddNewPage(axl_rtl_THandleTable* pTable)
{
	axl_rtl_THandleTablePage* pPage;

	if (pTable->m_PageCount >= pTable->m_DirectorySize) // grow directory
	{
		void* pvNewDirectory;

		pTable->m_DirectorySize = pTable->m_PageCount - pTable->m_PageCount % axl_rtl_EHandleTableDef_DirectoryGrow + axl_rtl_EHandleTableDef_DirectoryGrow;
		pvNewDirectory = axl_mem_Allocate(pTable->m_DirectorySize * sizeof(axl_rtl_THandleTablePage*));

		memcpy(pvNewDirectory, pTable->m_ppDirectory, pTable->m_PageCount * sizeof(axl_rtl_THandleTablePage*));

		if (pTable->m_ppDirectory != &pTable->m_pBuiltInDirectory)
			axl_mem_Free(pTable->m_ppDirectory);

		pTable->m_ppDirectory = (axl_rtl_THandleTablePage**) pvNewDirectory;
	}

	pPage = (axl_rtl_THandleTablePage*) axl_mem_Allocate(sizeof(axl_rtl_THandleTablePage));
	pPage->m_HandleBase = pTable->m_PageCount * 32 + 1;
	pPage->m_Map = 0;
	pPage->m_ShadowMap = 0;

	axl_rtl_TListEx_InsertHead(&pTable->m_AvailablePageList, (axl_rtl_TListEntry*) pPage); // free page always goes to the head

	pTable->m_ppDirectory[pTable->m_PageCount] = pPage;

	pTable->m_PageCount++;
	return pPage;
}

ulong_t 
axl_rtl_THandleTable_AddHandle(
	axl_rtl_THandleTable* pTable, 
	void* pvValue
	)
{
	axl_rtl_THandleTablePage* pPage;
	ulong_t ActiveMap;
	ulong_t Mask;
	ulong_t Idx;


	pPage = (axl_rtl_THandleTablePage*) axl_rtl_TListEx_GetHead(&pTable->m_AvailablePageList);
	if (!pPage)
	{
		pPage = axl_rtl_THandleTable_AddNewPage(pTable);
		if (!pPage)
			return 0;
	}

	ActiveMap = pPage->m_Map | pPage->m_ShadowMap;
	ASSERT(ActiveMap != -1);

	Mask = (ActiveMap + 1) & ~ActiveMap;
	Idx = axl_rtl_GetLoBitIdx32(Mask);

	pPage->m_Handles[Idx] = pvValue;
	pPage->m_Map |= Mask;
	pPage->m_ShadowMap |= Mask | (Mask - 1); // mask out all the handles LESS or EQUAL than current

	if (pPage->m_ShadowMap == -1)
		pPage->m_ShadowMap = 0;
	
	pTable->m_HandleCount++;

	if (pPage->m_Map == -1)
	{
		// mov used page to tail
		axl_rtl_TListEx_Remove(&pTable->m_AvailablePageList, (axl_rtl_TListEntry*) pPage);
		axl_rtl_TListEx_InsertTail(&pTable->m_ExhaustedPageList, (axl_rtl_TListEntry*) pPage);
	}

	return pPage->m_HandleBase + Idx;
}

bool_t 
axl_rtl_THandleTable_FindHandleEx(
	axl_rtl_THandleTable* pTable, 
	ulong_t Handle, 
	void** ppvValue
	)
{
	axl_rtl_THandleTablePage* pPage;
	ulong_t Idx;

	ulong_t PageIdx = (Handle - 1) / 32;
	if (PageIdx > pTable->m_PageCount)
		return false;

	pPage = pTable->m_ppDirectory[PageIdx];

	Idx = (Handle - 1) % 32;
	if (!(pPage->m_Map & (1 << Idx)))
		return false;

	ASSERT(ppvValue);
	*ppvValue = pPage->m_Handles[Idx];
	return true;
}

void*
axl_rtl_THandleTable_FindHandle(
	axl_rtl_THandleTable* pTable,
	ulong_t Handle
	)
{
	void* pValue;
	bool_t Result = axl_rtl_THandleTable_FindHandleEx(pTable, Handle, &pValue);
	return Result ? pValue : NULL;
}

bool_t 
axl_rtl_THandleTable_RemoveHandle(
	axl_rtl_THandleTable* pTable, 
	ulong_t Handle
	)
{
	axl_rtl_THandleTablePage* pPage;
	ulong_t Mask;
	bool_t WasExhausted;

	ulong_t PageIdx = (Handle - 1) / 32;
	if (PageIdx > pTable->m_PageCount)
		return false;

	pPage = pTable->m_ppDirectory[PageIdx];

	Mask = 1 << ((Handle - 1) % 32);

	if (!(pPage->m_Map & Mask))
		return false;

	WasExhausted = pPage->m_Map == -1;
	pPage->m_Map &= ~Mask;
	pTable->m_HandleCount--;

	if (WasExhausted)
	{
		// page reclaimed
		axl_rtl_TListEx_Remove(&pTable->m_ExhaustedPageList, (axl_rtl_TListEntry*) pPage);
		axl_rtl_TListEx_InsertTail(&pTable->m_AvailablePageList, (axl_rtl_TListEntry*) pPage);
	}
	
	return true;
}

void 
axl_rtl_THandleTable_RemoveAllHandles(axl_rtl_THandleTable* pTable)
{
	axl_rtl_TListEntry* p;

	axl_rtl_TListEx_InsertListTail(&pTable->m_AvailablePageList, &pTable->m_ExhaustedPageList);

	p = axl_rtl_TListEx_GetHead(&pTable->m_AvailablePageList);
	for (; p; p = p->m_pNext)
	{
		axl_rtl_THandleTablePage* pPage = (axl_rtl_THandleTablePage*) p;
		pPage->m_Map = 0; // but keep the shadow map!
	}

	pTable->m_HandleCount = 0;
}

//.............................................................................

} // namespace rtl
} // namespace axl

