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
#include "axl_io_SerialPortEnumerator.h"
#include "axl_sys_win_DeviceInfo.h"

#define _AXL_ENUMERATE_DEVICE_CLASS 1

#include <initguid.h>
#include <devguid.h>

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

protected:
	static
	SerialPortDesc*
	createPortDesc(
		sys::win::DeviceInfo* deviceInfo,
		uint_t mask
	);
};

//..............................................................................

size_t
SerialPortEnumerator::createPortList(
	sl::List<SerialPortDesc>* portList,
	uint_t mask
) {
	bool result;

	portList->clear();

	sys::win::DeviceInfoSet deviceInfoSet;

#ifdef _AXL_ENUMERATE_DEVICE_CLASS
	result = deviceInfoSet.create(GUID_DEVCLASS_PORTS, DIGCF_PRESENT);
#else
	result = deviceInfoSet.create(GUID_DEVINTERFACE_COMPORT, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
#endif

	if (!result)
		return 0;

	for (size_t i = 0;; i++) {
		sys::win::DeviceInfo deviceInfo;
		result = deviceInfoSet.getDeviceInfo(i, &deviceInfo);
		if (!result)
			break;

		SerialPortDesc* portDesc = createPortDesc(&deviceInfo, mask);
		if (portDesc)
			portList->insertTail(portDesc);
	}

	return portList->getCount();
}

SerialPortDesc*
SerialPortEnumerator::createPortDesc(
	sys::win::DeviceInfo* deviceInfo,
	uint_t mask
) {
	sys::win::RegKey devRegKey;
	bool result = deviceInfo->openDeviceRegistryKey(&devRegKey, KEY_QUERY_VALUE);
	if (!result)
		return NULL;

	SerialPortDesc* portDesc = AXL_MEM_NEW(SerialPortDesc);
	portDesc->m_deviceName = devRegKey.queryStringValue(L"PortName");

	if (mask & SerialPortDescMask_Description) {
		deviceInfo->getDeviceRegistryProperty(SPDRP_FRIENDLYNAME, &portDesc->m_description);

		if (portDesc->m_description.isEmpty()) // try another property
			deviceInfo->getDeviceRegistryProperty(SPDRP_DEVICEDESC, &portDesc->m_description);
	}

	if (mask & SerialPortDescMask_Manufacturer)
		deviceInfo->getDeviceRegistryProperty(SPDRP_MFG, &portDesc->m_manufacturer);

	if (mask & SerialPortDescMask_HardwareIds)
		deviceInfo->getDeviceRegistryProperty(SPDRP_HARDWAREID, &portDesc->m_hardwareIds);

	if (mask & SerialPortDescMask_Driver)
		deviceInfo->getDeviceDriverPath(&portDesc->m_driver);

	if (mask & SerialPortDescMask_Location)
		deviceInfo->getDeviceRegistryProperty(SPDRP_LOCATION_PATHS, &portDesc->m_location);

#if (0)
	struct DeviceProp {
		const char* m_name;
		uint_t m_id;
	};

	static DeviceProp propTable[] = {
		{ "SPDRP_DEVICEDESC",                   SPDRP_DEVICEDESC                  }, // (0x00000000)  // DeviceDesc (R/W)
		{ "SPDRP_HARDWAREID",                   SPDRP_HARDWAREID                  }, // (0x00000001)  // HardwareID (R/W)
		{ "SPDRP_COMPATIBLEIDS",                SPDRP_COMPATIBLEIDS               }, // (0x00000002)  // CompatibleIDs (R/W)
		{ "SPDRP_SERVICE",                      SPDRP_SERVICE                     }, // (0x00000004)  // Service (R/W)
		{ "SPDRP_CLASS",                        SPDRP_CLASS                       }, // (0x00000007)  // Class (R--tied to ClassGUID)
		{ "SPDRP_CLASSGUID",                    SPDRP_CLASSGUID                   }, // (0x00000008)  // ClassGUID (R/W)
		{ "SPDRP_DRIVER",                       SPDRP_DRIVER                      }, // (0x00000009)  // Driver (R/W)
		{ "SPDRP_CONFIGFLAGS",                  SPDRP_CONFIGFLAGS                 }, // (0x0000000A)  // ConfigFlags (R/W)
		{ "SPDRP_MFG",                          SPDRP_MFG                         }, // (0x0000000B)  // Mfg (R/W)
		{ "SPDRP_FRIENDLYNAME",                 SPDRP_FRIENDLYNAME                }, // (0x0000000C)  // FriendlyName (R/W)
		{ "SPDRP_LOCATION_INFORMATION",         SPDRP_LOCATION_INFORMATION        }, // (0x0000000D)  // LocationInformation (R/W)
		{ "SPDRP_PHYSICAL_DEVICE_OBJECT_NAME",  SPDRP_PHYSICAL_DEVICE_OBJECT_NAME }, // (0x0000000E)  // PhysicalDeviceObjectName (R)
		{ "SPDRP_CAPABILITIES",                 SPDRP_CAPABILITIES                }, // (0x0000000F)  // Capabilities (R)
		{ "SPDRP_UI_NUMBER",                    SPDRP_UI_NUMBER                   }, // (0x00000010)  // UiNumber (R)
		{ "SPDRP_UPPERFILTERS",                 SPDRP_UPPERFILTERS                }, // (0x00000011)  // UpperFilters (R/W)
		{ "SPDRP_LOWERFILTERS",                 SPDRP_LOWERFILTERS                }, // (0x00000012)  // LowerFilters (R/W)
		{ "SPDRP_BUSTYPEGUID",                  SPDRP_BUSTYPEGUID                 }, // (0x00000013)  // BusTypeGUID (R)
		{ "SPDRP_LEGACYBUSTYPE",                SPDRP_LEGACYBUSTYPE               }, // (0x00000014)  // LegacyBusType (R)
		{ "SPDRP_BUSNUMBER",                    SPDRP_BUSNUMBER                   }, // (0x00000015)  // BusNumber (R)
		{ "SPDRP_ENUMERATOR_NAME",              SPDRP_ENUMERATOR_NAME             }, // (0x00000016)  // Enumerator Name (R)
		{ "SPDRP_SECURITY",                     SPDRP_SECURITY                    }, // (0x00000017)  // Security (R/W, binary form)
		{ "SPDRP_SECURITY_SDS",                 SPDRP_SECURITY_SDS                }, // (0x00000018)  // Security (W, SDS form)
		{ "SPDRP_DEVTYPE",                      SPDRP_DEVTYPE                     }, // (0x00000019)  // Device Type (R/W)
		{ "SPDRP_EXCLUSIVE",                    SPDRP_EXCLUSIVE                   }, // (0x0000001A)  // Device is exclusive-access (R/W)
		{ "SPDRP_CHARACTERISTICS",              SPDRP_CHARACTERISTICS             }, // (0x0000001B)  // Device Characteristics (R/W)
		{ "SPDRP_ADDRESS",                      SPDRP_ADDRESS                     }, // (0x0000001C)  // Device Address (R)
		{ "SPDRP_UI_NUMBER_DESC_FORMAT",        SPDRP_UI_NUMBER_DESC_FORMAT       }, // (0X0000001D)  // UiNumberDescFormat (R/W)
		{ "SPDRP_DEVICE_POWER_DATA",            SPDRP_DEVICE_POWER_DATA           }, // (0x0000001E)  // Device Power Data (R)
		{ "SPDRP_REMOVAL_POLICY",               SPDRP_REMOVAL_POLICY              }, // (0x0000001F)  // Removal Policy (R)
		{ "SPDRP_REMOVAL_POLICY_HW_DEFAULT",    SPDRP_REMOVAL_POLICY_HW_DEFAULT   }, // (0x00000020)  // Hardware Removal Policy (R)
		{ "SPDRP_REMOVAL_POLICY_OVERRIDE",      SPDRP_REMOVAL_POLICY_OVERRIDE     }, // (0x00000021)  // Removal Policy Override (RW)
		{ "SPDRP_INSTALL_STATE",                SPDRP_INSTALL_STATE               }, // (0x00000022)  // Device Install State (R)
		{ "SPDRP_LOCATION_PATHS",               SPDRP_LOCATION_PATHS              }, // (0x00000023)  // Device Location Paths (R)
		{ "SPDRP_BASE_CONTAINERID",             SPDRP_BASE_CONTAINERID            }, // (0x00000024)  // Base ContainerID (R)
	};

	for (size_t i = 0; i < countof(propTable); i++) {
		sl::String value;
		deviceInfo->getDeviceRegistryProperty(propTable[i].m_id, &value);
		printf("%s: %s\n", propTable[i].m_name, value.sz());
	}
#endif

	return portDesc;
}

//..............................................................................

size_t
enumerateSerialPorts(
	sl::List<SerialPortDesc>* portList,
	uint_t mask
) {
	return SerialPortEnumerator::createPortList(portList, mask);
}

//..............................................................................

} // namespace io
} // namespace axl
