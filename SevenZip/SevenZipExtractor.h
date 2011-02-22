#pragma once


#include "SevenZipLibrary.h"


namespace SevenZip
{
	class SevenZipExtractor
	{
	private:

		const SevenZipLibrary& m_library;
		CString m_archivePath;

	public:

		SevenZipExtractor( const SevenZipLibrary& library, const CString& archivePath );
		virtual ~SevenZipExtractor();

		virtual void ExtractArchive( const CString& directory );

	private:

		void ExtractArchive( const CComPtr< IStream >& archiveStream, const CString& directory );
	};
}
