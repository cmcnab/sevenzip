#pragma once


#include <exception>


namespace SevenZip
{
	CString StrFmt( const TCHAR* format, ... );
	CString GetWinErrMsg( const CString& contextMessage, DWORD lastError );
	CString GetCOMErrMsg( const CString& contextMessage, HRESULT lastError );


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
}
