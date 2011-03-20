#pragma once


#include "FileInfo.h"


namespace SevenZip
{
	class FileSys
	{
	public:

		static CString GetPath( const CString& filePath );
		static CString GetFileName( const CString& filePathOrName );
		static CString AppendPath( const CString& left, const CString& right );
		static CString ExtractRelativePath( const CString& basePath, const CString& fullPath );

		static bool DirectoryExists( const CString& path );
		static bool IsDirectoryEmptyRecursive( const CString& path );

		static bool CreateDirectoryTree( const CString& path );

		static std::vector< FilePathInfo > GetFilesInDirectory( const CAtlString& directory, const CString& searchPattern, bool recursive );

		static CComPtr< IStream > OpenFileToRead( const CString& filePath );
		static CComPtr< IStream > OpenFileToWrite( const CString& filePath );
	};
}
