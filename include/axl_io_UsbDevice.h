// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_USBDEVICE_H

#include "axl_io_UsbError.h"
#include "axl_rtl_Handle.h"
#include "axl_rtl_Array.h"

namespace axl {
namespace io {

//.............................................................................

class CCloseUsbContext
{
public:
	void
	operator () (libusb_context* h)
	{
		libusb_exit (h);
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CUsbContext: public rtl::CHandleT <libusb_context*, CCloseUsbContext>
{
public:
	CUsbContext ()
	{
		Open ();
	}

	bool 
	Open ();
};

//.............................................................................

class CFreeUsbDeviceList
{
public:
	void
	operator () (libusb_device** h)
	{
		libusb_free_device_list (h, true);
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CUsbDeviceList: public rtl::CHandleT <libusb_device**, CFreeUsbDeviceList>
{
public:
	size_t
	EnumerateDevices (libusb_context* pContext);
};

//.............................................................................

class CFreeUsbConfigDescriptor
{
public:
	void
	operator () (libusb_config_descriptor* h)
	{
		libusb_free_config_descriptor (h);
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CUsbConfigDescriptor: public rtl::CHandleT <libusb_config_descriptor*, CFreeUsbConfigDescriptor>
{
};

//.............................................................................

class CUsbDevice
{
public:
	enum EDef
	{
		EDef_BufferSize = 256,
	};

protected:
	libusb_device* m_pDevice;
	libusb_device_handle* m_pOpenHandle;

public:
	CUsbDevice ()
	{
		m_pDevice = NULL;
		m_pOpenHandle = NULL;
	}

	~CUsbDevice ()
	{
		SetDevice (NULL);
	}

	libusb_device* 
	GetDevice ()
	{
		return m_pDevice;
	}

	libusb_device_handle* 
	GetOpenHandle ()
	{
		return m_pOpenHandle;
	}
	
	bool
	IsOpen ()
	{
		return m_pOpenHandle != NULL;
	}

	uint8_t 
	GetBusNumber ()
	{
		ASSERT (m_pDevice);
		return libusb_get_bus_number (m_pDevice);
	}

	uint8_t
	GetDeviceAddress ()
	{
		ASSERT (m_pDevice);
		return libusb_get_device_address (m_pDevice);
	}

	libusb_speed 
	GetDeviceSpeed ()
	{
		ASSERT (m_pDevice);
		return (libusb_speed) libusb_get_device_speed (m_pDevice);
	}

	size_t
	GetMaxPacketSize (uint_t Endpoint);

	size_t
	GetMaxIsoPacketSize (uint_t Endpoint);
	
	// open-close

	void
	RefDevice ()
	{
		ASSERT (m_pDevice);
		libusb_ref_device (m_pDevice);
	}

	void
	UnrefDevice ()
	{
		ASSERT (m_pDevice);
		libusb_unref_device (m_pDevice);
	}

	void
	SetDevice (libusb_device* pDevice);

	void
	Close ();

	bool
	Open ();

	bool
	Open (libusb_device* pDevice)
	{
		SetDevice (pDevice);
		return Open ();
	}

	bool
	Open (
		uint_t VendorId,
		uint_t ProductId
		);

	int
	GetConfiguration ();

	bool
	SetConfiguration (int Configuration);

	bool
	ClaimInterface (int Interface);

	bool
	ReleaseInterface (int Interface);

	bool
	SetInterfaceAltSetting (
		int Interface,
		int AltSetting
		);

	bool
	ClearHalt (uint_t Endpoint);

	bool
	ResetDevice ();

	bool
	IsKernelDriverActive (int Interface);

	bool
	AttachKernelDriver (int Interface);

	bool
	DetachKernelDriver (int Interface);

	// descriptors

	rtl::CArrayT <char>
	GetDesrciptor (
		libusb_descriptor_type Type, 
		size_t Index
		)
	{
		rtl::CArrayT <char> Descriptor;
		GetDesrciptor (Type, Index, &Descriptor);
		return Descriptor;
	}
	
	bool
	GetDesrciptor (
		libusb_descriptor_type Type, 
		size_t Index,
		rtl::CArrayT <char>* pDescriptor
		);

	bool
	GetDeviceDescriptor (libusb_device_descriptor* pDescriptor);

	bool
	GetConfigDescriptor (
		int Configuration,
		libusb_config_descriptor** pDescriptor
		);

	bool
	GetActiveConfigDescriptor (libusb_config_descriptor** pDescriptor);

	rtl::CString 
	GetStringDesrciptor (
		size_t Index,
		uint_t LangId
		)
	{
		rtl::CString String;
		GetStringDesrciptor (Index, LangId, &String);
		return String;
	}

	bool
	GetStringDesrciptor (
		size_t Index,
		uint_t LangId,
		rtl::CString* pString
		);

	rtl::CString 
	GetStringDesrciptor (size_t Index)
	{
		rtl::CString String;
		GetStringDesrciptor (Index, &String);
		return String;
	}

	bool
	GetStringDesrciptor (
		size_t Index,
		rtl::CString* pString
		);

	// synchronous transfers

	size_t 
	ControlTransfer (
		uint_t RequestType,
		uint_t Request,
		uint_t Value,
		size_t Index,
		void* p,
		size_t Size,
		uint_t Timeout = -1
		);

	size_t 
	BulkTransfer (
		uint_t Endpoint,
		void* p,
		size_t Size,
		uint_t Timeout = -1
		);

	size_t 
	InterruptTransfer (
		uint_t Endpoint,
		void* p,
		size_t Size,
		uint_t Timeout = -1
		);
};

//.............................................................................

} // namespace io {
} // namespace axl {
