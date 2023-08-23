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

#define _AXL_IO_HIDDB_H

#include "axl_io_HidPch.h"

namespace axl {
namespace io {

class HidDb;
class HidUsagePageDirIniParser;
class HidUsageIniParser;

//..............................................................................

// abstraction layer (e.g., to load from a ZIP archive)

class HidDbLoader: public rc::RefCount {
protected:
	sl::String m_path;
	sl::String m_fileName;

public:
	virtual ~HidDbLoader() {}

	const sl::String&
	getPath() const {
		return m_path;
	}

	const sl::String&
	getFileName() const {
		return m_fileName;
	}

	virtual
	bool
	load(
		sl::Array<char>* buffer,
		const sl::StringRef& fileName
	) const = 0;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class HidDbFileLoader: public HidDbLoader {
protected:
	sl::String m_dir;

public:
	HidDbFileLoader() {}

	HidDbFileLoader(const sl::StringRef& fileName) {
		initialize(fileName);
	}

	void
	initialize(const sl::StringRef& fileName);

	virtual
	bool
	load(
		sl::Array<char>* buffer,
		const sl::StringRef& fileName
	) const;
};

//..............................................................................

class HidUsagePage: public sl::ListLink {
	friend class HidUsagePageDirIniParser;
	friend class HidUsageIniParser;
	friend class HidDb;

protected:
	rc::Ptr<HidDbLoader> m_loader;

	union {
		uint_t m_flags;
		struct {
			uint_t m_id             : 31;
			uint_t m_isNameMapReady : 1;
		};
	};

	sl::String m_name;
	sl::String m_fileName;
	sl::String m_format;

	sl::SimpleHashTable<uint_t, sl::String> m_usageNameMap;

public:
	HidUsagePage(HidDbLoader* loader) {
		m_loader = loader;
		m_flags = 0;
	}

	uint_t
	getId() const {
		return m_id;
	}

	const sl::String&
	getName() const {
		return m_name;
	}

	sl::String
	getUsageName(uint_t usage) const;

protected:
	bool
	load();
};

//..............................................................................

class HidDb {
	friend class HidUsagePageDirIniParser;
	friend class HidUsageIniParser;

protected:
	sl::List<HidUsagePage> m_usagePageList;
	sl::SimpleHashTable<uint_t, const HidUsagePage*> m_usagePageMap;

public:
	void
	clear();

	bool
	load(HidDbLoader* loader);

	bool
	load(const sl::StringRef& fileName)  {
		return load(AXL_RC_NEW_ARGS(HidDbFileLoader, (fileName)));
	}

	const HidUsagePage*
	getUsagePage(uint_t page) const;
};

//..............................................................................

} // namespace io
} // namespace axl
