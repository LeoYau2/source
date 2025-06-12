@echo off

set "inputPath=X:\Blade.Runner.2049\copy_3"
set "donePath=X:\Blade.Runner.2049\copy_done_3"
set "rgbPath=P:\rgb\input"
set "gainedrgbPath=P:\rgb\gained"
set "outputPath=X:\output"
set "width=3840"
set "height=2160"
set "gain=10"
set "framerate=24000/1001"
set "nvenc_qp=10"

mkdir "%rgbPath%"
mkdir "%gainedrgbPath%"

:loop

for %%a in ("%inputPath%\*.mkv") do (
	ffmpeg -hide_banner -hwaccel nvdec -y -i "%inputPath%\%%~na.mkv" -pix_fmt rgb48 -r %framerate% -f rawvideo "%rgbPath%\%%~na.mp4"
	rgb_hdr_brightness_gain.exe %rgbPath%\%%~na.mp4 %gainedrgbPath%\%%~na.mp4 %width% %height% %gain%
	del "%rgbPath%\%%~na.mp4"
	ffmpeg -hide_banner -y -f rawvideo -s:v %width%x%height% -r %framerate% -pix_fmt rgb48 -i "%gainedrgbPath%\%%~na.mp4" -c:v hevc_nvenc -qp %nvenc_qp% -pix_fmt yuv420p10le -r %framerate% -color_primaries bt2020 -color_trc smpte2084 -colorspace bt2020nc %outputPath%\%%~na.mkv
	del "%gainedrgbPath%\%%~na.mp4"
	move "%inputPath%\%%~na.mkv" "%donePath%\%%~na.mkv"
	goto loop
)
