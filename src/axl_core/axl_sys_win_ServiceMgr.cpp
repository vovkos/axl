#include "pch.h"
#include "axl_sys_win_ServiceMgr.h"

namespace axl {
namespace sys {
namespace win {

//.............................................................................

bool 
ServiceMgr::open (
	const wchar_t* machineName,
	const wchar_t* databaseName,
	dword_t access
	)
{
	close ();

	m_h = ::OpenSCManagerW (machineName, databaseName, access);
	return err::complete (m_h != NULL);
}

bool
ServiceMgr::createService (
	Service* service,
	const wchar_t* name,
	const wchar_t* displayName,
	dword_t access,
	dword_t serviceType,
	dword_t startType,
	dword_t errorControl,
	const wchar_t* binaryFilePath,
	const wchar_t* loadOrderGroup,
	dword_t* tagId,
	const wchar_t* dependencies,
	const wchar_t* startName,
	const wchar_t* password
	)
{
	SC_HANDLE h = ::CreateServiceW (
		m_h, 
		name, 
		displayName,
		access, 
		serviceType, 
		startType,
		errorControl,
		binaryFilePath,
		loadOrderGroup,
		tagId,
		dependencies,
		startName,
		password
		);

	if (!h)
	{
		err::setLastSystemError ();
		return false;
	}

	service->attach (h);
	return true;
}

bool
ServiceMgr::openService (
	Service* service,
	const wchar_t* name,
	dword_t access
	)
{
	SC_HANDLE h = ::OpenServiceW (m_h, name, access);
	if (!h)
	{
		err::setLastSystemError ();
		return false;
	}

	service->attach (h);
	return true;
}

//.............................................................................

} // namespace win
} // namespace sys
} // namespace axl
