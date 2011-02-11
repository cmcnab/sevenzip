#pragma once


#include "FileInfo.h"


namespace SevenZip
{
	struct ArchiveFileInfo : public FileInfo
	{
		int			Index;
		UINT		CRC;
		bool		Encrypted;
		CString		Comment;
	};
}
