#pragma once


#include "SevenZipLibrary.h"
#include "FileInfo.h"


namespace SevenZip
{
	class SevenZipCompressor
	{
	private:

		const SevenZipLibrary& m_library;

	public:

		SevenZipCompressor( const SevenZipLibrary& library );
		virtual ~SevenZipCompressor();

		//const std::vector< ArchiveFileInfo >& GetArchiveFileData();
		//const std::vector< ArchiveProperty >& GetArchiveProperties();

		virtual void CompressDirectory( const CString& directory, const CString& archiveName, bool recursion = true );

	private:

		virtual void CompressDirectory( const CString& directory, const CComPtr< IStream >& archiveStream, bool recursion );
		virtual void CompressFiles( const CComPtr< IStream >& archiveStream, const std::vector< FilePathInfo >& filePaths );
	};
}
