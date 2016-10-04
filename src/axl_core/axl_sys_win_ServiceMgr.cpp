#include "pch.h"
#include "axl_sys_win_ServiceMgr.h"

namespace axl {
namespace sys {
namespace win {

//.............................................................................

bool 
ServiceMgr::open (
	const sl::StringRef_w& machineName,
	const sl::StringRef_w& databaseName,
	dword_t access
	)
{
	close ();

	m_h = ::OpenSCManagerW (machineName.szn (), databaseName.szn (), access);
	return err::complete (m_h != NULL);
}

bool
ServiceMgr::createService (
	Service* service,
	const sl::StringRef_w& name,
	const sl::StringRef_w& displayName,
	dword_t access,
	dword_t serviceType,
	dword_t startType,
	dword_t errorControl,
	const sl::StringRef_w& binaryFilePath,
	const sl::StringRef_w& loadOrderGroup,
	dword_t* tagId,
	const sl::StringRef_w& dependencies,
	const sl::StringRef_w& startName,
	const sl::StringRef_w& password
	)
{
	SC_HANDLE h = ::CreateServiceW (
		m_h, 
		name.szn (), 
		displayName.szn (),
		access, 
		serviceType, 
		startType,
		errorControl,
		binaryFilePath.szn (),
		loadOrderGroup.szn (),
		tagId,
		dependencies.szn (),
		startName.szn (),
		password.szn ()
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
	const sl::StringRef_w& name,
	dword_t access
	)
{
	SC_HANDLE h = ::OpenServiceW (m_h, name.szn (), access);
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
