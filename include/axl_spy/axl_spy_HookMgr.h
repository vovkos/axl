#pragma once

#include "axl_sl_RbTree.h"

namespace axl {
namespace spy {

//..............................................................................

class HookMgr
{
protected:
	sl::RbTree<size_t, size_t> m_frameMap;

public:
	void
	addFrame(
		size_t frameBase,
		size_t originalRet
		);

	size_t
	removeFrame(size_t frameBase);

	size_t
	findOriginalRet(size_t frameBase);

protected:
	void
	cleanup(const sl::RbTreeIterator<size_t, size_t>& it);
};

//..............................................................................

} // namespace spy
} // namespace axl
