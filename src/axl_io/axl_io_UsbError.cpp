#include "pch.h"
#include "axl_io_UsbError.h"

namespace axl {
namespace io {

//.............................................................................

err::TError*
CUsbError::Create (int Error)
{
	err::TError* pError = GetBuffer (sizeof (err::TError));
	if (!pError)
		return NULL;

	pError->m_Size = sizeof (err::TError);
	pError->m_Guid = GUID_UsbError;
	pError->m_Code = Error;
	return pError;
}

//.............................................................................

} // namespace io
} // namespace axl
