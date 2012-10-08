#include "stdafx.h"
#include "axl_pg_ClassMgr.h"

namespace axl {
namespace pg {

//.............................................................................

void 
CClass::Export (lua::CLuaState* pLuaState)
{
	pLuaState->CreateTable (0, 2);
	pLuaState->SetMemberString ("Name", m_Name);
	if (m_pBaseClass)
		pLuaState->SetMemberString ("BaseClass", m_pBaseClass->m_Name);

	pLuaState->SetMemberString ("Members", m_Members);

	pLuaState->CreateTable (0, 3);
	pLuaState->SetMemberString ("FilePath", m_SrcPos.m_FilePath);
	pLuaState->SetMemberInteger ("Line", m_SrcPos.m_Line);
	pLuaState->SetMemberInteger ("Col", m_SrcPos.m_Col);
	pLuaState->SetMember ("SrcPos");

}

//.............................................................................

CClass*
CClassMgr::GetClass (const rtl::CString& Name)
{
	rtl::CHashTableMapIteratorT <const tchar_t*, CClass*> It = m_ClassMap.Goto (Name);
	if (It->m_Value)
		return It->m_Value;

	CClass* pClass = AXL_MEM_NEW (CClass);
	pClass->m_Flags |= EClassFlag_IsNamed;
	pClass->m_Name = Name;
	m_ClassList.InsertTail (pClass);
	It->m_Value = pClass;
	return pClass;
}

CClass*
CClassMgr::CreateUnnamedClass ()
{
	CClass* pClass = AXL_MEM_NEW (CClass);
	pClass->m_Name.Format (_T("_cls%d"), m_ClassList.GetCount () + 1);
	m_ClassList.InsertTail (pClass); // don't add to class map
	return pClass;
}

void
CClassMgr::DeleteClass (CClass* pClass)
{
	if (pClass->m_Flags & EClassFlag_IsNamed)
		m_ClassMap.DeleteByKey (pClass->m_Name);

	m_ClassList.Delete (pClass);		
}

bool
CClassMgr::Verify ()
{
	rtl::CIteratorT <CClass> Class = m_ClassList.GetHead ();
	for (; Class; Class++)
	{
		CClass* pClass = *Class;

		if ((pClass->m_Flags & EClassFlag_IsUsed) && !(pClass->m_Flags & EClassFlag_IsDefined))
		{
			err::SetFormatStringError (_T("class '%s' is not defined"), pClass->m_Name);
			return false;
		}
	}

	return true;
}

void
CClassMgr::DeleteUnusedClasses ()
{
	rtl::CIteratorT <CClass> Class = m_ClassList.GetHead ();
	while (Class)
	{
		CClass* pClass = *Class++;
		if (!(pClass->m_Flags & EClassFlag_IsUsed))
			DeleteClass (pClass);
	}
}

void
CClassMgr::DeleteUnreachableClasses ()
{
	rtl::CIteratorT <CClass> Class = m_ClassList.GetHead ();
	while (Class)
	{
		CClass* pClass = *Class++;
		if (!(pClass->m_Flags & EClassFlag_IsReachable))
			DeleteClass (pClass);
	}
}

//.............................................................................

} // namespace pg {
} // namespace axl {
