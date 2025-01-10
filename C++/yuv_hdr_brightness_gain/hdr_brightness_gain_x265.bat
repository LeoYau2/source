@echo off

set "inputPath=X:\Blade.Runner.2049\copy"
set "donePath=X:\Blade.Runner.2049\copy_done"
set "yuvPath=P:\yuv\input"
set "gainedyuvPath=P:\yuv\gained"
set "outputPath=X:\output"
set "width=3840"
set "height=2160"
set "gain=10"
set "framerate=24000/1001"
set "x265_crf=12"
set "x265_preset=5"

:loop

for %%a in ("%inputPath%\*.mkv") do (
	ffmpeg -hide_banner -y -hwaccel nvdec -i "%inputPath%\%%~na.mkv" -pix_fmt yuv420p10le "%yuvPath%\%%~na.yuv"
	yuv_hdr_brightness_gain.exe %yuvPath%\%%~na.yuv %gainedyuvPath%\%%~na.yuv %width% %height% %gain%
	rem copy "%yuvPath%\%%~na.yuv" "%gainedyuvPath%\%%~na.yuv"
	del "%yuvPath%\%%~na.yuv"
	ffmpeg -hide_banner -y -f rawvideo -s:v %width%x%height% -r %framerate% -pix_fmt yuv420p10le -i "%gainedyuvPath%\%%~na.yuv" -c:v libx265 -x265-params "preset=%x265_preset%:crf=%x265_crf%:cbqpoffs=-10:crqpoffs=-10:vbv-bufsize=40000:aq-mode=3:psy-rd=1.5:psy-rdoq=0.7" -pix_fmt yuv420p10le -color_primaries bt2020 -color_trc smpte2084 -colorspace bt2020nc %outputPath%\%%~na.mkv
	del "%gainedyuvPath%\%%~na.yuv"
	move "%inputPath%\%%~na.mkv" "%donePath%\%%~na.mkv"
	goto loop
)
