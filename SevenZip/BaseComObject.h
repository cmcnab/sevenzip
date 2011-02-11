#pragma once


namespace SevenZip
{
	class BaseComObject : public IUnknown
	{
	private:

		long m_refCount;

	public:

		BaseComObject();
		virtual ~BaseComObject();

		STDMETHOD(QueryInterface)( REFIID iid, void** ppvObject );
		STDMETHOD_(ULONG, AddRef)();
		STDMETHOD_(ULONG, Release)();
	};
}
