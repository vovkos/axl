// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_TRANSFORM_H

#include "axl_log_PacketFile.h"
#include "axl_obj_Root.h"
#include "axl_rtl_Array.h"

namespace axl {
namespace log {

//.............................................................................

struct ITransform: public obj::IRoot
{
	// {CFAF2B93-663D-4BBD-8E4C-BD5E26D89F92}
	AXL_OBJ_INTERFACE (
		ITransform,
		0xcfaf2b93, 0x663d, 0x4bbd, 0x8e, 0x4c, 0xbd, 0x5e, 0x26, 0xd8, 0x9f, 0x92
		)

	virtual 
	bool
	Transform (
		CPacketFile* pTargetFile, 
		uint_t Code, 
		const void* p, 
		size_t Size
		) = 0;

	virtual 
	void 
	Reset ()
	{ 
	}
};

//.............................................................................

class CTransformMgr
{
protected:
	struct TEntry: rtl::TListLink
	{
		ITransform* m_pTransform;
		CPacketFile m_TargetFile;
		TPacketFileHdr m_PrevFileHdrSnapshot;
	};

protected:
	CPacketFile* m_pSrcFile;
	rtl::CStdListT <TEntry> m_Stack;

public:
	CTransformMgr ()
	{
		m_pSrcFile = NULL;
	}

	void 
	Clear ()
	{
		m_Stack.Clear ();
		m_pSrcFile = NULL;
	}

	CPacketFile*
	GetSrcFile ()
	{
		return m_pSrcFile;
	}

	void
	SetSrcFile (CPacketFile* pFile);

	CPacketFile*
	GetTargetFile ()
	{
		return !m_Stack.IsEmpty () ? &m_Stack.GetTail ()->m_TargetFile : m_pSrcFile;
	}

	CPacketFile*
	AddTransform (
		ITransform* pTransform,
		const char* pFileName,
		uint_t OpenFlags = 0
		);

	CPacketFile*
	SetTransform (
		ITransform* pTransform,
		const char* pFileName,
		uint_t OpenFlags = 0
		)
	{
		m_Stack.Clear ();
		return AddTransform (pTransform, pFileName, OpenFlags);
	}

	bool
	Transform ();	

protected:
	bool
	TransformFile (
		ITransform* pTransform,
		CPacketFile* pDstFile,
		CPacketFile* pSrcFile,
		TPacketFileHdr* pSrcFileHdrSnapshot
		);
};

//.............................................................................

} // namespace log 
} // namespace axl
