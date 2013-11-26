// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

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

#define AXL_JNC_API_STD_FUNCTION(StdFunc, Function) \
	pFunction = pModule->m_FunctionMgr.GetStdFunction (StdFunc); \
	ASSERT (pFunction); \
	pRuntime->MapFunction (pFunction->GetLlvmFunction (), pvoid_cast (Function));

#define AXL_JNC_API_PROPERTY(Name, Getter, Setter) \
	pProperty = pNamespace->GetPropertyByName (Name); \
	if (!pProperty) \
		return false; \
	pRuntime->MapFunction (pProperty->GetGetter ()->GetLlvmFunction (), pvoid_cast (Getter)); \
	pRuntime->MapFunction (pProperty->GetSetter ()->GetLlvmFunction (), pvoid_cast (Setter));

#define AXL_JNC_API_AUTOGET_PROPERTY(Name, Setter) \
	pProperty = pNamespace->GetPropertyByName (Name); \
	if (!pProperty) \
		return false; \
	pRuntime->MapFunction (pProperty->GetSetter ()->GetLlvmFunction (), pvoid_cast (Setter));

//.............................................................................

} // namespace axl
} // namespace jnc
