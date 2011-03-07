#pragma once


#include "SevenZipLibrary.h"
#include "FileInfo.h"
#include "CompressionLevel.h"


namespace SevenZip
{
	class SevenZipCompressor
	{
	private:

		const SevenZipLibrary& m_library;
		CString m_archivePath;
		CompressionLevelEnum m_compressionLevel;

	public:

		SevenZipCompressor( const SevenZipLibrary& library, const CString& archivePath );
		virtual ~SevenZipCompressor();

		void SetCompressionLevel( const CompressionLevelEnum& level );

		virtual void CompressDirectory( const CString& directory, bool recursion = true );

	private:

		void CompressDirectory( const CString& directory, const CComPtr< IStream >& archiveStream, bool recursion );
		void CompressFiles( const CComPtr< IStream >& archiveStream, const CString& pathPrefix, const std::vector< FilePathInfo >& filePaths );
		void SetCompressionProperties( IUnknown* outArchive );
	};
}
