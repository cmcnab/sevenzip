#include "StdAfx.h"
#include "PathScanner.h"
#include "FileSys.h"


namespace SevenZip
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

		ExamineEntries( directory, searchPattern, directories, cb );
	}
}

void PathScanner::ExamineEntries( const CString& directory, const CString& searchPattern, std::deque< CString >& subdirs, Callback& cb )
{
	WIN32_FIND_DATA fdata;
	CString findStr = FileSys::AppendPath( directory, searchPattern );

	HANDLE hFile = FindFirstFile( directory, &fdata );
	if ( hFile != INVALID_HANDLE_VALUE )
	{
		FilePathInfo fpInfo = ConvertFindInfo( directory, fdata );
		cb.EnterDirectory( directory );
		
		do
		{
			bool isSpecial = IsSpecialFileName( fpInfo.FileName );
			bool descend = false;

			if ( !isSpecial )
			{
				// Call the main entry callback
				// If the user set exit to true we need to end the scan now
				// (including any recursive calls we may be in)
				//CString tmp = m_pathStack;
				//tmp += _T( "\\" );
				//tmp += fdata.cFileName;

				bool exit = false;
				descend = cb.Entry( fpInfo, exit );

				if ( exit )
				{
					break;
				}
			}

			// If this is a directory and the user returned true, continue
			// on recursively
			if ( descend && fpInfo.IsDirectory && !isSpecial )
			{
				subdirs.push_back( fpInfo.FilePath );
			}
		} 
		while ( FindNextFile( hFile, &fdata ) );
		
		cb.LeaveDirectory( directory );
		FindClose( hFile );
	}
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

//void PathScanner::Push( const TCHAR* dir )
//{
//	if ( !m_pathStack.IsEmpty() && m_pathStack.Right( 1 ) != _T( "\\" ) )
//	{
//		m_pathStack += _T( "\\" );
//	}
//	m_pathStack += dir;
//}
//
//void PathScanner::Pop()
//{
//	int a = m_pathStack.ReverseFind( _T( '\\' ) );
//	if ( a >= 0 )
//	{
//		m_pathStack = m_pathStack.Left( a );
//	}
//}

bool PathScanner::IsSpecialFileName( const TCHAR* fileName )
{
	return _tcscmp( fileName, _T( "." ) ) == 0 || _tcscmp( fileName, _T( ".." ) ) == 0;
}

}
