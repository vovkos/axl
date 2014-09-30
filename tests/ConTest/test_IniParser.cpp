#include "pch.h"

using namespace axl;

namespace test_IniParser { 

//.............................................................................

class CMyParser: public ini::CParserT <CMyParser>
{
protected:
	enum ESection
	{
		ESection_Undefined,
		ESection_Session,
		ESection_Scripts,
	};

	enum EKey
	{
		EKey_Undefined,
		EKey_Name,
		EKey_Description,
		EKey_Guid,
		EKey_Icon,
		EKey_LogRepresenterClass,
		EKey_SessionClass,
	};

	AXL_RTL_BEGIN_HASH_TABLE_MAP_STRING (CSectionNameMap, ESection)
		AXL_RTL_HASH_TABLE_MAP_ENTRY ("session", ESection_Session)
		AXL_RTL_HASH_TABLE_MAP_ENTRY ("scripts", ESection_Scripts)
	AXL_RTL_END_HASH_TABLE_MAP ()

	AXL_RTL_BEGIN_HASH_TABLE_MAP_STRING (CKeyNameMap, EKey)
		AXL_RTL_HASH_TABLE_MAP_ENTRY ("name", EKey_Name)
		AXL_RTL_HASH_TABLE_MAP_ENTRY ("description", EKey_Description)
		AXL_RTL_HASH_TABLE_MAP_ENTRY ("guid", EKey_Guid)
		AXL_RTL_HASH_TABLE_MAP_ENTRY ("icon", EKey_Icon)
		AXL_RTL_HASH_TABLE_MAP_ENTRY ("session-class", EKey_SessionClass)
		AXL_RTL_HASH_TABLE_MAP_ENTRY ("log-representer-class", EKey_LogRepresenterClass)
	AXL_RTL_END_HASH_TABLE_MAP ()

public:
	bool 
	onSection (const char* name)
	{
		CSectionNameMap::CIterator it = CSectionNameMap::find (name);
		ESection section = it ? it->m_value : ESection_Undefined;
		printf ("OnSection '%s'\n", name);
		return true;
	}

	bool
	onKeyValue (
		const char* name,
		const char* value
		)
	{
		CKeyNameMap::CIterator it = CKeyNameMap::find (name);
		EKey key = it ? it->m_value : EKey_Undefined;
		printf ("OnKeyValue '%s' = '%s'\n", name, value);
		return true;
	}
};

void
run ()
{
	bool result;

	const char* filePath = "D:/Prj/Ninja/ioninja/scripts/nj_ssn/nj_ssn_Serial/nj_ssn_Serial.njini";

	io::CMappedFile file;
	result = file.open (filePath, io::EFileFlag_ReadOnly);
	if (!result)
	{
		printf ("error opening file: %s\n", err::getError ()->getDescription ().cc ());
		return;
	}

	const char* p = (const char*) file.view ();
	uint64_t size = file.getSize ();

	CMyParser parser;
	result = parser.parse (filePath, p, (size_t) size);
	if (!result)
	{
		printf ("error parsing file: %s\n", err::getError ()->getDescription ().cc ());
		return;
	}	
}

//.............................................................................

} // namespace test_IniParser
