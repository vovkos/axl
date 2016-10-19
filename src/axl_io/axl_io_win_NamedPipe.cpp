#include "pch.h"
#include "axl_io_win_NamedPipe.h"

namespace axl {
namespace io {
namespace win {

//..............................................................................

bool
NamedPipe::create (
	const sl::StringRef_w& name,
	uint_t openMode,
	uint_t pipeMode,
	dword_t maxInstanceCount,
	dword_t txBufferSize,
	dword_t rxBufferSize,
	uint_t timeout,
	SECURITY_ATTRIBUTES* secAttr
	)
{
	close ();

	m_h = ::CreateNamedPipeW (
		name.sz (),
		openMode,
		pipeMode,
		maxInstanceCount,
		txBufferSize,
		rxBufferSize,
		timeout,
		secAttr
		);

	return err::complete (m_h != INVALID_HANDLE_VALUE);
}

bool
NamedPipe::open (
	const sl::StringRef_w& name,
	uint_t access,
	uint_t flags
	)
{
	close ();

	m_h = ::CreateFileW (name.sz (), access, 0, NULL, OPEN_EXISTING, flags, NULL);
	return err::complete (m_h != INVALID_HANDLE_VALUE);
}

//..............................................................................

} // namespace win
} // namespace io
} // namespace axl
