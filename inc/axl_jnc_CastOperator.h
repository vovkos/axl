// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Value.h"
#include "axl_rtl_Endianness.h"

namespace axl {
namespace jnc {

//.............................................................................

enum ECast
{
	ECast_Implicit,
	ECast_Explicit,
};

//.............................................................................

[uuid ("2f7742e5-396e-4444-bbc6-56eb475403bf")]
struct ICastOperatorLo: obj::IRoot
{	
public:
	virtual
	bool
	ConstCast (
		const CValue& SrcValue, // EValue_Const
		const CValue& DstValue  // EValue_Const
		) = 0;

	virtual
	bool
	LlvmCast (
		CModule* pModule,
		const CValue& Value, // EValue_Variable or EValue_LlvmRegister
		CType* pType,
		CValue* pResultValue // EValue_Variable or EValue_LlvmRegister
		) = 0;
};

//.............................................................................

class CCastOperator: public rtl::TListLink
{
protected:
	friend class COperatorMgr;

	ECast m_CastKind;
	size_t m_Price;

	rtl::CStringA m_Signature;
	CType* m_pSrcType;
	CType* m_pDstType;
	ICastOperatorLo* m_pOperatorLo;

public:
	CCastOperator ();

	ECast
	GetCastKind ()
	{
		return m_CastKind;
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
	Cast (
		const CValue& Value,
		CType* pType,
		CValue* pResultValue
		);
};

//.............................................................................

// superposition of 2 casts

class CSuperCast: 
	public ICastOperatorLo,
	public rtl::TListLink
{
public:
	AXL_OBJ_SIMPLE_CLASS (CSuperCast, ICastOperatorLo)

protected:
	friend class COperatorMgr;

	CType* m_pIntermediateType;

	ICastOperatorLo* m_pFirst;
	ICastOperatorLo* m_pSecond;

public:
	CSuperCast ();

	virtual
	bool
	ConstCast (
		const CValue& SrcValue,
		const CValue& DstValue
		);

	virtual
	bool
	LlvmCast (
		CModule* pModule,
		const CValue& Value,
		CType* pType,
		CValue* pResultValue
		);
};

//.............................................................................

// simple copy

class CCast_cpy: public ICastOperatorLo
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_cpy, ICastOperatorLo)

public:
	virtual
	bool
	ConstCast (
		const CValue& SrcValue,
		const CValue& DstValue
		);

	virtual
	bool
	LlvmCast (
		CModule* pModule,
		const CValue& Value,
		CType* pType,
		CValue* pResultValue
		);
};

//.............................................................................

// integer extensions and truncations

class CCast_int_trunc: public ICastOperatorLo
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_int_trunc, ICastOperatorLo)

public:
	virtual
	bool
	ConstCast (
		const CValue& SrcValue,
		const CValue& DstValue
		);

	virtual
	bool
	LlvmCast (
		CModule* pModule,
		const CValue& Value,
		CType* pType,
		CValue* pResultValue
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CCast_int_ext: public ICastOperatorLo
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_int_ext, ICastOperatorLo)

public:
	virtual
	bool
	ConstCast (
		const CValue& SrcValue,
		const CValue& DstValue
		);

	virtual
	bool
	LlvmCast (
		CModule* pModule,
		const CValue& Value,
		CType* pType,
		CValue* pResultValue
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CCast_int_ext_u: public ICastOperatorLo
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_int_ext_u, ICastOperatorLo)

public:
	virtual
	bool
	ConstCast (
		const CValue& SrcValue,
		const CValue& DstValue
		);

	virtual
	bool
	LlvmCast (
		CModule* pModule,
		const CValue& Value,
		CType* pType,
		CValue* pResultValue
		);
};

//.............................................................................

// swap endianness

class CCast_i16_swp: public ICastOperatorLo
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_i16_swp, ICastOperatorLo)

public:
	virtual
	bool
	ConstCast (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		*(int16_t*) DstValue.GetConstData () = rtl::SwapEndianness16 (*(int16_t*) SrcValue.GetConstData ());
		return true;
	}

	virtual
	bool
	LlvmCast (
		CModule* pModule,
		const CValue& Value,
		CType* pType,
		CValue* pResultValue
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CCast_i32_swp: public ICastOperatorLo
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_i32_swp, ICastOperatorLo)

public:
	virtual
	bool
	ConstCast (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		*(int32_t*) DstValue.GetConstData () = rtl::SwapEndianness32 (*(int32_t*) SrcValue.GetConstData ());
		return true;
	}

	virtual
	bool
	LlvmCast (
		CModule* pModule,
		const CValue& Value,
		CType* pType,
		CValue* pResultValue
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CCast_i64_swp: public ICastOperatorLo
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_i64_swp, ICastOperatorLo)

public:
	virtual
	bool
	ConstCast (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		*(int64_t*) DstValue.GetConstData () = rtl::SwapEndianness64 (*(int64_t*) SrcValue.GetConstData ());
		return true;
	}

	virtual
	bool
	LlvmCast (
		CModule* pModule,
		const CValue& Value,
		CType* pType,
		CValue* pResultValue
		);
};

//.............................................................................

class CCast_f64_f32: public ICastOperatorLo
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_int_ext_u, ICastOperatorLo)

public:
	virtual
	bool
	ConstCast (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		*(float*) DstValue.GetConstData () = (float) *(double*) SrcValue.GetConstData ();
		return true;
	}

	virtual
	bool
	LlvmCast (
		CModule* pModule,
		const CValue& Value,
		CType* pType,
		CValue* pResultValue
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CCast_f32_f64: public ICastOperatorLo
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_int_ext_u, ICastOperatorLo)

public:
	virtual
	bool
	ConstCast (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		*(double*) DstValue.GetConstData () = *(float*) SrcValue.GetConstData ();
		return true;
	}

	virtual
	bool
	LlvmCast (
		CModule* pModule,
		const CValue& Value,
		CType* pType,
		CValue* pResultValue
		);
};

//.............................................................................

// int <-> floating point

class CCast_int_fp: public ICastOperatorLo
{
public:
	virtual
	bool
	LlvmCast (
		CModule* pModule,
		const CValue& Value,
		CType* pType,
		CValue* pResultValue
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CCast_uint_fp: public ICastOperatorLo
{
public:
	virtual
	bool
	LlvmCast (
		CModule* pModule,
		const CValue& Value,
		CType* pType,
		CValue* pResultValue
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CCast_fp_int: public ICastOperatorLo
{
public:
	virtual
	bool
	LlvmCast (
		CModule* pModule,
		const CValue& Value,
		CType* pType,
		CValue* pResultValue
		);
};

//.............................................................................

// int32 -> floating point

class CCast_i32_f32: public CCast_int_fp
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_i32_f32, ICastOperatorLo)

public:
	virtual
	bool
	ConstCast (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		*(float*) DstValue.GetConstData () = (float) *(int32_t*) SrcValue.GetConstData ();
		return true;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CCast_i32u_f32: public CCast_uint_fp
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_i32u_f32, ICastOperatorLo)

public:
	virtual
	bool
	ConstCast (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		*(float*) DstValue.GetConstData () = (float) *(uint32_t*) SrcValue.GetConstData ();
		return true;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CCast_i32_f64: public CCast_int_fp
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_i32_f64, ICastOperatorLo)

public:
	virtual
	bool
	ConstCast (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		*(double*) DstValue.GetConstData () = *(int32_t*) SrcValue.GetConstData ();
		return true;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CCast_i32u_f64: public CCast_uint_fp
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_i32u_f64, ICastOperatorLo)

public:
	virtual
	bool
	ConstCast (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		*(double*) DstValue.GetConstData () = *(uint32_t*) SrcValue.GetConstData ();
		return true;
	}
};

//.............................................................................

// int64 -> floating point

class CCast_i64_f32: public CCast_int_fp
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_i64_f32, ICastOperatorLo)

public:
	virtual
	bool
	ConstCast (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		*(float*) DstValue.GetConstData () = (float) *(int64_t*) SrcValue.GetConstData ();
		return true;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CCast_i64u_f32: public CCast_uint_fp
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_i64u_f32, ICastOperatorLo)

public:
	virtual
	bool
	ConstCast (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		*(float*) DstValue.GetConstData () = (float) *(uint64_t*) SrcValue.GetConstData ();
		return true;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CCast_i64_f64: public CCast_int_fp
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_i64_f64, ICastOperatorLo)

public:
	virtual
	bool
	ConstCast (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		*(double*) DstValue.GetConstData () = (double) *(int64_t*) SrcValue.GetConstData ();
		return true;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CCast_i64u_f64: public CCast_uint_fp
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_i64u_f64, ICastOperatorLo)

public:
	virtual
	bool
	ConstCast (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		*(double*) DstValue.GetConstData () = (double) *(uint64_t*) SrcValue.GetConstData ();
		return true;
	}
};

//.............................................................................

// float -> int 

class CCast_f32_i32: public CCast_fp_int
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_f32_i32, ICastOperatorLo)

public:
	virtual
	bool
	ConstCast (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		*(int32_t*) DstValue.GetConstData () = (int32_t) *(float*) SrcValue.GetConstData ();
		return true;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CCast_f32_i64: public CCast_fp_int
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_f32_i64, ICastOperatorLo)

public:
	virtual
	bool
	ConstCast (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		*(int64_t*) DstValue.GetConstData () = (int64_t) *(float*) SrcValue.GetConstData ();
		return true;
	}
};

//.............................................................................

// double -> int 

class CCast_f64_i32: public CCast_fp_int
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_f64_i32, ICastOperatorLo)

public:
	virtual
	bool
	ConstCast (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		*(int32_t*) DstValue.GetConstData () = (int32_t) *(double*) SrcValue.GetConstData ();
		return true;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CCast_f64_i64: public CCast_fp_int
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_f64_i64, ICastOperatorLo)

public:
	virtual
	bool
	ConstCast (
		const CValue& SrcValue,
		const CValue& DstValue
		)
	{
		*(int64_t*) DstValue.GetConstData () = (int64_t) *(double*) SrcValue.GetConstData ();
		return true;
	}
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
