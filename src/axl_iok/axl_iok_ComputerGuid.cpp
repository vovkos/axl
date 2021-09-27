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
#include "axl_iok_ComputerGuid.h"

namespace axl {
namespace iok {

//..............................................................................

size_t
getComputerGuid(sl::Array<char>* guid) {
	guid->clear();

	mach_port_t masterPort;
	kern_return_t kernResult = IOMasterPort(MACH_PORT_NULL, &masterPort);
	if (kernResult != KERN_SUCCESS)
		return 0;

	CFMutableDictionaryRef matchingDict = IOBSDNameMatching(masterPort, 0, "en0");
	if (!matchingDict)
		return 0;

	io_iterator_t iterator;
	kernResult = IOServiceGetMatchingServices(masterPort, matchingDict, &iterator);
	if (kernResult != KERN_SUCCESS)
		return 0;

	CFDataRef macAddress = nil;
	while (io_object_t service = IOIteratorNext(iterator)) {
		io_object_t parentService;

		kernResult = IORegistryEntryGetParentEntry(
			service,
			kIOServicePlane,
			&parentService
		);

		if (kernResult == KERN_SUCCESS) {
			if (macAddress)
				CFRelease(macAddress);

			macAddress = (CFDataRef)IORegistryEntryCreateCFProperty(
				parentService,
				CFSTR("IOMACAddress"),
				kCFAllocatorDefault,
				0
			);

			IOObjectRelease(parentService);
		}

		IOObjectRelease(service);
	}

	IOObjectRelease(iterator);

	if (!macAddress)
		return 0;

	guid->copy((char*)CFDataGetBytePtr(macAddress), CFDataGetLength(macAddress));
	CFRelease(macAddress);
	return guid->getCount();
}

//..............................................................................

} // namespace iok
} // namespace axl
