// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

namespace axl {
namespace pg {

//.............................................................................

// forwards

class CClass;
class CGrammarNode;
class CSymbolNode;
class CBeaconNode;
class CDispatcherNode;
class CLaDfaNode;

//.............................................................................

enum ENode
{
	ENode_Undefined = 0,
	ENode_Epsilon,
	ENode_Token,
	ENode_Symbol,
	ENode_Sequence,
	ENode_Action,
	ENode_Argument,
	ENode_Beacon,
	ENode_Dispatcher,
	ENode_Conflict,
	ENode_LaDfa,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum ENodeFlag
{
	ENodeFlag_RecursionStopper = 0x0001,
	ENodeFlag_IsReachable      = 0x0002,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CNode: public rtl::TListLink
{
public:
	ENode m_Kind;
	size_t m_Index;
	size_t m_MasterIndex;

	rtl::CString m_Name;
	int m_Flags;

public:
	CNode ();

	virtual
	~CNode ()
	{
	}

	virtual 
	void
	Trace ();

	virtual 
	void
	Export (lua::CLuaState* pLuaState)
	{
	}

	bool
	IsReachable ()
	{ 
		return (m_Flags & ENodeFlag_IsReachable) != 0;
	}

	virtual
	bool
	MarkReachable ();

	virtual 
	rtl::CString 
	GetProductionString ()
	{
		return m_Name;
	}
};

//.............................................................................

enum EGrammarNodeFlag
{
	EGrammarNodeFlag_IsNullable = 0x0010,
	EGrammarNodeFlag_IsFinal    = 0x0020,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CGrammarNode: public CNode
{
public:
	lex::CSrcPos m_SrcPos;

	rtl::CArrayT <CSymbolNode*> m_FirstArray;
	rtl::CArrayT <CSymbolNode*> m_FollowArray;
	
	rtl::CBitMap m_FirstSet;
	rtl::CBitMap m_FollowSet;

public:
	virtual 
	void
	Trace ();

	bool
	IsNullable ()
	{ 
		return (m_Flags & EGrammarNodeFlag_IsNullable) != 0;
	}

	bool
	IsFinal ()
	{ 
		return (m_Flags & EGrammarNodeFlag_IsFinal) != 0;
	}

	bool
	MarkNullable ();

	bool
	MarkFinal ();

protected:
	void
	ExportSrcPos (
		lua::CLuaState* pLuaState,
		const lex::CLineCol& LineCol
		);
};

//.............................................................................

enum ESymbolNodeFlag
{
	ESymbolNodeFlag_IsNamed        = 0x0100,
	ESymbolNodeFlag_IsEofToken     = 0x0200,
	ESymbolNodeFlag_IsAnyToken     = 0x0400,
	ESymbolNodeFlag_IsPragma       = 0x0800,
	ESymbolNodeFlag_IsStart        = 0x1000,
	ESymbolNodeFlag_IsNoAst        = 0x2000,
	ESymbolNodeFlag_IsResolverUsed = 0x4000,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CSymbolNode: public CGrammarNode
{
public:
	int m_CharToken;

	CClass* m_pClass;
	CGrammarNode* m_pResolver;
	size_t m_ResolverPriority;
	rtl::CArrayT <CGrammarNode*> m_ProductionArray;

	rtl::CString m_Arg;
	rtl::CString m_Local;
	rtl::CString m_Enter;
	rtl::CString m_Leave;

	lex::CLineCol m_ArgLineCol;
	lex::CLineCol m_LocalLineCol;
	lex::CLineCol m_EnterLineCol;
	lex::CLineCol m_LeaveLineCol;

	rtl::CBoxListT <rtl::CString> m_ArgNameList;
	rtl::CBoxListT <rtl::CString> m_LocalNameList;
	rtl::CStringHashTable m_ArgNameSet;
	rtl::CStringHashTable m_LocalNameSet;

public:
	CSymbolNode ();

	rtl::CString
	GetArgName (size_t Index);

	void
	AddProduction (CGrammarNode* pNode);

	virtual
	bool
	MarkReachable ();

	virtual 
	void
	Trace ();

	virtual 
	void
	Export (lua::CLuaState* pLuaState);
};

//.............................................................................

class CSequenceNode: public CGrammarNode
{
public:
	rtl::CArrayT <CGrammarNode*> m_Sequence;

public:
	CSequenceNode ();

	void
	Append (CGrammarNode* pNode);

	virtual
	bool
	MarkReachable ();

	virtual 
	void
	Trace ();

	virtual 
	void
	Export (lua::CLuaState* pLuaState);

	virtual 
	rtl::CString 
	GetProductionString ();
};

//.............................................................................

class CUserNode: public CGrammarNode
{
public:
	CSymbolNode* m_pProductionSymbol;
	CDispatcherNode* m_pDispatcher;
	CGrammarNode* m_pResolver;

	CUserNode ();
};

//.............................................................................

class CActionNode: public CUserNode
{
public:
	rtl::CString m_UserCode;

public:
	CActionNode ();

	virtual 
	void
	Trace ();

	virtual 
	void
	Export (lua::CLuaState* pLuaState);
};

//.............................................................................

class CArgumentNode: public CUserNode
{
public:
	CSymbolNode* m_pTargetSymbol;
	rtl::CBoxListT <rtl::CString> m_ArgValueList;

public:
	CArgumentNode ();

	virtual 
	void
	Trace ();

	virtual 
	void
	Export (lua::CLuaState* pLuaState);
};

//.............................................................................

enum EBeaconNodeFlag
{
	EBeaconNodeFlag_IsAdded   = 0x100,
	EBeaconNodeFlag_IsDeleted = 0x200,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CBeaconNode: public CGrammarNode
{
public:
	rtl::CString m_Label;
	size_t m_SlotIndex;
	CSymbolNode* m_pTarget;
	CArgumentNode* m_pArgument;
	CGrammarNode* m_pResolver;

public:
	CBeaconNode ();

	virtual
	bool
	MarkReachable ();

	virtual 
	void
	Trace ();

	virtual 
	void
	Export (lua::CLuaState* pLuaState);
};

//.............................................................................

class CDispatcherNode: public CNode
{
public:
	CSymbolNode* m_pSymbol;
	rtl::CArrayT <CBeaconNode*> m_BeaconArray;

public:
	CDispatcherNode ()
	{
		m_Kind = ENode_Dispatcher;
	}

	virtual 
	void
	Trace ();

	virtual 
	void
	Export (lua::CLuaState* pLuaState);
};

//.............................................................................

class CConflictNode: public CNode
{
public:
	CSymbolNode* m_pSymbol;
	CSymbolNode* m_pToken;
	CNode* m_pResultNode; // lookahead DFA or immediate production
	rtl::CArrayT <CGrammarNode*> m_ProductionArray;

public:
	CConflictNode ();

	virtual 
	void
	Trace ();

	virtual 
	void
	Export (lua::CLuaState* pLuaState)
	{
		ASSERT (false); // all the conflicts should be resolved
	}

	void
	PushError ()
	{
		err::PushFormatStringError (_T("conflict at '%s':'%s'"), m_pSymbol->m_Name, m_pToken->m_Name);
	}
};

//.............................................................................

enum ELaDfaNodeFlag
{
	ELaDfaNodeFlag_IsLeaf     = 0x100,
	ELaDfaNodeFlag_IsResolved = 0x200,	
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CLaDfaNode: public CNode
{
public:
	CSymbolNode* m_pToken;
	CGrammarNode* m_pResolver;
	CNode* m_pResolverElse;
	CLaDfaNode* m_pResolverUplink;
	CNode* m_pProduction;

	rtl::CArrayT <CLaDfaNode*> m_TransitionArray;

public:
	CLaDfaNode ();

	virtual 
	void
	Trace ();

	virtual 
	void
	Export (lua::CLuaState* pLuaState);

protected:
	void
	ExportResolverMembers (lua::CLuaState* pLuaState);
};

//.............................................................................

template <typename T>
void 
TraceNodeList (
	const tchar_t* pName, 
	rtl::CIteratorT <T> Node
	)
{
	printf (_T("%s\n"), pName);

	for (; Node; Node++)
	{
		T* pNode = *Node;

		printf ("%3d/%-3d\t", pNode->m_Index, pNode->m_MasterIndex);
		pNode->Trace ();
	}
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
void 
TraceNodeArray (
	const tchar_t* pName, 
	const rtl::CArrayT <T*>* pArray
	)
{
	printf (_T("%s\n"), pName);

	size_t Count = pArray->GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		T* pNode = (*pArray) [i];

		printf ("%3d/%-3d ", pNode->m_Index, pNode->m_MasterIndex);
		pNode->Trace ();
	}
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
rtl::CString
NodeArrayToString (const rtl::CArrayT <T*>* pArray)
{
	size_t Count = pArray->GetCount ();
	if (!Count)
		return rtl::CString ();

	rtl::CString String = (*pArray) [0]->m_Name;

	for (size_t i = 1; i < Count; i++)
	{
		CNode* pNode = (*pArray) [i];
		String += _T(' ');				
		String += pNode->m_Name;
	}
		
	return String;
}

//.............................................................................

} // namespace pg {
} // namespace axl {
