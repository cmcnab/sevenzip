#pragma once


class PathScanner
{
public:

	class Callback
	{
	public:

		virtual ~Callback() {}

		virtual void BeginScan() {}
		virtual void EndScan() {}
		virtual bool Entry( const CString& fullpath, const WIN32_FIND_DATA& fdata, bool& exit ) = 0;
		virtual void EnterDirectory( const CString& path ) {}
		virtual void LeaveDirectory( const CString& path ) {}
	};

public:

	PathScanner();
	~PathScanner();

	void Scan( const CString& root, Callback& cb );
	void Scan( const CString& root, const CString& searchPattern, Callback& cb );

private:

	bool DoScan( Callback& cb );
	void Push( const TCHAR* dir );
	void Pop();
	bool IsSpecialFileName( const TCHAR* fileName );

private:

	CString m_pathStack;
	CString m_searchPattern;
};
