
#$OutputEncoding = [console]::InputEncoding = [console]::OutputEncoding = [Text.UTF8Encoding]::UTF8
#$OutputEncoding = New-Object -typename System.Text.UTF8Encoding
Get-ChildItem -Filter *.bmp

pause
mkdir png
foreach($file in Get-ChildItem -Filter *.bmp)
{
    $na = [io.path]::GetFileNameWithoutExtension("$file")
    ffmpeg -hide_banner -i "$na.bmp" -c:v png "png\$na.png"
    (Get-Item "png/$na.png").CreationTime=((Get-Item "$na.bmp").CreationTime)
    (Get-Item "png/$na.png").LastWriteTime=((Get-Item "$na.bmp").LastWriteTime)
    (Get-Item "png/$na.png").LastAccessTime=((Get-Item "$na.bmp").LastAccessTime)

    echo $na
    echo $file.FullName
}
(New-Object Media.SoundPlayer "C:\Windows\Media\notify.wav").Play();
pause
