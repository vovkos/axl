// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2010. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LLK_PARSER_H

#include "axl_llk_Node.h"
#include "axl_err_ParseError.h"

namespace axl {
namespace llk {
	
//.............................................................................

template <
	typename T,
	typename TToken = CStdToken
	>
class CParserT
{
public:
	typedef TToken CToken;
	typedef typename TToken::EToken EToken;
	typedef CAstNodeT <CToken> CAstNode;
	typedef CAstT <CAstNode> CAst;
	typedef CTokenNodeT <CToken> CTokenNode;
	typedef CSymbolNodeT <CAstNode> CSymbolNode;
	typedef CLaDfaNodeT <CToken> CLaDfaNode;

protected:
	enum EFlag
	{
		EFlag_IsBuildingAst = 1,
		EFlag_IsTokenMatch  = 2,
		EFlag_IsTokenSaved  = 4,
	};

	enum EMatchResult
	{
		EMatchResult_Fail,
		EMatchResult_NextToken,
		EMatchResult_Continue,
	};

	enum ELaDfaResult
	{
		ELaDfaResult_Fail,
		ELaDfaResult_Production,
		ELaDfaResult_Resolver,
	};		

	struct TLaDfaTransition
	{
		int m_Flags;
		size_t m_ProductionIndex;
		size_t m_ResolverIndex;
		size_t m_ResolverElseIndex;
	};

protected:
	rtl::CStdListT <CNode> m_NodeList;
	ref::CBufT <CAst> m_Ast;

	rtl::CArrayT <CNode*> m_PredictionStack;
	rtl::CArrayT <CSymbolNode*> m_SymbolStack;
	rtl::CArrayT <CLaDfaNode*> m_ResolverStack;
	
	rtl::CBoxListT <CToken> m_TokenList;
	CToken m_LastMatchedToken;

	int m_Flags;
	
public:
	CParserT ()
	{
		m_Flags = 0;
	}

	bool
	Create (
		int Symbol = T::StartSymbol,
		bool IsBuildingAst = false
		)
	{
		Clear ();

		if (IsBuildingAst)
			m_Flags |= EFlag_IsBuildingAst;

		PushPrediction (T::SymbolFirst + Symbol);
		return true;
	}

	ref::CBufT <CAst> 
	GetAst ()
	{
		return m_Ast;
	}

	void
	Clear ()
	{
		m_NodeList.Clear ();
		m_PredictionStack.Clear ();
		m_SymbolStack.Clear ();
		m_ResolverStack.Clear ();
		m_TokenList.Clear ();
		m_Ast.Release ();
		m_Flags = 0;
	}

	bool
	ParseToken (const CToken* pToken)
	{
		size_t* pParseTable = ((T*) this)->GetParseTable ();
		size_t TokenIndex = ((T*) this)->GetTokenIndex (pToken->m_Token);
		ASSERT (TokenIndex < T::TokenCount);

		// first check for pragma productions out of band

		if (T::StartPragmaSymbol != -1)
		{
			size_t ProductionIndex = pParseTable [T::StartPragmaSymbol * T::TokenCount + TokenIndex];
			if (ProductionIndex != -1 && ProductionIndex != 0)
				PushPrediction (ProductionIndex);
		}

		CToken Token = *pToken;

		m_Flags &= ~(EFlag_IsTokenMatch | EFlag_IsTokenSaved);

		for (;;)
		{
			EMatchResult MatchResult;

			CNode* pNode = GetPredictionTop ();
			if (!pNode)
			{
				MatchResult = MatchEmptyPredictionStack (&Token);				
			}
			else
			{
				// if resolver is active, we need to save all tokens for re-parse
				// at the same time we want to prevent saving them twice due to re-submission from lookahead dfa

				CLaDfaNode* pPreResolverNode = GetPreResolverTop ();
				if (pPreResolverNode && pNode->m_Kind != ENode_LaDfa && !(m_Flags & EFlag_IsTokenSaved))
				{
					pPreResolverNode->m_ResolverTokenList.InsertTail (Token);
					m_Flags |= EFlag_IsTokenSaved;
				}
				
				switch (pNode->m_Kind)
				{
				case ENode_Token:
					MatchResult = MatchTokenNode ((CTokenNode*) pNode, &Token, TokenIndex);				
					break;

				case ENode_Symbol:
					MatchResult = MatchSymbolNode ((CSymbolNode*) pNode, &Token, pParseTable, TokenIndex);
					break;

				case ENode_Sequence:
					MatchResult = MatchSequenceNode (pNode, &Token);
					break;

				case ENode_Action:
					MatchResult = MatchActionNode (pNode, &Token);
					break;

				case ENode_Argument:
					ASSERT (pNode->m_Flags & ENodeFlag_IsMatched); // was handled during matching ENode_Symbol
					PopPrediction (); 
					MatchResult = EMatchResult_Continue;
					break;

				case ENode_LaDfa:
					MatchResult = MatchLaDfaNode ((CLaDfaNode*) pNode, &Token);
					break;

				default:
					ASSERT (false);
				}
			}

			if (MatchResult == EMatchResult_Fail)
			{
				if (m_ResolverStack.IsEmpty ())
					return false;

				MatchResult = RollbackResolver ();
			}

			if (MatchResult == EMatchResult_NextToken)
			{
				bool Result = GetNextToken (&Token);
				if (!Result)
					return true; // no more tokens, we are done

				TokenIndex = ((T*) this)->GetTokenIndex (Token.m_Token);
				ASSERT (TokenIndex < T::TokenCount);

				m_Flags &= ~(EFlag_IsTokenMatch | EFlag_IsTokenSaved);
			}
			else
			{
				ASSERT (MatchResult == EMatchResult_Continue);
			}
		}
	}

protected:
	bool
	GetNextToken (CToken* pToken)
	{
		if (m_TokenList.IsEmpty ())
			return false;

		*pToken = m_TokenList.RemoveHead ();
		return true;
	}

	// match against different kinds of nodes on top of prediction stack
	
	EMatchResult 
	MatchEmptyPredictionStack (const CToken* pToken)
	{
		if ((m_Flags & EFlag_IsTokenMatch) || pToken->m_Token == T::EofToken)
			return EMatchResult_NextToken;

		if (m_ResolverStack.IsEmpty ()) // can't rollback so set error
			err::SetFormatStringError (_T("prediction stack empty while parsing '%s'"), pToken->GetName ());

		return EMatchResult_Fail;
	}

	EMatchResult 
	MatchTokenNode (
		CTokenNode* pNode,
		const CToken* pToken,
		size_t TokenIndex
		)
	{
		if (m_Flags & EFlag_IsTokenMatch)
			return EMatchResult_NextToken;

		if (pNode->m_Index != T::AnyToken && pNode->m_Index != TokenIndex)
		{
			if (m_ResolverStack.IsEmpty ()) // can't rollback so set error
			{
				int ExpectedToken = ((T*) this)->GetTokenFromIndex (pNode->m_Index);
				err::SetExpectedTokenError (CToken::GetName (ExpectedToken), pToken->GetName ());
			}

			return EMatchResult_Fail;
		}

		if (pNode->m_Flags & ENodeFlag_IsLocator)
		{
			pNode->m_Token = *pToken;
			pNode->m_Flags |= ENodeFlag_IsMatched;
		}

		m_LastMatchedToken = *pToken;
		m_Flags |= EFlag_IsTokenMatch;

		PopPrediction ();
		return EMatchResult_Continue;
	}

	EMatchResult 
	MatchSymbolNode (
		CSymbolNode* pNode,
		const CToken* pToken,
		size_t* pParseTable,
		size_t TokenIndex
		)
	{
		bool Result;

		if (pNode->m_Flags & ESymbolNodeFlag_IsStacked)
		{
			CSymbolNode* pTop = GetSymbolTop ();

			ASSERT (GetSymbolTop () == pNode);

			if (pNode->m_pAstNode)
				pNode->m_pAstNode->m_LastToken = m_LastMatchedToken;

			pNode->m_Flags |= ENodeFlag_IsMatched;

			if (pNode->m_Flags & ESymbolNodeFlag_HasLeave)
			{
				Result = ((T*) this)->Leave (pNode->m_Index);
				if (!Result)
					return EMatchResult_Fail;
			}

			PopSymbol ();
			PopPrediction ();
			return EMatchResult_Continue;
		}

		if (m_Flags & EFlag_IsTokenMatch)
			return EMatchResult_NextToken;

		if (pNode->m_Flags & ESymbolNodeFlag_IsNamed)
		{
			if (pNode->m_pAstNode)
			{
				pNode->m_pAstNode->m_FirstToken = *pToken;
				pNode->m_pAstNode->m_LastToken = *pToken;

				m_LastMatchedToken = *pToken;
			}
					
			CNode* pArgument = GetArgument ();
			if (pArgument)
			{
				((T*) this)->Argument (pArgument->m_Index, pNode);
				pArgument->m_Flags |= ENodeFlag_IsMatched;
			}

			PushSymbol (pNode);

			if (pNode->m_Flags & ESymbolNodeFlag_HasEnter)
			{
				Result = ((T*) this)->Enter (pNode->m_Index);
				if (!Result)
					return EMatchResult_Fail;
			}
		}

		size_t ProductionIndex = pParseTable [pNode->m_Index * T::TokenCount + TokenIndex];
		if (ProductionIndex == -1)
		{
			if (m_ResolverStack.IsEmpty ()) // can't rollback so set error
			{
				CSymbolNode* pSymbol = GetSymbolTop ();
				ASSERT (pSymbol);
				err::SetFormatStringError (_T("unexpected token '%s' in '%s'"), pToken->GetName (), ((T*) this)->GetSymbolName (pSymbol->m_Index));
			}
			
			return EMatchResult_Fail;
		}

		ASSERT (ProductionIndex < T::TotalCount);

		if (!(pNode->m_Flags & ESymbolNodeFlag_IsNamed))
			PopPrediction ();

		PushPrediction (ProductionIndex);
		return EMatchResult_Continue;
	}

	EMatchResult 
	MatchSequenceNode (
		CNode* pNode,
		const CToken* pToken
		)
	{
		if (m_Flags & EFlag_IsTokenMatch)
			return EMatchResult_NextToken;

		size_t* p = ((T*) this)->GetSequence (pNode->m_Index);

		PopPrediction ();
		for (; *p != -1; *p++) 
			PushPrediction (*p);

		return EMatchResult_Continue;
	}

	EMatchResult 
	MatchActionNode (
		CNode* pNode,
		const CToken* pToken
		)
	{
		bool Result = ((T*) this)->Action (pNode->m_Index);
		if (!Result)
			return EMatchResult_Fail;

		PopPrediction ();
		return EMatchResult_Continue;
	}

	EMatchResult 
	MatchLaDfaNode (
		CLaDfaNode* pNode,
		const CToken* pToken
		)
	{
		if (pNode->m_Flags & ELaDfaNodeFlag_IsPreResolver) 
		{
			ASSERT (GetPreResolverTop () == pNode);

			// successful match of resolver

			size_t ProductionIndex = pNode->m_ResolverThenIndex;
			ASSERT (ProductionIndex < T::LaDfaFirst);

			// add tokens for re-parse 

			m_TokenList.InsertListHead (&pNode->m_ResolverTokenList);
			m_TokenList.InsertListHead (&pNode->m_DfaTokenList);

			PopPreResolver ();
			PopPrediction ();						
			PushPrediction (ProductionIndex);

			return EMatchResult_NextToken;
		}

		TLaDfaTransition Transition = { 0 };
		
		ELaDfaResult Result = ((T*) this)->LaDfa (pNode->m_Index, pToken->m_Token, &Transition);
		switch (Result)
		{
		case ELaDfaResult_Fail:
			if (m_ResolverStack.IsEmpty ()) // can't rollback so set error
			{
				CSymbolNode* pSymbol = GetSymbolTop ();
				ASSERT (pSymbol);					
				err::SetFormatStringError (_T("unexpected token '%s' while trying to resolve conflict in '%s'"), pToken->GetName (), ((T*) this)->GetSymbolName (pSymbol->m_Index));
			}

			return EMatchResult_Fail;

		case ELaDfaResult_Production:
			if (Transition.m_ProductionIndex >= T::LaDfaFirst && 
				Transition.m_ProductionIndex < T::LaDfaEnd)
			{
				// stil in lookahead DFA, need more tokens...				
				pNode->m_DfaTokenList.InsertTail (*pToken);
				pNode->m_Index = Transition.m_ProductionIndex - T::LaDfaFirst;
				return EMatchResult_NextToken;
			}

			break;

		case ELaDfaResult_Resolver:
			pNode->m_Flags = Transition.m_Flags;
			pNode->m_ResolverThenIndex = Transition.m_ProductionIndex;
			pNode->m_ResolverElseIndex = Transition.m_ResolverElseIndex;
			PushPreResolver (pNode);
			PushPrediction (Transition.m_ResolverIndex);
			return EMatchResult_Continue;

		default:
			ASSERT (false);
		}

		// add tokens for re-parse

		m_TokenList.InsertHead (*pToken);
		m_TokenList.InsertListHead (&pNode->m_DfaTokenList);

		// resolve and continue parsing

		PopPrediction ();
		PushPrediction (Transition.m_ProductionIndex);
		return EMatchResult_NextToken;
	}

	// rollback

	EMatchResult
	RollbackResolver ()
	{
		CLaDfaNode* pLaDfaNode = GetPreResolverTop ();
		ASSERT (pLaDfaNode); 
		ASSERT (pLaDfaNode->m_Flags & ELaDfaNodeFlag_IsPreResolver);

		// keep popping prediction stack until pre-resolver dfa node 

		while (!m_PredictionStack.IsEmpty ())
		{
			CNode* pNode = GetPredictionTop ();

			if (pNode->m_Kind == ENode_Symbol && (pNode->m_Flags & ESymbolNodeFlag_IsStacked))
			{
				ASSERT (GetSymbolTop () == pNode);

				// do NOT call OnLeave () during resolver unwinding
				// cause OnLeave () assumes parsing of the symbol is complete

				PopSymbol ();
			}

			if (pNode == pLaDfaNode)
				break; // found it!!

			PopPrediction ();
		}
		
		ASSERT (GetPredictionTop () == pLaDfaNode);
		PopPreResolver ();

		// add tokens for re-parse

		m_TokenList.InsertListHead (&pLaDfaNode->m_ResolverTokenList);

		// switch to resolver-else branch

		if (pLaDfaNode->m_ResolverElseIndex >= T::LaDfaFirst && 
			pLaDfaNode->m_ResolverElseIndex < T::LaDfaEnd)
		{
			// still in lookahead DFA after rollback...

			pLaDfaNode->m_Index = pLaDfaNode->m_ResolverElseIndex - T::LaDfaFirst;
			pLaDfaNode->m_Flags &= ~ELaDfaNodeFlag_IsPreResolver;
			
			if (!(pLaDfaNode->m_Flags & ELaDfaNodeFlag_HasChainedResolver))
			{
				// if no chained resolver, advance one token forward

				rtl::CBoxListEntryT <CToken>* pEntry = m_TokenList.RemoveHeadEntry (); 
				pLaDfaNode->m_DfaTokenList.InsertTailEntry (pEntry);
			}
		}
		else
		{
			// add tokens from DFA for re-parse

			m_TokenList.InsertListHead (&pLaDfaNode->m_DfaTokenList);

			size_t ProductionIndex = pLaDfaNode->m_ResolverElseIndex;
			PopPrediction ();
			PushPrediction (ProductionIndex);
		}

		return EMatchResult_NextToken;
	}

	// create nodes

	static
	CSymbolNode*
	CreateStdSymbolNode (size_t Index)
	{
		CSymbolNode* pNode = AXL_MEM_NEW (CSymbolNode);
		pNode->m_Kind = ENode_Symbol;
		pNode->m_Flags |= ESymbolNodeFlag_IsNamed;
		pNode->m_Index = Index;
		return pNode;
	}

	CNode*
	CreateNode (size_t MasterIndex)
	{
		ASSERT (MasterIndex < T::TotalCount);

		CNode* pNode = NULL;

		if (MasterIndex < T::TokenEnd)
		{
			pNode = AXL_MEM_NEW (CTokenNode);
			pNode->m_Index = MasterIndex;
		}
		else if (MasterIndex < T::NamedSymbolEnd)
		{
			size_t Index = MasterIndex - T::SymbolFirst;
			CSymbolNode* pSymbolNode = ((T*) this)->CreateSymbolNode (Index);
			if (pSymbolNode->m_pAstNode && (m_Flags & EFlag_IsBuildingAst))
			{
				if (!m_Ast)
					m_Ast.Create ();

				m_Ast->Add (pSymbolNode->m_pAstNode);
				pSymbolNode->m_Flags |= ESymbolNodeFlag_KeepAst;
			}

			pNode = pSymbolNode;
		}
		else if (MasterIndex < T::SymbolEnd)
		{
			pNode = AXL_MEM_NEW (CSymbolNode);
			pNode->m_Index = MasterIndex - T::SymbolFirst;
		}
		else if (MasterIndex < T::SequenceEnd)
		{
			pNode = AXL_MEM_NEW (CNode);
			pNode->m_Kind = ENode_Sequence;
			pNode->m_Index = MasterIndex - T::SequenceFirst;
		}
		else if (MasterIndex < T::ActionEnd)
		{
			pNode = AXL_MEM_NEW (CNode);
			pNode->m_Kind = ENode_Action;
			pNode->m_Index = MasterIndex - T::ActionFirst;
		}
		else if (MasterIndex < T::ArgumentEnd)
		{
			pNode = AXL_MEM_NEW (CNode);
			pNode->m_Kind = ENode_Argument;
			pNode->m_Index = MasterIndex - T::ArgumentFirst;
		}
		else if (MasterIndex < T::BeaconEnd)
		{
			size_t* p = ((T*) this)->GetBeacon (MasterIndex - T::BeaconFirst);
			size_t SlotIndex = p [0];
			size_t TargetIndex = p [1];

			pNode = CreateNode (TargetIndex);
			ASSERT (pNode->m_Kind == ENode_Token || pNode->m_Kind == ENode_Symbol);

			pNode->m_Flags |= ENodeFlag_IsLocator;

			CSymbolNode* pSymbolNode = GetSymbolTop ();
			ASSERT (pSymbolNode);

			pSymbolNode->m_LocatorArray.EnsureCount (SlotIndex + 1);
			pSymbolNode->m_LocatorArray [SlotIndex] = pNode;
			pSymbolNode->m_LocatorList.InsertTail (pNode);
		}
		else 
		{
			ASSERT (MasterIndex < T::LaDfaEnd);

			pNode = AXL_MEM_NEW (CLaDfaNode);
			pNode->m_Index = MasterIndex - T::LaDfaFirst;
		}

		return pNode;
	}

	// prediction stack

	CNode*
	GetPredictionTop ()
	{
		size_t Count = m_PredictionStack.GetCount ();
		return Count ? m_PredictionStack [Count - 1] : NULL;
	}

	CNode*
	GetArgument ()
	{
		size_t Count = m_PredictionStack.GetCount ();
		if (Count < 2)
			return NULL;

		CNode* pNode = m_PredictionStack [Count - 2];
		return pNode->m_Kind == ENode_Argument ? pNode : NULL;
	}

	CNode*
	PushPrediction (size_t MasterIndex)
	{
		if (!MasterIndex) // check for epsilon production
			return NULL;

		CNode* pNode = CreateNode (MasterIndex);
		if (!(pNode->m_Flags & ENodeFlag_IsLocator))
			m_NodeList.InsertTail (pNode);
		m_PredictionStack.Append (pNode);
		return pNode;
	}

	void
	PopPrediction ()
	{
		size_t Count = m_PredictionStack.GetCount ();
		if (!Count)
		{
			ASSERT (false);
			return;
		}

		CNode* pNode = m_PredictionStack [Count - 1];
		if (!(pNode->m_Flags & ENodeFlag_IsLocator))
			m_NodeList.Delete (pNode);

		m_PredictionStack.SetCount (Count - 1);
	}

	// debug

	void
	TraceSymbolStack ()
	{
		intptr_t Count = m_SymbolStack.GetCount ();

		TRACE ("SYMBOL STACK (%d symbols):\n", Count);
		for (intptr_t i = 0; i < Count; i++)
		{
			CSymbolNode* pNode = m_SymbolStack [i];
			TRACE ("%s", ((T*) this)->GetSymbolName (pNode->m_Index));

			if (pNode->m_pAstNode)
				TRACE (" (%d:%d)", pNode->m_pAstNode->m_FirstToken.m_Pos.m_Line + 1, pNode->m_pAstNode->m_FirstToken.m_Pos.m_Col + 1);

			TRACE ("\n");
		}
	}

	void
	TracePredictionStack ()
	{
		intptr_t Count = m_PredictionStack.GetCount ();

		TRACE ("PREDICTION STACK (%d nodes):\n", Count);
		for (intptr_t i = 0; i < Count; i++)
		{
			CNode* pNode = m_PredictionStack [i];
			TRACE ("%s (%d)\n", GetNodeKindString (pNode->m_Kind), pNode->m_Index);
		}
	}

	void
	TraceTokenList ()
	{
		rtl::CBoxIteratorT <CToken> Token = m_TokenList.GetHead ();

		TRACE ("TOKEN LIST (%d tokens):\n", m_TokenList.GetCount ());
		for (; Token; Token++)
		{
			TRACE ("%s '%s'\n", Token->GetName (), Token->GetText ());
		}
	}

	// symbol stack

	size_t 
	GetSymbolStackSize ()
	{
		return m_SymbolStack.GetCount ();
	}

	CAstNode*
	GetAst (size_t Index)
	{
		size_t Count = m_SymbolStack.GetCount ();
		return Index < Count ? m_SymbolStack [Count - Index - 1]->m_pAstNode : NULL;
	}

	CAstNode*
	GetAstTop ()
	{
		size_t Count = m_SymbolStack.GetCount ();
		for (intptr_t i = Count - 1; i >= 0; i--)
		{
			CSymbolNode* pNode = m_SymbolStack [i];
			if (pNode->m_pAstNode)
				return pNode->m_pAstNode;
		}

		return NULL;
	}

	CSymbolNode*
	GetSymbolTop ()
	{
		size_t Count = m_SymbolStack.GetCount ();
		return Count ? m_SymbolStack [Count - 1] : NULL;
	}

	void
	PushSymbol (CSymbolNode* pNode)
	{
		if ((m_Flags & EFlag_IsBuildingAst) && pNode->m_pAstNode)
		{
			CAstNode* pAstTop = GetAstTop ();
			if (pAstTop)
			{
				pNode->m_pAstNode->m_pParent = pAstTop;
				pAstTop->m_Children.Append (pNode->m_pAstNode);
			}
		}

		m_SymbolStack.Append (pNode);
		pNode->m_Flags |= ESymbolNodeFlag_IsStacked;
	}

	void
	PopSymbol()
	{
		size_t Count = m_SymbolStack.GetCount ();
		if (!Count)
		{
			ASSERT (false);
			return;
		}

		CSymbolNode* pNode = m_SymbolStack [Count - 1];
		pNode->m_Flags |= ESymbolNodeFlag_IsStacked;

		m_SymbolStack.SetCount (Count - 1);
	}

	// resolver stack

	CLaDfaNode*
	GetPreResolverTop ()
	{
		size_t Count = m_ResolverStack.GetCount ();
		return Count ? m_ResolverStack [Count - 1] : NULL;
	}
	
	void
	PushPreResolver (CLaDfaNode* pNode)
	{
		m_ResolverStack.Append (pNode);
		pNode->m_Flags |= ELaDfaNodeFlag_IsPreResolver;
	}

	void
	PopPreResolver ()
	{
		size_t Count = m_ResolverStack.GetCount ();
		if (!Count)
		{
			ASSERT (false);
			return;
		}

		CLaDfaNode* pNode = m_ResolverStack [Count - 1];
		pNode->m_Flags &= ~ELaDfaNodeFlag_IsPreResolver;

		m_ResolverStack.SetCount (Count - 1);
	}

	// locators

	CNode* 
	GetLocator (size_t Index)
	{
		CSymbolNode* pSymbolNode = GetSymbolTop ();
		if (!pSymbolNode)
			return NULL;
				
		size_t Count = pSymbolNode->m_LocatorArray.GetCount ();
		if (Index >= Count)
			return NULL;

		CNode* pNode = pSymbolNode->m_LocatorArray [Index];
		if (!pNode || !(pNode->m_Flags & ENodeFlag_IsMatched))
			return NULL;

		return pNode;
	}

	CAstNode* 
	GetAstLocator (size_t Index)
	{
		CNode* pNode = GetLocator (Index);
		return pNode && pNode->m_Kind == ENode_Symbol ? ((CSymbolNode*) pNode)->m_pAstNode : NULL;
	}

	const CToken* 
	GetTokenLocator (size_t Index)
	{
		CNode* pNode = GetLocator (Index);
		return pNode && pNode->m_Kind == ENode_Token ? &((CTokenNode*) pNode)->m_Token : NULL;
	}

	bool
	IsValidLocator (CAstNode& Ast)
	{
		return &Ast != NULL;
	}

	bool
	IsValidLocator (const CToken& Token)
	{
		return &Token != NULL;
	}

	// must be implemented in derived class:

	// static
	// size_t*
	// GetParseTable ();

	// static
	// size_t
	// GetTokenIndex (int Token);

	// static
	// int
	// GetTokenFromIndex (size_t Index);

	// static
	// const tchar_t*
	// GetSymbolName (size_t Index);

	// static
	// CNode*
	// CreateSymbolNode (size_t Index); // allocate node & ast with AXL_MEM_NEW () !!

	// static
	// size_t*
	// GetSequence (size_t Index);

	// static
	// size_t*
	// GetBeacon (size_t Index);

	// bool
	// Action (size_t Index);

	// bool
	// Argument (
	//	size_t Index,
	//	CSymbolNode* pSymbol
	//	);

	// bool
	// Enter (size_t Index)

	// bool
	// Leave (size_t Index)

	// ELaDfaResult
	// LaDfa (
	//	size_t Index,
	//	int LookaheadToken,
	//	TLaDfaTransition* pTransition
	//	)
};

//.............................................................................

} // namespace llk
} // namespace axl
