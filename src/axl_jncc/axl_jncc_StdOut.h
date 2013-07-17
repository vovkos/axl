#pragma once

//.............................................................................

struct IStdOut: obj::IRoot
{
	// {BA9E5892-996B-4496-AFB5-6D1F841AE687}
	AXL_OBJ_INTERFACE (
		IStdOut,
		0xba9e5892, 0x996b, 0x4496, 0xaf, 0xb5, 0x6d, 0x1f, 0x84, 0x1a, 0xe6, 0x87
		)

	virtual 
	void 
	Printf_va (
		const char* pFormat,
		axl_va_list va
		) = 0;

	void
	Printf (
		const char* pFormat,
		...
		)
	{
		AXL_VA_DECL (va, pFormat);
		Printf_va (pFormat, va);
	}
};

//.............................................................................

class CStdOut: public IStdOut
{
public:
	// {3C3231D2-6623-4277-AF0C-20BD8369D3ED}
	AXL_OBJ_CLASS (
		CStdOut,
		IStdOut,
		0x3c3231d2, 0x6623, 0x4277, 0xaf, 0x0c, 0x20, 0xbd, 0x83, 0x69, 0xd3, 0xed
		)
		
	virtual 
	void 
	Printf_va (
		const char* pFormat,
		axl_va_list va
		)
	{
		vprintf (pFormat, va.m_va);
	}
};

//.............................................................................

class CFileStdOut: public IStdOut
{
public:
	// {36AE9EE1-A923-4035-83B7-24DE380DB77E}
	AXL_OBJ_CLASS (
		CSocketStdOut,
		IStdOut,
		0x36ae9ee1, 0xa923, 0x4035, 0x83, 0xb7, 0x24, 0xde, 0x38, 0x0d, 0xb7, 0x7e
		)

public:
	FILE* m_pFile;

public:
	CFileStdOut ()
	{
		m_pFile = NULL;
	}

	virtual 
	void 
	Printf_va (
		const char* pFormat,
		axl_va_list va
		)
	{
		ASSERT (m_pFile);
		vfprintf (m_pFile, pFormat, va.m_va);
	}
}

//.............................................................................

class CSocketStdOut: public IStdOut
{
public:
	// {16B9EA98-7FF1-47D9-AB5B-CB6ABEF49B5F}
	AXL_OBJ_CLASS (
		CSocketStdOut,
		IStdOut,
		0x16b9ea98, 0x7ff1, 0x47d9, 0xab, 0x5b, 0xcb, 0x6a, 0xbe, 0xf4, 0x9b, 0x5f
		)

public:
	SOCKET m_Socket;

public:
	virtual 
	void 
	Printf_va (
		const char* pFormat,
		axl_va_list va
		)
	{
		rtl::CString String;
		String.Format_va (pFormat, va);
		send (m_Socket, String.cc (), String.GetLength (), 0);
	}
}

//.............................................................................
