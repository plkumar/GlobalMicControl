#include "pch.h"
#include "resource.h"
#include "MicControl.h"
#include "windows.h"
#include "mmdeviceapi.h"
#include "mmsystem.h"
#include "endpointvolume.h"
#include <set>

namespace {
	const auto MUTE_KEY = VK_F15;
	const auto UNMUTE_KEY = VK_F16;
	const auto PTT_PTM_MOUSE_BUTTON = XBUTTON2;
	const unsigned int PTT_RELEASE_DELAY_MS = 250;
};

MicControl::MicControl():selectedDevice(NULL)
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
	free(micEndpointVolume);
	free(deviceEnumerator);
	micEndpointVolume=NULL;
	deviceEnumerator=NULL;
}

HRESULT MicControl::GetDevices(std::set<DeviceIdNameMap>* deviceList)
{
	
	IMMDeviceCollection* deviceCollection;
	deviceEnumerator->EnumAudioEndpoints(EDataFlow::eCapture, ERole::eMultimedia, &deviceCollection);

	UINT deviceCount = 0;
	deviceCollection->GetCount(&deviceCount);

	IPropertyStore* pProps = NULL;
	LPWSTR pwszID = NULL;
	
	GUID IDevice_FriendlyName = { 0xa45c254e, 0xdf1c, 0x4efd, { 0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0 } };
	static PROPERTYKEY key;
	key.pid = 14;
	key.fmtid = IDevice_FriendlyName;

	for (UINT i=0; i<deviceCount; i++)
	{
		IMMDevice* micDevicePtr;
		deviceCollection->Item(i, &micDevicePtr);
		LPWSTR lDeviceId;
		micDevicePtr->GetId(&lDeviceId);
		auto hr = micDevicePtr->OpenPropertyStore(
			STGM_READ, &pProps);

		if (hr != S_OK) {
			return S_FALSE;
		}

		PROPVARIANT varName;
		// Initialize container for property value.
		PropVariantInit(&varName);
		//pProps.
		// Get the endpoint's friendly-name property.
		hr = pProps->GetValue(key, &varName);

		if (hr != S_OK) {
			return S_FALSE;
		}
		CString deviceName = varName.pwszVal;
		deviceList->insert(std::make_pair(CString(lDeviceId),deviceName));
		PropVariantClear(&varName);
	}

	return S_OK;
}

void MicControl::SetMute(MuteBehavior newMuteState)
{

	/*IMMDevice* micDevicePtr;
	deviceEnumerator->GetDefaultAudioEndpoint(EDataFlow::eCapture, ERole::eCommunications, &micDevicePtr);*/

	IMMDevice* micDevicePtr;
	if (selectedDevice == NULL)
	{
		deviceEnumerator->GetDefaultAudioEndpoint(EDataFlow::eCapture, ERole::eCommunications, &micDevicePtr);
	}
	else {
		micDevicePtr = selectedDevice;
	}

	micDevicePtr->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, nullptr, (void**)&micEndpointVolume);
	BOOL wasMuted;
	micEndpointVolume->GetMute(&wasMuted);
	if (wasMuted && newMuteState == MuteBehavior::MUTE) {
		return;
	}
	if (!wasMuted && newMuteState == MuteBehavior::UNMUTE) {
		return;
	}

	const auto muteWav = MAKEINTRESOURCE(IDR_MUTE);
	const auto unmuteWav = MAKEINTRESOURCE(IDR_UNMUTE);
	//const auto muteWav = L"mute.wav";
	//const auto unmuteWav = L"unmute.wav";
	const auto feedbackWav = wasMuted ? unmuteWav : muteWav;

	if (!wasMuted) {
		micEndpointVolume->SetMute(TRUE, nullptr);
	}
	else {
		micEndpointVolume->SetMute(FALSE, nullptr);
	}
	PlaySound(feedbackWav, AfxGetStaticModuleState()->m_hCurrentInstanceHandle, SND_SYSTEM | SND_ASYNC | SND_RESOURCE);
	//PlaySound(feedbackWav, AfxGetStaticModuleState()->m_hCurrentInstanceHandle, SND_SYSTEM |SND_ASYNC | SND_FILENAME);
	//PlayResource(L"mute.wav");
}

MuteBehavior MicControl::GetMuteState()
{
	IMMDevice* micDevicePtr;
	if (selectedDevice == NULL)
	{
		deviceEnumerator->GetDefaultAudioEndpoint(EDataFlow::eCapture, ERole::eCommunications, &micDevicePtr);
	}
	else {
		micDevicePtr = selectedDevice;
	}

	micDevicePtr->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, nullptr, (void**)&micEndpointVolume);
	BOOL wasMuted;
	micEndpointVolume->GetMute(&wasMuted);
	return wasMuted == TRUE ? MuteBehavior::MUTE : MuteBehavior::UNMUTE;
}

HRESULT MicControl::SetSelectedDevice(CString deviceId)
{
	IMMDeviceCollection* deviceCollection;
	deviceEnumerator->EnumAudioEndpoints(EDataFlow::eCapture, ERole::eMultimedia, &deviceCollection);

	UINT deviceCount = 0;
	deviceCollection->GetCount(&deviceCount);	

	if (deviceCount == 0) return E_FAIL;

	for (UINT i = 0; i < deviceCount; i++)
	{
		IMMDevice* micDevicePtr;
		deviceCollection->Item(i, &micDevicePtr);
		LPWSTR lDeviceId;
		if (micDevicePtr->GetId(&lDeviceId) == S_OK && lDeviceId == deviceId)
		{
			this->selectedDevice = micDevicePtr;
		}
	}

	return S_OK;
}

//CString MicControl::GetDefaultDeviceName()
//{
//	IMMDevice* micDevicePtr;
//	IPropertyStore* pProps = NULL;
//	LPWSTR pwszID = NULL;
//	
//	deviceEnumerator->GetDefaultAudioEndpoint(EDataFlow::eCapture, ERole::eCommunications, &micDevicePtr);
//
//	auto hr = micDevicePtr->OpenPropertyStore(
//		STGM_READ, &pProps);
//	
//	if (hr != S_OK) return NULL;
//
//	static PROPERTYKEY key;
//
//	GUID IDevice_FriendlyName = { 0xa45c254e, 0xdf1c, 0x4efd, { 0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0 } };
//	key.pid = 14;
//	key.fmtid = IDevice_FriendlyName;
//	PROPVARIANT varName;
//
//	// Initialize container for property value.
//	PropVariantInit(&varName);
//	//pProps.
//	// Get the endpoint's friendly-name property.
//	hr = pProps->GetValue(key, &varName);
//	
//	if (hr != S_OK) return NULL;
//	CString deviceName = varName.pwszVal;
//	PropVariantClear(&varName);
//
//	return deviceName;
//}

DeviceIdNameMap MicControl::GetDefaultDevice()
{
	DeviceIdNameMap defaultDevice = DeviceIdNameMap();
	IMMDevice* micDevicePtr;
	IPropertyStore* pProps = NULL;
	LPWSTR pwszID = NULL;

	deviceEnumerator->GetDefaultAudioEndpoint(EDataFlow::eCapture, ERole::eCommunications, &micDevicePtr);

	micDevicePtr->GetId(&pwszID);
	defaultDevice.first = pwszID;

	auto hr = micDevicePtr->OpenPropertyStore(
		STGM_READ, &pProps);

	if (hr != S_OK) return defaultDevice;

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

	if (hr != S_OK) return defaultDevice;

	defaultDevice.second = varName.pwszVal;
	PropVariantClear(&varName);
	
	return defaultDevice;
}
