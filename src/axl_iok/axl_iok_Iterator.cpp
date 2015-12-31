#include "pch.h"
#include "axl_iok_Iterator.h"

namespace axl {
namespace iok {

//.............................................................................

Object
Iterator::next ()
{
	io_object_t p = ::IOIteratorNext (m_p);

	Object object;
	object.attach (p);
	return object;
}

//.............................................................................

} // namespace iok
} // namespace axl
