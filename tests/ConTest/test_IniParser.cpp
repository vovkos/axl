#include "pch.h"

namespace test_IniParser { 

//.............................................................................

class MyParser: public ini::Parser <MyParser>
{
protected:
	enum SectionKind
	{
		SectionKind_Undefined,
		SectionKind_Session,
		SectionKind_Scripts,
	};

	enum KeyKind
	{
		KeyKind_Undefined,
		KeyKind_Name,
		KeyKind_Description,
		KeyKind_Guid,
		KeyKind_Icon,
		KeyKind_LogRepresenterClass,
		KeyKind_SessionClass,
	};

	AXL_RTL_BEGIN_HASH_TABLE_MAP_STRING (SectionNameMap, SectionKind)
		AXL_RTL_HASH_TABLE_MAP_ENTRY ("session", SectionKind_Session)
		AXL_RTL_HASH_TABLE_MAP_ENTRY ("scripts", SectionKind_Scripts)
	AXL_RTL_END_HASH_TABLE_MAP ()

	AXL_RTL_BEGIN_HASH_TABLE_MAP_STRING (KeyNameMap, KeyKind)
		AXL_RTL_HASH_TABLE_MAP_ENTRY ("name", KeyKind_Name)
		AXL_RTL_HASH_TABLE_MAP_ENTRY ("description", KeyKind_Description)
		AXL_RTL_HASH_TABLE_MAP_ENTRY ("guid", KeyKind_Guid)
		AXL_RTL_HASH_TABLE_MAP_ENTRY ("icon", KeyKind_Icon)
		AXL_RTL_HASH_TABLE_MAP_ENTRY ("session-class", KeyKind_SessionClass)
		AXL_RTL_HASH_TABLE_MAP_ENTRY ("log-representer-class", KeyKind_LogRepresenterClass)
	AXL_RTL_END_HASH_TABLE_MAP ()

public:
	bool 
	onSection (const char* name)
	{
		SectionNameMap::Iterator it = SectionNameMap::find (name);
		SectionKind section = it ? it->m_value : SectionKind_Undefined;
		printf ("OnSection '%s'\n", name);
		return true;
	}

	bool
	onKeyValue (
		const char* name,
		const char* value
		)
	{
		KeyNameMap::Iterator it = KeyNameMap::find (name);
		KeyKind key = it ? it->m_value : KeyKind_Undefined;
		printf ("OnKeyValue '%s' = '%s'\n", name, value);
		return true;
	}
};

void
run ()
{
	bool result;

	const char* filePath = "D:/Prj/Ninja/ioninja/scripts/nj_ssn/nj_ssn_Serial/nj_ssn_Serial.njini";

	io::MappedFile file;
	result = file.open (filePath, io::FileFlag_ReadOnly);
	if (!result)
	{
		printf ("error opening file: %s\n", err::getError ()->getDescription ().cc ());
		return;
	}

	const char* p = (const char*) file.view ();
	uint64_t size = file.getSize ();

	MyParser parser;
	result = parser.parse (filePath, p, (size_t) size);
	if (!result)
	{
		printf ("error parsing file: %s\n", err::getError ()->getDescription ().cc ());
		return;
	}	
}

//.............................................................................

} // namespace test_IniParser
