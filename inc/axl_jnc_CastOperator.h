// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Value.h"
#include "axl_rtl_SwapByteOrder.h"

namespace axl {
namespace jnc {

//.............................................................................

enum ECast
{
	ECast_None,
	ECast_Identitiy,
	ECast_Implicit,
	ECast_Explicit,
};

//.............................................................................

[uuid ("2f7742e5-396e-4444-bbc6-56eb475403bf")]
struct ICastOperator: obj::IRoot
{	
protected:
	CModule* m_pModule;
	size_t m_Price;

public:
	ICastOperator ();

	CModule*
	GetModule ()
	{
		return m_pModule;
	}

	size_t
	GetPrice ()
	{
		return m_Price;
	}

	virtual
	ECast
	GetCastKind (
		CType* pSrcType,
		CType* pDstType
		)
	{
		return ECast_Implicit;
	}

	virtual
	bool
	ConstCast (
		const CValue& SrcValue,
		const CValue& DstValue
		) = 0;

	virtual
	bool
	LlvmCast (
		const CValue& Value,
		CType* pType,
		CValue* pResultValue
		) = 0;

	bool
	Cast (
		const CValue& OpValue,
		CType* pType,
		CValue* pResultValue
		);
};

//.............................................................................

// superposition of 2 casts

class CSuperCast: 
	public ICastOperator,
	public rtl::TListLink
{
public:
	AXL_OBJ_SIMPLE_CLASS (CSuperCast, ICastOperator)

protected:
	friend class COperatorMgr;

	CType* m_pIntermediateType;

	ICastOperator* m_pFirst;
	ICastOperator* m_pSecond;

public:
	CSuperCast ();

	virtual
	ECast
	GetCastKind (
		CType* pSrcType,
		CType* pDstType
		);

	virtual
	bool
	ConstCast (
		const CValue& SrcValue,
		const CValue& DstValue
		);

	virtual
	bool
	LlvmCast (
		const CValue& Value,
		CType* pType,
		CValue* pResultValue
		);
};

//.............................................................................

// simple copy

class CCast_cpy: public ICastOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_cpy, ICastOperator)

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
		const CValue& Value,
		CType* pType,
		CValue* pResultValue
		);
};

//.............................................................................

// integer extensions, truncations & byte order swap

class CCast_int_trunc: public ICastOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_int_trunc, ICastOperator)

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
		const CValue& Value,
		CType* pType,
		CValue* pResultValue
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CCast_int_ext: public ICastOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_int_ext, ICastOperator)

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
		const CValue& Value,
		CType* pType,
		CValue* pResultValue
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CCast_int_ext_u: public ICastOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_int_ext_u, ICastOperator)

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
		const CValue& Value,
		CType* pType,
		CValue* pResultValue
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CCast_int_swp: public ICastOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_int_swp, ICastOperator)

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
		const CValue& Value,
		CType* pType,
		CValue* pResultValue
		);
};

//.............................................................................

// float extensions & truncations

class CCast_f64_f32: public ICastOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_int_ext_u, ICastOperator)

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
		const CValue& Value,
		CType* pType,
		CValue* pResultValue
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CCast_f32_f64: public ICastOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_int_ext_u, ICastOperator)

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
		const CValue& Value,
		CType* pType,
		CValue* pResultValue
		);
};

//.............................................................................

// numeric -> bool (common for both integer & fp)

class CCast_num_bool: public ICastOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_num_bool, ICastOperator)

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
		const CValue& Value,
		CType* pType,
		CValue* pResultValue
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// bool -> int

class CCast_bool_int: public ICastOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_bool_int, ICastOperator)

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
		const CValue& Value,
		CType* pType,
		CValue* pResultValue
		);
};

//.............................................................................

// int <-> floating point

class CCast_int_fp: public ICastOperator
{
public:
	virtual
	bool
	LlvmCast (
		const CValue& Value,
		CType* pType,
		CValue* pResultValue
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CCast_uint_fp: public ICastOperator
{
public:
	virtual
	bool
	LlvmCast (
		const CValue& Value,
		CType* pType,
		CValue* pResultValue
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CCast_fp_int: public ICastOperator
{
public:
	virtual
	bool
	LlvmCast (
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
	AXL_OBJ_SIMPLE_CLASS (CCast_i32_f32, ICastOperator)

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
	AXL_OBJ_SIMPLE_CLASS (CCast_i32u_f32, ICastOperator)

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
	AXL_OBJ_SIMPLE_CLASS (CCast_i32_f64, ICastOperator)

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
	AXL_OBJ_SIMPLE_CLASS (CCast_i32u_f64, ICastOperator)

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
	AXL_OBJ_SIMPLE_CLASS (CCast_i64_f32, ICastOperator)

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
	AXL_OBJ_SIMPLE_CLASS (CCast_i64u_f32, ICastOperator)

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
	AXL_OBJ_SIMPLE_CLASS (CCast_i64_f64, ICastOperator)

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
	AXL_OBJ_SIMPLE_CLASS (CCast_i64u_f64, ICastOperator)

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
	AXL_OBJ_SIMPLE_CLASS (CCast_f32_i32, ICastOperator)

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
	AXL_OBJ_SIMPLE_CLASS (CCast_f32_i64, ICastOperator)

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
	AXL_OBJ_SIMPLE_CLASS (CCast_f64_i32, ICastOperator)

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
	AXL_OBJ_SIMPLE_CLASS (CCast_f64_i64, ICastOperator)

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
