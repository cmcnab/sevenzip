#pragma once


namespace SevenZip
{
namespace intl
{
	struct FileInfo
	{
		CString		FileName;
		FILETIME	LastWriteTime;
		FILETIME	CreationTime;
		FILETIME	LastAccessTime;
		ULONGLONG	Size;
		UINT		Attributes;
		bool		IsDirectory;
	};

	struct FilePathInfo : public FileInfo
	{
		CString		FilePath;
	};
}
}
