#include "StdAfx.h"
#include "PathScanner.h"
#include "FileSys.h"


namespace SevenZip
{
namespace intl
{

void PathScanner::Scan( const CString& root, Callback& cb )
{
	Scan( root, _T( "*" ), cb );
}

void PathScanner::Scan( const CString& root, const CString& searchPattern, Callback& cb )
{
	std::deque< CString > directories;
	directories.push_back( root );

	while ( !directories.empty() )
	{
		CString directory = directories.front();
		directories.pop_front();

		if ( ExamineFiles( directory, searchPattern, cb ) )
		{
			break;
		}

		ExamineDirectories( directory, directories, cb );
	}
}

bool PathScanner::ExamineFiles( const CString& directory, const CString& searchPattern, Callback& cb )
{
	CString findStr = FileSys::AppendPath( directory, searchPattern );
	bool exit = false;

	WIN32_FIND_DATA fdata;
	HANDLE hFile = FindFirstFile( findStr, &fdata );
	if ( hFile == INVALID_HANDLE_VALUE )
	{
		return exit;
	}

	cb.EnterDirectory( directory );

	do
	{
		FilePathInfo fpInfo = ConvertFindInfo( directory, fdata );
		if ( !fpInfo.IsDirectory && !IsSpecialFileName( fpInfo.FileName ) )
		{
			cb.ExamineFile( fpInfo, exit );
		}
	} 
	while ( !exit && FindNextFile( hFile, &fdata ) );

	if ( !exit )
	{
		cb.LeaveDirectory( directory );
	}

	FindClose( hFile );
	return exit;
}

void PathScanner::ExamineDirectories( const CString& directory, std::deque< CString >& subDirs, Callback& cb )
{
	CString findStr = FileSys::AppendPath( directory, _T( "*" ) );

	WIN32_FIND_DATA fdata;
	HANDLE hFile = FindFirstFile( findStr, &fdata );
	if ( hFile == INVALID_HANDLE_VALUE )
	{
		return;
	}

	do
	{
		FilePathInfo fpInfo = ConvertFindInfo( directory, fdata );
		if ( fpInfo.IsDirectory && !IsSpecialFileName( fpInfo.FileName ) && cb.ShouldDescend( fpInfo ) )
		{
			subDirs.push_back( fpInfo.FilePath );
		}
	} 
	while ( FindNextFile( hFile, &fdata ) );
		
	FindClose( hFile );
}

bool PathScanner::IsAllFilesPattern( const CString& searchPattern )
{
	return searchPattern == _T( "*" ) || searchPattern == _T( "*.*" );
}

bool PathScanner::IsSpecialFileName( const CString& fileName )
{
	return fileName == _T( "." ) || fileName == _T( ".." );
}

bool PathScanner::IsDirectory( const WIN32_FIND_DATA& fdata )
{
	return ( fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) != 0;
}

FilePathInfo PathScanner::ConvertFindInfo( const CString& directory, const WIN32_FIND_DATA& fdata )
{
	FilePathInfo file;
	file.FileName		= fdata.cFileName;
	file.FilePath		= FileSys::AppendPath( directory, file.FileName );
	file.LastWriteTime	= fdata.ftLastWriteTime;
	file.CreationTime	= fdata.ftCreationTime;
	file.LastAccessTime	= fdata.ftLastAccessTime;
	file.Attributes		= fdata.dwFileAttributes;
	file.IsDirectory	= IsDirectory( fdata );

	ULARGE_INTEGER size;
	size.LowPart = fdata.nFileSizeLow;
	size.HighPart = fdata.nFileSizeHigh;
	file.Size = size.QuadPart;

	return file;
}

}
}
