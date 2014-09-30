#include "pch.h"
#include "axl_mem_Alloc.h"
#include "axl_g_Module.h"

namespace axl {
namespace mem {

//.............................................................................

#ifdef _DEBUG

void* 
CStdAlloc::CAlloc::operator () (
	size_t size,
	const char* tag,
	const char* filePath,
	int line
	)
{
	size_t fullSize = size + sizeof (CTracker::TBlockHdr);

	CTracker::TBlockHdr* hdr = (CTracker::TBlockHdr*) CMalloc::alloc (fullSize);
	if (!hdr)
		return NULL;

	hdr->m_size = size;
	hdr->m_tag = tag;
	hdr->m_filePath = filePath;
	hdr->m_line = line;

	g::getModule ()->getMemTracker ()->add (hdr);

	return hdr + 1;
}

void 
CStdAlloc::CFree::operator () (void* p)
{
	CTracker::TBlockHdr* hdr = (CTracker::TBlockHdr*) p - 1;
	
	g::getModule ()->getMemTracker ()->remove (hdr);

	CMalloc::free (hdr);
}

#endif

//.............................................................................

} // namespace err
} // namespace axl
