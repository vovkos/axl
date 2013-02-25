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
		_T("alias"),                       // EModuleItem_Alias,
		_T("type"),                        // EModuleItem_Type,
		_T("const"),                       // EModuleItem_Const,
		_T("variable"),                    // EModuleItem_Variable,
		_T("function"),                    // EModuleItem_Function,
		_T("property"),                    // EModuleItem_Property,
		_T("enum-member"),                 // EModuleItem_EnumConst,
		_T("struct-member"),               // EModuleItem_StructField,
	};

	return ItemKind >= 0 && ItemKind < EModuleItem__Count ? 
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
		_T("static"),                   // EStorage_Static,
		_T("local"),                    // EStorage_Local,
		_T("member"),                   // EStorage_Member,
		_T("mutable"),                  // EStorage_Mutable,
		_T("abstract"),                 // EStorage_Abstract,
		_T("virtual"),                  // EStorage_Virtual,
		_T("override"),                 // EStorage_Override,
	};

	return StorageKind >= 0 && StorageKind < EStorage__Count ? 
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

	return AccessKind >= 0 && AccessKind < EAccess__Count ? 
		StringTable [AccessKind] : 
		StringTable [EAccess_Undefined];
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
