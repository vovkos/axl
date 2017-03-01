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

#include "pch.h"
#include "axl_zip_ZipReader.h"
#include "axl_err_Error.h"
#include "miniz.c"
#include "axl_sys_Time.h"

namespace axl {
namespace zip {

//..............................................................................

void
ZipReader::close ()
{
	if (!m_zip)
		return;

	mz_zip_reader_end (m_zip);
	AXL_MEM_FREE (m_zip);
	m_zip = NULL;
}

bool
ZipReader::openFile (const sl::StringRef& fileName)
{
	close ();

	m_zip = AXL_MEM_NEW (mz_zip_archive);
	mz_bool result = mz_zip_reader_init_file (m_zip, fileName.sz (), MZ_ZIP_FLAG_DO_NOT_SORT_CENTRAL_DIRECTORY);
	return result ? true : err::fail (err::SystemErrorCode_Unsuccessful);
}

bool
ZipReader::openMem (
	const void* p,
	size_t size
	)
{
	close ();

	m_zip = AXL_MEM_NEW (mz_zip_archive);
	mz_bool result = mz_zip_reader_init_mem (m_zip, p, size, MZ_ZIP_FLAG_DO_NOT_SORT_CENTRAL_DIRECTORY);
	return result ? true : err::fail (err::SystemErrorCode_Unsuccessful);
}

size_t
ZipReader::getFileCount ()
{
	ASSERT (isOpen ());
	return mz_zip_reader_get_num_files (m_zip);
}

sl::String
ZipReader::getFileName (size_t index)
{
	ASSERT (isOpen ());
	size_t size = mz_zip_reader_get_filename (m_zip, index, NULL, 0);

	sl::String fileName;
	char* p = fileName.createBuffer (size);
	mz_zip_reader_get_filename (m_zip, index, p, size);
	fileName.chop (1);
	return fileName;
}

bool
ZipReader::getFileInfo (
	size_t index,
	ZipFileInfo* fileInfo,
	sl::String* comment
	)
{
	ASSERT (isOpen ());

	mz_zip_archive_file_stat stat;
	mz_bool result = mz_zip_reader_file_stat (m_zip, index, &stat);
	if (!result)
		return err::fail (err::SystemErrorCode_Unsuccessful);

	fileInfo->m_centralDirOffset = stat.m_central_dir_ofs;
	fileInfo->m_versionMadeBy = stat.m_version_made_by;
	fileInfo->m_versionNeeded = stat.m_version_needed;
	fileInfo->m_bitFlags = stat.m_bit_flag;
	fileInfo->m_method = stat.m_method;
	fileInfo->m_timestamp = (stat.m_time + AXL_SYS_EPOCH_DIFF) * 10000000;
	fileInfo->m_crc32 = stat.m_crc32;
	fileInfo->m_compressedSize = stat.m_comp_size;
	fileInfo->m_uncompressedSize = stat.m_uncomp_size;
	fileInfo->m_internalAttr = stat.m_internal_attr;
	fileInfo->m_externalAttr = stat.m_external_attr;
	fileInfo->m_localHeaderOffset = stat.m_local_header_ofs;

	if (comment)
		*comment = stat.m_comment;

	return true;
}

bool
ZipReader::isDirectoryFile (size_t index)
{
	ASSERT (isOpen ());
	return mz_zip_reader_is_file_a_directory (m_zip, index) != 0;
}

bool
ZipReader::isFileEncrypted (size_t index)
{
	ASSERT (isOpen ());
	return mz_zip_reader_is_file_encrypted (m_zip, index) != 0;
}

bool
ZipReader::extractFileToMem (
	size_t index,
	void* p,
	size_t size
	)
{
	ASSERT (isOpen ());

	char readBuffer [1024];
	mz_bool result = mz_zip_reader_extract_to_mem_no_alloc (
		m_zip,
		index,
		p,
		size,
		0,
		readBuffer,
		sizeof (readBuffer)
		);

	return result ? true : err::fail (err::SystemErrorCode_Unsuccessful);
}

bool
ZipReader::extractFileToMem (
	size_t index,
	sl::Array <char>* buffer
	)
{
	ASSERT (isOpen ());

	mz_zip_archive_file_stat stat;
	bool result =
		mz_zip_reader_file_stat (m_zip, index, &stat) &&
		buffer->reserve ((size_t) stat.m_uncomp_size + 1);

	if (!result)
		return false;

	buffer->p () [(size_t) stat.m_uncomp_size] = 0; // ensure zero termination
	buffer->setCount ((size_t) stat.m_uncomp_size);

	return extractFileToMem (index, buffer->p (), (size_t) stat.m_uncomp_size);
}

bool
ZipReader::extractFileToFile (
	size_t index,
	const sl::StringRef& fileName
	)
{
	ASSERT (isOpen ());

	mz_bool result = mz_zip_reader_extract_to_file (
		m_zip,
		index,
		fileName.sz (),
		0
		);

	return result ? true : err::fail (err::SystemErrorCode_Unsuccessful);
}

//..............................................................................

} // namespace zip
} // namespace axl
