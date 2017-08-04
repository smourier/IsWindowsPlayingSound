#include "stdafx.h"

#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)
#define __WFILE__ WIDEN(__FILE__)
#define HRCHECK(__expr) {hr=(__expr);if(FAILED(hr)){wprintf(L"FAILURE 0x%08X (%i)\n\tline: %u file: '%s'\n\texpr: '" WIDEN(#__expr) L"'\n",hr, hr, __LINE__,__WFILE__);goto cleanup;}}
#define RELEASE(__p) {if(__p!=nullptr){__p->Release();__p=nullptr;}}

int main(int argc, char *argv[])
{
	BOOL playing = FALSE;
	BOOL loopmode = FALSE;
	float epsilon = 1E-07;
	float value = 0;
	HRESULT hr = S_OK;
	IMMDeviceEnumerator* pEnumerator = NULL;
	IMMDevice *pDevice = NULL;
	IAudioMeterInformation *pMeter = NULL;

	// Parse optional args
	// "loop" -> sets a loop mode for easy testing
	// <float value> -> changes epsilon
	for (int i = 1; i < argc; i++)
	{
		if (!strcmp(argv[i], "loop"))
		{
			loopmode = TRUE;
			continue;
		}

		float eps = atof(argv[i]);
		if (eps != 0.0)
		{
			epsilon = eps;
			continue;
		}
	}

	CoInitialize(NULL);
	HRCHECK(CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pEnumerator));
	HRCHECK(pEnumerator->GetDefaultAudioEndpoint(EDataFlow::eRender, ERole::eMultimedia, &pDevice));
	HRCHECK(pDevice->Activate(__uuidof(IAudioMeterInformation), CLSCTX_ALL, NULL, (void**)&pMeter));
	do
	{
		HRCHECK(pMeter->GetPeakValue(&value));
		playing = value > epsilon;
		if (!loopmode)
			break;

		printf("%.10f playing:%i\n", value, playing);
		Sleep(100);
	} while (TRUE);

cleanup:
	RELEASE(pMeter);
	RELEASE(pDevice);
	RELEASE(pEnumerator);
	CoUninitialize();
	if (FAILED(hr))
	{
		printf("An error occurred: 0x%08X\n", hr);
		return hr;
	}

	if (playing)
	{
		printf("Windows is playing a sound.\n");
	}
	else
	{
		printf("Windows is not playing a sound.\n");
	}
	return playing;
}

