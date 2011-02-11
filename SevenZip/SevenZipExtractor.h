#pragma once


#include "ArchiveFileInfo.h"
#include "ArchiveProperty.h"


namespace SevenZip
{
	class SevenZipExtractor
	{
	private:

		std::vector< ArchiveFileInfo > m_archiveFileInfoCollection;
		std::vector< ArchiveProperty > m_archiveProperties;

	public:

		SevenZipExtractor();
		virtual ~SevenZipExtractor();

		const std::vector< ArchiveFileInfo >& GetArchiveFileData();
		const std::vector< ArchiveProperty >& GetArchiveProperties();

		virtual void ExtractArchive( const CString& directory );

	private:

		void GetArchiveInfo();
	};
}
