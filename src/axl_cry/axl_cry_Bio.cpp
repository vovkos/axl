//..............................................................................
//
//  This file is part of the AXL library.
//
//  AXL is distributed under the MIT license.
//  For details see accompanying license.txt file,
//  the public copy of which is also available at:
//  http://tibbo.com/downloads/archive/axl/license.txt
//
//..............................................................................

#include "pch.h"
#include "axl_cry_Bio.h"

namespace axl {
namespace cry {

//..............................................................................

bool
Bio::create (
	const void* p,
	size_t size
	)
{
	close ();
	
	m_h = BIO_new_mem_buf ((void*) p, size);
	if (!m_h)
	{
		// BIO_new_mem_buf doesn't always set crypto error
		err::setError (err::SystemErrorCode_InsufficientResources); 
		return false;
	}
	
	return true;
}

//..............................................................................

} // namespace cry
} // namespace axl
