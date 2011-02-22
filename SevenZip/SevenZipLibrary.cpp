#include "stdafx.h"
#include "SevenZipLibrary.h"


namespace SevenZip
{

const CString DefaultLibraryPath = _T( "7z.dll" );

SevenZipLibrary::SevenZipLibrary()
	: m_dll( NULL )
	, m_func( NULL )
{
}

SevenZipLibrary::~SevenZipLibrary()
{
	Free();
}

void SevenZipLibrary::Load()
{
	Load( DefaultLibraryPath );
}

void SevenZipLibrary::Load( const CString& libraryPath )
{
	Free();
	m_dll = LoadLibrary( libraryPath );
	if ( m_dll == NULL )
	{
		throw SevenZipWindowsException( _T( "LoadLibrary" ), GetLastError() );
	}

	m_func = reinterpret_cast< CreateObjectFunc >( GetProcAddress( m_dll, "CreateObject" ) );
	if ( m_func == NULL )
	{
		throw SevenZipException( _T( "Loaded library is missing required CreateObject function" ) );
	}
}

void SevenZipLibrary::Free()
{
	if ( m_dll != NULL )
	{
		FreeLibrary( m_dll );
		m_dll = NULL;
		m_func = NULL;
	}
}

void SevenZipLibrary::CreateObject( const GUID& clsID, const GUID& interfaceID, void** outObject ) const
{
	if ( m_func == NULL )
	{
		throw SevenZipException( _T( "Library is not loaded" ) );
	}

	HRESULT hr = m_func( &clsID, &interfaceID, outObject );
	if ( FAILED( hr ) )
	{
		throw SevenZipCOMException( _T( "CreateObject" ), hr );
	}
}

}
