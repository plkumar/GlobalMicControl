#pragma once
#include "windows.h"
#include "mmdeviceapi.h"
#include "mmsystem.h"
#include "endpointvolume.h"
#include <set>

typedef std::pair<CString, CString> DeviceIdNameMap;

enum class MuteBehavior {
	TOGGLE = 0,
	MUTE = 1,
	UNMUTE = 2,
};


class MicControl
{
public:
	MicControl();
	~MicControl();
	HRESULT GetDevices(std::set<DeviceIdNameMap>* deviceList);
	void SetMute(MuteBehavior newMuteState);
	MuteBehavior GetMuteState();
	HRESULT SetSelectedDevice(CString deviceId);

	//CString GetDefaultDeviceName();
	DeviceIdNameMap GetDefaultDevice();

private:
	// Global Variables:
	IAudioEndpointVolume* micEndpointVolume;
	IMMDeviceEnumerator* deviceEnumerator;
	IMMDevice* selectedDevice;
};

