#include "pch.h"
#include "MMNotificationClient.h"

CMMNotificationClient::CMMNotificationClient(IMMDeviceEnumerator* _pEnum) : _pEnum(NULL)
{
    // Register for audio device change notifications from Windows
    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
    const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
    HRESULT hr = CoCreateInstance(
        CLSID_MMDeviceEnumerator, NULL,
        CLSCTX_ALL, IID_IMMDeviceEnumerator,
        (void**)&_pEnum);
    if (_pEnum)
        _pEnum->RegisterEndpointNotificationCallback(this);
}

//************************************************************************************************
HRESULT STDMETHODCALLTYPE CMMNotificationClient::OnDefaultDeviceChanged(EDataFlow flow, ERole role,
    LPCWSTR pwstrDeviceId)
{
    

    return S_OK;
}

HRESULT __stdcall CMMNotificationClient::OnDeviceAdded(LPCWSTR pwstrDeviceId)
{
    return E_NOTIMPL;
}

HRESULT __stdcall CMMNotificationClient::OnDeviceRemoved(LPCWSTR pwstrDeviceId)
{
    return E_NOTIMPL;
}

HRESULT __stdcall CMMNotificationClient::OnDeviceStateChanged(LPCWSTR pwstrDeviceId, DWORD dwNewState)
{
    return E_NOTIMPL;
}

HRESULT __stdcall CMMNotificationClient::OnPropertyValueChanged(LPCWSTR pwstrDeviceId, const PROPERTYKEY key)
{
    return E_NOTIMPL;
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