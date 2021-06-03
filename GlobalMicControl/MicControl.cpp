#include "pch.h"
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

void MicControl::SetMute(MuteBehavior SetTo, bool IsButtonUp)
{
	//IMMDeviceEnumerator* de;
	/*CoCreateInstance(
		__uuidof(MMDeviceEnumerator), NULL,
		CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),
		(void**)&de
	);*/

	IMMDevice* micDevicePtr;
	de->GetDefaultAudioEndpoint(EDataFlow::eCapture, ERole::eCommunications, &micDevicePtr);

	micDevicePtr->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, nullptr, (void**)&micVolume);
	BOOL wasMuted;
	micVolume->GetMute(&wasMuted);
	if (wasMuted && SetTo == MuteBehavior::MUTE) {
		return;
	}
	if (!wasMuted && SetTo == MuteBehavior::UNMUTE) {
		return;
	}

	/*const auto muteWav = MAKEINTRESOURCE(IDR_MUTE);
	const auto unmuteWav = MAKEINTRESOURCE(IDR_UNMUTE);*/
	const auto muteWav = L"mute.wav";
	const auto unmuteWav = L"unmute.wav";
	const auto feedbackWav = wasMuted ? unmuteWav : muteWav;

	if (!wasMuted) {
		//if (PTT_RELEASE_DELAY_MS > 0 && IsButtonUp) {
		//	// Push to talk released. Everyone releases the button once they've started the last syllable,
		//	// but that's not understandable if you actually cut the microphone there. Wait a little longer.
		//	muteTimer = SetTimer(
		//		nullptr,
		//		0,
		//		PTT_RELEASE_DELAY_MS,
		//		[](HWND hWnd, UINT, UINT_PTR idTimer, DWORD) {
		//			muteTimer = 0;
		//			KillTimer(hWnd, idTimer);
		//			micVolume->SetMute(TRUE, nullptr);
		//			PlaySound(MAKEINTRESOURCE(IDR_MUTE), hInst, SND_ASYNC | SND_RESOURCE);
		//		}
		//	);
		//	return;
		//}
		micVolume->SetMute(TRUE, nullptr);
	}
	else {
		micVolume->SetMute(FALSE, nullptr);
	}
	//PlaySound(feedbackWav, hInst, SND_ASYNC | SND_RESOURCE);
	PlaySound(feedbackWav, hInst, SND_SYNC | SND_FILENAME);
}

MuteBehavior MicControl::GetMuteState()
{
	if (FAILED(CoInitializeEx(NULL, COINIT_MULTITHREADED)))
	{
		TRACE(L"ERROR");
	}

	IMMDeviceEnumerator* de;
	CoCreateInstance(
		__uuidof(MMDeviceEnumerator), NULL,
		CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),
		(void**)&de
	);

	IMMDevice* micDevicePtr;
	de->GetDefaultAudioEndpoint(EDataFlow::eCapture, ERole::eCommunications, &micDevicePtr);

	micDevicePtr->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, nullptr, (void**)&micVolume);
	BOOL wasMuted;
	micVolume->GetMute(&wasMuted);
	return wasMuted == TRUE ? MuteBehavior::MUTE : MuteBehavior::UNMUTE;
}

MicControl::MicControl(HINSTANCE hInstance)
{
	hInst = hInstance;

	/*IMMDeviceEnumerator* de;
	CoCreateInstance(
		__uuidof(MMDeviceEnumerator), NULL,
		CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),
		(void**)&de
	);

	IMMDevice* micDevicePtr;
	de->GetDefaultAudioEndpoint(EDataFlow::eCapture, ERole::eCommunications, &micDevicePtr);

	micDevicePtr->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, nullptr, (void**)&micVolume);

	if (CoCreateInstance(
		__uuidof(MMDeviceEnumerator), NULL,
		CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),
		(void**)&de
	) != S_OK)
	{
		TRACE("ERROR initializing MMDeviceEnumerator");
	}*/
}

BOOL MicControl::PlayResource(LPCWSTR lpName)
{
	//BOOL bRtn;
	//LPSTR lpRes;
	//HRSRC hResInfo;
	//HMODULE hModule;

	//// Find the WAVE resource. 

	//hResInfo = FindResource(hModule, lpName, L"WAVE");
	//if (hResInfo == NULL)
	//	return FALSE;

	//// Load the WAVE resource. 

	//auto hRes = LoadResource(hModule, hResInfo);
	//if (hRes == NULL)
	//	return FALSE;

	//// Lock the WAVE resource and play it. 

	//LockResource(hRes);
	//if (lpRes != NULL) {
	//	bRtn = sndPlaySound(hRes, SND_MEMORY | SND_SYNC |
	//		SND_NODEFAULT);
	//	UnlockResource(hRes);
	//}
	//else
	//	bRtn = 0;

	//// Free the WAVE resource and return success or failure. 

	//FreeResource(hRes);
	//return bRtn;
	return FALSE;
}
