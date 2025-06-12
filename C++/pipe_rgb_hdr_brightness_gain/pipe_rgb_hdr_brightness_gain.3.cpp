#include <iostream>
#include <cstdio>
#include <cstdlib>

int main() {
    // 開啟ffmpeg輸入管道
    FILE* inputPipe = popen("ffmpeg -hide_banner -i D:\\Movies\\Blade.Runner.1982.The.Final.Cut.2160p.BluRay.x265.10bit.HDR.TrueHD.7.1.Atmos-DEPTH\\Sample\\blade.runner.1982.the.final.cut.2160p.uhd.bluray.x265-depth.sample.mkv -map 0:0 -f rawvideo -pix_fmt rgb48le -", "r");
    if (!inputPipe) {
        std::cerr << "Failed to open input pipe" << std::endl;
        return 1;
    }

    // 開啟ffmpeg輸出管道
    FILE* outputPipe = popen("ffmpeg -hide_banner -f rawvideo -pix_fmt rgb48 -s:v 3840x1600 -r 24000/1001 -i - -pix_fmt yuv420p10le -c:v libx265 -preset 0 -deblock 6:6 -x265-params \"crf=21:cbqpoffs=-10:crqpoffs=-10:qcomp=0.5:aq-mode=0:psy-rd=0:psy-rdoq=0\" -color_primaries bt2020 -color_trc smpte2084 -colorspace bt2020nc D:\\Movies\\Blade.Runner.1982.The.Final.Cut.2160p.BluRay.x265.10bit.HDR.TrueHD.7.1.Atmos-DEPTH\\Sample\\blade.runner.1982.the.final.cut.2160p.uhd.bluray.x265-depth.sample.2.mkv", "w");
    if (!outputPipe) {
        std::cerr << "Failed to open output pipe" << std::endl;
        pclose(inputPipe);
        return 1;
    }

    // 讀取和寫入數據
    const size_t bufferSize = 3840 * 1600 * 6; // 每個像素6個字節（rgb48）
    char buffer[bufferSize];
    while (true) {
        size_t bytesRead = fread(buffer, 1, bufferSize, inputPipe);
        if (bytesRead == 0) {
            if (feof(inputPipe)) {
                std::cout << "End of input stream" << std::endl;
                break;
            }
            if (ferror(inputPipe)) {
                std::cerr << "Error reading from input pipe" << std::endl;
                break;
            }
        }
        size_t bytesWritten = fwrite(buffer, 1, bytesRead, outputPipe);
        if (bytesWritten != bytesRead) {
            std::cerr << "Error writing to output pipe" << std::endl;
            break;
        }
    }

    // 關閉管道
    pclose(inputPipe);
    pclose(outputPipe);

    return 0;
}
