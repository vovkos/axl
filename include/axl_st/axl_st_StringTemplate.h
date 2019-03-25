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

#define _AXL_ST_STRINGTEMPLATE_H

#include "axl_st_StringTemplateLexer.h"

namespace axl {
namespace st {

//..............................................................................

bool
gotoEndOfScriptSnippet(
	Lexer* lexer,
	lex::RagelTokenPos* endPos
	);

//..............................................................................

template <typename T>
class StringTemplate
{
protected:
	struct EmitContext: public sl::ListLink
	{
		StringTemplate* m_self;
		sl::String* m_output;
		sl::StringRef m_frame;
		lex::LineCol m_lineCol;
	};

protected:
	sl::AuxList<EmitContext> m_emitContextStack;

public:
	void
	clear()
	{
		m_emitContextStack.clear();
	}

	bool
	process(
		sl::String* output,
		const sl::StringRef& fileName,
		const sl::StringRef& frame
		)
	{
		bool result;

		if (output)
		{
			output->clear();
			output->reserve(frame.getLength());
		}
		else if (!m_emitContextStack.isEmpty())
		{
			output = m_emitContextStack.getTail()->m_output; // append to the last output buffer
		}
		else
		{
			err::setError(err::SystemErrorCode_InvalidParameter);
			return false;
		}

		sl::String scriptSource;
		result = createScript(&scriptSource, fileName, frame);
		if (!result)
			return false;

		if (scriptSource.isEmpty())
		{
			output->copy(frame);
			return true;
		}

		EmitContext emitContext;
		emitContext.m_self = this;
		emitContext.m_output = output;
		emitContext.m_frame = frame;
		m_emitContextStack.insertTail(&emitContext);

		result = static_cast<T*> (this)->runScript(fileName, scriptSource);

		ASSERT(*m_emitContextStack.getTail() == &emitContext);
		m_emitContextStack.removeTail();

		return result;
	}

	bool
	processFile(
		sl::String* output,
		const sl::StringRef& fileName
		)
	{
		io::SimpleMappedFile file;

		return
			file.open(fileName, io::FileFlag_ReadOnly) &&
			process(
				output,
				fileName,
				sl::StringRef((const char*) file.p(), file.getMappingSize())
				);
	}

	bool
	processFileToFile(
		const sl::StringRef& targetFileName,
		const sl::StringRef& frameFileName
		)
	{
		bool result;

		sl::String stringBuffer;
		result = processFile(&stringBuffer, frameFileName);
		if (!result)
			return false;

		io::File targetFile;
		result = targetFile.open(targetFileName);
		if (!result)
			return false;

		size_t size = stringBuffer.getLength();

		result = targetFile.write(stringBuffer, size) != -1;
		if (!result)
			return false;

		targetFile.setSize(size);
		return true;
	}

protected:
	bool
	createScript(
		sl::String* scriptSource,
		const sl::StringRef& fileName,
		const sl::StringRef& frame
		)
	{
		bool result;

		Lexer lexer;
		lexer.create(fileName, frame);

		scriptSource->clear();

		size_t offset = 0;
		Token::Pos pos;

		int line = 0;

		for (;;)
		{
			const Token* token = lexer.getToken();

			if (token->m_token == TokenKind_Error)
			{
				err::setFormatStringError("invalid character '\\x%02x'", (uchar_t) token->m_data.m_integer);
				return false;
			}

			if (token->m_pos.m_offset > offset)
				static_cast<T*> (this)->createPassthroughCall(
					scriptSource,
					offset,
					token->m_pos.m_offset - offset
					);

			if (token->m_token == TokenKind_Eof)
				return true;

			if (token->m_pos.m_line > line)
				scriptSource->append('\n', token->m_pos.m_line - line);

			offset = token->m_pos.m_offset + token->m_pos.m_length;

			switch (token->m_token)
			{
			case TokenKind_Data:
				static_cast<T*> (this)->createEmitCall(scriptSource, token->m_data.m_string);
				pos = token->m_pos;
				lexer.nextToken();
				break;

			case TokenKind_OpenCode:
				result = gotoEndOfScriptSnippet(&lexer, &pos);
				if (!result)
					return false;

				scriptSource->append(frame.getSubString(offset, pos.m_offset - offset));
				scriptSource->append(";");

				if (*(pos.m_p + pos.m_length - 1) == '\n')
					pos.m_line--; // fix line cause we have eaten a line-feed with close bracket
				break;

			case TokenKind_OpenData_r:
			case TokenKind_OpenData_c:
				result = gotoEndOfScriptSnippet(&lexer, &pos);
				if (!result)
					return false;

				static_cast<T*> (this)->createEmitCall(
					scriptSource,
					frame.getSubString(offset, pos.m_offset - offset)
					);
				break;

			default:
				ASSERT(false);
			}

			offset = pos.m_offset + pos.m_length;
			line = pos.m_line;
		}
	}
};

//..............................................................................

} // namespace st
} // namespace axl
