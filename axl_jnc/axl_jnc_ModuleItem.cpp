#include "stdafx.h"
#include "axl_jnc_ModuleItem.h"

namespace axl {
namespace jnc {

//.............................................................................

const tchar_t*
CModuleItem::GetItemKindString (EModuleItem ItemKind)
{
	switch (ItemKind)
	{
	case EModuleItem_Namespace:
		return _T("namespace");

	case EModuleItem_Scope:	
		return _T("scope");

	case EModuleItem_Alias:
		return _T("alias");

	case EModuleItem_Variable:
		return _T("variable");

	case EModuleItem_Function:
		return _T("function");

	case EModuleItem_GlobalFunction:
		return _T("global-function");

	case EModuleItem_Type:
		return _T("type");

	case EModuleItem_Const:
		return _T("const");

	default:
		return _T("<undefined>");
	}
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
