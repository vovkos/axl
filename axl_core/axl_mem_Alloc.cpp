#include "stdafx.h"
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
//
//
//struct IFin: obj::IRoot
//{
//	virtual
//	void
//	Fin () = 0;
//};
//
//class CFinEntry: public rtl::TListLink
//{
//public:
//	IFin* m_pFin;
//
//public:
//	~CFinEntry ()
//	{
//		obj::IClass* pClass;
//		void* p = m_pFin->GetObject (&pClass);		
//		pClass->Destruct (p);
//	}
//
//	// followed by fin copy
//};
//
//void Test (IFin* pFin)
//{
//	rtl::CStdListT <CFinEntry> List;
//
//	obj::CPtrT <obj::ICopy> Copy = pFin;
//	if (!Copy)
//		return;
//
//	obj::IClass* pClass;
//	void* p = pFin->GetObject (&pClass);
//
//	CFinEntry* pEntry = AXL_MEM_NEW_EXTRA (CFinEntry, pClass->GetSize ());
//	Copy->Copy (pEntry + 1, p);
//
//	List.InsertTail (pEntry);	
//}
//


//.............................................................................

} // namespace err
} // namespace axl
