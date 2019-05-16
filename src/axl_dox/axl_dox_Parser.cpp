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
#include "axl_dox_Parser.h"
#include "axl_dox_Module.h"
#include "axl_dox_Lexer.h"
#include "axl_dox_Host.h"

namespace axl {
namespace dox {

//..............................................................................

void
Parser::init()
{
	m_block = NULL;
	m_parentBlock = NULL;
	m_blockTargetKind = BlockTargetKind_None;
	m_overloadIdx = 0;
	m_lastPos.clear();
}

void
Parser::reset()
{
	init();

	m_firstIndent.clear();
	m_indent.clear();
	m_overloadName.clear();
	m_groupStack.clear();
}

void
Parser::setBlockTarget(
	int tokenKind,
	const sl::StringRef& name
	)
{
	switch (tokenKind)
	{
	case TokenKind_Overload:
		if (m_overloadName == name)
		{
			m_overloadIdx++;
			break;
		}

		// else fall through

	case TokenKind_Function:
		m_overloadName = name;
		m_overloadIdx = 0;
		break;

	default:
		m_overloadName.clear();
		m_overloadIdx = 0;
	}

	m_module->setBlockTarget(m_block, tokenKind, name, m_overloadIdx);

	m_blockTargetKind = m_block == m_parentBlock ?
		BlockTargetKind_Compound :
		BlockTargetKind_Member;
}

Block*
Parser::popBlock()
{
	// only pick up unassigned non-group blocks

	Block* doxyBlock = NULL;

	if (m_block && !m_blockTargetKind)
	{
		if (m_block->getBlockKind() == BlockKind_Footnote)
			m_block = ((Footnote*)m_block)->getParent();

		if (m_block->getBlockKind() != BlockKind_Group)
			doxyBlock = m_block;
	}

	m_block = NULL;
	m_blockTargetKind = BlockTargetKind_None;

	if (!m_groupStack.isEmpty())
	{
		GroupStackEntry entry = m_groupStack.getBack();

		if (entry.m_namespace == m_module->getHost()->getCurrentNamespace())
		{
			if (!doxyBlock)
				doxyBlock = m_module->createBlock();

			if (!doxyBlock->m_group)
				doxyBlock->m_group = entry.m_group;
		}
	}

	return doxyBlock;
}

void
Parser::addComment(
	const sl::StringRef& comment,
	const lex::LineCol& pos,
	bool isSingleLine,
	handle_t lastDeclaredItem
	)
{
	Host* host = m_module->getHost();

	bool canAppend = isSingleLine && pos.m_line == m_lastPos.m_line + 1;

	if (!m_block || !canAppend)
	{
		m_block = m_module->createBlock();
		m_blockTargetKind = BlockTargetKind_None;
	}

	if (!m_block->m_source.isEmpty())
		m_block->m_source.append('\n');

	m_block->m_source.append(comment);

	if (isSingleLine)
		m_lastPos = pos;

	if (lastDeclaredItem)
	{
		host->setItemBlock(lastDeclaredItem, m_block);
		m_blockTargetKind = BlockTargetKind_Member;
	}

	Lexer lexer;
	lexer.create("doxy", comment);
	lexer.setLineCol(pos);

	int lastTokenLine = -1;

	for (;;)
	{
		const DoxyToken* token = lexer.getToken();
		Footnote* footnote;
		Param* param;
		size_t i;
		bool isParamUsed;

		sl::StringRef commandParam;
		bool isParentBlock = false;

		switch (token->m_token)
		{
		case TokenKind_Error:
			m_block = NULL;
			m_blockTargetKind = BlockTargetKind_None;
			return;

		case TokenKind_Eof:
			return;

		case TokenKind_Namespace:
		case TokenKind_Enum:
		case TokenKind_Struct:
		case TokenKind_Union:
		case TokenKind_Class:
			isParentBlock = true;
			// fall through

		case TokenKind_EnumValue:
		case TokenKind_Alias:
		case TokenKind_Variable:
		case TokenKind_Field:
		case TokenKind_Function:
		case TokenKind_Overload:
		case TokenKind_Property:
		case TokenKind_Event:
		case TokenKind_Typedef:
			commandParam = lexer.getCommandParam();
			if (commandParam.isEmpty())
				break; // ignore

			if (m_blockTargetKind || m_block->getBlockKind() == BlockKind_Footnote) // create a new one
				m_block = m_module->createBlock();

			if (isParentBlock)
				m_parentBlock = m_block;

			setBlockTarget((TokenKind)token->m_token, commandParam);
			lexer.nextToken();
			break;

		case TokenKind_Group:
			commandParam = lexer.getCommandParam();
			if (commandParam.isEmpty())
				break; // ignore

			i = commandParam.findOneOf(" \t");
			if (i == -1)
			{
				m_block = m_module->getGroup(commandParam);
			}
			else
			{
				Group* group = m_module->getGroup(commandParam.getSubString(0, i));
				group->m_title = commandParam.getSubString(i + 1).getLeftTrimmedString();

				m_block = group;
			}

			m_parentBlock = m_block;
			m_blockTargetKind = BlockTargetKind_Compound;
			m_block->m_currentDescription = &m_block->m_detailedDescription;
			lexer.nextToken();
			break;

		case TokenKind_InGroup:
			commandParam = lexer.getCommandParam();
			if (commandParam.isEmpty())
				break; // ignore

			if (!m_block->m_group)
			{
				Group* group = m_module->getGroup(commandParam);
				m_block->m_group = group;
				if (m_block->getBlockKind() == BlockKind_Group)
				{
					Group* innerGroup = (Group*)m_block;
					innerGroup->m_parentGroupListIt = group->addGroup(innerGroup);
				}
			}

			lexer.nextToken();
			break;

		case TokenKind_OpeningBrace:
			if (m_block->getBlockKind() == BlockKind_Group)
			{
				GroupStackEntry entry;
				entry.m_group = (Group*)m_block;
				entry.m_namespace = host->getCurrentNamespace();
				m_groupStack.append(entry);
			}

			break;

		case TokenKind_ClosingBrace:
			m_groupStack.pop();
			break;

		case TokenKind_Title:
			commandParam = lexer.getCommandParam();
			if (commandParam.isEmpty())
				break; // ignore

			m_block->m_title = commandParam;
			lexer.nextToken();
			break;

		case TokenKind_Import:
			commandParam = lexer.getCommandParam();
			if (commandParam.isEmpty())
				break; // ignore

			m_block->m_importList.insertTail(commandParam);
			lexer.nextToken();
			break;

		case TokenKind_SubGroup:
			m_block->m_internalDescription += "%subgroup%";
			break;

		case TokenKind_GroupOrder:
			commandParam = lexer.getCommandParam();
			if (commandParam.isEmpty())
				break; // ignore

			m_block->m_internalDescription += "%grouporder(" + commandParam + ")";
			lexer.nextToken();
			break;

		case TokenKind_Footnote:
			commandParam = lexer.getCommandParam();
			if (commandParam.isEmpty())
				break; // ignore

			if (m_block->getBlockKind() == BlockKind_Footnote)
			{
				m_block = ((Footnote*)m_block)->getParent();
				ASSERT(m_block->getBlockKind() != BlockKind_Footnote);
			}
			else if (m_block->m_currentDescription->isEmpty() && m_parentBlock)
			{
				// standalone footnotes go to the parent block
				m_block = m_parentBlock;
			}

			footnote = m_module->createFootnote();
			footnote->m_refId = commandParam;
			footnote->m_parent = m_block;
			footnote->m_parent->m_footnoteArray.append(footnote);

			m_block = footnote;
			lexer.nextToken();
			break;

		case TokenKind_Brief:
			m_block->m_currentDescription = &m_block->m_briefDescription;

			if (!m_block->m_currentDescription->isEmpty())
				m_block->m_currentDescription->append('\n');

			break;

		case TokenKind_Details:
			m_block->m_currentDescription = &m_block->m_detailedDescription;

			if (!m_block->m_currentDescription->isEmpty())
				m_block->m_currentDescription->append('\n');

			break;

		case TokenKind_Param:
			commandParam = lexer.getCommandParam();
			if (commandParam.isEmpty())
				break; // ignore

			param = AXL_MEM_NEW(Param);

			i = commandParam.findOneOf(" \t");
			if (i == -1)
			{
				param->m_name = commandParam;
			}
			else
			{
				param->m_name = commandParam.getSubString(0, i);
				param->m_description = commandParam.getSubString(i + 1).getLeftTrimmedString();
			}

			m_block->m_paramList.insertTail(param);
			m_block->m_currentDescription = &param->m_description;
			lexer.nextToken();
			break;

		case TokenKind_Return:
			m_block->m_currentDescription = &m_block->m_returnDescription;
			break;

		case TokenKind_SeeAlso:
			m_block->m_currentDescription = &m_block->m_seeAlsoDescription;
			break;

		case TokenKind_Internal:
			m_block->m_currentDescription = &m_block->m_internalDescription;
			break;

		case TokenKind_CustomCommand:
			commandParam = lexer.getCommandParam();
			isParamUsed = host->processCustomCommand(token->m_data.m_string, commandParam, m_block);

			if (!commandParam.isEmpty() && isParamUsed)
				lexer.nextToken();
			break;

		case TokenKind_Text:
			if (m_block->m_currentDescription->isEmpty())
			{
				m_block->m_currentDescription->copy(token->m_data.m_string.getLeftTrimmedString());
				m_firstIndent = m_indent;
			}
			else
			{
				if (!m_indent.isEmpty())
				{
					size_t indentLength = m_indent.getLength();
					size_t firstIndentLength = m_firstIndent.getLength();
					size_t commonIndentLength = AXL_MIN(indentLength, firstIndentLength);

					size_t i = 0;
					for (; i < commonIndentLength; i++)
						if (m_indent[i] != m_firstIndent[i])
							break;

					if (i < indentLength)
						m_block->m_currentDescription->append(m_indent.sz() + i, indentLength - i);

					m_indent.clear(); // do this once per line
				}

				if (!m_block->m_currentDescription->isEmpty() &&
					!isspace(m_block->m_currentDescription->getEnd()[-1]))
					m_block->m_currentDescription->append(' ');

				m_block->m_currentDescription->append(token->m_data.m_string);
			}

			break;

		case '\n':
			if (lastTokenLine != token->m_pos.m_line &&
				m_block->m_currentDescription != &m_block->m_detailedDescription &&
				!m_block->m_currentDescription->isEmpty()) // empty line ends paragraph
			{
				m_block->m_currentDescription = &m_block->m_detailedDescription;
			}
			else if (!m_block->m_currentDescription->isEmpty())
			{
				m_block->m_currentDescription->append('\n');
			}

			m_indent = token->m_data.m_string;
			break;
		}

		lastTokenLine = token->m_pos.m_line;
		lexer.nextToken();
	}
}

//..............................................................................

} // namespace dox
} // namespace axl
