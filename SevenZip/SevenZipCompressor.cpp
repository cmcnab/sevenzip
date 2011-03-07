#include "stdafx.h"
#include "SevenZipCompressor.h"
#include "GUIDs.h"
#include "FileSys.h"
#include "ArchiveUpdateCallback.h"
#include "OutStreamWrapper.h"
#include "PropVariant.h"


namespace SevenZip
{

SevenZipCompressor::SevenZipCompressor( const SevenZipLibrary& library, const CString& archivePath )
	: m_library( library )
	, m_archivePath( archivePath )
{
}

SevenZipCompressor::~SevenZipCompressor()
{
}

void SevenZipCompressor::SetCompressionLevel( const CompressionLevelEnum& level )
{
	m_compressionLevel = level;
}

void SevenZipCompressor::CompressDirectory( const CString& directory, bool recursion )
{
	CComPtr< IStream > fileStream = FileSys::OpenFileToWrite( m_archivePath );
	if ( fileStream == nullptr )
	{
		throw SevenZipException( StrFmt( _T( "Could not create archive \"%s\"" ), m_archivePath.GetString() ) );
	}
	
	CompressDirectory( directory, fileStream, recursion );
}

void SevenZipCompressor::CompressDirectory( const CString& directory, const CComPtr< IStream >& archiveStream, bool recursion )
{
	if ( !FileSys::DirectoryExists( directory ) )
	{
		throw SevenZipException( StrFmt( _T( "Directory \"%s\" does not exist" ), directory.GetString() ) );
	}
	
	if ( FileSys::IsDirectoryEmptyRecursive( directory ) )
	{
		throw SevenZipException( StrFmt( _T( "Directory \"%s\" is empty" ), directory.GetString() ) );
	}

	std::vector< FilePathInfo > files;
	if ( recursion )
	{
		files = FileSys::GetFilesInDirectoryRecursive( directory );
	}
	else
	{
		files = FileSys::GetFilesInDirectory( directory );
	}

	CString pathPrefix = FileSys::GetPath( directory );
	CompressFiles( archiveStream, pathPrefix, files );
}

void SevenZipCompressor::CompressFiles( const CComPtr< IStream >& archiveStream, const CString& pathPrefix, const std::vector< FilePathInfo >& filePaths )
{
	CComPtr< IOutArchive > archiver;
	m_library.CreateObject( CLSID_CFormat7z, IID_IOutArchive, reinterpret_cast< void** >( &archiver ) );

	SetCompressionProperties( archiver );

	CComPtr< OutStreamWrapper > outFile = new OutStreamWrapper( archiveStream );
	CComPtr< ArchiveUpdateCallback > callback = new ArchiveUpdateCallback( pathPrefix, filePaths );

	HRESULT hr = archiver->UpdateItems( outFile, filePaths.size(), callback );
	if ( hr != S_OK ) // returning S_FALSE also indicates error
	{
		throw SevenZipCOMException( _T( "Create archive" ), hr );
	}
}

void SevenZipCompressor::SetCompressionProperties( IUnknown* outArchive )
{
	const size_t numProps = 1;
	const wchar_t* names[numProps] = { L"x" };
	CPropVariant values[numProps] = { static_cast< UInt32 >( m_compressionLevel.GetValue() ) };

	CComPtr< ISetProperties > setter;
	outArchive->QueryInterface( IID_ISetProperties, (void**)&setter );
	if ( setter == nullptr )
	{
		throw SevenZipException( _T( "Archive does not support setting compression properties" ) );
	}

	HRESULT hr = setter->SetProperties( names, values, numProps );
	if ( hr != S_OK )
	{
		throw SevenZipCOMException( _T( "Setting compression properties" ), hr );
	}
}

}
