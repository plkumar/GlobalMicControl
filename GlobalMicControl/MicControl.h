#pragma once
#include "windows.h"
#include "mmdeviceapi.h"
#include "mmsystem.h"
#include "endpointvolume.h"

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
	void SetMute(MuteBehavior newMuteState);
	MuteBehavior GetMuteState();

	CString GetDefaultDeviceName();

private:
	// Global Variables:
	IAudioEndpointVolume* micEndpointVolume;
	IMMDeviceEnumerator* deviceEnumerator;

private:
	BOOL PlayResource(LPCWSTR lpName);
};

