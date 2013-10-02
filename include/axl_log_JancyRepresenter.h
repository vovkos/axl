// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_JANCYREPRESENTER_H

#include "axl_log_Representer.h"
#include "axl_jnc_ClassType.h"
#include "axl_jnc_Runtime.h"

namespace axl {
namespace log {

struct TJancyRepresenterTarget;
struct TJancyRepresenter;

//.............................................................................

typedef 
void
(*FJancyRepresenterTarget_AddPart) (
	TJancyRepresenterTarget* pTarget, 
	EPart PartKind,
	uint_t PartCode,
	jnc::TDataPtr Ptr,
	size_t Size
	);

struct TJancyRepresenterTargetVTable
{
	FJancyRepresenterTarget_AddPart m_pfAddPart;
};

struct TJancyRepresenterTarget: jnc::TInterface
{
	uint_t m_PacketCode;
	uint64_t m_Timestamp;
	TLineAttr m_LineAttr;

	IRepresenterTarget* m_pActualTarget;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef 
bool 
(*FJancyRepresenter_Represent) (
	TJancyRepresenter* pRepresenter,
	TJancyRepresenterTarget* pTarget, 
	uint_t PacketCode, 
	jnc::TDataPtr Ptr,
	size_t Size, 
	uint64_t FoldFlags
	);

struct TJancyRepresenterVTable
{
	FJancyRepresenter_Represent m_pfRepresent;
};

struct TJancyRepresenter: jnc::TInterface
{
};

//.............................................................................
	
class CJancyRepresenter: public IRepresenter
{
public:
	AXL_OBJ_CLASS_0 (CJancyRepresenter, IRepresenter)

protected:
	TJancyRepresenter* m_pJancyRepresenter;
	jnc::CClassType* m_pJancyRepresenterTargetType;
	jnc::CRuntime* m_pRuntime;

	static TJancyRepresenterTargetVTable s_JancyRepresenterTargetVTable;

public:
	CJancyRepresenter ();
	
	~CJancyRepresenter ()
	{
		Destroy ();
	}

	bool
	Create (
		jnc::CClassType* pType,
		jnc::CRuntime* pRuntime
		);

	void
	Destroy ();

	virtual 
	bool 
	Represent (
		IRepresenterTarget* pTarget, 
		uint_t PacketCode, 
		const void* p, 
		size_t Size, 
		uint64_t FoldFlags
		);

protected:
	static
	void
	JancyRepresenterTarget_AddPart (
		TJancyRepresenterTarget* pTarget, 
		EPart PartKind,
		uint_t PartCode,
		jnc::TDataPtr Ptr,
		size_t Size
		);
};

//.............................................................................

} // namespace log
} // namespace axl
