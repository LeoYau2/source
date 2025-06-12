@echo off

:loop
set "folderPath=X:\yuv\input"

:: 利用 dir 命令列出檔案列表，使用 find /c 計算行數（即檔案數）
for /f %%i in ('dir /a-d /b "%folderPath%" ^| find /c /v ""') do set "fileCount=%%i"

:: 檢查檔案數是否少於兩個
if %fileCount% lss 2 (
    echo Found %fileCount% files.
    for %%a in ("X:\Blade.Runner.2049\copy\*.mkv") do (
	move "X:\Blade.Runner.2049\copy\%%~na.mkv" "X:\Blade.Runner.2049\copy_done\%%~na.mkv"
	ffmpeg -hide_banner -hwaccel nvdec -i "X:\Blade.Runner.2049\copy_done\%%~na.mkv" -pix_fmt yuv420p10le "X:\yuv\input\%%~na.yuv"
	goto loop
)
    
) else (
    echo Found %fileCount% files. Waiting...
    timeout /nobreak /t 1 >nul
)

goto loop
