setlocal EnableDelayedExpansion

set /p input_container=input container:
set /p output_container=output container:
set /p ffmpeg_params=ffmpeg params:
set /p preset=preset:
set /p qp=qp:
set /p opus_bitrate=opus bitrate:

mkdir %output_container%qp%qp%
for %%a in ("*.%input_container%") do (ffmpeg -hide_banner -hwaccel nvdec -i "%%a" -map 0:0 -map 0:1? %ffmpeg_params% -c:v libx265 -qp %qp% -preset %preset% -c:a libopus -b:a %opus_bitrate% "%output_container%qp%qp%/%%~na.qp%qp%.%output_container%"
)
pause