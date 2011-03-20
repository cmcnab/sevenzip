#pragma once


#include "FileInfo.h"


namespace SevenZip
{
	class PathScanner
	{
	public:

		class Callback
		{
		public:

			virtual ~Callback() {}

			virtual void BeginScan() {}
			virtual void EndScan() {}
			virtual bool Entry( const FilePathInfo& file, bool& exit ) = 0;
			virtual void EnterDirectory( const CString& path ) {}
			virtual void LeaveDirectory( const CString& path ) {}
		};

	public:

		static void Scan( const CString& root, Callback& cb );
		static void Scan( const CString& root, const CString& searchPattern, Callback& cb );

	private:

		static void ExamineEntries( const CString& directory, const CString& searchPattern, std::deque< CString >& subdirs, Callback& cb );

		static bool IsSpecialFileName( const TCHAR* fileName );
		static bool IsDirectory( const WIN32_FIND_DATA& fdata );
		static FilePathInfo ConvertFindInfo( const CString& directory, const WIN32_FIND_DATA& fdata );
	};
}
