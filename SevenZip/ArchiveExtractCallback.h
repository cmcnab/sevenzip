#pragma once


#include <7zip/Archive/IArchive.h>
#include <7zip/IPassword.h>


namespace SevenZip
{
	class ArchiveExtractCallback : public IArchiveExtractCallback, public ICryptoGetTextPassword
	{
	private:

		long m_refCount;
		CComPtr< IInArchive > m_archiveHandler;
		CString m_directory;

	public:

		ArchiveExtractCallback( const CComPtr< IInArchive >& archiveHandler, const CString& directory );
		virtual ~ArchiveExtractCallback();

		STDMETHOD(QueryInterface)( REFIID iid, void** ppvObject );
		STDMETHOD_(ULONG, AddRef)();
		STDMETHOD_(ULONG, Release)();

		// IProgress
		STDMETHOD(SetTotal)( UInt64 size );
		STDMETHOD(SetCompleted)( const UInt64* completeValue );

		// IArchiveExtractCallback
		STDMETHOD(GetStream)( UInt32 index, ISequentialOutStream** outStream, Int32 askExtractMode );
		STDMETHOD(PrepareOperation)( Int32 askExtractMode );
		STDMETHOD(SetOperationResult)( Int32 resultEOperationResult );

		// ICryptoGetTextPassword
		STDMETHOD(CryptoGetTextPassword)( BSTR* password );

	private:

		HRESULT GetPropertyFilePath( UInt32 index, CString& filePath );
		HRESULT GetPropertyAttributes( UInt32 index, UINT& attributes );
		HRESULT GetPropertyModifiedTime( UInt32 index, FILETIME& modifiedTime );
		HRESULT GetPropertySize( UInt32 index, UInt64& size );
	};
}
