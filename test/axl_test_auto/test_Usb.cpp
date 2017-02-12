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
#include "test.h"

namespace {

//..............................................................................

void
run ()
{
	io::registerUsbErrorProvider ();

	bool result = io::getUsbDefaultContext ()->createDefault ();
	if (!result)
	{
		printf ("Cannot initialize libusb: %s\n", err::getLastErrorDescription ().sz ());
		ASSERT (false);
		return;
	}

	io::UsbDeviceList deviceList;
	size_t count = deviceList.enumerateDevices ();
	if (count == -1)
	{
		printf ("Cannot enumerate USB devices (%s)\n", err::getLastErrorDescription ().sz ());
		ASSERT (false);
		return;
	}

	sl::String bufferString;
	bufferString.createBuffer (4096);

	libusb_device** pp = deviceList;
	for (; *pp; pp++)
	{
		io::UsbDevice device;
		device.setDevice (*pp);

		libusb_device_descriptor descriptor;
		device.getDeviceDescriptor (&descriptor);

		printf (
			"VID_%04x&PID_%04x (Bus: %d Address: %d)\n",
			descriptor.idVendor,
			descriptor.idProduct,
			device.getBusNumber (),
			device.getDeviceAddress ()
			);

		result = device.open ();
		if (!result)
			continue;

		result = device.getStringDesrciptor (descriptor.iManufacturer, &bufferString);
		if (result)
			printf ("\tManufacturer:  %s\n", bufferString.sz ());

		result = device.getStringDesrciptor (descriptor.iProduct, &bufferString);
		if (result)
			printf ("\tProduct name:  %s\n", bufferString.sz ());

		result = device.getStringDesrciptor (descriptor.iSerialNumber, &bufferString);
		if (result)
			printf ("\tSerial number: %s\n", bufferString.sz ());
	}
}

//..............................................................................

ADD_TEST_CASE ("test_Usb", run)

}
