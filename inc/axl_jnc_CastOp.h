// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Value.h"
#include "axl_rtl_SwapByteOrder.h"

namespace axl {
namespace jnc {

class CFunctionType;
class CFunctionPtrType;

//.............................................................................

// ordered from the worst to the best

enum ECast
{
	ECast_None,
	ECast_Lossy,
	ECast_Lossless,
	ECast_Identitiy,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

err::CError
SetCastError (
	CType* pSrcType,
	CType* pDstType
	);

//.............................................................................

[uuid ("2f7742e5-396e-4444-bbc6-56eb475403bf")]
struct ICastOperator: obj::IRoot
{	
protected:
	CModule* m_pModule;
	int m_OpFlags;
	size_t m_Price;

public:
	ICastOperator ();

	CModule*
	GetModule ()
	{
		return m_pModule;
	}

	int 
	GetOpFlags ()
	{
		return m_OpFlags;
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
		) = 0;

	virtual
	bool
	ConstCast (
		const CValue& SrcValue,
		const CValue& DstValue
		);

	virtual
	bool
	LlvmCast (
		EAlloc AllocKind,
		const CValue& Value,
		CType* pType,
		CValue* pResultValue
		) = 0;

	bool
	Cast (
		EAlloc AllocKind,
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
		EAlloc AllocKind,
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
	ECast
	GetCastKind (
		CType* pSrcType,
		CType* pDstType
		)
	{
		return pSrcType->Cmp (pDstType) == 0 ? ECast_Identitiy : ECast_Lossless;
	}

	virtual
	bool
	ConstCast (
		const CValue& SrcValue,
		const CValue& DstValue
		);

	virtual
	bool
	LlvmCast (
		EAlloc AllocKind,
		const CValue& Value,
		CType* pType,
		CValue* pResultValue
		);
};

//.............................................................................

// load reference

class CCast_load: public ICastOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_load, ICastOperator)

public:
	virtual
	ECast
	GetCastKind (
		CType* pSrcType,
		CType* pDstType
		);

	virtual
	bool
	LlvmCast (
		EAlloc AllocKind,
		const CValue& Value,
		CType* pType,
		CValue* pResultValue
		);
};

//.............................................................................

// get property

class CCast_getp: public ICastOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_getp, ICastOperator)

public:
	virtual
	ECast
	GetCastKind (
		CType* pSrcType,
		CType* pDstType
		);

	virtual
	bool
	LlvmCast (
		EAlloc AllocKind,
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
	ECast
	GetCastKind (
		CType* pSrcType,
		CType* pDstType
		)
	{
		return ECast_Lossy;
	}

	virtual
	bool
	ConstCast (
		const CValue& SrcValue,
		const CValue& DstValue
		);

	virtual
	bool
	LlvmCast (
		EAlloc AllocKind,
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
	ECast
	GetCastKind (
		CType* pSrcType,
		CType* pDstType
		)
	{
		return ECast_Lossless;
	}

	virtual
	bool
	ConstCast (
		const CValue& SrcValue,
		const CValue& DstValue
		);

	virtual
	bool
	LlvmCast (
		EAlloc AllocKind,
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
	ECast
	GetCastKind (
		CType* pSrcType,
		CType* pDstType
		)
	{
		return ECast_Lossless;
	}

	virtual
	bool
	ConstCast (
		const CValue& SrcValue,
		const CValue& DstValue
		);

	virtual
	bool
	LlvmCast (
		EAlloc AllocKind,
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
	ECast
	GetCastKind (
		CType* pSrcType,
		CType* pDstType
		)
	{
		return ECast_Lossless;
	}

	virtual
	bool
	ConstCast (
		const CValue& SrcValue,
		const CValue& DstValue
		);

	virtual
	bool
	LlvmCast (
		EAlloc AllocKind,
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
	ECast
	GetCastKind (
		CType* pSrcType,
		CType* pDstType
		)
	{
		return ECast_Lossy;
	}

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
		EAlloc AllocKind,
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
	ECast
	GetCastKind (
		CType* pSrcType,
		CType* pDstType
		)
	{
		return ECast_Lossless;
	}

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
		EAlloc AllocKind,
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
		EAlloc AllocKind,
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
		EAlloc AllocKind,
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
		EAlloc AllocKind,
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
	ECast
	GetCastKind (
		CType* pSrcType,
		CType* pDstType
		)
	{
		return ECast_Lossy;
	}

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
	ECast
	GetCastKind (
		CType* pSrcType,
		CType* pDstType
		)
	{
		return ECast_Lossy;
	}

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
	ECast
	GetCastKind (
		CType* pSrcType,
		CType* pDstType
		)
	{
		return ECast_Lossless;
	}

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
	ECast
	GetCastKind (
		CType* pSrcType,
		CType* pDstType
		)
	{
		return ECast_Lossless;
	}

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
	ECast
	GetCastKind (
		CType* pSrcType,
		CType* pDstType
		)
	{
		return ECast_Lossy;
	}

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
	ECast
	GetCastKind (
		CType* pSrcType,
		CType* pDstType
		)
	{
		return ECast_Lossy;
	}

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
	ECast
	GetCastKind (
		CType* pSrcType,
		CType* pDstType
		)
	{
		return ECast_Lossy;
	}

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
	ECast
	GetCastKind (
		CType* pSrcType,
		CType* pDstType
		)
	{
		return ECast_Lossy;
	}

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
	ECast
	GetCastKind (
		CType* pSrcType,
		CType* pDstType
		)
	{
		return ECast_Lossy;
	}

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
	ECast
	GetCastKind (
		CType* pSrcType,
		CType* pDstType
		)
	{
		return ECast_Lossy;
	}

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
	ECast
	GetCastKind (
		CType* pSrcType,
		CType* pDstType
		)
	{
		return ECast_Lossy;
	}

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
	ECast
	GetCastKind (
		CType* pSrcType,
		CType* pDstType
		)
	{
		return ECast_Lossy;
	}

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

// numeric -> bool (common for both integer & fp)

class CCast_num_bool: public ICastOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_num_bool, ICastOperator)

public:
	virtual
	ECast
	GetCastKind (
		CType* pSrcType,
		CType* pDstType
		)
	{
		return ECast_Lossless;
	}

	virtual
	bool
	ConstCast (
		const CValue& SrcValue,
		const CValue& DstValue
		);

	virtual
	bool
	LlvmCast (
		EAlloc AllocKind,
		const CValue& Value,
		CType* pType,
		CValue* pResultValue
		);
};

//.............................................................................

// bitfield <-> int

class CBitFieldType;

class CCast_getbf: public ICastOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_getbf, ICastOperator)

public:
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
		EAlloc AllocKind,
		const CValue& Value,
		CType* pType,
		CValue* pResultValue
		);

protected:
	template <
		typename T,
		EType TypeKind
		>
	bool
	CastImpl (
		const CValue& RawValue,
		CType* pDstType,
		CValue* pResultValue
		)
	{
		bool Result;

		CBitFieldType* pSrcType = (CBitFieldType*) RawValue.GetType ();
		CType* pSrcBaseType = pSrcType->GetBaseType ();
		size_t BitOffset = pSrcType->GetBitOffset ();
		size_t BitCount = pSrcType->GetBitCount ();

		T Mask = ((T) 1 << BitCount) - 1;

		CValue Value (RawValue, pSrcBaseType);
		CValue MaskValue (Mask, TypeKind);
		CValue OffsetValue (BitOffset, TypeKind);

		Result = 
			m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_Shr, &Value, OffsetValue) &&
			m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_BwAnd, &Value, MaskValue);

		if (!Result)
			return false;

		if (pSrcBaseType->IsSignedType ()) // extend with sign bit
		{
			T SignBit = (T) 1 << (BitCount - 1);

			CValue SignBitValue (SignBit, TypeKind);
			CValue OneValue (1, TypeKind);

			CValue SignExtValue;
			Result = 
				m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_BwAnd, &SignBitValue, Value) &&
				m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_Sub, SignBitValue, OneValue, &SignExtValue) &&
				m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_BwNot, &SignExtValue) &&
				m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_BwOr, &Value, SignExtValue);

			if (!Result)
				return false;
		}

		return m_pModule->m_OperatorMgr.CastOperator (Value, pDstType, pResultValue);
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CCast_setbf: public ICastOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_setbf, ICastOperator)

public:
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
		EAlloc AllocKind,
		const CValue& Value,
		CType* pType,
		CValue* pResultValue
		);

protected:
	template <
		typename T,
		EType TypeKind
		>
	bool
	CastImpl (
		const CValue& RawValue,
		CBitFieldType* pDstType,
		CValue* pResultValue
		)
	{
		CType* pDstBaseType = pDstType->GetBaseType ();
		size_t BitOffset = pDstType->GetBitOffset ();
		size_t BitCount = pDstType->GetBitCount ();

		CValue Value;

		bool Result = m_pModule->m_OperatorMgr.CastOperator (RawValue, pDstBaseType, &Value);
		if (!Result)
			return false;

		T Mask = (((T) 1 << BitCount) - 1) << BitOffset;

		CValue MaskValue (Mask, TypeKind);
		CValue OffsetValue (BitOffset, TypeKind);

		return 
			m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_Shl, &Value, OffsetValue) &&
			m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_BwAnd, Value, MaskValue, pResultValue);
	}
};

//.............................................................................

// safe / unsafe ptr -> safe / unsafe ptr

class CCast_ptr: public ICastOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_ptr, ICastOperator)

public:
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
		EAlloc AllocKind,
		const CValue& Value,
		CType* pType,
		CValue* pResultValue
		);

protected:
	bool
	LlvmCast_ptr (
		const CValue& Value,
		intptr_t Offset,
		rtl::CArrayT <size_t>* pLlvmIndexArray,
		CDataPtrType* pPointerType,
		CValue* pResultValue
		);

	bool
	LlvmCast_ptr_u (
		const CValue& Value,
		intptr_t Offset,
		rtl::CArrayT <size_t>* pLlvmIndexArray,
		CDataPtrType* pPointerType,
		CValue* pResultValue
		);
};

//.............................................................................

// arr -> arr

class CCast_arr: public ICastOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_arr, ICastOperator)

public:
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
		EAlloc AllocKind,
		const CValue& Value,
		CType* pType,
		CValue* pResultValue
		);
};

//.............................................................................

// arr -> safe / unsafe ptr

class CCast_arr_ptr: public ICastOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_arr_ptr, ICastOperator)

public:
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
		EAlloc AllocKind,
		const CValue& Value,
		CType* pType,
		CValue* pResultValue
		);
};

//.............................................................................

// safe / unsafe fn pointer -> safe fn pointer

class CCast_fn: public ICastOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_fn, ICastOperator)

public:
	virtual
	ECast
	GetCastKind (
		CType* pSrcType,
		CType* pDstType
		);

	virtual
	bool
	LlvmCast (
		EAlloc AllocKind,
		const CValue& Value,
		CType* pType,
		CValue* pResultValue
		);

protected:
	// case 1: no conversion required, no closure object needs to be created

	bool
	LlvmCast_case1 (
		const CValue& PfnValue,
		const CValue& IfaceValue,
		CFunctionType* pSrcFunctionType,
		CFunctionPtrType* pFunctionPtrType,
		CValue* pResultValue
		);

	// case 2: conversion is required, but no closure object needs to be created

	bool
	LlvmCast_case2 (
		CFunction* pFunction,
		const CValue& IfaceValue,
		CFunctionPtrType* pFunctionPtrType,
		CValue* pResultValue
		);

	// case 3: closure object needs to be created (so conversion is required even if function signatures match)

	bool
	LlvmCast_case3 (
		EAlloc AllocKind,
		const CValue& PfnValue,
		CClosure* pClosure,
		CFunctionType* pSrcFunctionType,
		CFunctionPtrType* pFunctionPtrType,
		CValue* pResultValue
		);
};

//.............................................................................

// property / property pointer / variable reference -> property pointer

class CCast_prop: public ICastOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_prop, ICastOperator)

public:
	virtual
	ECast
	GetCastKind (
		CType* pSrcType,
		CType* pDstType
		);

	virtual
	bool
	LlvmCast (
		EAlloc AllocKind,
		const CValue& Value,
		CType* pType,
		CValue* pResultValue
		);
};

//.............................................................................

// iface / class -> iface / class

class CCast_class: public ICastOperator
{
public:
	AXL_OBJ_SIMPLE_CLASS (CCast_class, ICastOperator)

public:
	virtual
	ECast
	GetCastKind (
		CType* pSrcType,
		CType* pDstType
		);

	virtual
	bool
	LlvmCast (
		EAlloc AllocKind,
		const CValue& Value,
		CType* pType,
		CValue* pResultValue
		);
};

//.............................................................................

} // namespace jnc {
} // namespace axl {


