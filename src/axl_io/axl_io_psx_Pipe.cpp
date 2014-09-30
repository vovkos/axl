#include "pch.h"
#include "axl_io_psx_Pipe.h"

namespace axl {
namespace io {
namespace psx {

//.............................................................................

bool
CPipe::create ()
{
	close ();

	int fd [2];
	int result = pipe (fd);
	if (result == -1)
		return err::failWithLastSystemError ();

	m_readFile.attach (fd [0]);
	m_writeFile.attach (fd [1]);
	return true;
}

//.............................................................................

} // namespace psx
} // namespace io
} // namespace axl

