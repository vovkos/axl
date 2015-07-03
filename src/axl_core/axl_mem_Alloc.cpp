#include "pch.h"
#include "axl_mem_Alloc.h"
#include "axl_g_Module.h"

namespace axl {
namespace mem {

//.............................................................................

#ifdef _AXL_DEBUG

void* 
StdAlloc::Alloc::operator () (
	size_t size,
	const char* tag,
	const char* filePath,
	int line
	)
{
	size_t fullSize = size + sizeof (Tracker::BlockHdr);

	Tracker::BlockHdr* hdr = (Tracker::BlockHdr*) Malloc::alloc (fullSize);
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
StdAlloc::Free::operator () (void* p)
{
	Tracker::BlockHdr* hdr = (Tracker::BlockHdr*) p - 1;
	
	g::getModule ()->getMemTracker ()->remove (hdr);

	Malloc::free (hdr);
}

#endif

//.............................................................................

} // namespace err
} // namespace axl
