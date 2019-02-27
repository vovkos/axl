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

#define _AXL_SYS_WIN_NTDLL_H

#include "axl_g_Pch.h"
#include <winternl.h>

// we'll add native NT funcs/structs here as needed

namespace axl {
namespace sys {
namespace win {

//..............................................................................

enum
{
	STATUS_NO_MORE_FILES          = 0x80000006,
	STATUS_NO_MORE_ENTRIES        = 0x8000001a,
	STATUS_BUFFER_TOO_SMALL       = 0xc0000023,

	DIRECTORY_QUERY               = 0x0001,
	DIRECTORY_TRAVERSE            = 0x0002,
	DIRECTORY_CREATE_OBJECT       = 0x0004,
	DIRECTORY_CREATE_SUBDIRECTORY = 0x0008,
};

enum FILE_INFORMATION_CLASS
{
	FileDirectoryInformation                 =  1,
	FileFullDirectoryInformation,            // 2
	FileBothDirectoryInformation,            // 3
	FileBasicInformation,                    // 4
	FileStandardInformation,                 // 5
	FileInternalInformation,                 // 6
	FileEaInformation,                       // 7
	FileAccessInformation,                   // 8
	FileNameInformation,                     // 9
	FileRenameInformation,                   // 10
	FileLinkInformation,                     // 11
	FileNamesInformation,                    // 12
	FileDispositionInformation,              // 13
	FilePositionInformation,                 // 14
	FileFullEaInformation,                   // 15
	FileModeInformation,                     // 16
	FileAlignmentInformation,                // 17
	FileAllInformation,                      // 18
	FileAllocationInformation,               // 19
	FileEndOfFileInformation,                // 20
	FileAlternateNameInformation,            // 21
	FileStreamInformation,                   // 22
	FilePipeInformation,                     // 23
	FilePipeLocalInformation,                // 24
	FilePipeRemoteInformation,               // 25
	FileMailslotQueryInformation,            // 26
	FileMailslotSetInformation,              // 27
	FileCompressionInformation,              // 28
	FileObjectIdInformation,                 // 29
	FileCompletionInformation,               // 30
	FileMoveClusterInformation,              // 31
	FileQuotaInformation,                    // 32
	FileReparsePointInformation,             // 33
	FileNetworkOpenInformation,              // 34
	FileAttributeTagInformation,             // 35
	FileTrackingInformation,                 // 36
	FileIdBothDirectoryInformation,          // 37
	FileIdFullDirectoryInformation,          // 38
	FileValidDataLengthInformation,          // 39
	FileShortNameInformation,                // 40
	FileIoCompletionNotificationInformation, // 41
	FileIoStatusBlockRangeInformation,       // 42
	FileIoPriorityHintInformation,           // 43
	FileSfioReserveInformation,              // 44
	FileSfioVolumeInformation,               // 45
	FileHardLinkInformation,                 // 46
	FileProcessIdsUsingFileInformation,      // 47
	FileNormalizedNameInformation,           // 48
	FileNetworkPhysicalNameInformation,      // 49
	FileIdGlobalTxDirectoryInformation,      // 50
	FileIsRemoteDeviceInformation,           // 51
	FileAttributeCacheInformation,           // 52
	FileNumaNodeInformation,                 // 53
	FileStandardLinkInformation,             // 54
	FileRemoteProtocolInformation,           // 55
	FileMaximumInformation
};

enum OBJECT_INFORMATION_CLASS
{
	ObjectBasicInformation,
	ObjectNameInformation,
	ObjectTypeInformation,
	ObjectAllInformation,
	ObjectDataInformation
};

struct FILE_BASIC_INFORMATION
{
	LARGE_INTEGER CreationTime;
	LARGE_INTEGER LastAccessTime;
	LARGE_INTEGER LastWriteTime;
	LARGE_INTEGER ChangeTime;
	ULONG FileAttributes;
};

struct FILE_DIRECTORY_INFORMATION
{
	ULONG NextEntryOffset;
	ULONG FileIndex;
	LARGE_INTEGER CreationTime;
	LARGE_INTEGER LastAccessTime;
	LARGE_INTEGER LastWriteTime;
	LARGE_INTEGER ChangeTime;
	LARGE_INTEGER EndOfFile;
	LARGE_INTEGER AllocationSize;
	ULONG FileAttributes;
	ULONG FileNameLength;
	WCHAR FileName[1];
};

struct OBJECT_NAME_INFORMATION
{
	UNICODE_STRING Name; // defined in winternl.h
	WCHAR NameBuffer[1];
};

struct OBJECT_DIRECTORY_INFORMATION
{
	UNICODE_STRING Name;
	UNICODE_STRING TypeName;
};

//..............................................................................

typedef
NTSTATUS
NTAPI
NtQueryDirectoryFileFunc(
	IN HANDLE FileHandle,
	IN HANDLE Event,
	IN PVOID ApcRoutine,
	IN PVOID ApcContext,
	OUT PIO_STATUS_BLOCK IoStatusBlock,
	OUT PVOID FileInformation,
	IN ULONG Length,
	IN INT FileInformationClass,
	IN BOOLEAN ReturnSingleEntry,
	IN PVOID FileName,
	IN BOOLEAN RestartScan
	);

typedef
NTSTATUS
NTAPI
NtOpenDirectoryObjectFunc(
	OUT PHANDLE DirectoryHandle,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes
	);

typedef
NTSTATUS
NTAPI
NtQueryDirectoryObjectFunc(
	IN HANDLE DirectoryHandle,
	OUT PVOID Buffer,
	IN ULONG Length,
	IN BOOLEAN ReturnSingleEntry,
	IN BOOLEAN RestartScan,
	IN PULONG Context,
	OUT PULONG ReturnLength
	);

typedef
NTSTATUS
NTAPI
NtOpenSymbolicLinkObjectFunc(
	OUT PHANDLE LinkHandle,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes
	);

typedef
NTSTATUS
NTAPI
NtQuerySymbolicLinkObjectFunc(
	IN HANDLE LinkHandle,
	OUT PUNICODE_STRING LinkTarget,
	OUT PULONG ReturnedLength
	);

typedef
NTSTATUS
NTAPI
NtQueryInformationFileFunc(
	HANDLE FileHandle,
	PIO_STATUS_BLOCK IoStatusBlock,
	PVOID FileInformation,
	ULONG Length,
	FILE_INFORMATION_CLASS FileInformationClass
	);

typedef
NTSTATUS
NTAPI
NtQueryObjectFunc(
	IN HANDLE FileHandle,
	IN OBJECT_INFORMATION_CLASS InformationClass,
	OUT PVOID Buffer,
	IN ULONG BufferSize,
	OUT PULONG ReturnedLength
	);

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

extern AXL_SELECT_ANY NtQueryDirectoryFileFunc* ntQueryDirectoryFile = NULL;
extern AXL_SELECT_ANY NtOpenDirectoryObjectFunc* ntOpenDirectoryObject = NULL;
extern AXL_SELECT_ANY NtQueryDirectoryObjectFunc* ntQueryDirectoryObject = NULL;
extern AXL_SELECT_ANY NtOpenSymbolicLinkObjectFunc* ntOpenSymbolicLinkObject = NULL;
extern AXL_SELECT_ANY NtQuerySymbolicLinkObjectFunc* ntQuerySymbolicLinkObject = NULL;
extern AXL_SELECT_ANY NtQueryInformationFileFunc* ntQueryInformationFile = NULL;
extern AXL_SELECT_ANY NtQueryObjectFunc* ntQueryObject = NULL;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
initNtDllFunctions();

//..............................................................................

} // namespace win
} // namespace sys
} // namespace axl
