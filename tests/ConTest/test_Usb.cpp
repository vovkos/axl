#include "pch.h"

using namespace axl;

namespace test_Usb { 

//.............................................................................

void 
Run ()
{
	bool Result;

	io::CUsbContext Context;

	io::CUsbDeviceList DeviceList;
	size_t Count = DeviceList.EnumerateDevices (Context);
	if (Count == -1)
	{
		printf ("Cannot enumerate USB devices (%s)\n", err::GetError ()->GetDescription ().cc ());
		return;
	}

	rtl::CString BufferString;
	BufferString.GetBuffer (4096);

	libusb_device** pp = DeviceList;
	for (; *pp; pp++)
	{
		io::CUsbDevice Device;
		Device.SetDevice (*pp);

		libusb_device_descriptor Descriptor;
		Device.GetDeviceDescriptor (&Descriptor);

		printf (
			"VID_%04x&PID_%04x (Bus: %d Address: %d)\n", 
			Descriptor.idVendor,
			Descriptor.idProduct,
			Device.GetBusNumber (), 
			Device.GetDeviceAddress ()
			);

		Result = Device.Open ();
		if (!Result)
			continue;

		Result = Device.GetStringDesrciptor (Descriptor.iManufacturer, &BufferString);
		if (Result)
			printf ("\tManufacturer:  %s\n", BufferString.cc ());

		Result = Device.GetStringDesrciptor (Descriptor.iProduct, &BufferString);
		if (Result)
			printf ("\tProduct name:  %s\n", BufferString.cc ());

		Result = Device.GetStringDesrciptor (Descriptor.iSerialNumber, &BufferString);
		if (Result)
			printf ("\tSerial number: %s\n", BufferString.cc ());
	}

}

//.............................................................................

} // namespace test_Vso
