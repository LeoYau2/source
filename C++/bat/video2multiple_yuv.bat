@echo off
setlocal enabledelayedexpansion

set "input_video=C:\Users\Leo\Sync\source\python\input_540p.mkv"
set "output_directory=X:\yuv"
set "frame_limit=16"
set "output_counter=1"

:LOOP
set "output_file=!output_directory!\!output_counter!.mkv"

set "fileCount=0"
for %%I in ("%output_directory%\*") do (
    set /a fileCount+=1
)

if %fileCount% equ 0 (
    REM Run ffmpeg command
    ffmpeg -i "!input_video!" -start_number 100 -frames:v %frame_limit% -vf "setpts=PTS-STARTPTS" "!output_file!"

    REM Increment output counter
    set /a output_counter+=1
    goto LOOP
) else (
    echo Output directory is not empty. Waiting for files to be cleared.
    sleep 1
    goto LOOP
)