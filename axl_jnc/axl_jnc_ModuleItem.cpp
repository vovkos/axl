#include "stdafx.h"
#include "axl_jnc_ModuleItem.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

const tchar_t*
GetModuleItemKindString (EModuleItem ItemKind)
{
	static const tchar_t* StringTable [EModuleItem__Count] = 
	{
		_T("undefined-module-item-kind"),  // EModuleItem_Undefined = 0,
		_T("namespace"),                   // EModuleItem_Namespace,	
		_T("scope"),                       // EModuleItem_Scope,	
		_T("type"),                        // EModuleItem_Type,
		_T("typedef"),                     // EModuleItem_Typedef,
		_T("alias"),                       // EModuleItem_Alias,
		_T("const"),                       // EModuleItem_Const,
		_T("variable"),                    // EModuleItem_Variable,
		_T("function"),                    // EModuleItem_Function,
		_T("property"),                    // EModuleItem_Property,
		_T("property-template"),           // EModuleItem_PropertyTemplate,
		_T("autoev"),                      // EModuleItem_AutoEv,
		_T("enum-member"),                 // EModuleItem_EnumConst,
		_T("struct-member"),               // EModuleItem_StructField,
	};

	return (size_t) ItemKind < EModuleItem__Count ? 
		StringTable [ItemKind] : 
		StringTable [EModuleItem_Undefined];
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const tchar_t*
GetStorageKindString (EStorage StorageKind)
{
	static const tchar_t* StringTable [EStorage__Count] = 
	{
		_T("undefined-storage-class"),  // EStorage_Undefined = 0,
		_T("typedef"),                  // EStorage_Typedef,
		_T("alias"),                    // EStorage_Alias,
		_T("static"),                   // EStorage_Static,
		_T("stack"),                    // EStorage_Stack,
		_T("heap"),                     // EStorage_Heap,
		_T("uheap"),                    // EStorage_UHeap,
		_T("member"),                   // EStorage_Member,
		_T("abstract"),                 // EStorage_Abstract,
		_T("virtual"),                  // EStorage_Virtual,
		_T("override"),                 // EStorage_Override,
	};

	return (size_t) StorageKind < EStorage__Count ? 
		StringTable [StorageKind] : 
		StringTable [EStorage_Undefined];
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const tchar_t*
GetAccessKindString (EAccess AccessKind)
{
	static const tchar_t* StringTable [EAccess__Count] = 
	{
		_T("undefined-access-kind"), // EAccess_Undefined = 0,
		_T("public"),                // EAccess_Public,
		_T("protected"),             // EAccess_Protected,
	};

	return (size_t) AccessKind < EAccess__Count ? 
		StringTable [AccessKind] : 
		StringTable [EAccess_Undefined];
}

//.............................................................................

CModuleItemDecl::CModuleItemDecl ()
{
	m_StorageKind = EStorage_Undefined;
	m_AccessKind = EAccess_Undefined;
	m_pParentNamespace = NULL;
	m_pAttributeBlock = NULL;
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

CModuleItem::CModuleItem ()
{
	m_pModule = NULL;
	m_ItemKind = EModuleItem_Undefined;
	m_pItemDecl = NULL;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
