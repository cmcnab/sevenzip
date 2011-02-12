#include "StdAfx.h"
#include "ArchiveExtractCallback.h"
#include "PropVariant.h"
#include "FileSys.h"
//#include "InStreamWrapper.h"


namespace SevenZip
{

const CString EmptyFileAlias = _T( "[Content]" );


UInt64 ConvertPropVariantToUInt64(const PROPVARIANT &prop)
{
	switch (prop.vt)
	{
		case VT_UI1: return prop.bVal;
		case VT_UI2: return prop.uiVal;
		case VT_UI4: return prop.ulVal;
		case VT_UI8: return (UInt64)prop.uhVal.QuadPart;
		default: throw 151199;
	}
}


ArchiveExtractCallback::ArchiveExtractCallback( const CComPtr< IInArchive >& archiveHandler, const CString& directory )
	: m_refCount( 0 )
	, m_archiveHandler( archiveHandler )
	, m_directory( directory )
{
}

ArchiveExtractCallback::~ArchiveExtractCallback()
{
}

STDMETHODIMP ArchiveExtractCallback::QueryInterface( REFIID iid, void** ppvObject )
{
	if ( iid == __uuidof( IUnknown ) )
	{
		*ppvObject = reinterpret_cast< IUnknown* >( this );
		AddRef();
		return S_OK;
	}

	if ( iid == IID_IArchiveExtractCallback )
	{
		*ppvObject = static_cast< IArchiveExtractCallback* >( this );
		AddRef();
		return S_OK;
	}

	if ( iid == IID_ICryptoGetTextPassword )
	{
		*ppvObject = static_cast< ICryptoGetTextPassword* >( this );
		AddRef();
		return S_OK;
	}

	return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) ArchiveExtractCallback::AddRef()
{
	return static_cast< ULONG >( InterlockedIncrement( &m_refCount ) );
}

STDMETHODIMP_(ULONG) ArchiveExtractCallback::Release()
{
	ULONG res = static_cast< ULONG >( InterlockedDecrement( &m_refCount ) );
	if ( res == 0 )
	{
		delete this;
	}
	return res;
}

STDMETHODIMP ArchiveExtractCallback::SetTotal( UInt64 size )
{
	return S_OK;
}

STDMETHODIMP ArchiveExtractCallback::SetCompleted( const UInt64* completeValue )
{
	return S_OK;
}

STDMETHODIMP ArchiveExtractCallback::GetStream( UInt32 index, ISequentialOutStream** outStream, Int32 askExtractMode )
{
	CString filePath;
	HRESULT hr = GetPropertyFilePath( index, filePath );
	if ( hr != S_OK )
	{
		return hr;
	}

	if ( askExtractMode != NArchive::NExtract::NAskMode::kExtract )
	{
		return S_OK;
	}

	FileInfo fileInfo;
	//hr = GetPropertyAttributes( index, fileInfo.Attributes );


	return S_OK;
}

STDMETHODIMP ArchiveExtractCallback::PrepareOperation( Int32 askExtractMode )
{
	return S_OK;
}

STDMETHODIMP ArchiveExtractCallback::SetOperationResult( Int32 operationResult )
{
	return S_OK;
}

STDMETHODIMP ArchiveExtractCallback::CryptoGetTextPassword( BSTR* password )
{
	return E_ABORT;
}

HRESULT ArchiveExtractCallback::GetPropertyFilePath( UInt32 index, CString& filePath )
{
	CPropVariant prop;
	HRESULT hr = m_archiveHandler->GetProperty( index, kpidPath, &prop );
	if ( hr != S_OK )
	{
		return hr;
	}

	if ( prop.vt == VT_EMPTY )
	{
		filePath = EmptyFileAlias;
		return S_OK;
	}
	else if ( prop.vt != VT_BSTR )
	{
		return E_FAIL;
	}

	filePath = prop.bstrVal;
	return S_OK;
}

HRESULT ArchiveExtractCallback::GetPropertyAttributes( UInt32 index, UINT& attributes )
{
	CPropVariant prop;
	HRESULT hr = m_archiveHandler->GetProperty( index, kpidAttrib, &prop );
	if ( hr != S_OK )
	{
		return hr;
	}

	if ( prop.vt == VT_EMPTY )
	{
		attributes = 0;
		//AttribDefined = false;
		return S_OK;
	}
	else if ( prop.vt != VT_UI4 )
	{
		return E_FAIL;
	}

	attributes = prop.ulVal;
	//AttribDefined = true;
	return S_OK;
}

HRESULT ArchiveExtractCallback::GetPropertyModifiedTime( UInt32 index, FILETIME& modifiedTime )
{
	CPropVariant prop;
	HRESULT hr = m_archiveHandler->GetProperty( index, kpidMTime, &prop );
	if ( hr != S_OK )
	{
		return hr;
	}

	if ( prop.vt == VT_EMPTY )
	{
		return S_FALSE;
	}
	else if ( prop.vt != VT_FILETIME )
	{
		return E_FAIL;
	}

	modifiedTime = prop.filetime;
	//MTimeDefined = true;
	return S_OK;
}

HRESULT ArchiveExtractCallback::GetPropertySize( UInt32 index, UInt64& size )
{
	CPropVariant prop;
	HRESULT hr = m_archiveHandler->GetProperty( index, kpidPath, &prop );
	if ( hr != S_OK )
	{
		return hr;
	}

	switch ( prop.vt )
	{
	case VT_EMPTY:
		return S_FALSE;
	case VT_UI1: 
		size = prop.bVal;
		break;
	case VT_UI2:
		size = prop.uiVal;
		break;
	case VT_UI4:
		size = prop.ulVal;
		break;
	case VT_UI8:
		size = (UInt64)prop.uhVal.QuadPart;
		break;
	default:
		return E_FAIL;
	}

	return S_OK;
}

}
