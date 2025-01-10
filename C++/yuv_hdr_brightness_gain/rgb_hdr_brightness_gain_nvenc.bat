@echo off

set "inputPath=C:\Users\Leo\Downloads\1917.2019.2160p.UHD.BluRay.x265.10bit.HDR.DTS-HD.MA.TrueHD.7.1.Atmos-SWTYBLZ\qp4"
set "donePath=C:\Users\Leo\Downloads\1917.2019.2160p.UHD.BluRay.x265.10bit.HDR.DTS-HD.MA.TrueHD.7.1.Atmos-SWTYBLZ\copy_done"
set "rgbPath=P:\rgb\input"
set "gainedrgbPath=P:\rgb\gained"
set "outputPath=C:\Users\Leo\Downloads\1917.2019.2160p.UHD.BluRay.x265.10bit.HDR.DTS-HD.MA.TrueHD.7.1.Atmos-SWTYBLZ\output"
set "width=3840"
set "height=1606"
set "gain=5"
set "framerate=24000/1001"
set "nvenc_qp=6"

mkdir "%rgbPath%"
mkdir "%gainedrgbPath%"

:loop

for %%a in ("%inputPath%\*.mkv") do (
	ffmpeg -hide_banner -hwaccel nvdec -y -i "%inputPath%\%%~na.mkv" -pix_fmt rgb48 -r 24000/1001 -f rawvideo "%rgbPath%\%%~na.mp4"
	rgb_hdr_brightness_gain.exe %rgbPath%\%%~na.mp4 %gainedrgbPath%\%%~na.mp4 %width% %height% %gain%
	del "%rgbPath%\%%~na.mp4"
	ffmpeg -hide_banner -y -f rawvideo -s:v %width%x%height% -r 24000/1001 -pix_fmt rgb48 -i "%gainedrgbPath%\%%~na.mp4" -c:v hevc_nvenc -qp %nvenc_qp% -pix_fmt yuv420p10le -r 24000/1001 -color_primaries bt2020 -color_trc smpte2084 -colorspace bt2020nc %outputPath%\%%~na.mkv
	del "%gainedrgbPath%\%%~na.mp4"
	move "%inputPath%\%%~na.mkv" "%donePath%\%%~na.mkv"
	goto loop
)
