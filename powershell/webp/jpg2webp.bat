setlocal EnableDelayedExpansion
chcp 65001

mkdir webp

for %%a in ("*.jpg") do (
C:\users\%USERNAME%\ffmpeg -i "%%a" -pix_fmt yuv420p -c:v libwebp -preset 0 -quality 70 "webp\%%~na.webp")

pause