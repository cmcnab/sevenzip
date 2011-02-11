#include "StdAfx.h"
#include "ArchiveUpdateCallback.h"
#include "PropVariant.h"
#include "FileSys.h"
#include "InStreamWrapper.h"


namespace SevenZip
{

ArchiveUpdateCallback::ArchiveUpdateCallback( const std::vector< FilePathInfo >& filePaths )
	: m_refCount( 0 )
	, m_filePaths( filePaths )
{
}

ArchiveUpdateCallback::~ArchiveUpdateCallback()
{
}

STDMETHODIMP ArchiveUpdateCallback::QueryInterface( REFIID iid, void** ppvObject )
{
	if ( iid == __uuidof( IUnknown ) )
	{
		*ppvObject = reinterpret_cast< IUnknown* >( this );
		AddRef();
		return S_OK;
	}

	if ( iid == IID_IArchiveUpdateCallback2 )
	{
		*ppvObject = static_cast< IArchiveUpdateCallback2* >( this );
		AddRef();
		return S_OK;
	}

	if ( iid == IID_ICryptoGetTextPassword2 )
	{
		*ppvObject = static_cast< ICryptoGetTextPassword2* >( this );
		AddRef();
		return S_OK;
	}

	if ( iid == IID_ICompressProgressInfo )
	{
		*ppvObject = static_cast< ICompressProgressInfo* >( this );
		AddRef();
		return S_OK;
	}

	return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) ArchiveUpdateCallback::AddRef()
{
	return static_cast< ULONG >( InterlockedIncrement( &m_refCount ) );
}

STDMETHODIMP_(ULONG) ArchiveUpdateCallback::Release()
{
	ULONG res = static_cast< ULONG >( InterlockedDecrement( &m_refCount ) );
	if ( res == 0 )
	{
		delete this;
	}
	return res;
}

STDMETHODIMP ArchiveUpdateCallback::SetTotal( UInt64 size )
{
	return S_OK;
}

STDMETHODIMP ArchiveUpdateCallback::SetCompleted( const UInt64* completeValue )
{
	return S_OK;
}

STDMETHODIMP ArchiveUpdateCallback::GetUpdateItemInfo( UInt32 index, Int32* newData, Int32* newProperties, UInt32* indexInArchive )
{
	// Setting info for Create mode (vs. Append mode).
	if ( newData != NULL )
	{
		*newData = 1;
	}

	if ( newProperties != NULL )
	{
		*newProperties = 1;
	}

	if ( indexInArchive != NULL )
	{
		*indexInArchive = static_cast< UInt32 >( -1 ); // TODO: UInt32.Max
	}

	return S_OK;
}

STDMETHODIMP ArchiveUpdateCallback::GetProperty( UInt32 index, PROPID propID, PROPVARIANT* value )
{
	CPropVariant prop;

	if ( propID == kpidIsAnti )
	{
		prop = false;
		prop.Detach( value );
		return S_OK;
	}

	if ( index >= m_filePaths.size() )
	{
		return E_INVALIDARG;
	}

	const FilePathInfo& fileInfo = m_filePaths.at( index );
	switch ( propID )
	{
		case kpidPath:		prop = fileInfo.FileName; break; // TODO: relative path
		case kpidIsDir:		prop = fileInfo.IsDirectory; break;
		case kpidSize:		prop = fileInfo.Size; break;
		case kpidAttrib:	prop = fileInfo.Attributes; break;
		case kpidCTime:		prop = fileInfo.CreationTime; break;
		case kpidATime:		prop = fileInfo.LastAccessTime; break;
		case kpidMTime:		prop = fileInfo.LastWriteTime; break;
	}

	prop.Detach( value );
	return S_OK;
}

STDMETHODIMP ArchiveUpdateCallback::GetStream( UInt32 index, ISequentialInStream** inStream )
{
	if ( index >= m_filePaths.size() )
	{
		return E_INVALIDARG;
	}

	const FilePathInfo& fileInfo = m_filePaths.at( index );
	if ( fileInfo.IsDirectory )
	{
		return S_OK;
	}

	CComPtr< IStream > fileStream = FileSys::OpenFileToRead( fileInfo.FilePath );
	if ( fileStream == nullptr )
	{
		return HRESULT_FROM_WIN32( GetLastError() );
	}

	CComPtr< InStreamWrapper > wrapperStream = new InStreamWrapper( fileStream );
	*inStream = wrapperStream.Detach();

	return S_OK;
}

STDMETHODIMP ArchiveUpdateCallback::SetOperationResult( Int32 operationResult )
{
	return S_OK;
}

STDMETHODIMP ArchiveUpdateCallback::GetVolumeSize( UInt32 index, UInt64* size )
{
	//if (VolumesSizes.Size() == 0)
	//return S_FALSE;
	//if (index >= (UInt32)VolumesSizes.Size())
	//index = VolumesSizes.Size() - 1;
	//*size = VolumesSizes[index];
	return S_OK;
}

STDMETHODIMP ArchiveUpdateCallback::GetVolumeStream( UInt32 index, ISequentialOutStream** volumeStream )
{
  //wchar_t temp[16];
  //ConvertUInt32ToString(index + 1, temp);
  //UString res = temp;
  //while (res.Length() < 2)
  //  res = UString(L'0') + res;
  //UString fileName = VolName;
  //fileName += L'.';
  //fileName += res;
  //fileName += VolExt;
  //COutFileStream *streamSpec = new COutFileStream;
  //CMyComPtr<ISequentialOutStream> streamLoc(streamSpec);
  //if (!streamSpec->Create(fileName, false))
  //  return ::GetLastError();
  //*volumeStream = streamLoc.Detach();
	return S_OK;
}

STDMETHODIMP ArchiveUpdateCallback::CryptoGetTextPassword2( Int32* passwordIsDefined, BSTR* password )
{
	*passwordIsDefined = 0;
	*password = SysAllocString( _T( "" ) );
	return *password != 0 ? S_OK : E_OUTOFMEMORY;
}

STDMETHODIMP ArchiveUpdateCallback::SetRatioInfo( const UInt64* inSize, const UInt64* outSize )
{
	return S_OK;
}

}
