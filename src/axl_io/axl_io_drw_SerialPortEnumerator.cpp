#include "pch.h"
#include "axl_io_Serial.h"
#include "axl_iok_RegistryEntry.h"

namespace axl {
namespace io {

//.............................................................................

class SerialPortEnumerator
{
public:
	static
	size_t
	createPortList (sl::StdList <SerialPortDesc>* portList);
};

//.............................................................................

#ifdef _AXL_DEBUG
size_t
printDeviceProperties (const iok::RegistryEntry& device)
{
	size_t indent = 0;

	iok::RegistryEntry parent = device.getParentEntry ();
	if (parent)
		indent = printDeviceProperties (parent);

	cf::MutableDictionaryRef propDict = device.getAllProperties ();

	sl::Array <const void*> keyArray;
	sl::Array <const void*> valueArray;

	size_t count = propDict.getCount ();
	propDict.getKeysAndValues (&keyArray, &valueArray);

	for (size_t i = 0; i < indent; i++)
		printf ("\t");

	printf ("deviceName = %s\n", device.getName ().cc ());
	for (size_t i = 0; i < count; i++)
	{
		for (size_t i = 0; i < indent; i++)
			printf ("\t");

		printf (
			"%s = %s\n",
			cf::TypeRef (keyArray [i]).toString ().cc (),
			cf::TypeRef (valueArray [i]).toString ().cc ()
			); 
	}

	printf ("\n");

	return indent + 1;
}
#endif

size_t
SerialPortEnumerator::createPortList (sl::StdList <SerialPortDesc>* portList)
{
	bool result;

	portList->clear ();

	cf::MutableDictionaryRef dict = iok::createServiceMatchingDictionary (kIOSerialBSDServiceValue);
	dict.setValue (CFSTR (kIOSerialBSDTypeKey), CFSTR (kIOSerialBSDAllTypes));

	iok::Iterator it = iok::findMatchingServices (dict);
	iok::RegistryEntry port = it.next ().p ();
	for (; port; port = it.next ().p ())
	{
#ifdef _AXL_DEBUG
		printDeviceProperties (port);
#endif

		sl::String name;
		sl::String description;

		cf::TypeRef prop = port.getProperty (kIOCalloutDeviceKey).p ();
		if (!prop)
		{
			prop = port.getProperty (kIODialinDeviceKey).p ();
			if (!prop)
				continue;
		}

		name = prop.toString ();

		prop = port.getProperty (kIOTTYDeviceKey);
		description = prop ? prop.toString () : port.getName ();

		SerialPortDesc* portDesc = AXL_MEM_NEW (SerialPortDesc);
		portDesc->m_deviceName = name;
		portDesc->m_description = description;
		portList->insertTail (portDesc);
	}

	return portList->getCount ();
}

//.............................................................................

size_t
createSerialPortDescList (sl::StdList <SerialPortDesc>* portList)
{
	return SerialPortEnumerator::createPortList (portList);
}

//.............................................................................

} // namespace io
} // namespace axl
