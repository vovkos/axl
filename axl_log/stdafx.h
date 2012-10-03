// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#ifndef _WIN32_WINNT            // Specifies that the minimum required platform is Windows Vista.
#define _WIN32_WINNT 0x0600     // Change this to the appropriate value to target other versions of Windows.
#endif

#include <new>
#include <typeinfo>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <crtdbg.h>

#include <tchar.h>
#include <winsock2.h>
#include <cguid.h>
#include <ws2ipdef.h>