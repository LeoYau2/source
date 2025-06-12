setlocal EnableDelayedExpansion

rem set /p input_path=input path:
rem set /p output_path=output path:
set "gain=2"
rem set /p gain=brightness gain:
set "input_path=D:\Movies\Blade.Runner.1982.The.Final.Cut.2160p.BluRay.x265.10bit.HDR.TrueHD.7.1.Atmos-DEPTH\Sample\blade.runner.1982.the.final.cut.2160p.uhd.bluray.x265-depth.sample.mkv"
set "output_path=D:\Movies\Blade.Runner.1982.The.Final.Cut.2160p.BluRay.x265.10bit.HDR.TrueHD.7.1.Atmos-DEPTH\Sample\blade.runner.1982.the.final.cut.2160p.uhd.bluray.x265-depth.sample.2.mkv"
set "width=3840"
set "height=1600"
set "framerate=24000/1001"
rem echo %input_path%
pause

rem ffmpeg -hide_banner -i %input_path% -map 0:0 -f rawvideo -pix_fmt rgb48 - | C:\Users\Leo\Sync\source\C++\pipe_rgb_hdr_brightness_gain\pipe_rgb_hdr_brightness_gain.3.exe | ffmpeg -hide_banner -f rawvideo -pix_fmt rgb48 -s:v %width%x%height% -r %framerate% -i - -pix_fmt yuv420p10le -c:v libx265 -preset 0 -deblock 6:6 -x265-params "crf=21:cbqpoffs=-10:crqpoffs=-10:qcomp=0.5:aq-mode=0:psy-rd=0:psy-rdoq=0" -color_primaries bt2020 -color_trc smpte2084 -colorspace bt2020nc %output_path%

ffmpeg -hide_banner -i %input_path% -map 0:0 -f rawvideo -pix_fmt rgb48 - | C:\Users\Leo\Sync\source\C++\pipe_rgb_hdr_brightness_gain\pipe_rgb_hdr_brightness_gain.4.exe %width% %height% %gain% | ffmpeg -hide_banner -f rawvideo -pix_fmt rgb48 -s:v %width%x%height% -r %framerate% -i - -pix_fmt yuv420p10le -c:v libx265 -preset 0 -deblock 6:6 -x265-params "crf=21:cbqpoffs=-10:crqpoffs=-10:qcomp=0.5:aq-mode=0:psy-rd=0:psy-rdoq=0" -color_primaries bt2020 -color_trc smpte2084 -colorspace bt2020nc %output_path%

rem ffmpeg -hide_banner -i %input_path% -map 0:0 -f rawvideo -pix_fmt rgb48le - | C:\Users\Leo\Sync\source\C++\pipe_rgb_hdr_brightness_gain\pipe_rgb_hdr_brightness_gain.2.exe %width% %height%

rem ffmpeg -hide_banner -i %input_path% -map 0:0 -f rawvideo -pix_fmt rgb48le - | C:\Users\Leo\Sync\source\C++\pipe_rgb_hdr_brightness_gain\pipe_rgb_hdr_brightness_gain.2.exe %width% %height%

pause