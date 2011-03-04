#include "stdafx.h"
#include "FileSys.h"
#include "../PathScanner.h"


namespace SevenZip
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

	virtual bool Entry( const CString& fullpath, const WIN32_FIND_DATA& fdata, bool& exit )
	{
		m_files.push_back( ConvertFindInfo( fullpath, fdata ) );
		return m_recursive;
	}

private:

	static FilePathInfo ConvertFindInfo( const CString& fullpath, const WIN32_FIND_DATA& fdata )
	{
		FilePathInfo file;
		file.FilePath		= fullpath;
		file.FileName		= fdata.cFileName;
		file.LastWriteTime	= fdata.ftLastWriteTime;
		file.CreationTime	= fdata.ftCreationTime;
		file.LastAccessTime	= fdata.ftLastAccessTime;
		file.Attributes		= fdata.dwFileAttributes;
		file.IsDirectory	= ( fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) != 0;

		ULARGE_INTEGER size;
		size.LowPart = fdata.nFileSizeLow;
		size.HighPart = fdata.nFileSizeHigh;
		file.Size = size.QuadPart;

		return file;
	}
};

CString FileSys::GetPath( const CString& filePath )
{
	int pos = filePath.ReverseFind( _T( '\\' ) );
	if ( pos <= 0 )
	{
		pos = filePath.ReverseFind( _T( '/' ) );
		if ( pos <= 0 )
		{
			return CString();
		}
	}

	return filePath.Left( pos );
}

CString FileSys::AppendPath( const CString& first, const CString& second )
{
	if ( first.GetLength() == 0 )
	{
		return second;
	}

	TCHAR lastChar = first[ first.GetLength() - 1 ];
	if ( lastChar == _T( '\\' ) || lastChar == _T( '/' ) )
	{
		return first + second;
	}
	else
	{
		return first + _T( "\\" ) + second;
	}
}

bool FileSys::DirectoryExists( const CString& path )
{
	return true;
}

bool FileSys::IsDirectoryEmptyRecursive( const CString& path )
{
	return false;
}

bool FileSys::CreateDirectoryTree( const CString& path )
{
	int ret = SHCreateDirectoryEx( NULL, path, NULL );
	return ret == ERROR_SUCCESS;
}

std::vector< FilePathInfo > FileSys::GetFilesInDirectory( const CString& directory )
{
	return GetFilesInDirectory( directory, _T( "*" ) );
}

std::vector< FilePathInfo > FileSys::GetFilesInDirectory( const CString& directory, const CString& searchPattern )
{
	ScannerCallback cb( false );
	PathScanner scanner;

	scanner.Scan( directory, searchPattern, cb );

	return cb.GetFiles();
}

std::vector< FilePathInfo > FileSys::GetFilesInDirectoryRecursive( const CString& directory )
{
	return GetFilesInDirectoryRecursive( directory, _T( "*" ) );
}

std::vector< FilePathInfo > FileSys::GetFilesInDirectoryRecursive( const CString& directory, const CString& searchPattern )
{
	ScannerCallback cb( true );
	PathScanner scanner;

	scanner.Scan( directory, searchPattern, cb );

	return cb.GetFiles();
}

CComPtr< IStream > FileSys::OpenFileToRead( const CString& filePath )
{
	CComPtr< IStream > fileStream;

	if ( FAILED( SHCreateStreamOnFileEx( filePath, STGM_READ, FILE_ATTRIBUTE_NORMAL, FALSE, NULL, &fileStream ) ) )
	{
		return nullptr;
	}

	return fileStream;
}

CComPtr< IStream > FileSys::OpenFileToWrite( const CString& filePath )
{
	CComPtr< IStream > fileStream;

	if ( FAILED( SHCreateStreamOnFileEx( filePath, STGM_CREATE | STGM_WRITE, FILE_ATTRIBUTE_NORMAL, TRUE, NULL, &fileStream ) ) )
	{
		return nullptr;
	}

	return fileStream;
}

}
