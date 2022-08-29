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
#include "axl_io_Serial.h"
#include "axl_iok_RegistryEntry.h"

#if (_AXL_DEBUG)
#define _AXL_IO_DRW_PRINT_DEVICE_PROPERTIES 1
#endif

namespace axl {
namespace io {

//..............................................................................

class SerialPortEnumerator {
public:
	static
	size_t
	createPortList(
		sl::List<SerialPortDesc>* portList,
		uint_t mask
	);
};

//..............................................................................

#ifdef _AXL_IO_DRW_PRINT_DEVICE_PROPERTIES

inline
void
printIndent(size_t indent) {
	for (size_t i = 0; i < indent; i++)
		printf("\t");
}

size_t
printDeviceProperties(const iok::RegistryEntry& device) {
	size_t indent = 0;

	iok::RegistryEntry parent = device.getParentEntry();
	if (parent)
		indent = printDeviceProperties(parent);

	cf::MutableDictionary propDict = device.getAllProperties();

	sl::Array<const void*> keyArray;
	sl::Array<const void*> valueArray;

	size_t count = propDict.getCount();
	propDict.getKeysAndValues(&keyArray, &valueArray);

	printIndent(indent);
	printf("deviceName = %s\n", device.getName().sz());

	sl::String className = device.getClass().sz();
	printIndent(indent);
	printf("className = %s", className.sz());

	for (;;) {
		className = iok::getSuperClass(className);
		if (className.isEmpty())
			break;

		printf(": %s", className.sz());
	}

	printf("\n");

	printIndent(indent);
	printf("path = %s\n", device.getPath().sz());

	printIndent(indent);
	printf("location = %s\n", device.getLocationInPlane().sz());

	for (size_t i = 0; i < count; i++) {
		printIndent(indent);
		printf(
			"%s = %s\n",
			cf::Type(keyArray[i]).toString().sz(),
			cf::Type(valueArray[i]).toString().sz()
		);
	}

	printf("\n");

	return indent + 1;
}
#endif

size_t
SerialPortEnumerator::createPortList(
	sl::List<SerialPortDesc>* portList,
	uint_t mask
) {
	portList->clear();

	cf::MutableDictionary dict = iok::createServiceMatchingDictionary(kIOSerialBSDServiceValue);
	dict.setValue(CFSTR(kIOSerialBSDTypeKey), CFSTR(kIOSerialBSDAllTypes));

	iok::Iterator it = iok::findMatchingServices(dict);
	iok::RegistryEntry port = it.next().p();
	for (; port; port = it.next().p()) {
#ifdef _AXL_IO_DRW_PRINT_DEVICE_PROPERTIES
		printDeviceProperties(port);
#endif

		cf::Type prop = port.getProperty(kIOCalloutDeviceKey);
		if (!prop) {
			prop = port.getProperty(kIODialinDeviceKey);
			if (!prop)
				continue;
		}

		SerialPortDesc* portDesc = AXL_MEM_NEW(SerialPortDesc);
		portDesc->m_deviceName = prop.toString();

		if (mask & SerialPortDescMask_Location)
			portDesc->m_location = port.getPath();

		// special handling for USB serial ports

		iok::RegistryEntry parent = port.getParentEntry();
		if (parent && parent.conformsTo("AppleUSBSerial")) {
			if (mask & SerialPortDescMask_Driver) {
				prop = parent.getProperty("CFBundleIdentifier");
				if (prop)
					portDesc->m_driver = prop.toString();
			}

			iok::RegistryEntry device = parent.findConformingEntry("IOUSBDevice");
			if (device) {
				if (mask & SerialPortDescMask_Description) {
					prop = device.getProperty(kUSBProductString);
					portDesc->m_description = prop ? prop.toString() : device.getName();
				}

				if ((mask & SerialPortDescMask_Manufacturer)) {
					prop = device.getProperty(kUSBVendorString);
					if (prop)
						portDesc->m_manufacturer = prop.toString();
					else {
						prop = device.getProperty(kUSBVendorID);
						if (prop)
							portDesc->m_manufacturer.format("VID_%04x", prop.toInt());
					}
				}

				if (mask & SerialPortDescMask_HardwareIds) {
					prop = device.getProperty(kUSBVendorID);
					uint_t vendorId = prop ? prop.toInt() : 0;
					prop = device.getProperty(kUSBProductID);
					uint_t productId = prop ? prop.toInt() : 0;
					portDesc->m_hardwareIds.format("%04x:%04x", vendorId, productId);
				}
			} else {
				if (mask & SerialPortDescMask_Description) {
					prop = port.getProperty(kIOTTYDeviceKey);
					portDesc->m_description = prop ? prop.toString() : port.getName();
				}

				if (mask & SerialPortDescMask_Manufacturer) {
					prop = parent.getProperty(kUSBVendorID);
					if (prop)
						portDesc->m_manufacturer.format("VID_%04x", prop.toInt());
				}

				if (mask & SerialPortDescMask_HardwareIds) {
					prop = parent.getProperty(kUSBVendorID);
					uint_t vendorId = prop ? prop.toInt() : 0;
					prop = parent.getProperty(kUSBProductID);
					uint_t productId = prop ? prop.toInt() : 0;
					portDesc->m_hardwareIds.format("%04x:%04x", vendorId, productId);
				}
			}
		} else { // for non-USB-to-serial ports, there's not much we can extract...
			if (mask & SerialPortDescMask_Description) {
				prop = port.getProperty(kIOTTYDeviceKey);
				portDesc->m_description = prop ? prop.toString() : port.getName();
			}
		}

		portList->insertTail(portDesc);
	}

	return portList->getCount();
}

//..............................................................................

size_t
createSerialPortDescList(
	sl::List<SerialPortDesc>* portList,
	uint_t mask
) {
	return SerialPortEnumerator::createPortList(portList, mask);
}

//..............................................................................

} // namespace io
} // namespace axl
