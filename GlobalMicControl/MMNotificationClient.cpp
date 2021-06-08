#include "pch.h"
#include "MMNotificationClient.h"

CMMNotificationClient::CMMNotificationClient() : _pEnum(NULL), _cRef(1)
{
    // Register for audio device change notifications from Windows
    if (SUCCEEDED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED)))
    {
        const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
        const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
        HRESULT hr = CoCreateInstance(
            CLSID_MMDeviceEnumerator, NULL,
            CLSCTX_ALL, IID_IMMDeviceEnumerator,
            (void**)&_pEnum);
        if (SUCCEEDED(hr) && _pEnum)
            _pEnum->RegisterEndpointNotificationCallback(this);
    }
}

//************************************************************************************************
HRESULT STDMETHODCALLTYPE CMMNotificationClient::OnDefaultDeviceChanged(EDataFlow flow, ERole role,
    LPCWSTR pwstrDeviceId)
{
    

    return S_OK;
}

HRESULT __stdcall CMMNotificationClient::OnDeviceAdded(LPCWSTR pwstrDeviceId)
{
    return S_OK;
}

HRESULT __stdcall CMMNotificationClient::OnDeviceRemoved(LPCWSTR pwstrDeviceId)
{
    return S_OK;
}

HRESULT __stdcall CMMNotificationClient::OnDeviceStateChanged(LPCWSTR pwstrDeviceId, DWORD dwNewState)
{
    return S_OK;
}

HRESULT __stdcall CMMNotificationClient::OnPropertyValueChanged(LPCWSTR pwstrDeviceId, const PROPERTYKEY key)
{
    return S_OK;
}

//************************************************************************************************
CMMNotificationClient::~CMMNotificationClient()
{
    // Stop receiving audio device notifications
    if (_pEnum)
    {
        _pEnum->UnregisterEndpointNotificationCallback(this);
        _pEnum->Release();
    }
}

ULONG __stdcall CMMNotificationClient::AddRef()
{
    return InterlockedIncrement(&_cRef);
}

ULONG __stdcall CMMNotificationClient::Release()
{
    return InterlockedDecrement(&_cRef);
}

HRESULT __stdcall CMMNotificationClient::QueryInterface(REFIID riid, VOID** ppvInterface)
{
    if (IID_IUnknown == riid)
	{
		AddRef();
		*ppvInterface = static_cast<IUnknown*>(this);
	}
	else if (__uuidof(IMMNotificationClient) == riid)
	{
		AddRef();
		*ppvInterface = static_cast<IMMNotificationClient*>(this);
	}
	else
	{
		*ppvInterface = nullptr;
		return E_NOINTERFACE;
	}
	return S_OK;
}
