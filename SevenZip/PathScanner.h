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
			virtual bool ShouldDescend( const FilePathInfo& directory ) = 0;
			virtual void EnterDirectory( const CString& path ) {}
			virtual void LeaveDirectory( const CString& path ) {}
			virtual void ExamineFile( const FilePathInfo& file, bool& exit ) = 0;
		};

	public:

		static void Scan( const CString& root, Callback& cb );
		static void Scan( const CString& root, const CString& searchPattern, Callback& cb );

	private:

		static bool ExamineFiles( const CString& directory, const CString& searchPattern, Callback& cb );
		static void ExamineDirectories( const CString& directory, std::deque< CString >& subDirs, Callback& cb );

		static bool IsAllFilesPattern( const CString& searchPattern );
		static bool IsSpecialFileName( const CString& fileName );
		static bool IsDirectory( const WIN32_FIND_DATA& fdata );
		static FilePathInfo ConvertFindInfo( const CString& directory, const WIN32_FIND_DATA& fdata );
	};
}
