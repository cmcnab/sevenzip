#include "stdafx.h"
#include "SevenZipExtractor.h"


namespace SevenZip
{

SevenZipExtractor::SevenZipExtractor()
{
}

SevenZipExtractor::~SevenZipExtractor()
{
}

const std::vector< ArchiveFileInfo >& SevenZipExtractor::GetArchiveFileData()
{
	return m_archiveFileInfoCollection;
}

const std::vector< ArchiveProperty >& SevenZipExtractor::GetArchiveProperties()
{
	return m_archiveProperties;
}

void SevenZipExtractor::ExtractArchive( const CString& directory )
{
}

void SevenZipExtractor::GetArchiveInfo()
{
}

}
