#include "stdafx.h"
#include "SevenZip/SevenZipCompressor.h"
#include "SevenZip/SevenZipExtractor.h"


int Compress()
{
	try
	{
		SevenZip::SevenZipLibrary lib;
		lib.Load();

		SevenZip::SevenZipCompressor compressor( lib, _T( "C:\\Temp\\7zTest\\Test1.7z" ) );
		compressor.CompressDirectory( _T( "C:\\Temp\\7zTest\\Test1" ) );
	}
	catch ( SevenZip::SevenZipException& ex )
	{
		_tprintf( _T( "Error: %s\n" ), ex.GetMessage().GetString() );
		return 1;
	}

	return 0;
}

int Extract()
{
	try
	{
		SevenZip::SevenZipLibrary lib;
		lib.Load();

		SevenZip::SevenZipExtractor extractor( lib, _T( "C:\\Temp\\7zTest\\Test1.7z" ) );
		extractor.ExtractArchive( _T( "C:\\Temp\\7zTest\\Test1-Ext" ) );
	}
	catch ( SevenZip::SevenZipException& ex )
	{
		_tprintf( _T( "Error: %s\n" ), ex.GetMessage().GetString() );
		return 1;
	}

	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	return Extract();
}

