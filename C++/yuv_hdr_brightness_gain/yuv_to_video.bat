@echo off

:loop
set "folderPath=X:\yuv\gained"

:: 利用 dir 命令列出檔案列表，使用 find /c 計算行數（即檔案數）
for /f %%i in ('dir /a-d /b "%folderPath%" ^| find /c /v ""') do set "fileCount=%%i"

:: 檢查檔案數是否少於兩個
if %fileCount% eql 0 (
    echo No files. Waiting...
    timeout /nobreak /t 1 >nul
) else (
    for %%a in ("%folderPath%\*.yuv") do (ffmpeg -hide_banner -hwaccel nvdec -i "%%a" "X:\yuv\gained_mkv\%%~na.mkv")
	
)

goto loop
