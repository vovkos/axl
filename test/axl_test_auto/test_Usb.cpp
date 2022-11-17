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
#include "axl_io_UsbDevice.h"
#include "axl_io_UsbContext.h"
#include "test.h"


namespace {

//..............................................................................

void
run() {
	io::registerUsbErrorProvider();

	bool result = io::getUsbDefaultContext()->createDefault();
	if (!result) {
		printf("Cannot initialize libusb: %s\n", err::getLastErrorDescription().sz());
		// travis-ci on linux VMs fails here with LIBUSB_ERROR_OTHER
		// keep assert commented until finding a workaround
		// ASSERT(false);
		return;
	}

	io::UsbDeviceList deviceList;
	size_t count = deviceList.enumerateDevices();
	if (count == -1) {
		printf("Cannot enumerate USB devices (%s)\n", err::getLastErrorDescription().sz());
		ASSERT(false);
		return;
	}

	sl::String_utf16 bufferString;
	bufferString.createBuffer(4096);

	libusb_device** pp = deviceList;
	for (; *pp; pp++) {
		io::UsbDevice device;
		device.setDevice(*pp);

		libusb_device_descriptor descriptor;
		device.getDeviceDescriptor(&descriptor);

		printf(
			"VID_%04x&PID_%04x (Bus: %d Address: %d)\n",
			descriptor.idVendor,
			descriptor.idProduct,
			device.getBusNumber(),
			device.getDeviceAddress()
		);

		result = device.open();
		if (!result)
			continue;

		result = descriptor.iManufacturer && device.getStringDescriptor(&bufferString, descriptor.iManufacturer) != -1;
		if (result)
			printf("\tManufacturer:  %s\n", bufferString.s2().sz());

		result = descriptor.iProduct && device.getStringDescriptor(&bufferString, descriptor.iProduct) != -1;
		if (result)
			printf("\tProduct name:  %s\n", bufferString.s2().sz());

		result = descriptor.iSerialNumber && device.getStringDescriptor(&bufferString, descriptor.iSerialNumber) != -1;
		if (result)
			printf("\tSerial number: %s\n", bufferString.s2().sz());
	}
}

//..............................................................................

ADD_TEST_CASE("test_Usb", run)

}
