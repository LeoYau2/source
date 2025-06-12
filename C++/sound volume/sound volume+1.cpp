#include <windows.h>
#include <mmsystem.h>
#include <cstdio>

int main() {
  //...获取deviceId
    int deviceId = 3; //要控制的设备
    int deviceCount = waveOutGetNumDevs();
  // 打开设备
    HWAVEOUT hWaveOut;
    WAVEFORMATEX wfx;
    MMRESULT result;
    wfx.nSamplesPerSec = 96000;
    wfx.wBitsPerSample = 24;
    wfx.nChannels = 2;
    wfx.cbSize = 0; /* size of _extra_ info */
    wfx.wFormatTag = WAVE_FORMAT_PCM;
    wfx.nBlockAlign = (wfx.wBitsPerSample >> 3) * wfx.nChannels;
    wfx.nAvgBytesPerSec = wfx.nBlockAlign * wfx.nSamplesPerSec;
    // 设置其他格式信息
    //waveOutOpen(&hWaveOut, deviceId, &wfx, NULL, NULL, 0);
    waveOutOpen(&hWaveOut, deviceId, &wfx, 0, 0, CALLBACK_NULL);
  // 获取原始音量
    DWORD volume;
    waveOutGetVolume(hWaveOut, &volume);
    printf("%u\n",volume);
  // 音量增加
    if (volume < 0xFFFFFFFF) {
        volume += 0x00080008;
    }
    volume = 0x8FFF8FFF;
  // 设置新音量
    //waveOutSetVolume(hWaveOut, volume);
  // 关闭设备
    waveOutGetVolume(hWaveOut, &volume);
    waveOutClose(hWaveOut);
    printf("%u\n",volume);
    system("pause");
    return 0;
}
