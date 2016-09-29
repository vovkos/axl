// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

//.............................................................................

// no pragma once -- allow multiple inclusion of this file for re-enabling warning suppression

#if (_AXL_CPP_MSC)
#	pragma warning (disable: 4146) // warning C4146: unary minus operator applied to unsigned type, result still unsigned
#	pragma warning (disable: 4267) // warning C4267: 'var' : conversion from 'size_t' to 'type', possible loss of data
#	pragma warning (disable: 4355) // warning C4355: 'this' : used in base member initializer list
#endif

#if (_AXL_CPP_GCC)
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
