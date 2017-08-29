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

#pragma once

#define _AXL_CRY_BIO_H

#include "axl_cry_Pch.h"

namespace axl {
namespace cry {

//..............................................................................

class FreeBio
{
public:
	void
	operator () (BIO* h)
	{
		BIO_free (h);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Bio: public sl::Handle <BIO*, FreeBio>
{
public:
	Bio ()
	{
	}

	Bio (
		const void* p,
		size_t size = -1
		)
	{
		create (p, size);
	}

	bool
	create (
		const void* p,
		size_t size = -1
		);
};

//..............................................................................

} // namespace cry
} // namespace axl
