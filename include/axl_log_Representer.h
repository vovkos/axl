// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_REPRESENTER_H

#include "axl_log_MergeCriteria.h"
#include "axl_log_Line.h"
#include "axl_rtl_Array.h"

namespace axl {
namespace log {

//.............................................................................

enum EPart
{
	EPart_PlainText = 0,
	EPart_HyperText,
	EPart_Bin,
}; 

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
ELine 
GetLineKindFromPartKind (
	EPart PartKind,
	EBinView BinViewKind
	)
{
	return 
		PartKind != EPart_Bin ? ELine_Text : 
		BinViewKind == EBinView_Text ? ELine_BinText : ELine_BinHex;
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
ELine 
GetLineKindFromPartKind (
	EPart PartKind,
	const TBinDataConfig& BinDataConfig
	)
{
	return GetLineKindFromPartKind (PartKind, (EBinView) BinDataConfig.m_BinViewKind);
}

//.............................................................................

struct IRepresenterTarget
{
protected:
	uint_t m_PacketCode;
	uint64_t m_Timestamp;
	TBinDataConfig m_BinDataConfig;

public:
	TLineAttr m_LineAttr;
	
public:
	IRepresenterTarget ()
	{
		m_PacketCode = 0;
		m_Timestamp = 0;
	}

	uint_t 
	GetPacketCode ()
	{
		return m_PacketCode;
	}

	uint64_t 
	GetTimestamp ()
	{
		return m_Timestamp;
	}

	const TBinDataConfig*
	GetBinDataConfig ()
	{
		return &m_BinDataConfig;
	}

	virtual 
	void 
	AddPart (
		EPart PartKind,
		uint_t PartCode,
		const void* p,
		size_t Size
		) = 0;

	virtual 
	void 
	Break () = 0;

	void
	AddPlainText0 (
		const char* pText,
		size_t Length = -1
		)
	{
		AddText0 (
			EPart_PlainText, 
			m_PacketCode, 
			pText, 
			Length
			);
	}

	void
	AddPlainText0 (
		uint_t PartCode,
		const char* pText,
		size_t Length = -1
		)
	{
		AddText0 (
			EPart_PlainText, 
			PartCode, 
			pText, 
			Length
			);
	}

	void
	AddPlainText (
		const char* pFormat,
		...
		)
	{
		AXL_VA_DECL (va, pFormat);

		AddText_va (
			EPart_PlainText, 
			m_PacketCode, 
			pFormat, 
			va
			);
	}

	void
	AddPlainText (
		uint_t PartCode,
		const char* pFormat,
		...
		)
	{
		AXL_VA_DECL (va, pFormat);

		AddText_va (
			EPart_PlainText, 
			PartCode, 
			pFormat, 
			va
			);
	}

	void
	AddPlainText_va (
		const char* pFormat,
		axl_va_list va
		)
	{
		AddText_va (
			EPart_PlainText, 
			m_PacketCode, 
			pFormat, 
			va
			);
	}

	void
	AddPlainText_va (
		uint_t PartCode,
		const char* pFormat,
		axl_va_list va
		)
	{
		AddText_va (
			EPart_PlainText, 
			PartCode, 
			pFormat, 
			va
			);
	}

	void
	AddHyperText0 (
		const char* pText,
		size_t Length = -1
		)
	{
		AddText0 (
			EPart_HyperText, 
			m_PacketCode, 
			pText, 
			Length
			);
	}

	void
	AddHyperText0 (
		uint_t PartCode,
		const char* pText,
		size_t Length = -1
		)
	{
		AddText0 (
			EPart_HyperText, 
			PartCode, 
			pText, 
			Length
			);
	}

	void
	AddHyperText (
		const char* pFormat,
		...
		)
	{
		AXL_VA_DECL (va, pFormat);

		AddText_va (
			EPart_HyperText, 
			m_PacketCode, 
			pFormat, 
			va
			);
	}

	void
	AddHyperText (
		uint_t PartCode,
		const char* pFormat,
		...
		)
	{
		AXL_VA_DECL (va, pFormat);

		AddText_va (
			EPart_HyperText, 
			PartCode, 
			pFormat, 
			va
			);
	}

	void
	AddHyperText_va (
		const char* pFormat,
		axl_va_list va
		)
	{
		AddText_va (
			EPart_HyperText, 
			m_PacketCode, 
			pFormat, 
			va
			);
	}

	void
	AddHyperText_va (
		uint_t PartCode,
		const char* pFormat,
		axl_va_list va
		)
	{
		AddText_va (
			EPart_HyperText, 
			PartCode, 
			pFormat, 
			va
			);
	}

	void
	AddBin (
		const void* p,
		size_t Size
		)
	{
		AddPart (
			EPart_Bin, 
			m_PacketCode, 
			p, 
			Size
			);
	}

	void
	AddBin (
		uint_t PartCode,
		const void* p,
		size_t Size
		)
	{
		AddPart (
			EPart_Bin, 
			PartCode, 
			p, 
			Size
			);
	}

protected:
	void
	AddText0 (
		EPart PartKind,
		uint_t PartCode,
		const char* pText,
		size_t Length
		);

	void
	AddText_va (
		EPart PartKind,
		uint_t PartCode,
		const char* pFormat,
		axl_va_list va
		);
};

//.............................................................................

struct IRepresenter
{
	virtual 
	bool 
	Represent (
		IRepresenterTarget* pTarget, 
		uint_t PacketCode, 
		const void* p, 
		size_t Size, 
		uint64_t FoldFlags
		) = 0;
};

//.............................................................................

} // namespace log 
} // namespace axl
