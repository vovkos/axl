// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Value.h"
#include "axl_rtl_Endianness.h"

namespace axl {
namespace jnc {

//.............................................................................

enum ECanMove
{
	ECanMove_None = 0,
	ECanMove_Explicit,
	ECanMove_Implicit,
};

//.............................................................................

[uuid ("2f7742e5-396e-4444-bbc6-56eb475403bf")]
struct IMoveOperatorLo: obj::IRoot
{	
public:
	virtual
	bool
	ConstMove (
		const CValue& SrcValue,
		const CValue& DstValue
		) = 0;

	virtual
	bool
	LlvmMove (
		const CValue& SrcValue,
		const CValue& DstValue
		) = 0;
};

//.............................................................................

class CMoveOperator: public rtl::TListLink
{
protected:
	friend class COperatorMgr;

	ECanMove m_CanMove;
	size_t m_Price;

	rtl::CStringA m_Signature;
	CType* m_pSrcType;
	CType* m_pDstType;
	IMoveOperatorLo* m_pOperatorLo;

public:
	CMoveOperator ();

	ECanMove
	GetCanMove ()
	{
		return m_CanMove;
	}

	size_t
	GetPrice ()
	{
		return m_Price;
	}

	CType* 
	GetSrcType () 
	{
		return m_pSrcType;
	}

	CType* 
	GetDstType ()
	{
		return m_pDstType;
	}

	bool
	Move (
		const CValue& SrcValue,
		const CValue& DstValue
		);
};

//.............................................................................

// superposition of moves

class CSuperMove: 
	public IMoveOperatorLo,
	public rtl::TListLink
{
public:
	AXL_OBJ_SIMPLE_CLASS (CSuperMove, IMoveOperatorLo)

protected:
	friend class COperatorMgr;

	CMoveOperator* m_pFirst;
	CMoveOperator* m_pSecond;

public:
	CSuperMove ()
	{
		m_pFirst = NULL;
		m_pSecond = NULL;
	}

	virtual
	bool
	ConstMove (
		const CValue& SrcValue,
		const CValue& DstValue
		);

	virtual
	bool
	LlvmMove (
		const CValue& SrcValue,
		const CValue& DstValue
		);
};

//.............................................................................

// simple moves

class CMove_8: public IMoveOperatorLo
{
public:
	AXL_OBJ_SIMPLE_CLASS (CMove_8, IMoveOperatorLo)

public:
	virtual
	bool
	ConstMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		*(int8_t*) DstValue.GetConstData () = *(int8_t*) SrcValue.GetConstData ();
		return true;
	}

	virtual
	bool
	LlvmMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		return true;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CMove_16: public IMoveOperatorLo
{
public:
	AXL_OBJ_SIMPLE_CLASS (CMove_16, IMoveOperatorLo)

public:
	virtual
	bool
	ConstMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		*(int16_t*) DstValue.GetConstData () = *(int16_t*) SrcValue.GetConstData ();
		return true;
	}

	virtual
	bool
	LlvmMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		return true;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CMove_32: public IMoveOperatorLo
{
public:
	AXL_OBJ_SIMPLE_CLASS (CMove_32, IMoveOperatorLo)

public:
	virtual
	bool
	ConstMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		*(int32_t*) DstValue.GetConstData () = *(int32_t*) SrcValue.GetConstData ();
		return true;
	}

	virtual
	bool
	LlvmMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		return true;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CMove_64: public IMoveOperatorLo
{
public:
	AXL_OBJ_SIMPLE_CLASS (CMove_64, IMoveOperatorLo)

public:
	virtual
	bool
	ConstMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		*(int64_t*) DstValue.GetConstData () = *(int64_t*) SrcValue.GetConstData ();
		return true;
	}

	virtual
	bool
	LlvmMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		return true;
	}
};

//.............................................................................

// swap endianness

class CMove_i16_swp: public IMoveOperatorLo
{
public:
	AXL_OBJ_SIMPLE_CLASS (CMove_i16_swp, IMoveOperatorLo)

public:
	virtual
	bool
	ConstMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		*(int16_t*) DstValue.GetConstData () = rtl::SwapEndianness16 (*(int16_t*) SrcValue.GetConstData ());
		return true;
	}

	virtual
	bool
	LlvmMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		return true;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CMove_i32_swp: public IMoveOperatorLo
{
public:
	AXL_OBJ_SIMPLE_CLASS (CMove_i32_swp, IMoveOperatorLo)

public:
	virtual
	bool
	ConstMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		*(int32_t*) DstValue.GetConstData () = rtl::SwapEndianness32 (*(int32_t*) SrcValue.GetConstData ());
		return true;
	}

	virtual
	bool
	LlvmMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		return true;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CMove_i64_swp: public IMoveOperatorLo
{
public:
	AXL_OBJ_SIMPLE_CLASS (CMove_i64_swp, IMoveOperatorLo)

public:
	virtual
	bool
	ConstMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		*(int64_t*) DstValue.GetConstData () = rtl::SwapEndianness64 (*(int64_t*) SrcValue.GetConstData ());
		return true;
	}

	virtual
	bool
	LlvmMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		return true;
	}
};

//.............................................................................

// int8 extensions

class CMove_i8_16: public IMoveOperatorLo
{
public:
	AXL_OBJ_SIMPLE_CLASS (CMove_i8_16, IMoveOperatorLo)

public:
	virtual
	bool
	ConstMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		*(int16_t*) DstValue.GetConstData () = *(int8_t*) SrcValue.GetConstData ();
		return true;
	}

	virtual
	bool
	LlvmMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		return true;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CMove_i8u_16: public IMoveOperatorLo
{
public:
	AXL_OBJ_SIMPLE_CLASS (CMove_i8u_16, IMoveOperatorLo)

public:
	virtual
	bool
	ConstMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		*(int16_t*) DstValue.GetConstData () = *(uint8_t*) SrcValue.GetConstData ();
		return true;
	}

	virtual
	bool
	LlvmMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		return true;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CMove_i8_32: public IMoveOperatorLo
{
public:
	AXL_OBJ_SIMPLE_CLASS (CMove_i8_32, IMoveOperatorLo)

public:
	virtual
	bool
	ConstMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		*(int32_t*) DstValue.GetConstData () = *(int8_t*) SrcValue.GetConstData ();
		return true;
	}

	virtual
	bool
	LlvmMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		return true;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CMove_i8u_32: public IMoveOperatorLo
{
public:
	AXL_OBJ_SIMPLE_CLASS (CMove_i8u_32, IMoveOperatorLo)

public:
	virtual
	bool
	ConstMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		*(int32_t*) DstValue.GetConstData () = *(uint8_t*) SrcValue.GetConstData ();
		return true;
	}

	virtual
	bool
	LlvmMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		return true;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CMove_i8_64: public IMoveOperatorLo
{
public:
	AXL_OBJ_SIMPLE_CLASS (CMove_i8_64, IMoveOperatorLo)

public:
	virtual
	bool
	ConstMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		*(int64_t*) DstValue.GetConstData () = *(int8_t*) SrcValue.GetConstData ();
		return true;
	}

	virtual
	bool
	LlvmMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		return true;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CMove_i8u_64: public IMoveOperatorLo
{
public:
	AXL_OBJ_SIMPLE_CLASS (CMove_i8u_64, IMoveOperatorLo)

public:
	virtual
	bool
	ConstMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		*(int64_t*) DstValue.GetConstData () = *(uint8_t*) SrcValue.GetConstData ();
		return true;
	}

	virtual
	bool
	LlvmMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		return true;
	}
};

//.............................................................................

// int16 extensions

class CMove_i16_32: public IMoveOperatorLo
{
public:
	AXL_OBJ_SIMPLE_CLASS (CMove_i16_32, IMoveOperatorLo)

public:
	virtual
	bool
	ConstMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		*(int32_t*) DstValue.GetConstData () = *(int16_t*) SrcValue.GetConstData ();
		return true;
	}

	virtual
	bool
	LlvmMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		return true;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CMove_i16u_32: public IMoveOperatorLo
{
public:
	AXL_OBJ_SIMPLE_CLASS (CMove_i16u_32, IMoveOperatorLo)

public:
	virtual
	bool
	ConstMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		*(int32_t*) DstValue.GetConstData () = *(uint16_t*) SrcValue.GetConstData ();
		return true;
	}

	virtual
	bool
	LlvmMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		return true;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CMove_i16_64: public IMoveOperatorLo
{
public:
	AXL_OBJ_SIMPLE_CLASS (CMove_i16_64, IMoveOperatorLo)

public:
	virtual
	bool
	ConstMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		*(int64_t*) DstValue.GetConstData () = *(int16_t*) SrcValue.GetConstData ();
		return true;
	}

	virtual
	bool
	LlvmMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		return true;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CMove_i16u_64: public IMoveOperatorLo
{
public:
	AXL_OBJ_SIMPLE_CLASS (CMove_i16u_64, IMoveOperatorLo)

public:
	virtual
	bool
	ConstMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		*(int64_t*) DstValue.GetConstData () = *(uint16_t*) SrcValue.GetConstData ();
		return true;
	}

	virtual
	bool
	LlvmMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		return true;
	}
};

//.............................................................................

// int32 extensions

class CMove_i32_64: public IMoveOperatorLo
{
public:
	AXL_OBJ_SIMPLE_CLASS (CMove_i32_64, IMoveOperatorLo)

public:
	virtual
	bool
	ConstMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		*(int64_t*) DstValue.GetConstData () = *(int32_t*) SrcValue.GetConstData ();
		return true;
	}

	virtual
	bool
	LlvmMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		return true;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CMove_i32u_64: public IMoveOperatorLo
{
public:
	AXL_OBJ_SIMPLE_CLASS (CMove_i32u_64, IMoveOperatorLo)

public:
	virtual
	bool
	ConstMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		*(int64_t*) DstValue.GetConstData () = *(uint32_t*) SrcValue.GetConstData ();
		return true;
	}

	virtual
	bool
	LlvmMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		return true;
	}
};

//.............................................................................

// int32 -> floating point

class CMove_i32_f32: public IMoveOperatorLo
{
public:
	AXL_OBJ_SIMPLE_CLASS (CMove_i32_f32, IMoveOperatorLo)

public:
	virtual
	bool
	ConstMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		*(float*) DstValue.GetConstData () = (float) *(int32_t*) SrcValue.GetConstData ();
		return true;
	}

	virtual
	bool
	LlvmMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		return true;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CMove_i32u_f32: public IMoveOperatorLo
{
public:
	AXL_OBJ_SIMPLE_CLASS (CMove_i32u_f32, IMoveOperatorLo)

public:
	virtual
	bool
	ConstMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		*(float*) DstValue.GetConstData () = (float) *(uint32_t*) SrcValue.GetConstData ();
		return true;
	}

	virtual
	bool
	LlvmMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		return true;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CMove_i32_f64: public IMoveOperatorLo
{
public:
	AXL_OBJ_SIMPLE_CLASS (CMove_i32_f64, IMoveOperatorLo)

public:
	virtual
	bool
	ConstMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		*(double*) DstValue.GetConstData () = *(int32_t*) SrcValue.GetConstData ();
		return true;
	}

	virtual
	bool
	LlvmMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		return true;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CMove_i32u_f64: public IMoveOperatorLo
{
public:
	AXL_OBJ_SIMPLE_CLASS (CMove_i32u_f64, IMoveOperatorLo)

public:
	virtual
	bool
	ConstMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		*(double*) DstValue.GetConstData () = *(uint32_t*) SrcValue.GetConstData ();
		return true;
	}

	virtual
	bool
	LlvmMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		return true;
	}
};

//.............................................................................

// int64 -> floating point

class CMove_i64_f32: public IMoveOperatorLo
{
public:
	AXL_OBJ_SIMPLE_CLASS (CMove_i64_f32, IMoveOperatorLo)

public:
	virtual
	bool
	ConstMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		*(float*) DstValue.GetConstData () = (float) *(int64_t*) SrcValue.GetConstData ();
		return true;
	}

	virtual
	bool
	LlvmMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		return true;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CMove_i64u_f32: public IMoveOperatorLo
{
public:
	AXL_OBJ_SIMPLE_CLASS (CMove_i64u_f32, IMoveOperatorLo)

public:
	virtual
	bool
	ConstMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		*(float*) DstValue.GetConstData () = (float) *(uint64_t*) SrcValue.GetConstData ();
		return true;
	}

	virtual
	bool
	LlvmMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		return true;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CMove_i64_f64: public IMoveOperatorLo
{
public:
	AXL_OBJ_SIMPLE_CLASS (CMove_i64_f64, IMoveOperatorLo)

public:
	virtual
	bool
	ConstMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		*(double*) DstValue.GetConstData () = (double) *(int64_t*) SrcValue.GetConstData ();
		return true;
	}

	virtual
	bool
	LlvmMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		return true;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CMove_i64u_f64: public IMoveOperatorLo
{
public:
	AXL_OBJ_SIMPLE_CLASS (CMove_i64u_f64, IMoveOperatorLo)

public:
	virtual
	bool
	ConstMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		*(double*) DstValue.GetConstData () = (double) *(uint64_t*) SrcValue.GetConstData ();
		return true;
	}

	virtual
	bool
	LlvmMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		return true;
	}
};

//.............................................................................

// float -> int 

class CMove_f32_i32: public IMoveOperatorLo
{
public:
	AXL_OBJ_SIMPLE_CLASS (CMove_f32_i32, IMoveOperatorLo)

public:
	virtual
	bool
	ConstMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		*(int32_t*) DstValue.GetConstData () = (int32_t) *(float*) SrcValue.GetConstData ();
		return true;
	}

	virtual
	bool
	LlvmMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		return true;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CMove_f32_i64: public IMoveOperatorLo
{
public:
	AXL_OBJ_SIMPLE_CLASS (CMove_f32_i64, IMoveOperatorLo)

public:
	virtual
	bool
	ConstMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		*(int64_t*) DstValue.GetConstData () = (int64_t) *(float*) SrcValue.GetConstData ();
		return true;
	}

	virtual
	bool
	LlvmMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		return true;
	}
};

//.............................................................................

// double -> int 

class CMove_f64_i32: public IMoveOperatorLo
{
public:
	AXL_OBJ_SIMPLE_CLASS (CMove_f64_i32, IMoveOperatorLo)

public:
	virtual
	bool
	ConstMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		*(int32_t*) DstValue.GetConstData () = (int32_t) *(double*) SrcValue.GetConstData ();
		return true;
	}

	virtual
	bool
	LlvmMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		return true;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CMove_f64_i64: public IMoveOperatorLo
{
public:
	AXL_OBJ_SIMPLE_CLASS (CMove_f64_i64, IMoveOperatorLo)

public:
	virtual
	bool
	ConstMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		*(int64_t*) DstValue.GetConstData () = (int64_t) *(double*) SrcValue.GetConstData ();
		return true;
	}

	virtual
	bool
	LlvmMove (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		return true;
	}
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
