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

namespace axl {
namespace io {

//..............................................................................

class SerialPortEnumerator {
public:
	static
	size_t
	createPortList(sl::List<SerialPortDesc>* portList);
};

//..............................................................................

#ifdef _AXL_DEBUG
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

	for (size_t i = 0; i < indent; i++)
		printf("\t");

	printf("deviceName = %s\n", device.getName().sz());
	for (size_t i = 0; i < count; i++) {
		for (size_t i = 0; i < indent; i++)
			printf("\t");

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
SerialPortEnumerator::createPortList(sl::List<SerialPortDesc>* portList) {
	portList->clear();

	cf::MutableDictionary dict = iok::createServiceMatchingDictionary(kIOSerialBSDServiceValue);
	dict.setValue(CFSTR(kIOSerialBSDTypeKey), CFSTR(kIOSerialBSDAllTypes));

	iok::Iterator it = iok::findMatchingServices(dict);
	iok::RegistryEntry port = it.next().p();
	for (; port; port = it.next().p()) {
#ifdef _AXL_DEBUG
		printDeviceProperties(port);
#endif

		sl::String name;
		sl::String description;

		cf::Type prop = port.getProperty(kIOCalloutDeviceKey);
		if (!prop) {
			prop = port.getProperty(kIODialinDeviceKey);
			if (!prop)
				continue;
		}

		name = prop.toString();

		prop = port.getProperty(kIOTTYDeviceKey);
		description = prop ? prop.toString() : port.getName();

		SerialPortDesc* portDesc = AXL_MEM_NEW(SerialPortDesc);
		portDesc->m_deviceName = name;
		portDesc->m_description = description;
		portList->insertTail(portDesc);
	}

	return portList->getCount();
}

//..............................................................................

size_t
createSerialPortDescList(sl::List<SerialPortDesc>* portList) {
	return SerialPortEnumerator::createPortList(portList);
}

//..............................................................................

} // namespace io
} // namespace axl
