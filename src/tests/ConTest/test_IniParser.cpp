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

	AXL_RTL_BEGIN_STRING_HASH_TABLE_MAP (CSectionNameMap, ESection)
		AXL_RTL_STRING_HASH_TABLE_MAP_ENTRY ("session", ESection_Session)
		AXL_RTL_STRING_HASH_TABLE_MAP_ENTRY ("scripts", ESection_Scripts)
	AXL_RTL_END_STRING_HASH_TABLE_MAP ()

	AXL_RTL_BEGIN_STRING_HASH_TABLE_MAP (CKeyNameMap, EKey)
		AXL_RTL_STRING_HASH_TABLE_MAP_ENTRY ("name", EKey_Name)
		AXL_RTL_STRING_HASH_TABLE_MAP_ENTRY ("description", EKey_Description)
		AXL_RTL_STRING_HASH_TABLE_MAP_ENTRY ("guid", EKey_Guid)
		AXL_RTL_STRING_HASH_TABLE_MAP_ENTRY ("icon", EKey_Icon)
		AXL_RTL_STRING_HASH_TABLE_MAP_ENTRY ("session-class", EKey_SessionClass)
		AXL_RTL_STRING_HASH_TABLE_MAP_ENTRY ("log-representer-class", EKey_LogRepresenterClass)
	AXL_RTL_END_STRING_HASH_TABLE_MAP ()

public:
	void 
	OnSection (const char* pName)
	{
		CSectionNameMap::CIterator It = CSectionNameMap::Find (pName);
		ESection Section = It ? It->m_Value : ESection_Undefined;
		printf ("OnSection '%s'\n", pName);
	}

	void 
	OnKeyValue (
		const char* pName,
		const char* pValue
		)
	{
		CKeyNameMap::CIterator It = CKeyNameMap::Find (pName);
		EKey Key = It ? It->m_Value : EKey_Undefined;
		printf ("OnKeyValue '%s' = '%s'\n", pName, pValue);
	}
};

void
Run ()
{
	bool Result;

	const char* pFilePath = "D:/Prj/Ninja/ioninja/scripts/nj_ssn/nj_ssn_Serial/nj_ssn_Serial.njini";

	io::CMappedFile File;
	Result = File.Open (pFilePath, io::EFileFlag_ReadOnly);
	if (!Result)
	{
		printf ("error opening file: %s\n", err::GetError ()->GetDescription ().cc ());
		return;
	}

	const char* p = (const char*) File.View ();
	uint64_t Size = File.GetSize ();

	CMyParser Parser;
	Result = Parser.Parse (pFilePath, p, (size_t) Size);
	if (!Result)
	{
		printf ("error parsing file: %s\n", err::GetError ()->GetDescription ().cc ());
		return;
	}	
}

//.............................................................................

} // namespace test_IniParser
