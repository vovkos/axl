#include "pch.h"
#include "axl_io_UsbDevice.h"

namespace axl {
namespace io {

//.............................................................................

bool 
CUsbContext::Open ()
{
	Close ();

	int Result = libusb_init (&m_h);
	return Result == 0 ? true : err::Fail (CUsbError ((int) Result));
}

//.............................................................................

size_t
CUsbDeviceList::EnumerateDevices (libusb_context* pContext)
{
	Close ();

	ssize_t Result = libusb_get_device_list (pContext, &m_h);
	return Result >= 0 ? Result : err::Fail <size_t> (-1, CUsbError ((int) Result));
}

//.............................................................................

size_t
CUsbDevice::GetMaxPacketSize (uint_t Endpoint)
{
	ASSERT (m_pDevice);
	
	int Result = libusb_get_max_packet_size (m_pDevice, (uchar_t) Endpoint);
	return Result >= 0 ? Result : err::Fail <size_t> (-1, CUsbError (Result));
}

size_t
CUsbDevice::GetMaxIsoPacketSize (uint_t Endpoint)
{
	ASSERT (m_pDevice);
	
	int Result = libusb_get_max_iso_packet_size (m_pDevice, (uchar_t) Endpoint);
	return Result >= 0 ? Result : err::Fail <size_t> (-1, CUsbError (Result));
}

void
CUsbDevice::SetDevice (libusb_device* pDevice)
{
	if (pDevice == m_pDevice)
		return;

	Close ();

	if (pDevice)
		libusb_ref_device (pDevice);

	if (m_pDevice)
		libusb_unref_device (m_pDevice);

	m_pDevice = pDevice;
}

void
CUsbDevice::Close ()
{
	if (!m_pOpenHandle)
		return;

	libusb_close (m_pOpenHandle);
	m_pOpenHandle = NULL;
}

bool
CUsbDevice::Open ()
{
	ASSERT (m_pDevice);

	Close ();

	int Result = libusb_open (m_pDevice, &m_pOpenHandle);
	return Result == 0 ? true : err::Fail (CUsbError (Result));
}

bool
CUsbDevice::Open (
	uint_t VendorId,
	uint_t ProductId
	)
{
	SetDevice (NULL);

	m_pOpenHandle = libusb_open_device_with_vid_pid (NULL, (uint16_t) VendorId, (uint16_t) ProductId);
	return m_pOpenHandle ? true : err::Fail (err::CError (err::EStatus_ObjectNameNotFound));
}

int
CUsbDevice::GetConfiguration ()
{
	ASSERT (m_pOpenHandle);

	int Configuration;
	int Result = libusb_get_configuration (m_pOpenHandle, &Configuration);
	return Result == 0 ? Configuration : err::Fail <int> (-1, CUsbError (Result));
}

bool
CUsbDevice::SetConfiguration (int Configuration)
{
	ASSERT (m_pOpenHandle);

	int Result = libusb_get_configuration (m_pOpenHandle, &Configuration);
	return Result == 0 ? true : err::Fail (CUsbError (Result));
}

bool
CUsbDevice::ClaimInterface (int Interface)
{
	ASSERT (m_pOpenHandle);

	int Result = libusb_claim_interface (m_pOpenHandle, Interface);
	return Result == 0 ? true : err::Fail (CUsbError (Result));
}

bool
CUsbDevice::ReleaseInterface (int Interface)
{
	ASSERT (m_pOpenHandle);

	int Result = libusb_release_interface (m_pOpenHandle, Interface);
	return Result == 0 ? true : err::Fail (CUsbError (Result));
}

bool
CUsbDevice::SetInterfaceAltSetting (
	int Interface,
	int AltSetting
	)
{
	ASSERT (m_pOpenHandle);

	int Result = libusb_set_interface_alt_setting (m_pOpenHandle, Interface, AltSetting);
	return Result == 0 ? true : err::Fail (CUsbError (Result));
}

bool
CUsbDevice::ClearHalt (uint_t Endpoint)
{
	ASSERT (m_pOpenHandle);

	int Result = libusb_clear_halt (m_pOpenHandle, (uchar_t) Endpoint);
	return Result == 0 ? true : err::Fail (CUsbError (Result));
}

bool
CUsbDevice::ResetDevice ()
{
	ASSERT (m_pOpenHandle);

	int Result = libusb_reset_device (m_pOpenHandle);
	return Result == 0 ? true : err::Fail (CUsbError (Result));
}

bool
CUsbDevice::IsKernelDriverActive (int Interface)
{
	ASSERT (m_pOpenHandle);

	int Result = libusb_kernel_driver_active (m_pOpenHandle, Interface);
	return Result == 1;
}

bool
CUsbDevice::AttachKernelDriver (int Interface)
{
	ASSERT (m_pOpenHandle);

	int Result = libusb_attach_kernel_driver (m_pOpenHandle, Interface);
	return Result == 0 ? true : err::Fail (CUsbError (Result));
}

bool
CUsbDevice::DetachKernelDriver (int Interface)
{
	ASSERT (m_pOpenHandle);

	int Result = libusb_detach_kernel_driver (m_pOpenHandle, Interface);
	return Result == 0 ? true : err::Fail (CUsbError (Result));
}

bool
CUsbDevice::GetDesrciptor (
	libusb_descriptor_type Type, 
	size_t Index,
	rtl::CArrayT <char>* pDescriptor
	)
{
	ASSERT (m_pOpenHandle);

	size_t Size = pDescriptor->GetCount ();
	Size = Size < EDef_BufferSize ? EDef_BufferSize : rtl::GetMinPower2Ge (Size);

	pDescriptor->SetCount (Size);

	for (;;)
	{
		int Result = libusb_get_descriptor (
			m_pOpenHandle, 
			(uint8_t) Type, 
			(uint8_t) Index, 
			(uchar_t*) pDescriptor->a (), 
			Size
			);

		if (Result >= 0)
		{
			Size = Result;
			break;
		}

		if (Result != LIBUSB_ERROR_OVERFLOW)
			return err::Fail (CUsbError (Result));
		
		Size *= 2;
		pDescriptor->SetCount (Size);
	}

	pDescriptor->SetCount (Size);
	return true;
}

bool
CUsbDevice::GetDeviceDescriptor (libusb_device_descriptor* pDescriptor)
{
	ASSERT (m_pDevice);

	int Result = libusb_get_device_descriptor (m_pDevice, pDescriptor);
	return Result == 0 ? true : err::Fail (CUsbError (Result));
}

bool
CUsbDevice::GetConfigDescriptor (
	int Configuration,
	libusb_config_descriptor** pDescriptor
	)
{
	ASSERT (m_pDevice);

	int Result = libusb_get_config_descriptor (m_pDevice, Configuration, pDescriptor);
	return Result == 0 ? true : err::Fail (CUsbError (Result));
}

bool
CUsbDevice::GetActiveConfigDescriptor (libusb_config_descriptor** pDescriptor)
{
	ASSERT (m_pDevice);

	int Result = libusb_get_active_config_descriptor (m_pDevice, pDescriptor);
	return Result == 0 ? true : err::Fail (CUsbError (Result));
}

bool
CUsbDevice::GetStringDesrciptor (
	size_t Index,
	uint_t LangId,
	rtl::CString* pString
	)
{
	ASSERT (m_pOpenHandle);

	char* p = pString->GetBuffer ();
	size_t Length = pString->GetBufferLength ();

	if (Length < EDef_BufferSize)
	{
		Length = EDef_BufferSize;
		p = pString->GetBuffer (Length - 1, false);
	}

	for (;;)
	{
		int Result = libusb_get_string_descriptor (
			m_pOpenHandle, 
			(uint8_t) Index, 
			(uint16_t) LangId, 
			(uchar_t*) p, 
			Length
			);

		if (Result >= 0)
		{
			Length = Result;
			break;
		}

		if (Result != LIBUSB_ERROR_OVERFLOW)
			return err::Fail (CUsbError (Result));
		
		Length *= 2;
		p = pString->GetBuffer (Length - 1, false);
	}

	pString->ReduceLength (Length);
	return true;
}

bool
CUsbDevice::GetStringDesrciptor (
	size_t Index,
	rtl::CString* pString
	)
{
	ASSERT (m_pOpenHandle);

	char* p = pString->GetBuffer ();
	size_t Length = pString->GetBufferLength ();

	if (Length < EDef_BufferSize)
	{
		Length = EDef_BufferSize;
		p = pString->GetBuffer (Length - 1, false);
	}

	for (;;)
	{
		int Result = libusb_get_string_descriptor_ascii (
			m_pOpenHandle, 
			(uint8_t) Index, 
			(uchar_t*) p, 
			Length
			);

		if (Result >= 0)
		{
			Length = Result;
			break;
		}

		if (Result != LIBUSB_ERROR_OVERFLOW)
			return err::Fail (CUsbError (Result));
		
		Length *= 2;
		p = pString->GetBuffer (Length - 1, false);
	}

	pString->ReduceLength (Length);
	return true;
}

size_t 
CUsbDevice::ControlTransfer (
	uint_t RequestType,
	uint_t Request,
	uint_t Value,
	size_t Index,
	void* p,
	size_t Size,
	uint_t Timeout
	)
{
	ASSERT (m_pOpenHandle);

	int Result = libusb_control_transfer (
		m_pOpenHandle, 
		(uint8_t) RequestType, 
		(uint8_t) Request, 
		(uint16_t) Value, 
		(uint16_t) Index, 
		(uchar_t*) p,
		(uint16_t) Size, 
		Timeout != -1 ? Timeout : 0
		);

	return Result >= 0 ? Result : err::Fail <size_t> (-1, CUsbError (Result));
}

size_t 
CUsbDevice::BulkTransfer (
	uint_t Endpoint,
	void* p,
	size_t Size,
	uint_t Timeout
	)
{
	ASSERT (m_pOpenHandle);

	int ActualSize;
	int Result = libusb_bulk_transfer (
		m_pOpenHandle,
		(uchar_t) Endpoint, 
		(uchar_t*) p, 
		(int) Size, 
		&ActualSize,
		Timeout != -1 ? Timeout : 0
		);

	return Result == 0 ? ActualSize : err::Fail <size_t> (-1, CUsbError (Result));
}

size_t 
CUsbDevice::InterruptTransfer (
	uint_t Endpoint,
	void* p,
	size_t Size,
	uint_t Timeout
	)
{
	ASSERT (m_pOpenHandle);

	int ActualSize;
	int Result = libusb_interrupt_transfer (
		m_pOpenHandle,
		(uchar_t) Endpoint, 
		(uchar_t*) p, 
		(int) Size, 
		&ActualSize,
		Timeout != -1 ? Timeout : 0
		);

	return Result == 0 ? ActualSize : err::Fail <size_t> (-1, CUsbError (Result));

}

//.............................................................................

} // namespace io {
} // namespace axl {
