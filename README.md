**Update**: This project is being moved from it original location on [Bitbucket](https://bitbucket.org/cmcnab/sevenzip).

SevenZip++
----------

This is a C++ wrapper for accessing the 7-zip COM-like API in 7z.dll and 7za.dll. This code is heavily based off of the Client7z sample found in the LZMA SDK ([http://www.7-zip.org/sdk.html](http://www.7-zip.org/sdk.html)).

The project itself is a static library and a small sample, with build files for Visual Studio 2010 and 2008.  To compile the library you will need the LZMA SDK for some headers, otherwise you can download the pre-complied lib files and headers on the [Downloads](https://bitbucket.org/cmcnab/sevenzip/downloads) page.

To use, first load the 7z DLL into memory:

	#include <7zpp/7zpp.h>
	
	SevenZip::SevenZipLibrary lib;
	lib.Load();

If the appropriate 7z DLL is not in your path you may wish to specify it explicitly in the call to load.  Note you may have the 64-bit version installed but are trying to load it from a 32-bit executable; keep that in mind if you encounter errors.

	lib.Load(_T("path\\to\\7za.dll"));

Then create and use a compressor:

	SevenZip::SevenZipCompressor compressor(lib, archiveName);
	compressor.CompressDirectory(targetDir);

Or an extractor:

	SevenZip::SevenZipExtractor extractor(lib, archiveName);
	extractor.ExtractArchive(destination);

Don't forget to wrap the operations in a try/catch block to handle errors:

	...
	catch (SevenZip::SevenZipException& ex)
	{
	    std::cerr << ex.GetMessage() << std::endl;
	}
