#include <windows.h>
#include <mmsystem.h>
#include <cstdio>

int main() {
    int deviceId = -1; // 初始化为-1，用于存储设备 ID
    int deviceCount = waveOutGetNumDevs();

    // 列举所有设备并获取信息
    for (int i = 0; i < deviceCount; ++i) {
        WAVEOUTCAPS deviceInfo;
        MMRESULT result = waveOutGetDevCaps(i, &deviceInfo, sizeof(deviceInfo));

        if (result == MMSYSERR_NOERROR) {
            // 输出设备信息，包括设备名称、描述等
            printf("Device %d: %s - %s\n", i, deviceInfo.szPname, deviceInfo.szPname);
            // 在这里你可以根据设备信息决定你要操作的特定设备
            // 例如，设备名称包含特定字符串时，设定该设备为要控制的设备
            // if (strstr(deviceInfo.szPname, "your_device_name") != NULL) {
            //     deviceId = i;
            //     break;
            // }
        }
    }

    if (deviceId == -1) {
        printf("Device not found or not set\n");
        system("pause");
        return 1;
    }

    // 其他操作...
    system("pause");
    return 0;
}
