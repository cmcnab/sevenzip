#include "stdafx.h"
#include "BaseComObject.h"


namespace SevenZip
{

BaseComObject::BaseComObject()
	: m_refCount( 0 )
{
}

BaseComObject::~BaseComObject()
{
}

STDMETHODIMP BaseComObject::QueryInterface( REFIID iid, void** ppvObject )
{
	if ( iid == __uuidof( IUnknown ) )
	{
		*ppvObject = static_cast< IUnknown* >( this );
		AddRef();
		return S_OK;
	}
	else
	{
		return E_NOINTERFACE;
	}
}

STDMETHODIMP_(ULONG) BaseComObject::AddRef()
{
	return static_cast< ULONG >( InterlockedIncrement( &m_refCount ) );
}

STDMETHODIMP_(ULONG) BaseComObject::Release()
{
	ULONG res = static_cast< ULONG >( InterlockedDecrement( &m_refCount ) );
	if ( res == 0 )
	{
		delete this;
	}
	return res;
}

}
