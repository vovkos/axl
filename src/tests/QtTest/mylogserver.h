#pragma once

#define MYLOGSERVER_H

#include "mylogrepresenter.h"
#include "jancycolorizer.h"

//.............................................................................

#ifdef _JANCY_REPRESENTER

class StdLib: public jnc::CStdLib
{
public:
	StdLib ();
	
	static
	int
	Printf (
		const char* pFormat,
		...
		);	
};

#endif

//.............................................................................

class MyLogServer: public log::CServer
{
protected:
#ifdef _JANCY_REPRESENTER
	jnc::CModule m_module;
	jnc::CRuntime m_runtime;
	StdLib m_stdlib;

	log::CJancyRepresenter m_logRepresenter;
#else
	MyLogRepresenter m_logRepresenter;
#endif

	JancyColorizer m_logColorizer;	

public:
	bool start (
		log::IClient* client,
		const char* logPacketFilePath,
		const char* logMergeFilePath,
		const char* logColorizerStateFilePath
		);

protected:
#ifdef _JANCY_REPRESENTER
	bool compile (const char* representerClassName, size_t FileCount, ...);
#endif
};


//.............................................................................
