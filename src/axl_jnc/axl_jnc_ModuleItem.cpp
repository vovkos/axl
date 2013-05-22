#include "pch.h"
#include "axl_jnc_ModuleItem.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

const char*
GetModuleItemKindString (EModuleItem ItemKind)
{
	static const char* StringTable [EModuleItem__Count] = 
	{
		"undefined-module-item-kind",  // EModuleItem_Undefined = 0,
		"namespace",                   // EModuleItem_Namespace,	
		"scope",                       // EModuleItem_Scope,	
		"type",                        // EModuleItem_Type,
		"typedef",                     // EModuleItem_Typedef,
		"alias",                       // EModuleItem_Alias,
		"const",                       // EModuleItem_Const,
		"variable",                    // EModuleItem_Variable,
		"function-arg",                // EModuleItem_FunctionArg,
		"function",                    // EModuleItem_Function,
		"property",                    // EModuleItem_Property,
		"property-template",           // EModuleItem_PropertyTemplate,
		"autoev",                      // EModuleItem_AutoEv,
		"enum-member",                 // EModuleItem_EnumConst,
		"struct-member",               // EModuleItem_StructField,
	};

	return (size_t) ItemKind < EModuleItem__Count ? 
		StringTable [ItemKind] : 
		StringTable [EModuleItem_Undefined];
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const char*
GetStorageKindString (EStorage StorageKind)
{
	static const char* StringTable [EStorage__Count] = 
	{
		"undefined-storage-class",  // EStorage_Undefined = 0,
		"typedef",                  // EStorage_Typedef,
		"alias",                    // EStorage_Alias,
		"static",                   // EStorage_Static,
		"thread",                   // EStorage_Thread,
		"stack",                    // EStorage_Stack,
		"heap",                     // EStorage_Heap,
		"uheap",                    // EStorage_UHeap,
		"member",                   // EStorage_Member,
		"abstract",                 // EStorage_Abstract,
		"virtual",                  // EStorage_Virtual,
		"override",                 // EStorage_Override,
	};

	return (size_t) StorageKind < EStorage__Count ? 
		StringTable [StorageKind] : 
		StringTable [EStorage_Undefined];
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const char*
GetAccessKindString (EAccess AccessKind)
{
	static const char* StringTable [EAccess__Count] = 
	{
		"undefined-access-kind", // EAccess_Undefined = 0,
		"public",                // EAccess_Public,
		"protected",             // EAccess_Protected,
	};

	return (size_t) AccessKind < EAccess__Count ? 
		StringTable [AccessKind] : 
		StringTable [EAccess_Undefined];
}

//.............................................................................

CModuleItemDecl::CModuleItemDecl ()
{
	m_StorageKind = EStorage_Undefined;
	m_AccessKind = EAccess_Public; // public by default
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
