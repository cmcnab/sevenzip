#include "stdafx.h"
#include "FileSys.h"
#include "PathScanner.h"
#include <ShlObj.h>


namespace SevenZip
{
namespace intl
{

class ScannerCallback : public PathScanner::Callback
{
private:

	std::vector< FilePathInfo > m_files;
	bool m_recursive;

public:

	ScannerCallback( bool recursive )
		: m_recursive( recursive )
	{}

	const std::vector< FilePathInfo >& GetFiles() { return m_files; }

	virtual bool ShouldDescend( const FilePathInfo& directory )
	{
		return m_recursive;
	}

	virtual void ExamineFile( const FilePathInfo& file, bool& exit )
	{
		m_files.push_back( file );
	}
};

CString FileSys::GetPath( const CString& filePath )
{
	// Find the last "\" or "/" in the string and return it and everything before it.
	int index  = filePath.ReverseFind( _T( '\\' ) );
	int index2 = filePath.ReverseFind( _T( '/' ) );

	if ( index2 > index )
	{
		index = index2;
	}

	if ( index >= filePath.GetLength() - 1 )
	{
		// Last char is path sep, the whole thing is a path.
		return filePath;
	}
	else if ( index < 0 )
	{
		// No path sep.
		return CAtlString();
	}
	else
	{
		return filePath.Left( index + 1 );
	}
}

CString FileSys::GetFileName( const CString& filePathOrName )
{
	// Find the last "\" or "/" in the string and return everything after it.
	int index  = filePathOrName.ReverseFind( _T( '\\' ) );
	int index2 = filePathOrName.ReverseFind( _T( '/' ) );

	if ( index2 > index )
	{
		index = index2;
	}

	if ( index >= filePathOrName.GetLength() - 1 )
	{
		// Last char is path sep, no filename.
		return CString();
	}
	else if ( index < 0 )
	{
		// No path sep, return the whole thing.
		return filePathOrName;
	}
	else
	{
		return filePathOrName.Mid( index + 1 );
	}
}

CString FileSys::AppendPath( const CString& left, const CString& right )
{
	if ( left.GetLength() == 0 )
	{
		return right;
	}

	TCHAR lastChar = left[ left.GetLength() - 1 ];
	if ( lastChar == _T( '\\' ) || lastChar == _T( '/' ) )
	{
		return left + right;
	}
	else
	{
		return left + _T( "\\" ) + right;
	}
}

CString FileSys::ExtractRelativePath( const CString& basePath, const CString& fullPath )
{
	if ( basePath.GetLength() >= fullPath.GetLength() )
	{
		return CString();
	}

	if ( basePath != fullPath.Left( basePath.GetLength() ) )
	{
		return CString();
	}

	return fullPath.Mid( basePath.GetLength() + 1 );
}

bool FileSys::DirectoryExists( const CString& path )
{
	// TODO:
	return true;
}

bool FileSys::IsDirectoryEmptyRecursive( const CString& path )
{
	// TODO:
	return false;
}

bool FileSys::CreateDirectoryTree( const CString& path )
{
	int ret = SHCreateDirectoryEx( NULL, path, NULL );
	return ret == ERROR_SUCCESS;
}

std::vector< FilePathInfo > FileSys::GetFilesInDirectory( const CString& directory, const CString& searchPattern, bool recursive )
{
	ScannerCallback cb( recursive );
	PathScanner::Scan( directory, searchPattern, cb );
	return cb.GetFiles();
}

CComPtr< IStream > FileSys::OpenFileToRead( const CString& filePath )
{
	CComPtr< IStream > fileStream;

	if ( FAILED( SHCreateStreamOnFileEx( filePath, STGM_READ, FILE_ATTRIBUTE_NORMAL, FALSE, NULL, &fileStream ) ) )
	{
		return NULL;
	}

	return fileStream;
}

CComPtr< IStream > FileSys::OpenFileToWrite( const CString& filePath )
{
	CComPtr< IStream > fileStream;

	if ( FAILED( SHCreateStreamOnFileEx( filePath, STGM_CREATE | STGM_WRITE, FILE_ATTRIBUTE_NORMAL, TRUE, NULL, &fileStream ) ) )
	{
		return NULL;
	}

	return fileStream;
}

}
}
