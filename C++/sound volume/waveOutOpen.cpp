#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
int main(int argc, char* argv[])
{
HWAVEOUT hWaveOut; /* device handle */
WAVEFORMATEX wfx; /* look this up in your documentation */
MMRESULT result;/* for waveOut return values */
/*
* first we need to set up the WAVEFORMATEX structure.
* the structure describes the format of the audio.
*/
wfx.nSamplesPerSec = 44100; /* sample rate */
wfx.wBitsPerSample = 16; /* sample size */
wfx.nChannels = 2; /* channels*/
/*
* WAVEFORMATEX also has other fields which need filling.
* as long as the three fields above are filled this should
* work for any PCM (pulse code modulation) format.
*/
wfx.cbSize = 0; /* size of _extra_ info */
wfx.wFormatTag = WAVE_FORMAT_PCM;
wfx.nBlockAlign = (wfx.wBitsPerSample >> 3) * wfx.nChannels;
wfx.nAvgBytesPerSec = wfx.nBlockAlign * wfx.nSamplesPerSec;
/*
* try to open the default wave device. WAVE_MAPPER is
* a constant defined in mmsystem.h, it always points to the
* default wave device on the system (some people have 2 or
* more sound cards).
*/
if(waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL) != MMSYSERR_NOERROR) {
fprintf(stderr, "unable to open WAVE_MAPPER device\n");
ExitProcess(1);
}
/*
* device is now open so print the success message
* and then close the device again.
*/
printf("The Wave Mapper device was opened successfully!\n");
waveOutClose(hWaveOut);
system("pause");
return 0;
}
