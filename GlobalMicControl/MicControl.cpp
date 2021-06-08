#include "pch.h"
#include "resource.h"
#include "MicControl.h"
#include "windows.h"
#include "mmdeviceapi.h"
#include "mmsystem.h"
#include "endpointvolume.h"

namespace {
	const auto MUTE_KEY = VK_F15;
	const auto UNMUTE_KEY = VK_F16;
	const auto PTT_PTM_MOUSE_BUTTON = XBUTTON2;
	const unsigned int PTT_RELEASE_DELAY_MS = 250;
};

MicControl::MicControl()
{
	if (FAILED(CoInitializeEx(NULL, COINIT_MULTITHREADED)))
	{
		TRACE(L"Failed to initialize COM environment!");
	}

	//IMMDeviceEnumerator* deviceEnumerator;
	if (CoCreateInstance(
		__uuidof(MMDeviceEnumerator), NULL,
		CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),
		(void**)&deviceEnumerator
	) != S_OK) {
		TRACE("Error creating instance for MMDeviceEnumerator.");
		return;
	}
}

MicControl::~MicControl()
{
	micEndpointVolume->Release();
	deviceEnumerator->Release();
	free(micEndpointVolume);
	free(deviceEnumerator);
	micEndpointVolume=NULL;
	deviceEnumerator=NULL;
}

void MicControl::SetMute(MuteBehavior newMuteState)
{

	IMMDevice* micDevicePtr;
	deviceEnumerator->GetDefaultAudioEndpoint(EDataFlow::eCapture, ERole::eCommunications, &micDevicePtr);

	micDevicePtr->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, nullptr, (void**)&micEndpointVolume);
	BOOL wasMuted;
	micEndpointVolume->GetMute(&wasMuted);
	if (wasMuted && newMuteState == MuteBehavior::MUTE) {
		return;
	}
	if (!wasMuted && newMuteState == MuteBehavior::UNMUTE) {
		return;
	}

	const auto feedbackWav = wasMuted ? MAKEINTRESOURCE(IDR_UNMUTE) : MAKEINTRESOURCE(IDR_MUTE);

	if (!wasMuted) {
		micEndpointVolume->SetMute(TRUE, nullptr);
	}
	else {
		micEndpointVolume->SetMute(FALSE, nullptr);
	}
	PlaySound(feedbackWav, AfxGetStaticModuleState()->m_hCurrentInstanceHandle, SND_SYSTEM | SND_ASYNC | SND_RESOURCE);
}

MuteBehavior MicControl::GetMuteState()
{
	IMMDevice* micDevicePtr;
	deviceEnumerator->GetDefaultAudioEndpoint(EDataFlow::eCapture, ERole::eCommunications, &micDevicePtr);

	micDevicePtr->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, nullptr, (void**)&micEndpointVolume);
	BOOL wasMuted;
	micEndpointVolume->GetMute(&wasMuted);
	return wasMuted == TRUE ? MuteBehavior::MUTE : MuteBehavior::UNMUTE;
}

CString MicControl::GetDefaultDeviceName()
{
	IMMDevice* micDevicePtr;
	IPropertyStore* pProps = NULL;
	LPWSTR pwszID = NULL;
	
	deviceEnumerator->GetDefaultAudioEndpoint(EDataFlow::eCapture, ERole::eCommunications, &micDevicePtr);

	auto hr = micDevicePtr->OpenPropertyStore(
		STGM_READ, &pProps);
	
	if (hr != S_OK) return NULL;

	static PROPERTYKEY key;

	GUID IDevice_FriendlyName = { 0xa45c254e, 0xdf1c, 0x4efd, { 0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0 } };
	key.pid = 14;
	key.fmtid = IDevice_FriendlyName;
	PROPVARIANT varName;

	// Initialize container for property value.
	PropVariantInit(&varName);
	//pProps.
	// Get the endpoint's friendly-name property.
	hr = pProps->GetValue(key, &varName);
	
	if (hr != S_OK) return NULL;
	CString deviceName = varName.pwszVal;
	PropVariantClear(&varName);

	return deviceName;
}

BOOL MicControl::PlayResource(LPCWSTR lpName)
{
	// TODO implement the playsound from resource
	return FALSE;
}


HRESULT MicControl::RegisterNotificationCallback(IMMNotificationClient* pClient) {
	HRESULT hr = S_OK;

	// get the default render endpoint
	hr = deviceEnumerator->RegisterEndpointNotificationCallback(pClient);
	
	if (FAILED(hr)) {
		TRACE("IMMDeviceEnumerator::RegisterEndpointNotificationCallback failed: hr = 0xx\n", hr);
		return hr;
	}
	return S_OK;
}

HRESULT MicControl::UnRegisterNotificationCallback(IMMNotificationClient* pClient) {
	HRESULT hr = S_OK;

	// get the default render endpoint
	hr = deviceEnumerator->UnregisterEndpointNotificationCallback(pClient);

	if (FAILED(hr)) {
		TRACE("IMMDeviceEnumerator::UnregisterEndpointNotificationCallback failed: hr = 0xx\n", hr);
		return hr;
	}
	return hr;
}