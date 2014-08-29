// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

//.............................................................................

// no pragma once -- allow multiple inclusion of this file for re-enabling warning suppression

// warning C4146: unary minus operator applied to unsigned type, result still unsigned
// warning C4267: 'var' : conversion from 'size_t' to 'type', possible loss of data
// warning C4355: 'this' : used in base member initializer list

#if (_AXL_CPP == AXL_CPP_MSC)
#	pragma warning (disable: 4146)
#	pragma warning (disable: 4267)
#	pragma warning (disable: 4355)
#endif

#if (_AXL_CPP == AXL_CPP_GCC)
#	pragma GCC diagnostic ignored "-Wunused-parameter"
#	pragma GCC diagnostic ignored "-Wparentheses"
#	pragma GCC diagnostic ignored "-Wsign-compare"
#	pragma GCC diagnostic ignored "-Wformat"
#	pragma GCC diagnostic ignored "-Wmultichar"
#	pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
#	pragma GCC diagnostic ignored "-Winvalid-offsetof"
#	pragma GCC diagnostic ignored "-Wunused-result"
#endif

//.............................................................................
