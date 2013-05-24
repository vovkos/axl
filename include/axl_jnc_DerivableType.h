// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_NamedType.h"

namespace axl {
namespace jnc {

class CDerivableType;
class CStructType;
class CClassType;
class CFunction;
class CProperty;

//.............................................................................

enum EBaseTypeFlag
{
	EBaseTypeFlag_Constructed = 1
};

//.............................................................................

class CBaseType: public rtl::TListLink
{
	friend class CDerivableType;
	friend class CStructType;
	friend class CClassType;

protected:
	CDerivableType* m_pType;

	int m_Flags;
	size_t m_Offset;
	size_t m_VTableIndex;
	uint_t m_LlvmIndex;

public:
	CBaseType ();

	int 
	GetFlags ()
	{
		return m_Flags;
	}

	CDerivableType*
	GetType ()
	{
		return m_pType;
	}

	size_t 
	GetOffset ()
	{
		return m_Offset;
	}

	size_t
	GetVTableIndex ()
	{
		return m_VTableIndex;
	}

	uint_t
	GetLlvmIndex ()
	{
		return m_LlvmIndex;
	}
	
	void
	MarkConstructed ()
	{
		m_Flags |= EBaseTypeFlag_Constructed;
	}
};

//.............................................................................

class CBaseTypeCoord
{
protected:
	char m_Buffer [256];

public:
	CDerivableType* m_pType;
	size_t m_Offset;
	rtl::CArrayT <int32_t> m_LlvmIndexArray;
	size_t m_VTableIndex;

public:
	CBaseTypeCoord ()
	{
		Init ();
	}

	CBaseTypeCoord (const CBaseTypeCoord& Coord)
	{
		Init ();
		*this = Coord;
	}

protected:
	void
	Init ();
};

//.............................................................................

class CDerivableType: public CNamedType
{
protected:
	rtl::CStringHashTableMapAT <CBaseType*> m_BaseTypeMap;
	rtl::CStdListT <CBaseType> m_BaseTypeList;
	
	// construction

	CFunction* m_pConstructor;
	CFunction* m_pStaticConstructor;
	CFunction* m_pStaticDestructor;

	CVariable* m_pStaticConstructorFlag; // 'once' semantics

	// overloaded operators

	rtl::CArrayT <CFunction*> m_UnaryOperatorTable;
	rtl::CArrayT <CFunction*> m_BinaryOperatorTable;
	rtl::CStringHashTableMapAT <CFunction*> m_CastOperatorMap;

public:
	CDerivableType ();

	rtl::CConstListT <CBaseType>
	GetBaseTypeList ()
	{
		return m_BaseTypeList;
	}

	CBaseType*
	AddBaseType (CDerivableType* pType);

	CBaseType*
	FindBaseType (CType* pType)
	{
		rtl::CStringHashTableMapIteratorAT <CBaseType*> It = m_BaseTypeMap.Find (pType->GetSignature ());
		return It ? It->m_Value : NULL;
	}

	bool
	FindBaseTypeTraverse (
		CType* pType,
		CBaseTypeCoord* pCoord = NULL
		)
	{
		return FindBaseTypeTraverseImpl (pType, pCoord, 0);
	}

	void
	ClearAllBaseTypeConstructedFlags ();

	CFunction* 
	GetConstructor ()
	{
		return m_pConstructor;
	}

	CFunction* 
	GetStaticConstructor ()
	{
		return m_pStaticConstructor;
	}

	CFunction* 
	GetStaticDestructor ()
	{
		return m_pStaticDestructor;
	}

	bool
	AddMethod (CFunction* pFunction);

	bool
	AddProperty (CProperty* pProperty);

protected:
	bool
	CreateDefaultConstructor ();

	bool
	FindBaseTypeTraverseImpl (
		CType* pType,
		CBaseTypeCoord* pCoord,
		size_t Level
		);

	virtual
	CModuleItem*
	FindItemTraverseImpl (
		const char* pName,
		CBaseTypeCoord* pCoord = NULL,
		uint_t Flags = 0
		)
	{
		return FindItemTraverseImpl (pName, pCoord, Flags, 0);
	}

	CModuleItem*
	FindItemTraverseImpl (
		const char* pName,
		CBaseTypeCoord* pCoord,
		uint_t Flags,
		size_t BaseTypeLevel
		);
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
