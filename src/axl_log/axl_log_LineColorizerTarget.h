// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_LINECOLORIZERTARGET_H

#include "axl_log_Colorizer.h"
#include "axl_log_BinLine.h"

namespace axl {
namespace log {
	
class CLineRepresenterTarget;

//.............................................................................

class CLineColorizerTarget: public IColorizerTarget
{
	friend class CLineRepresenterTarget;

public:
	AXL_OBJ_CLASS_0 (CLineColorizerTarget, IColorizerTarget);

protected:
	CBinLine* m_pLine;

public:
	virtual
	void 
	SetAttr (
		uint64_t BeginOffset, 
		uint64_t EndOffset, 
		const gui::TTextAttr& Attr
		);
};

//.............................................................................

} // namespace log
} // namespace axl
