#pragma once


#include "SevenZipLibrary.h"


namespace SevenZip
{
	class SevenZipExtractor
	{
	private:

		const SevenZipLibrary& m_library;
		TString m_archivePath;

	public:

		SevenZipExtractor( const SevenZipLibrary& library, const TString& archivePath );
		virtual ~SevenZipExtractor();

		virtual void ExtractArchive( const TString& directory );

	private:

		void ExtractArchive( const CComPtr< IStream >& archiveStream, const TString& directory );
	};
}
