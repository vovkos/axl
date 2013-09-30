// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_COLORIZERSTATECALCTARGET_H

#include "axl_log_Colorizer.h"
#include "axl_log_Representer.h"
#include "axl_log_MergeCriteria.h"
#include "axl_log_PacketFile.h"
#include "axl_log_Protocol.h"

namespace axl {
namespace log {
	
//.............................................................................

class CColorizerStateCalcTarget: 
	public IColorizerTarget,
	public IRepresenterTarget
{
	friend class CColorizeMgr;

public:
	AXL_OBJ_BEGIN_CLASS_0 (CColorizerStateCalcTarget)
		AXL_OBJ_INTERFACE_ENTRY (IColorizerTarget)
		AXL_OBJ_INTERFACE_ENTRY (IRepresenterTarget)
	AXL_OBJ_END_CLASS ()

protected:
	IColorizer* m_pColorizer;
	void* m_pColorizerState;
	TMergeCriteria m_MergeCriteria;

public:
	CColorizerStateCalcTarget ();

	void
	StartPacket (const TPacket* pPacket);

	// IRepresenter

	virtual 
	void 
	AddPart (
		EPart PartKind,
		uint_t PartCode,
		const void* p,
		size_t Size
		);

	virtual 
	void 
	Break ()
	{
		m_MergeCriteria.Break ();
	}

	// IColorizer

	virtual
	void 
	SetAttr (
		uint64_t BeginOffset, 
		uint64_t EndOffset,
		const gui::TTextAttr& Attr
		)
	{
		 // ignore actual set-color, we are calculating state
	}
};

//.............................................................................

} // namespace log 
} // namespace axl
