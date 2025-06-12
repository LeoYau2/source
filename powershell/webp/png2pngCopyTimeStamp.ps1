$OutputEncoding = [console]::InputEncoding = [console]::OutputEncoding = [Text.UTF8Encoding]::UTF8
$OutputEncoding = New-Object -typename System.Text.UTF8Encoding
Get-ChildItem -Filter *.png
Foreach-Object {
    echo www
}
pause
mkdir png
foreach($file in Get-ChildItem -Filter *.png)
{
    $na = [io.path]::GetFileNameWithoutExtension("$file")
    ffmpeg -hide_banner -i "$na.png" -c:v png "png\$na.png"
    (Get-Item "png/$na.png").CreationTime=((Get-Item "$na.png").CreationTime)
    (Get-Item "png/$na.png").LastWriteTime=((Get-Item "$na.png").LastWriteTime)
    (Get-Item "png/$na.png").LastAccessTime=((Get-Item "$na.png").LastAccessTime)

    echo $na
    echo $file.FullName
}
(New-Object Media.SoundPlayer "C:\Windows\Media\notify.wav").Play();
pause
