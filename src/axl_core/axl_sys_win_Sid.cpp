#include "pch.h"
#include "axl_sys_win_Sid.h"

namespace axl {
namespace sys {
namespace win {

//.............................................................................

bool 
Sid::create (
	SID_IDENTIFIER_AUTHORITY* identifierAuthority,
	size_t subAuthorityCount,
	dword_t subAuthority0,
	dword_t subAuthority1,
	dword_t subAuthority2,
	dword_t subAuthority3,
	dword_t subAuthority4,
	dword_t subAuthority5,
	dword_t subAuthority6,
	dword_t subAuthority7
	)
{
	close ();

	bool_t result = ::AllocateAndInitializeSid (
		identifierAuthority,
		subAuthorityCount,
		subAuthority0,
		subAuthority1,
		subAuthority2,
		subAuthority3,
		subAuthority4,
		subAuthority5,
		subAuthority6,
		subAuthority7,
		&m_h
		);

	return err::complete (result);
}

//.............................................................................

} // namespace win
} // namespace sys
} // namespace axl
