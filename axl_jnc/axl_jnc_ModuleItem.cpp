#include "stdafx.h"
#include "axl_jnc_ModuleItem.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

const tchar_t*
GetModuleItemKindString (EModuleItem ItemKind)
{
	switch (ItemKind)
	{
	case EModuleItem_Namespace:
		return _T("namespace");

	case EModuleItem_Scope:	
		return _T("scope");

	case EModuleItem_Alias:
		return _T("alias");

	case EModuleItem_Type:
		return _T("type");

	case EModuleItem_Const:
		return _T("const");

	case EModuleItem_Variable:
		return _T("variable");

	case EModuleItem_Function:
		return _T("function");

	case EModuleItem_Property:
		return _T("property");

	case EModuleItem_EnumMember:
		return _T("enum-member");

	case EModuleItem_StructMember:
		return _T("struct-member");

	case EModuleItem_UnionMember:
		return _T("union-member");

	default:
		return _T("undefined-module-item-kind");
	}
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const tchar_t*
GetStorageKindString (EStorage StorageKind)
{
	switch (StorageKind)
	{
	case EStorage_Typedef:
		return _T("typedef");

	case EStorage_Static:
		return _T("static");

	case EStorage_Virtual:
		return _T("virtual");

	case EStorage_NoVirtual:
		return _T("novirtual");

	default:
		return _T("undefined-storage-class");
	};
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const tchar_t*
GetAccessKindString (EAccess AccessKind)
{
	switch (AccessKind)
	{
	case EAccess_Public:
		return _T("public");

	case EAccess_PublicRead:
		return _T("publicread");

	case EAccess_Protected:
		return _T("protected");

	default:
		return _T("undefined-access-kind");
	};
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
