#pragma once


#include <exception>


namespace SevenZip
{
	CString StrFmt( const TCHAR* format, ... );


	class SevenZipException
	{
	protected:

		CString m_message;

	public:

		SevenZipException();
		SevenZipException( const CString& message );
		virtual ~SevenZipException();

		const CString& GetMessage() const;
	};


	class SevenZipWindowsException : public SevenZipException
	{
	public:

		SevenZipWindowsException( const CString& contextMessage, DWORD lastError );
	};


	class SevenZipCOMException : public SevenZipException
	{
	public:

		SevenZipCOMException( const CString& contextMessage, HRESULT lastError );
	};
}
