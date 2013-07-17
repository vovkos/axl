#pragma once

//.............................................................................

struct IOutStream: obj::IRoot
{
	// {BA9E5892-996B-4496-AFB5-6D1F841AE687}
	AXL_OBJ_INTERFACE (
		IOutStream,
		0xba9e5892, 0x996b, 0x4496, 0xaf, 0xb5, 0x6d, 0x1f, 0x84, 0x1a, 0xe6, 0x87
		)

	virtual 
	size_t
	Printf_va (
		const char* pFormat,
		axl_va_list va
		) = 0;

	size_t
	Printf (
		const char* pFormat,
		...
		)
	{
		AXL_VA_DECL (va, pFormat);
		return Printf_va (pFormat, va);
	}
};

//.............................................................................

class CFileOutStream: public IOutStream
{
public:
	// {36AE9EE1-A923-4035-83B7-24DE380DB77E}
	AXL_OBJ_CLASS (
		CFileOutStream,
		IOutStream,
		0x36ae9ee1, 0xa923, 0x4035, 0x83, 0xb7, 0x24, 0xde, 0x38, 0x0d, 0xb7, 0x7e
		)

public:
	FILE* m_pFile;

public:
	CFileOutStream ()
	{
		m_pFile = stdout;
	}

	virtual 
	size_t 
	Printf_va (
		const char* pFormat,
		axl_va_list va
		)
	{
		return vfprintf (m_pFile, pFormat, va.m_va);
	}
};

//.............................................................................

class CSocketOutStream: public IOutStream
{
public:
	// {16B9EA98-7FF1-47D9-AB5B-CB6ABEF49B5F}
	AXL_OBJ_CLASS (
		CSocketOutStream,
		IOutStream,
		0x16b9ea98, 0x7ff1, 0x47d9, 0xab, 0x5b, 0xcb, 0x6a, 0xbe, 0xf4, 0x9b, 0x5f
		)

public:
	SOCKET m_Socket;

public:
	CSocketOutStream ()
	{
		m_Socket = INVALID_SOCKET;
	}

	virtual 
	size_t
	Printf_va (
		const char* pFormat,
		axl_va_list va
		)
	{
		ASSERT (m_Socket != INVALID_SOCKET);

		rtl::CString String;
		String.Format_va (pFormat, va);
		return send (m_Socket, String, String.GetLength (), 0);
	}
};

//.............................................................................
