#pragma once

#define MYLOGSERVER_H

#include "mylogrepresenter.h"
#include "jancycolorizer.h"

//.............................................................................

#ifdef _JANCY_REPRESENTER

class stdLib: public jnc::CStdLib
{
public:
	stdLib ();
	
	static
	int
	printf (
		const char* format,
		...
		);	
};

#endif

//.............................................................................

class myLogServer: public log::CServer
{
protected:
#ifdef _JANCY_REPRESENTER
	jnc::CModule m_module;
	jnc::CRuntime m_runtime;
	stdLib m_stdlib;

	log::CJancyRepresenter m_logRepresenter;
#else
	myLogRepresenter m_logRepresenter;
#endif

	jancyColorizer m_logColorizer;	

public:
	bool start (
		log::CClientPeer* client,
		const char* logPacketFilePath,
		const char* logMergeFilePath,
		const char* logColorizerStateFilePath
		);

protected:
#ifdef _JANCY_REPRESENTER
	bool compile (const char* representerClassName, size_t fileCount, ...);
#endif
};


//.............................................................................
