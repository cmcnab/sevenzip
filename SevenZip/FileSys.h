#pragma once


#include "FileInfo.h"


namespace SevenZip
{
	class FileSys
	{
	public:

		static CString GetPath( const CString& filePath );
		static CString AppendPath( const CString& first, const CString& second );

		static bool DirectoryExists( const CString& directory );
		static bool IsDirectoryEmptyRecursive( const CString& directory );

		static std::vector< FilePathInfo > GetFilesInDirectory( const CString& directory );
		static std::vector< FilePathInfo > GetFilesInDirectory( const CString& directory, const CString& searchPattern );

		static std::vector< FilePathInfo > GetFilesInDirectoryRecursive( const CString& directory );
		static std::vector< FilePathInfo > GetFilesInDirectoryRecursive( const CString& directory, const CString& searchPattern );

		static CComPtr< IStream > OpenFileToRead( const CString& filePath );
		static CComPtr< IStream > OpenFileToWrite( const CString& filePath );
	};
}
