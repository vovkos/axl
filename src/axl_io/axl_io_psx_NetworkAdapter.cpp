#include "pch.h"
#include "axl_io_NetworkAdapter.h"
#include "axl_err_Error.h"

namespace axl {
namespace io {

//.............................................................................

size_t
buildNetworkAdapterDescList (rtl::StdList <NetworkAdapterDesc>* list)
{
	list->clear ();

	return list->getCount ();
}

//.............................................................................

} // namespace axl {
} // namespace io {
