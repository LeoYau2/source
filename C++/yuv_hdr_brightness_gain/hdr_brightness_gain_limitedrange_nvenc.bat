@echo off

set "inputPath=X:\Blade.Runner.2049\copy"
set "donePath=X:\Blade.Runner.2049\copy_done"
set "yuvPath=P:\yuv\input"
set "gainedyuvPath=P:\yuv\gained"
set "outputPath=X:\output"
set "width=3840"
set "height=2160"
set "gain=12"
set "framerate=24000/1001"
set "nvenc_qp=10"

:loop

for %%a in ("%inputPath%\*.mkv") do (
	ffmpeg -hide_banner -y -hwaccel nvdec -i "%inputPath%\%%~na.mkv" -pix_fmt yuv420p10le -vf scale=out_range=limited "%yuvPath%\%%~na.yuv"
	yuv_hdr_brightness_gain.v3.exe %yuvPath%\%%~na.yuv %gainedyuvPath%\%%~na.yuv %width% %height% %gain%
	rem copy "%yuvPath%\%%~na.yuv" "%gainedyuvPath%\%%~na.yuv"
	del "%yuvPath%\%%~na.yuv"
	ffmpeg -hide_banner -y -f rawvideo -s:v %width%x%height% -r %framerate% -pix_fmt yuv420p10le -color_range 1 -i "%gainedyuvPath%\%%~na.yuv" -c:v hevc_nvenc -qp %nvenc_qp% -pix_fmt yuv420p10le -color_primaries bt2020 -color_trc smpte2084 -colorspace bt2020nc %outputPath%\%%~na.mkv
	del "%gainedyuvPath%\%%~na.yuv"
	move "%inputPath%\%%~na.mkv" "%donePath%\%%~na.mkv"
	goto loop
)
