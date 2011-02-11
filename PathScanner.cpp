#include "StdAfx.h"
#include "PathScanner.h"


PathScanner::PathScanner()
{
}

PathScanner::~PathScanner()
{
}

void PathScanner::Scan( const CString& root, Callback& cb )
{
	Scan( root, _T( "*" ), cb );
}

void PathScanner::Scan( const CString& root, const CString& searchPattern, Callback& cb )
{
	m_pathStack = root;
	m_searchPattern = searchPattern;
	
	cb.BeginScan();
	DoScan( cb );
	cb.EndScan();
}


bool PathScanner::DoScan( Callback& cb )
{
	WIN32_FIND_DATA fdata;
	HANDLE			hFile;
	bool			recur	= false;
	bool			exit	= false;

	Push( m_searchPattern );
	hFile = FindFirstFile( m_pathStack, &fdata );
	Pop();

	if ( hFile != INVALID_HANDLE_VALUE )
	{
		cb.EnterDirectory( m_pathStack );
		
		do
		{
			bool isSpecial = IsSpecialFileName( fdata.cFileName );

			if ( !isSpecial )
			{
				// Call the main entry callback
				// If the user set exit to true we need to end the scan now
				// (including any recursive calls we may be in)
				CString tmp = m_pathStack;
				tmp += _T( "\\" );
				tmp += fdata.cFileName;

				recur = cb.Entry( tmp, fdata, exit );

				if ( exit )
				{
					break;
				}
			}

			// If this is a directory and the user returned true, continue
			// on recursively
			if ( recur && ( fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) && !isSpecial )
			{
				Push( fdata.cFileName );
				exit = DoScan( cb );
				Pop();
			}
		} 
		while ( !exit && FindNextFile( hFile, &fdata ) );
		
		// TODO: should I not call this callback when exiting?
		cb.LeaveDirectory( m_pathStack );

		FindClose( hFile );
	}

	return exit;
}

void PathScanner::Push( const TCHAR* dir )
{
	if ( !m_pathStack.IsEmpty() && m_pathStack.Right( 1 ) != _T( "\\" ) )
	{
		m_pathStack += _T( "\\" );
	}
	m_pathStack += dir;
}

void PathScanner::Pop()
{
	int a = m_pathStack.ReverseFind( _T( '\\' ) );
	if ( a >= 0 )
	{
		m_pathStack = m_pathStack.Left( a );
	}
}

bool PathScanner::IsSpecialFileName( const TCHAR* fileName )
{
	return _tcscmp( fileName, _T( "." ) ) == 0 || _tcscmp( fileName, _T( ".." ) ) == 0;
}
