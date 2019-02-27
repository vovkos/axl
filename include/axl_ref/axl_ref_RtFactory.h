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

#pragma once

#define _AXL_REF_RTFACTORY_H

#include "axl_ref_Ptr.h"
#include "axl_mem_New.h"

namespace axl {
namespace ref {

//..............................................................................

template <typename Alloc = mem::StdAllocator>
class RtFactory
{
protected:
	class Box:
		public RefCount,
		public obj::ITypeSimpleImplT<Box>
	{
	protected:
		obj::IType* m_type;

	public:
		Box()
		{
			m_type = NULL;
		}

		~Box()
		{
			ASSERT(m_type);
			m_type->destruct(this + 1);
		}

		void
		setType(obj::IType* type)
		{
			m_type = type;
			m_guid = type->getGuid();
		}

		// IRoot

		virtual
		void*
		getObject(obj::IType** type_o)
		{
			if (type_o)
				*type_o = this;

			return this;
		}

		// IType

		virtual
		size_t
		getInterfaceOffset(const sl::Guid& guid)
		{
			ASSERT(m_type);

			if (guid == AXL_OBJ_GUIDOF(RefCount))
				return 0;

			size_t offset = m_type->getInterfaceOffset(guid);
			return offset != -1 ? offset + sizeof(Box) : -1;
		}
	};

public:
	typedef Alloc Alloc;

	class New
	{
	public:
#ifdef _AXL_DEBUG
		Ptr<void>
		operator () (
			obj::IType* type,
			const char* filePath,
			int line,
			size_t extra = 0
			) const
		{
			size_t size = type->getSize() + extra;
			size_t refCountOffset = type->getInterfaceOffset(AXL_OBJ_GUIDOF(RefCount));

			if (refCountOffset != -1)
			{
				void* p = Alloc::alloc(size, type->getName(), filePath, line);
				type->construct(p);
				RefCount* refCount = (RefCount*)((uchar_t*)p + refCountOffset);
				refCount->setFree(&Alloc::free);
				return Ptr<void> (p, refCount);
			}
			else
			{
				Ptr<Box> box = mem::StdFactory<Box, Alloc>::operatorNew(filePath, line, size);
				type->construct(box + 1);
				box->setType(type);
				box->setFree(&Alloc::free);
				return Ptr<void> (box + 1, box.getRefCount());
			}
		}
#else
		Ptr<void>
		operator () (
			obj::IType* type,
			size_t extra = 0
			) const
		{
			size_t size = type->getSize() + extra;
			size_t refCountOffset = type->getInterfaceOffset(AXL_OBJ_GUIDOF(RefCount));

			if (refCountOffset != -1)
			{
				void* p = Alloc::alloc(size);
				type->construct(p);
				RefCount* refCount = (RefCount*)((uchar_t*)p + refCountOffset);
				refCount->setFree(&Alloc::free);
				return Ptr<void> (p, refCount);
			}
			else
			{
				Ptr<Box> box = mem::StdFactory<Box, Alloc>::operatorNew(size);
				box->setClass(type);
				box->setFree(&Alloc::free);
				return Ptr<void> (box + 1, box.getRefCount());
			}
		}
#endif
	};

public:

#ifdef _AXL_DEBUG
	static
	Ptr<void>
	operatorNew(
		obj::IType* type,
		const char* filePath,
		int line,
		size_t extra = 0
		)
	{
		return New() (type, filePath, line, extra);
	}
#else
	static
	Ptr<void>
	operatorNew(
		obj::IType* type,
		size_t extra = 0
		)
	{
		return New() (type, extra);
	}
#endif
};

typedef RtFactory<mem::StdAllocator> RtFactory;

//..............................................................................

#ifdef _AXL_DEBUG

#define AXL_REF_RT_NEW(Type) \
	ref::RtFactory::operatorNew(Type, __FILE__, __LINE__)

#define AXL_REF_RT_NEW_EXTRA(Type, extra) \
	ref::RtFactory::operatorNew(Type, __FILE__, __LINE__, extra)

#else

#define AXL_REF_RT_NEW(Type) \
	ref::RtFactory::operatorNew(Type)

#define AXL_REF_RT_NEW_EXTRA(Type, extra) \
	ref::RtFactory::operatorNew(Type, extra)

#endif

//..............................................................................

} // namespace ref
} // namespace axl
