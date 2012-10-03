// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_pg_NodeMgr.h"

namespace axl {
namespace pg {

//.............................................................................

class CParseTableBuilder
{
protected:
	CNodeMgr* m_pNodeMgr;
	rtl::CArrayT <CNode*>* m_pParseTable;

public:
	CParseTableBuilder (
		CNodeMgr* pNodeMgr,
		rtl::CArrayT <CNode*>* pParseTable
		)
	{
		m_pNodeMgr = pNodeMgr;
		m_pParseTable = pParseTable;
	}

	bool
	Build ();

protected:
	void
	CalcFirstFollow ();

	void
	BuildFirstFollowArrays (CGrammarNode* pNode);

	void
	AddProductionToParseTable (
		CSymbolNode* pSymbol,
		CGrammarNode* pProduction
		);

	void
	AddAnyTokenProductionToParseTable (
		CSymbolNode* pSymbol,
		CGrammarNode* pProduction
		);

	size_t 
	AddParseTableEntry (
		CSymbolNode* pSymbol,
		CSymbolNode* pToken,
		CGrammarNode* pProduction
		); // returns number of conflicting productions
};

//.............................................................................

} // namespace pg {
} // namespace axl {
