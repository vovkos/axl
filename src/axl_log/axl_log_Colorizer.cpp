#include "pch.h"
#include "axl_log_Colorizer.h"

namespace axl {
namespace log {

//.............................................................................

struct TColorizerStackStateEntry
{
	intptr_t m_State;
	ref::CPtrT <void> m_StateEx;
};

typedef rtl::CArrayT <TColorizerStackStateEntry> CColorizerStackState;

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
CColorizerStack::Colorize (
	CColorizeTarget* pTarget, 
	TColorizeBlock* pBlock, 
	bool SaveStateHint
	)
{
	size_t Count = GetCount ();

	ref::CPtrT <CColorizerStackState> State (
		(CColorizerStackState*) (void*) pTarget->m_ColorizerStateEx,
		pTarget->m_ColorizerStateEx.GetRefCount ()
		);
	
	if (State && State->GetCount () == Count)
	{
		ref::CPtrT <void> SavedState = SaveStateHint ? pTarget->m_ColorizerStateEx : ref::EPtr_Null;

		for (size_t i = 0; i < Count; i++, pTarget->IncrementMetric ())
		{
			IColorizer* pColorizer = (*this) [i];
			TColorizerStackStateEntry* pStateEntry = &(*State) [i];

			pTarget->m_ColorizerState = pStateEntry->m_State;
			pTarget->m_ColorizerStateEx = pStateEntry->m_StateEx;

			pColorizer->Colorize (pTarget, pBlock, SaveStateHint);

			pStateEntry->m_State = pTarget->m_ColorizerState;
			pStateEntry->m_StateEx = pTarget->m_ColorizerStateEx;
		}

		pTarget->m_ColorizerStateEx = State;
	}
	else if (SaveStateHint)
	{
		State = AXL_REF_NEW (ref::CBoxT <CColorizerStackState>);
		State->SetCount (Count);

		for (size_t i = 0; i < Count; i++, pTarget->IncrementMetric ())
		{
			IColorizer* pColorizer = (*this) [i];

			pTarget->m_ColorizerState = 0;
			pTarget->m_ColorizerStateEx = ref::EPtr_Null;

			pColorizer->Colorize (pTarget, pBlock, SaveStateHint);

			TColorizerStackStateEntry* pStateEntry = &(*State) [i];
			pStateEntry->m_State = pTarget->m_ColorizerState;
			pStateEntry->m_StateEx = pTarget->m_ColorizerStateEx;
		}

		pTarget->m_ColorizerStateEx = State;
	}
	else
	{
		for (size_t i = 0; i < Count; i++, pTarget->IncrementMetric ())
		{
			IColorizer* pColorizer = (*this) [i];
			pColorizer->Colorize (pTarget, pBlock, SaveStateHint);
		}
	}
}

//.............................................................................

} // namespace log {
} // namespace axl {
