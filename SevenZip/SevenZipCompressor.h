#pragma once


#include "SevenZipLibrary.h"
#include "FileInfo.h"


namespace SevenZip
{
	class SevenZipCompressor
	{
	private:

		const SevenZipLibrary& m_library;
		CString m_archivePath;

	public:

		SevenZipCompressor( const SevenZipLibrary& library, const CString& archivePath );
		virtual ~SevenZipCompressor();

		virtual void CompressDirectory( const CString& directory, bool recursion = true );

	private:

		virtual void CompressDirectory( const CString& directory, const CComPtr< IStream >& archiveStream, bool recursion );
		virtual void CompressFiles( const CComPtr< IStream >& archiveStream, const CString& pathPrefix, const std::vector< FilePathInfo >& filePaths );
	};
}
