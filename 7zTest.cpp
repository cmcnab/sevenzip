#include "stdafx.h"
#include "SevenZip/SevenZipCompressor.h"




int Run()
{
	try
	{
		SevenZip::SevenZipLibrary lib;
		lib.Load();

		SevenZip::SevenZipCompressor compressor( lib );
		compressor.CompressDirectory( _T( "D:\\Temp\\7zTest\\Test1" ), _T( "D:\\Temp\\7zTest\\Test1.7z" ) );
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
	return Run();
}

