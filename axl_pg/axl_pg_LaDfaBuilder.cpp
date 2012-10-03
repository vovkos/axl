#include "stdafx.h"
#include "axl_pg_LaDfaBuilder.h"

namespace axl {
namespace pg {

//.............................................................................
	
CLaDfaThread::CLaDfaThread ()
{
	m_Match = ELaDfaThreadMatch_None;
	m_pState = NULL;
	m_pProduction = NULL;
	m_pResolver = NULL;
}

//.............................................................................

CLaDfaState::CLaDfaState ()
{
	m_Index = -1;
	m_Flags = 0;
	m_pDfaNode = NULL;
	m_pFromState = NULL;
	m_pToken = NULL;
}

CLaDfaThread*
CLaDfaState::CreateThread (CLaDfaThread* pSrc)
{
	CLaDfaThread* pThread = AXL_MEM_NEW (CLaDfaThread);
	pThread->m_pState = this;

	if (pSrc)
	{
		ASSERT (!pSrc->m_pResolver);

		pThread->m_pProduction = pSrc->m_pProduction;
		pThread->m_Stack = pSrc->m_Stack;
	}

	m_ActiveThreadList.InsertTail (pThread);
	return pThread;
}

bool
CLaDfaState::CalcResolved ()
{
	rtl::CIteratorT <CLaDfaThread> Thread;

	if (m_ActiveThreadList.IsEmpty ())
	{
		m_pDfaNode->m_Flags |= ELaDfaNodeFlag_IsResolved;
		return true;
	}

	Thread = m_ActiveThreadList.GetHead ();

	CNode* pOriginalProduction = Thread->m_pProduction;

	for (Thread++; Thread; Thread++)
	{
		if (Thread->m_pProduction != pOriginalProduction)
			return false;
	}

	Thread = m_CompleteThreadList.GetHead ();
	for (; Thread; Thread++)
	{
		if (Thread->m_pProduction != pOriginalProduction)
			return false;
	}

	m_pDfaNode->m_Flags |= ELaDfaNodeFlag_IsResolved;
	return true;
}

CNode* 
CLaDfaState::GetDefaultProduction ()
{
	return 
		!m_CompleteThreadList.IsEmpty () ? m_CompleteThreadList.GetHead ()->m_pProduction :
		!m_EpsilonThreadList.IsEmpty () ? m_EpsilonThreadList.GetHead ()->m_pProduction : 
		m_pFromState ? m_pFromState->GetDefaultProduction () : NULL;
}

//.............................................................................

CLaDfaBuilder::CLaDfaBuilder (	
	CNodeMgr* pNodeMgr,
	rtl::CArrayT <CNode*>* pParseTable,
	size_t LookeaheadLimit
	)
{
	m_pNodeMgr = pNodeMgr;
	m_pParseTable = pParseTable;
	m_LookeaheadLimit = LookeaheadLimit;
	m_Lookeahead = 1;
}

CNode*
CLaDfaBuilder::Build (
	CConfig* pConfig,
	CConflictNode* pConflict,
	size_t* pLookahead
	)
{
	ASSERT (pConflict->m_Kind == ENode_Conflict);

	size_t TokenCount = m_pNodeMgr->m_TokenArray.GetCount ();

	m_StateList.Clear ();

	CLaDfaState* pState0 = CreateState ();
	pState0->m_pDfaNode = m_pNodeMgr->CreateLaDfaNode ();

	size_t Count = pConflict->m_ProductionArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CNode* pProduction = pConflict->m_ProductionArray [i];
		CLaDfaThread* pThread = pState0->CreateThread ();
		pThread->m_pProduction = pProduction;
	
		if (pProduction->m_Kind != ENode_Epsilon)
			pThread->m_Stack.Append (pProduction);
	}

	CLaDfaState* pState1 = Transition (pState0, pConflict->m_pToken);

	size_t Lookahead = 1;

	if (!pState1->IsResolved ())
	{
		rtl::CArrayT <CLaDfaState*> StateArray;
		StateArray.Append (pState1);

		while (!StateArray.IsEmpty () && Lookahead < m_LookeaheadLimit) 
		{
			Lookahead++;

			rtl::CArrayT <CLaDfaState*> NextStateArray;

			size_t StateCount = StateArray.GetCount ();		
			for (size_t j = 0; j < StateCount; j++)
			{
				CLaDfaState* pState = StateArray [j];

				for (size_t k = 0; k < TokenCount; k++)
				{
					CSymbolNode* pToken = m_pNodeMgr->m_TokenArray [k];
			
					CLaDfaState* pNewState = Transition (pState, pToken);				
					if (pNewState && !pNewState->IsResolved ())
						NextStateArray.Append (pNewState);
				}
			}

			StateArray = NextStateArray;
		} 

		if (!StateArray.IsEmpty ())
		{
			err::SetFormatStringError (
				_T("conflict at %s:%s could not be resolved with %d token lookahead"),
				pConflict->m_pSymbol->m_Name,
				pConflict->m_pToken->m_Name,
				m_LookeaheadLimit
				);

			err::PushSrcPosError (pConflict->m_pSymbol->m_SrcPos);
			return NULL;
		}
	}

	if (Lookahead > m_Lookeahead)
		m_Lookeahead = Lookahead;

	if (pLookahead)
		*pLookahead = Lookahead;

	rtl::CIteratorT <CLaDfaState> State = m_StateList.GetHead ();
	for (; State; State++)
	{
		CLaDfaState* pState = *State;

		if (pState->m_CompleteThreadList.GetCount () > 1 || 
			pState->m_EpsilonThreadList.GetCount () > 1)
		{
			err::SetFormatStringError (_T("ambiguous grammar: multiple productions complete with %s"), pState->m_pToken->m_Name);
			err::PushSrcPosError (pConflict->m_pSymbol->m_SrcPos);
			return NULL;
		}

		rtl::CIteratorT <CLaDfaThread> ResolverThread = pState->m_ResolverThreadList.GetHead ();
		for (; ResolverThread; ResolverThread++) // chain all resolvers
		{
			CLaDfaNode* pDfaElse = m_pNodeMgr->CreateLaDfaNode ();
			pDfaElse->m_Flags = pState->m_pDfaNode->m_Flags;
			pDfaElse->m_TransitionArray = pState->m_pDfaNode->m_TransitionArray;

			pState->m_pDfaNode->m_pResolver = ResolverThread->m_pResolver;
			pState->m_pDfaNode->m_pProduction = ResolverThread->m_pProduction;
			pState->m_pDfaNode->m_pResolverElse = pDfaElse;
			pState->m_pDfaNode->m_TransitionArray.Clear ();

			pDfaElse->m_pResolverUplink = pState->m_pDfaNode;
			pState->m_pDfaNode = pDfaElse;
		}
		
		if (pState->IsResolved ())
		{
			pState->m_pDfaNode->m_Flags |= ELaDfaNodeFlag_IsLeaf;
			pState->m_pDfaNode->m_pProduction = 
				!pState->m_ActiveThreadList.IsEmpty () ? pState->m_ActiveThreadList.GetHead ()->m_pProduction :
				!pState->m_CompleteThreadList.IsEmpty () ? pState->m_CompleteThreadList.GetHead ()->m_pProduction :
				!pState->m_EpsilonThreadList.IsEmpty () ? pState->m_EpsilonThreadList.GetHead ()->m_pProduction : NULL;

			if (!pState->m_pDfaNode->m_pProduction && 
				pState->m_pDfaNode->m_pResolverUplink)
			{
				// here we handle situation like
				// sym: resolver ({1}) 'a' | resolver ({2}) 'a'
				// we have a chain of 2 resolver with empty tail
				// so we can safely eliminate the 2nd resolver

				CLaDfaNode* pUplink = pState->m_pDfaNode->m_pResolverUplink;
				pUplink->m_Flags |= ELaDfaNodeFlag_IsLeaf;
				pUplink->m_pResolver = NULL;
				pUplink->m_pResolverElse = NULL;

				m_pNodeMgr->DeleteLaDfaNode (pState->m_pDfaNode);
				pState->m_pDfaNode = pUplink;
			}
		}
		else
		{
			pState->m_pDfaNode->m_pProduction = pState->GetDefaultProduction ();
		}
	}

	if (pConfig->m_Flags & EConfigFlag_IsVerbose)
		Trace ();

	if (pState1->m_ResolverThreadList.IsEmpty () &&
		(pState1->m_pDfaNode->m_Flags & ELaDfaNodeFlag_IsLeaf))
	{
		// can happen on active-vs-complete-vs-epsion conflicts

		pState0->m_pDfaNode->m_Flags |= ELaDfaNodeFlag_IsLeaf; // don't index state0
		return pState1->m_pDfaNode->m_pProduction; 
	}

	return pState0->m_pDfaNode;
}

void
CLaDfaBuilder::Trace ()
{
	rtl::CIteratorT <CLaDfaState> State = m_StateList.GetHead ();
	for (; State; State++)
	{
		CLaDfaState* pState = *State;

		printf (
			"%3d %s %d/%d/%d/%d (a/r/c/e)\n", 
			pState->m_Index, 
			pState->IsResolved () ? "*" : " ",
			pState->m_ActiveThreadList.GetCount (),
			pState->m_ResolverThreadList.GetCount (),
			pState->m_CompleteThreadList.GetCount (),
			pState->m_EpsilonThreadList.GetCount ()
			);

		rtl::CIteratorT <CLaDfaThread> Thread;

		if (!pState->m_ActiveThreadList.IsEmpty ())
		{
			printf ("\tACTIVE:   ");

			Thread = pState->m_ActiveThreadList.GetHead ();
			for (; Thread; Thread++)
				printf ("%s ", Thread->m_pProduction->m_Name);

			printf ("\n");
		}
				
		if (!pState->m_ResolverThreadList.IsEmpty ())
		{
			printf ("\tRESOLVER: ");

			Thread = pState->m_ResolverThreadList.GetHead ();
			for (; Thread; Thread++)
				printf ("%s ", Thread->m_pProduction->m_Name);

			printf ("\n");
		}

		if (!pState->m_CompleteThreadList.IsEmpty ())
		{
			printf ("\tCOMPLETE: ");

			Thread = pState->m_CompleteThreadList.GetHead ();
			for (; Thread; Thread++)
				printf ("%s ", Thread->m_pProduction->m_Name);

			printf ("\n");
		}

		if (!pState->m_EpsilonThreadList.IsEmpty ())
		{
			printf ("\tEPSILON: ");

			Thread = pState->m_EpsilonThreadList.GetHead ();
			for (; Thread; Thread++)
				printf ("%s ", Thread->m_pProduction->m_Name);

			printf ("\n");
		}

		if (!pState->IsResolved ())
		{
			size_t MoveCount = pState->m_TransitionArray.GetCount ();
			for (size_t i = 0; i < MoveCount; i++)
			{
				CLaDfaState* pMoveTo = pState->m_TransitionArray [i];
				printf ("\t%s -> %d\n", pMoveTo->m_pToken->m_Name, pMoveTo->m_Index);
			}
		}
	}
}

CLaDfaState* 
CLaDfaBuilder::CreateState ()
{
	CLaDfaState* pState = AXL_MEM_NEW (CLaDfaState);
	pState->m_Index = m_StateList.GetCount ();
	m_StateList.InsertTail (pState);

	return pState;
}

CLaDfaState*
CLaDfaBuilder::Transition (
	CLaDfaState* pState,
	CSymbolNode* pToken
	)
{
	CLaDfaState* pNewState = CreateState ();
	pNewState->m_pToken = pToken;
	pNewState->m_pFromState = pState;

	rtl::CIteratorT <CLaDfaThread> Thread = pState->m_ActiveThreadList.GetHead ();
	for (; Thread; Thread++)
	{
		CLaDfaThread* pNewThread = pNewState->CreateThread (*Thread);
		ProcessThread (pNewThread);
	}

	Thread = pNewState->m_ActiveThreadList.GetHead ();
	while (Thread)
	{
		CLaDfaThread* pThread = *Thread++;

		if (pThread->m_Match == ELaDfaThreadMatch_AnyToken && (pNewState->m_Flags & ELaDfaStateFlag_HasTokenMatch)) 
		{
			pNewState->m_ActiveThreadList.Delete (pThread); // delete anytoken thread in favor of concrete token
		}
		else if (pThread->m_Stack.IsEmpty ())
		{
			pNewState->m_ActiveThreadList.Remove (pThread);

			if (pThread->m_Match)
				pNewState->m_CompleteThreadList.InsertTail (pThread);
			else
				pNewState->m_EpsilonThreadList.InsertTail (pThread);
		}
	}

	if (pNewState->IsEmpty ())
	{
		m_StateList.Delete (pNewState);
		return NULL;
	}

	pNewState->m_pDfaNode = m_pNodeMgr->CreateLaDfaNode ();
	pNewState->m_pDfaNode->m_pToken = pToken;
	pNewState->CalcResolved  ();

	pState->m_pDfaNode->m_TransitionArray.Append (pNewState->m_pDfaNode);
	pState->m_TransitionArray.Append (pNewState);
	return pNewState;
}

void
CLaDfaBuilder::ProcessThread (CLaDfaThread* pThread)
{
	CSymbolNode* pToken = pThread->m_pState->m_pToken;

	pThread->m_Match = ELaDfaThreadMatch_None;

	size_t TokenCount = m_pNodeMgr->m_TokenArray.GetCount ();
	for (;;)
	{
		if (pThread->m_Stack.IsEmpty ())
			break;

		CNode* pNode = pThread->m_Stack.GetBack ();
		CNode* pProduction;
		CSymbolNode* pSymbol;
		CConflictNode* pConflict;
		CSequenceNode* pSequence;		
		size_t ChildrenCount;

		switch (pNode->m_Kind)
		{
		case ENode_Token:
			if (pThread->m_Match)
				return;

			ASSERT (pNode->m_MasterIndex);

			if (pNode->m_Flags & ESymbolNodeFlag_IsAnyToken)
			{
				pThread->m_Stack.Pop ();
				pThread->m_Match = ELaDfaThreadMatch_AnyToken;
				break;
			}

			if (pNode != pToken) // could happen after epsilon production
			{
				pThread->m_pState->m_ActiveThreadList.Delete (pThread);
				return;
			} 

			pThread->m_Stack.Pop ();
			pThread->m_Match = ELaDfaThreadMatch_Token;
			pThread->m_pState->m_Flags |= ELaDfaStateFlag_HasTokenMatch;
			break;

		case ENode_Symbol:
			if (pThread->m_Match)
				return;

			pProduction = (*m_pParseTable) [pNode->m_Index * TokenCount + pToken->m_Index];
			if (!pProduction)  // could happen after epsilon production
			{
				pThread->m_pState->m_ActiveThreadList.Delete (pThread);
				return;
			}

			// ok this thread seems to stay active, let's check if we can eliminate it with resolver

			pSymbol = (CSymbolNode*) pNode;
			if (pSymbol->m_pResolver)
			{
				pThread->m_pResolver = pSymbol->m_pResolver;
				pThread->m_pState->m_ActiveThreadList.Remove (pThread);
				pThread->m_pState->m_ResolverThreadList.InsertTail (pThread);
				return;
			}
					
			pThread->m_Stack.Pop ();

			if (pProduction->m_Kind != ENode_Epsilon)
				pThread->m_Stack.Append (pProduction);

			break;

		case ENode_Sequence:
			if (pThread->m_Match)
				return;

			pThread->m_Stack.Pop ();
			
			pSequence = (CSequenceNode*) pNode;
			ChildrenCount = pSequence->m_Sequence.GetCount ();
			for (intptr_t i = ChildrenCount - 1; i >= 0; i--)
			{
				CNode* pChild = pSequence->m_Sequence [i];
				pThread->m_Stack.Append (pChild);
			}

			break;

		case ENode_Beacon:
			pThread->m_Stack.Pop ();
			pThread->m_Stack.Append (((CBeaconNode*) pNode)->m_pTarget);
			break;

		case ENode_Action: 
		case ENode_Argument: 
			pThread->m_Stack.Pop ();
			break;

		case ENode_Conflict:
			pThread->m_Stack.Pop ();

			pConflict = (CConflictNode*) pNode;
			ChildrenCount = pConflict->m_ProductionArray.GetCount ();
			for (size_t i = 0; i < ChildrenCount; i++)
			{
				CNode* pChild = pConflict->m_ProductionArray [i];
				CLaDfaThread* pNewThread = pThread->m_pState->CreateThread (pThread);

				if (pChild->m_Kind != ENode_Epsilon)
					pNewThread->m_Stack.Append (pChild);

				ProcessThread (pNewThread);
			}

			pThread->m_pState->m_ActiveThreadList.Delete (pThread);
			return;

		default:
			ASSERT (false);
		}
	}
}

//.............................................................................

} // namespace pg {
} // namespace axl {
