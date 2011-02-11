#include "stdafx.h"
#include "SevenZipException.h"


namespace SevenZip
{

CString StrFmt( const TCHAR* format, ... )
{
	CString result;
	TCHAR*	buffer;
	int		sz;
	va_list	args;

	va_start( args, format );

	sz		= _vsctprintf( format, args ) + 1;
	buffer	= result.GetBuffer( sz );
	sz		= _vsntprintf_s( buffer, sz, _TRUNCATE, format, args );
	result	.ReleaseBuffer( sz );

	va_end( args );

	return result;
}

SevenZipException::SevenZipException()
{
}

SevenZipException::SevenZipException( const CString& message )
	: m_message( message )
{
}

SevenZipException::~SevenZipException()
{
}

const CString& SevenZipException::GetMessage() const
{
	return m_message;
}



SevenZipWindowsException::SevenZipWindowsException( const CString& contextMessage, DWORD lastError )
{
	// TODO: use FormatMessage to get the appropriate message from the 
	m_message.Format( _T( "%s: GetLastError = %lu" ), contextMessage.GetString(), lastError );
}



SevenZipCOMException::SevenZipCOMException( const CString& contextMessage, HRESULT lastError )
{
	// TODO: use FormatMessage to get the appropriate message from the 
	m_message.Format( _T( "%s: HRESULT = 0x%08X" ), contextMessage.GetString(), lastError );
}

}
