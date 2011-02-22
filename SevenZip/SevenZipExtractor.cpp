#include "stdafx.h"
#include "SevenZipExtractor.h"
#include "GUIDs.h"
#include "FileSys.h"
#include "ArchiveOpenCallback.h"
#include "ArchiveExtractCallback.h"
#include "InStreamWrapper.h"


namespace SevenZip
{

SevenZipExtractor::SevenZipExtractor( const SevenZipLibrary& library, const CString& archivePath )
	: m_library( library )
	, m_archivePath( archivePath )
{
}

SevenZipExtractor::~SevenZipExtractor()
{
}

void SevenZipExtractor::ExtractArchive( const CString& destDirectory )
{
	CComPtr< IStream > fileStream = FileSys::OpenFileToRead( m_archivePath );
	if ( fileStream == nullptr )
	{
		throw SevenZipException( StrFmt( _T( "Could not open archive \"%s\"" ), m_archivePath.GetString() ) );
	}

	ExtractArchive( fileStream, destDirectory );
}

void SevenZipExtractor::ExtractArchive( const CComPtr< IStream >& archiveStream, const CString& destDirectory )
{
	CComPtr< IInArchive > archive;
	m_library.CreateObject( CLSID_CFormat7z, IID_IInArchive, (void**)&archive );

	CComPtr< InStreamWrapper > inFile = new InStreamWrapper( archiveStream );
	CComPtr< ArchiveOpenCallback > openCallback = new ArchiveOpenCallback();

	HRESULT hr = archive->Open( inFile, 0, openCallback );
	if ( hr != S_OK )
	{
		throw SevenZipCOMException( _T( "Open archive" ), hr );
	}

	CComPtr< ArchiveExtractCallback > extractCallback = new ArchiveExtractCallback( archive, destDirectory );

	hr = archive->Extract( nullptr, -1, false, extractCallback );
	if ( hr != S_OK ) // returning S_FALSE also indicates error
	{
		throw SevenZipCOMException( _T( "Extract archive" ), hr );
	}
}

}
