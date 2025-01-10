setlocal EnableDelayedExpansion
chcp 65001

mkdir jpg

for %%a in ("*.webp") do (
C:\users\%USERNAME%\ffmpeg -i "%%a" -qscale:v 1 -qmin 1 "jpg\%%~na.jpg")

pause