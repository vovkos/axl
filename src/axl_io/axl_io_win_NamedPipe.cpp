#include "pch.h"
#include "axl_io_win_NamedPipe.h"

namespace axl {
namespace io {
namespace win {

//.............................................................................

bool 
CNamedPipe::Create (
	const wchar_t* pName,
	uint_t OpenMode,
	uint_t PipeMode,
	dword_t MaxInstanceCount,
	dword_t TxBufferSize,
	dword_t RxBufferSize,
	uint_t Timeout,
	SECURITY_ATTRIBUTES* pSecAttr
	)
{
	Close ();

	m_h = ::CreateNamedPipeW (
		pName, 
		OpenMode, 
		PipeMode, 
		MaxInstanceCount, 
		TxBufferSize, 
		RxBufferSize, 
		Timeout, 
		pSecAttr
		);

	return err::Complete (m_h != INVALID_HANDLE_VALUE);
}

bool 
CNamedPipe::Open (
	const wchar_t* pName,
	uint_t Access,
	uint_t Flags
	)
{
	Close ();

	m_h = ::CreateFileW (pName, Access, 0, NULL, OPEN_EXISTING, Flags, NULL);
	return err::Complete (m_h != INVALID_HANDLE_VALUE);
}

//.............................................................................

} // namespace win
} // namespace io
} // namespace axl
