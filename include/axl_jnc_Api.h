// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Value.h"
#include "axl_jnc_Module.h"
#include "axl_jnc_Runtime.h"

namespace axl {
namespace jnc {

//.............................................................................

#define AXL_JNC_API_BEGIN_CLASS(Name, Class, Slot) \
static \
size_t \
GetApiClassSlot () \
{ \
	return Slot; \
} \
static \
const char* \
GetApiClassName () \
{ \
	return Name; \
} \
static \
bool \
Export (axl::jnc::CRuntime* pRuntime) \
{ \
	bool Result = true; \
	axl::jnc::CFunction* pFunction = NULL; \
	axl::jnc::CProperty* pProperty = NULL; \
	axl::jnc::CModule* pModule = pRuntime->GetModule (); \
	axl::jnc::CClassType* pType = pModule->GetApiClassType (GetApiClassSlot (), GetApiClassName ()); \
	if (!pType) \
		return false; \
	axl::jnc::CNamespace* pNamespace = pType;

#define AXL_JNC_API_END_CLASS() \
	return true; \
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_JNC_API_BEGIN_VTABLE() \
static \
void* \
GetApiClassVTable () \
{ \
	static void* VTable [] = \
	{

#define AXL_JNC_API_END_VTABLE() \
	}; \
	return VTable; \
}

#define AXL_JNC_API_VTABLE_FUNCTION(Function) \
	pvoid_cast (Function),

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_JNC_API_BEGIN_LIB() \
static \
bool \
Export (axl::jnc::CRuntime* pRuntime) \
{ \
	bool Result = true; \
	axl::jnc::CFunction* pFunction = NULL; \
	axl::jnc::CProperty* pProperty = NULL; \
	axl::jnc::CModule* pModule = pRuntime->GetModule (); \
	axl::jnc::CNamespace* pNamespace = pModule->m_NamespaceMgr.GetGlobalNamespace ();

#define AXL_JNC_API_END_LIB() \
	return true; \
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_JNC_API_LIB(Lib) \
	Result = Lib::Export (pRuntime); \
	if (!Result) \
		return false;

#define AXL_JNC_API_CLASS(Class) \
	Result = Class::Export (pRuntime); \
	if (!Result) \
		return false;

#define AXL_JNC_API_FUNCTION(Name, Function) \
	pFunction = pNamespace->GetFunctionByName (Name); \
	if (!pFunction) \
		return false; \
	pRuntime->MapFunction (pFunction->GetLlvmFunction (), pvoid_cast (Function));

#define AXL_JNC_API_STD_FUNCTION_FORCED(StdFunc, Function) \
	pFunction = pModule->m_FunctionMgr.GetStdFunction (StdFunc); \
	ASSERT (pFunction); \
	pRuntime->MapFunction (pFunction->GetLlvmFunction (), pvoid_cast (Function));

#define AXL_JNC_API_STD_FUNCTION(StdFunc, Function) \
	if (pModule->m_FunctionMgr.IsStdFunctionUsed (StdFunc)) \
	{ \
		AXL_JNC_API_STD_FUNCTION_FORCED (StdFunc, Function); \
	}

#define AXL_JNC_API_PROPERTY(Name, Getter, Setter) \
	pProperty = pNamespace->GetPropertyByName (Name); \
	if (!pProperty) \
		return false; \
	pRuntime->MapFunction (pProperty->GetGetter ()->GetLlvmFunction (), pvoid_cast (Getter)); \
	pRuntime->MapFunction (pProperty->GetSetter ()->GetLlvmFunction (), pvoid_cast (Setter));

#define AXL_JNC_API_CONST_PROPERTY(Name, Getter) \
	pProperty = pNamespace->GetPropertyByName (Name); \
	if (!pProperty) \
		return false; \
	pRuntime->MapFunction (pProperty->GetGetter ()->GetLlvmFunction (), pvoid_cast (Getter));

#define AXL_JNC_API_AUTOGET_PROPERTY(Name, Setter) \
	pProperty = pNamespace->GetPropertyByName (Name); \
	if (!pProperty) \
		return false; \
	pRuntime->MapFunction (pProperty->GetSetter ()->GetLlvmFunction (), pvoid_cast (Setter));

//.............................................................................

void
Prime (
	TIfaceHdr* pThis,
	CClassType* pType,
	void* pVTable,
	TObjHdr* pRoot = NULL
	);

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class CApiClassT: public jnc::TIfaceHdr
{
public:
	CApiClassT ()
	{
		m_pObject = NULL;
		m_pVTable = NULL;
	}

	void
	Prime (TObjHdr* pRoot = NULL)
	{
		jnc::Prime (this, GetApiClassType (), T::GetApiClassVTable ());
	}

	static
	CClassType*
	GetApiClassType ()
	{
		CModule* pModule = GetCurrentThreadModule ();
		return pModule->GetApiClassType (T::GetApiClassSlot (), T::GetApiClassName ());
	}

	// override in derived class

	// static
	// size_t
	// GetApiClassSlot ();

	// static
	// const char*
	// GetApiClassName ();

	static
	void*
	GetApiClassVTable () // optional override
	{
		return NULL;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CSimpleMulticast: public jnc::TMulticast
{
public:
	static
	CMulticastClassType*
	GetApiClassType ()
	{
		CModule* pModule = GetCurrentThreadModule ();
		return (CMulticastClassType*) pModule->m_TypeMgr.GetStdType (EStdType_SimpleMulticast);
	}
};

//.............................................................................

} // namespace axl
} // namespace jnc
