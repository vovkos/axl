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

#pragma once

#define _AXL_XML_EXPAT_PARSER_H

#include "axl_xml_ExpatError.h"

namespace axl {
namespace xml {

//..............................................................................

class FreeExpatParser
{
public:
	void
	operator () (XML_Parser h)
	{
		::XML_ParserFree(h);
	}
};

//..............................................................................

class ExpatParserRoot: public sl::Handle<XML_Parser, FreeExpatParser>
{
public:
	// creation methods

	bool
	create(const sl::StringRef& encoding = NULL);

	bool
	createNs(
		const sl::StringRef& encoding,
		char separator
		);

	bool
	createNs(char separator)
	{
		return createNs(NULL, separator);
	}

	// informational methods

	XML_Error
	getLastErrorCode() const
	{
		return ::XML_GetErrorCode(m_h);
	}

	err::Error
	getLastError() const
	{
		XML_Error errorCode = getLastErrorCode();
		return errorCode ? (err::Error)ExpatError(errorCode) : (err::Error) &err::g_noError;
	}

	int
	getLineNumber() const
	{
		return ::XML_GetCurrentLineNumber(m_h);
	}

	int
	getColumnNumber() const
	{
		return ::XML_GetCurrentColumnNumber(m_h);
	}

	size_t
	getByteOffset() const
	{
		return ::XML_GetCurrentByteIndex(m_h);
	}

	// handlers

	void*
	getUserData()
	{
		return XML_GetUserData(m_h);
	}

	void
	setUserData(void* userData)
	{
		::XML_SetUserData(m_h, userData);
	}

	void
	setElementHandler(
		XML_StartElementHandler startHandler,
		XML_EndElementHandler endHandler
		)
	{
		::XML_SetElementHandler(m_h, startHandler, endHandler);
	}

	void
	setStartElementHandler(XML_StartElementHandler handler)
	{
		::XML_SetStartElementHandler(m_h, handler);
	}

	void
	setEndElementHandler(XML_EndElementHandler handler)
	{
		::XML_SetEndElementHandler(m_h, handler);
	}

	void
	setCharacterDataHandler(XML_CharacterDataHandler handler)
	{
		::XML_SetCharacterDataHandler(m_h, handler);
	}

	void
	setProcessingInstructionHandler(XML_ProcessingInstructionHandler handler)
	{
		::XML_SetProcessingInstructionHandler(m_h, handler);
	}

	void
	setCommentHandler(XML_CommentHandler handler)
	{
		::XML_SetCommentHandler(m_h, handler);
	}

	void
	setCdataSectionHandler(
		XML_StartCdataSectionHandler startHandler,
		XML_EndCdataSectionHandler endHandler
		)
	{
		::XML_SetCdataSectionHandler(m_h, startHandler, endHandler);
	}

	void
	setStartCdataSectionHandler(XML_StartCdataSectionHandler handler)
	{
		::XML_SetStartCdataSectionHandler(m_h, handler);
	}

	void
	setEndCdataSectionHandler(XML_EndCdataSectionHandler handler)
	{
		::XML_SetEndCdataSectionHandler(m_h, handler);
	}

	void
	setDefaultHandler(XML_DefaultHandler handler)
	{
		::XML_SetDefaultHandler(m_h, handler);
	}

	void
	setDefaultHandlerExpand(XML_DefaultHandler handler)
	{
		::XML_SetDefaultHandlerExpand(m_h, handler);
	}

	void
	setDoctypeDeclHandler(
		XML_StartDoctypeDeclHandler startHandler,
		XML_EndDoctypeDeclHandler endHandler
		)
	{
		::XML_SetDoctypeDeclHandler(m_h, startHandler, endHandler);
	}

	void
	setStartDoctypeDeclHandler(XML_StartDoctypeDeclHandler handler)
	{
		::XML_SetStartDoctypeDeclHandler(m_h, handler);
	}

	void
	setEndDoctypeDeclHandler(XML_EndDoctypeDeclHandler handler)
	{
		::XML_SetEndDoctypeDeclHandler(m_h, handler);
	}

	void
	setNotationDeclHandler(XML_NotationDeclHandler handler)
	{
		::XML_SetNotationDeclHandler(m_h, handler);
	}

	void
	setNamespaceDeclHandler(
		XML_StartNamespaceDeclHandler startHandler,
		XML_EndNamespaceDeclHandler endHandler
		)
	{
		::XML_SetNamespaceDeclHandler(m_h, startHandler, endHandler);
	}

	void
	setStartNamespaceDeclHandler(XML_StartNamespaceDeclHandler handler)
	{
		::XML_SetStartNamespaceDeclHandler(m_h, handler);
	}

	void
	setEndNamespaceDeclHandler(XML_EndNamespaceDeclHandler handler)
	{
		::XML_SetEndNamespaceDeclHandler(m_h, handler);
	}

	void
	setNotStandaloneHandler(XML_NotStandaloneHandler handler)
	{
		::XML_SetNotStandaloneHandler(m_h, handler);
	}

	void
	setUnknownEncodingHandler(XML_UnknownEncodingHandler handler)
	{
		::XML_SetUnknownEncodingHandler(m_h, handler, getUserData());
	}

	// parsing methods

	bool
	parse(
		const void* p,
		size_t size,
		bool isFinal
		)
	{
		XML_Status status = ::XML_Parse(m_h, (const char*) p, size, isFinal);
		return complete(status == XML_STATUS_OK, false);
	}

	bool
	parseBuffer(
		size_t size,
		bool isFinal
		)
	{
		XML_Status status = ::XML_ParseBuffer(m_h, size, isFinal);
		return complete(status == XML_STATUS_OK, false);
	}

	bool
	parseFile(
		const sl::StringRef& fileName,
		size_t blockSize = -1
		);

	void*
	getBuffer(size_t size)
	{
		void* p = ::XML_GetBuffer(m_h, size);
		return complete<void*> (p, NULL);
	}

protected:
	template <typename T>
	T
	complete(
		T result,
		T failResult
		)
	{
		if (result == failResult)
			err::setError(getLastError());

		return result;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class ExpatParser: public ExpatParserRoot
{
public:
	ExpatParser()
	{
		create();
	}

	bool
	create(const sl::StringRef& encoding = NULL)
	{
		return ExpatParserRoot::create(encoding) && setHandlers();
	}

	bool
	createNs(
		const sl::StringRef& encoding,
		char separator
		)
	{
		return ExpatParserRoot::createNs(encoding, separator) && setHandlers();
	}

	bool
	createNs(char separator)
	{
		return ExpatParserRoot::createNs(separator) && setHandlers();
	}

protected:
	// overridables

	void
	onStartElement(
		const char* name,
		const char** attributes
		)
	{
	}

	void
	onEndElement(const char* name)
	{
	}

	void
	onCharacterData(
		const char* string,
		size_t length
		)
	{
	}

	void
	onProcessingInstruction(
		const char* target,
		const char* data
		)
	{
	}

	void
	onComment(const char* data)
	{
	}

	void
	onStartCdataSection()
	{
	}

	void
	onEndCdataSection()
	{
	}

	void
	onDefault(
		const char* string,
		size_t length
		)
	{
	}

	void
	onDefaultExpand(
		const char* string,
		size_t length
		)
	{
	}

	void
	onStartDoctypeDecl(
		const char* doctypeName,
		const char* systemId,
		const char* publicId,
		bool hasInternalSubset
		)
	{
	}

	void
	onEndDoctypeDecl()
	{
	}

	void
	onNotationDecl(
		const char* notationName,
		const char* base,
		const char* systemId,
		const char* publicId
		)
	{
	}

	void
	onStartNamespaceDecl(
		const char* prefix,
		const char* uri
		)
	{
	}

	void
	onEndNamespaceDecl(const char* prefix)
	{
	}

	bool
	onNotStandalone()
	{
		return false;
	}

	bool
	onUnknownEncoding(
		const char* name,
		XML_Encoding* encoding
		)
	{
		return false;
	}

protected:
	bool
	setHandlers()
	{
		setUserData(static_cast<T*> (this));

		if (&T::onStartElement != &ExpatParser::onStartElement)
			setStartElementHandler(startElementHandler);

		if (&T::onEndElement != &ExpatParser::onEndElement)
			setEndElementHandler(endElementHandler);

		if (&T::onCharacterData != &ExpatParser::onCharacterData)
			setCharacterDataHandler(characterDataHandler);

		if (&T::onProcessingInstruction != &ExpatParser::onProcessingInstruction)
			setProcessingInstructionHandler(processingInstructionHandler);

		if (&T::onComment != &ExpatParser::onComment)
			setCommentHandler(commentHandler);

		if (&T::onStartCdataSection != &ExpatParser::onStartCdataSection)
			setStartCdataSectionHandler(startCdataSectionHandler);

		if (&T::onEndCdataSection != &ExpatParser::onEndCdataSection)
			setEndCdataSectionHandler(endCdataSectionHandler);

		if (&T::onDefault != &ExpatParser::onDefault)
			setDefaultHandler(defaultHandler);
		else if (&T::onDefaultExpand != &ExpatParser::onDefaultExpand)
			setDefaultHandlerExpand(defaultHandlerExpand);

		if (&T::onStartDoctypeDecl != &ExpatParser::onStartDoctypeDecl)
			setStartDoctypeDeclHandler(startDoctypeDeclHandler);

		if (&T::onEndDoctypeDecl != &ExpatParser::onEndDoctypeDecl)
			setEndDoctypeDeclHandler(endDoctypeDeclHandler);

		if (&T::onNotationDecl != &ExpatParser::onNotationDecl)
			setNotationDeclHandler(notationDeclHandler);

		if (&T::onStartNamespaceDecl != &ExpatParser::onStartNamespaceDecl)
			setStartNamespaceDeclHandler(startNamespaceDeclHandler);

		if (&T::onEndNamespaceDecl != &ExpatParser::onEndNamespaceDecl)
			setEndNamespaceDeclHandler(endNamespaceDeclHandler);

		if (&T::onNotStandalone != &ExpatParser::onNotStandalone)
			setNotStandaloneHandler(notStandaloneHandler);

		if (&T::onUnknownEncoding != &ExpatParser::onUnknownEncoding)
			setUnknownEncodingHandler(unknownEncodingHandler);

		return true;
	}

	static
	void
	XMLCALL
	startElementHandler(
		void* userData,
		const XML_Char* name,
		const XML_Char** attributes
		)
	{
		((T*)userData)->onStartElement(name, attributes);
	}

	static
	void
	XMLCALL
	endElementHandler(
		void* userData,
		const XML_Char* name
		)
	{
		((T*)userData)->onEndElement(name);
	}

	static
	void
	XMLCALL
	characterDataHandler(
		void* userData,
		const XML_Char* string,
		int length
		)
	{
		((T*)userData)->onCharacterData(string, length);
	}

	static
	void
	XMLCALL
	processingInstructionHandler(
		void* userData,
		const XML_Char* target,
		const XML_Char* data
		)
	{
		((T*)userData)->onProcessingInstruction(target, data);
	}

	static
	void
	XMLCALL
	commentHandler(
		void* userData,
		const XML_Char* data
		)
	{
		((T*)userData)->onComment(data);
	}

	static
	void
	XMLCALL
	startCdataSectionHandler(void* userData)
	{
		((T*)userData)->onStartCdataSection();
	}

	static
	void
	XMLCALL
	endCdataSectionHandler(void* userData)
	{
		((T*)userData)->onEndCdataSection();
	}

	static
	void
	XMLCALL
	defaultHandler(
		void* userData,
		const XML_Char* string,
		int length
		)
	{
		((T*)userData)->onDefault(string, length);
	}

	static
	void
	XMLCALL
	defaultHandlerExpand(
		void* userData,
		const XML_Char* string,
		int length
		)
	{
		((T*)userData)->onDefaultExpand(string, length);
	}

	static
	void
	XMLCALL
	startDoctypeDeclHandler(
		void* userData,
		const XML_Char* doctypeName,
		const XML_Char* systemId,
		const XML_Char* publicId,
		int hasInternalSubset
		)
	{
		((T*)userData)->onStartDoctypeDecl(doctypeName, systemId, publicId, hasInternalSubset != 0);
	}

	static
	void
	XMLCALL
	endDoctypeDeclHandler(void* userData)
	{
		((T*)userData)->onEndDoctypeDecl();
	}

	static
	void
	XMLCALL
	notationDeclHandler(
		void* userData,
		const XML_Char* notationName,
		const XML_Char* base,
		const XML_Char* systemId,
		const XML_Char* publicId
		)
	{
		((T*)userData)->onNotationDecl(notationName, base, systemId, publicId);
	}

	static
	void
	XMLCALL
	startNamespaceDeclHandler(
		void* userData,
		const XML_Char* prefix,
		const XML_Char* uri
		)
	{
		((T*)userData)->onStartNamespaceDecl(prefix, uri);
	}

	static
	void
	XMLCALL
	endNamespaceDeclHandler(
		void* userData,
		const XML_Char* prefix
		)
	{
		((T*)userData)->onEndNamespaceDecl(prefix);
	}

	static
	int
	XMLCALL
	notStandaloneHandler(void* userData)
	{
		return ((T*)userData)->onNotStandalone();
	}

	static
	int
	XMLCALL
	unknownEncodingHandler(
		void* userData,
		const XML_Char* name,
		XML_Encoding* encoding
		)
	{
		return ((T*)userData)->onUnknownEncoding(name, encoding);
	}
};

//..............................................................................

} // namespace xml
} // namespace axl
