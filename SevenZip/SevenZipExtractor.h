#pragma once


#include "SevenZipLibrary.h"
#include "ArchiveFileInfo.h"
#include "ArchiveProperty.h"


namespace SevenZip
{
	class SevenZipExtractor
	{
	private:

		const SevenZipLibrary& m_library;
		CString m_archivePath;

		//std::vector< ArchiveFileInfo > m_archiveFileInfoCollection;
		//std::vector< ArchiveProperty > m_archiveProperties;

	public:

		SevenZipExtractor( const SevenZipLibrary& library, const CString& archivePath );
		virtual ~SevenZipExtractor();

		//const std::vector< ArchiveFileInfo >& GetArchiveFileData();
		//const std::vector< ArchiveProperty >& GetArchiveProperties();

		virtual void ExtractArchive( const CString& directory );

	private:

		void GetArchiveInfo();
	};
}
