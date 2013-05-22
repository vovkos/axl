#include "pch.h"
#include "axl_mem_Alloc.h"
#include "axl_g_Module.h"

namespace axl {
namespace mem {

//.............................................................................

#ifdef _DEBUG

void* 
CStdAlloc::CAlloc::operator () (
	size_t Size,
	const char* pTag,
	const char* pFilePath,
	int Line
	)
{
	size_t FullSize = Size + sizeof (CTracker::TBlockHdr);

	CTracker::TBlockHdr* pHdr = (CTracker::TBlockHdr*) CMalloc::Alloc (FullSize);
	if (!pHdr)
		return NULL;

	pHdr->m_Size = Size;
	pHdr->m_pTag = pTag;
	pHdr->m_pFilePath = pFilePath;
	pHdr->m_Line = Line;

	g::GetModule ()->GetMemTracker ()->Add (pHdr);

	return pHdr + 1;
}

void 
CStdAlloc::CFree::operator () (void* p)
{
	CTracker::TBlockHdr* pHdr = (CTracker::TBlockHdr*) p - 1;
	
	g::GetModule ()->GetMemTracker ()->Remove (pHdr);

	CMalloc::Free (pHdr);
}

#endif

//.............................................................................

} // namespace err
} // namespace axl
