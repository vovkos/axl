// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_REF_CLONE_H

#include "axl_ref_RtFactory.h"

namespace axl {
namespace ref {

//.............................................................................

template <typename I>
CPtrT <I>
Clone (I* pSrc)
{
	obj::IType* pType;
	void* pSrcObject = pSrc->GetObject (&pType);

	size_t Offset = (uchar_t*) pSrc - (uchar_t*) pSrcObject;

	CPtrT <void> Result = AXL_REF_RT_NEW (pType);

	pType->Copy (Result, pSrcObject);

	return CPtrT <I> (
		(I*) ((uchar_t*) (void*) Result + Offset), 
		Result.GetRefCount ()
		);
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename I>
CPtrT <I>
Clone (const CPtrT <I>& Src)
{
	return Clone <I> (Src.p ());
}

//.............................................................................

template <typename I>
CPtrT <I>
GetPtrOrClone (I* pSrc)
{
	IRefCount* pRefCount = (IRefCount*) pSrc->GetInterface (AXL_OBJ_GUIDOF (IRefCount));
	return pRefCount ? 
		CPtrT <I> (pSrc, pRefCount) : 
		Clone (pSrc);
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename I>
CPtrT <I>
GetPtrOrClone (const CPtrT <I>& Src)
{
	return GetPtrOrClone <I> (Src.GetObject ());
}

//.............................................................................

} // namespace ref
} // namespace axl
