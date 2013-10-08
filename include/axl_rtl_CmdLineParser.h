// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_RTL_CMDLINEPARSER_H

#include "axl_rtl_String.h"

namespace axl {
namespace rtl {

//.............................................................................

class CCmdLineParserRoot
{
protected:		
	static
	size_t
	ExtractArg (
		const char* p,
		const char* pEnd,
		rtl::CString* pArg
		);

	static
	bool
	ParseArg (
		const char* p,
		rtl::CString* pSwitchName,
		rtl::CString* pValue
		);
};

//.............................................................................

template <typename T>
class CCmdLineParserT: protected CCmdLineParserRoot
{
public:
	bool
	Parse (
		const char* pCmdLine,
		size_t Length = -1
		)
	{
		bool Result;

		if (Length == -1)
			Length = strlen (pCmdLine);

		const char* p = pCmdLine;
		const char* pEnd = p + Length;

		rtl::CString Arg;
		rtl::CString SwitchName;
		rtl::CString Value;

		for (int i = 0; p < pEnd; i++)
		{
			size_t Length = ExtractArg (p, pEnd, &Arg);
			if (Length == -1)
				return false;

			if (ArgString.IsEmpty ())
				break;

			Result = 
				ParseArg (Arg, &Switch, &Value) &&
				ProcessArg (i, Switch, &Value);

			if (!Result)
				return false;

			p += Length;
		}

		return static_cast <T*> (this)->Finalize ();
	}

	bool
	Parse (
		const wchar_t* pCmdLine,
		size_t Length = -1
		)
	{
		rtl::CString CmdLine (pCmdLine, Length);
		return Parse (CmdLine, CmdLine.GetLength ());
	}

	bool
	Parse (
		int argc,
		const char* const* argv
		)
	{
		bool Result;

		rtl::CString SwitchName;
		rtl::CString Value;

		for (int i = 0; i < argc; i++)
		{
			Result = 
				ParseArg (argv [i], &SwitchName, &Value) &&
				ProcessArg (i, SwitchName, Value);

			if (!Result)
				return false;
		}

		return static_cast <T*> (this)->Finalize ();
	}

	bool
	Parse (
		int argc,
		const wchar_t* const* argv
		)
	{
		bool Result;

		rtl::CString SwitchName;
		rtl::CString Value;

		for (int i = 0; i < argc; i++)
		{
			Result = 
				ParseArg (rtl::CString (argv [i]), &SwitchName, &Value) &&
				ProcessArg (i, SwitchName, Value);

			if (!Result)
				return false;
		}

		return static_cast <T*> (this)->Finalize ();
	}

	// overridables

	bool 
	OnValue0 (const char* pValue)
	{
		return true;
	}

	// bool 
	// OnValue (const char* pValue);

	// bool 
	// OnSwitch (
	//		const char* pSwitch,
	//		const char* pValue
	//		);

	// bool 
	// OnSwitch (
	//		char Switch,
	//		const char* pValue
	//		);

	bool 
	Finalize ()
	{
		return true;
	}


protected:
	bool
	ProcessArg (
		int i,
		const rtl::CString& SwitchName,
		const rtl::CString& Value
		)
	{	
		T* pThis = static_cast <T*> (this);

		return SwitchName.IsEmpty () ? 
			i == 0 ? 
				pThis->OnValue0 (Value) : 
				pThis->OnValue (Value) :
			SwitchName.GetLength () == 1 ? 
				pThis->OnSwitch (SwitchName [0], Value) :
				pThis->OnSwitch (SwitchName, Value);
	}
};

//.............................................................................

} // namespace rtl
} // namespace axl
