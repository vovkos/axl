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

CModuleItem::CModuleItem ()
{
	m_ItemKind = EModuleItem_Undefined;
	m_pModule = NULL;
	m_pAttributeSet = NULL;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
