#include "pch.h"
#include "axl_io_psx_Pipe.h"

namespace axl {
namespace io {
namespace psx {

//.............................................................................

bool
CPipe::Create ()
{
	Close ();

	int fd [2];
	int Result = pipe (fd);
	if (Result == -1)
		return err::FailWithLastSystemError ();

	m_FileA.Attach (fd [0]);
	m_FileB.Attach (fd [1]);
	return true;
}

//.............................................................................

} // namespace psx
} // namespace io
} // namespace axl

