// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Decl.h"

namespace axl {
namespace jnc {

class CArrayType;
class CDataPtrType;
class CClassType;
class CClassPtrType;
class CFunctionType;
class CFunctionPtrType;
class CPropertyType;
class CPropertyPtrType;
class CMulticastType;
class CMcSnapshotType;
class CAutoEvType;
class CAutoEvPtrType;
class CImportType;
class CPrimaryImportType;
class CSecondaryImportType;

//.............................................................................

class CDeclTypeCalc
{
protected:
	CModule* m_pModule;
	rtl::CIteratorT <CDeclSuffix> m_Suffix;
	uint_t m_TypeModifiers;

public:
	CDeclTypeCalc ();

	CType*
	CalcType (
		CDeclarator* pDeclarator,
		uint_t* pDataPtrTypeFlags
		);

	CType*
	CalcDataType (
		CType* pType,
		uint_t TypeModifiers,
		uint_t* pDataPtrTypeFlags
		);

	CType*
	CalcPtrType (
		CType* pType,
		uint_t TypeModifiers
		);

protected:
	bool
	CheckUnusedModifiers ();

	uint_t 
	GetDataPtrTypeFlags ();

	CType*
	GetIntegerType (CType* pType);

	CArrayType*
	GetArrayType (CType* pElementType);

	CFunctionType*
	GetFunctionType (CType* pReturnType);

	CPropertyType*
	GetPropertyType (CType* pReturnType);

	CPropertyType*
	GetBindableDataType (CType* pDataType);

	CAutoEvType*
	GetAutoEvType (CType* pReturnType);

	CMulticastType*
	GetMulticastType (CType* pReturnType);

	CDataPtrType*
	GetDataPtrType (CType* pDataType);

	CClassPtrType*
	GetClassPtrType (CClassType* pClassType);

	CFunctionPtrType*
	GetFunctionPtrType (CFunctionType* pFunctionType);

	CPropertyPtrType*
	GetPropertyPtrType (CPropertyType* pPropertyType);

	CAutoEvPtrType*
	GetAutoEvPtrType (CAutoEvType* pAutoEvType);

	CType*
	GetImportPtrType (CImportType* pImportType);

	CType*
	PrepareReturnType (CType* pType);
};

//.............................................................................

} // namespace jnc {
} // namespace axl {


