$OutputEncoding = [console]::InputEncoding = [console]::OutputEncoding = [Text.UTF8Encoding]::UTF8
$OutputEncoding = New-Object -typename System.Text.UTF8Encoding
Get-ChildItem -Filter *.webp
Foreach-Object {
    echo www
}
pause
mkdir png
foreach($file in Get-ChildItem -Filter *.webp)
{
    $na = [io.path]::GetFileNameWithoutExtension("$file")
    ffmpeg -hide_banner -i "$na.webp" -c:v png "png\$na.png"
    (Get-Item "png/$na.png").CreationTime=((Get-Item "$na.webp").CreationTime)
    (Get-Item "png/$na.png").LastWriteTime=((Get-Item "$na.webp").LastWriteTime)
    (Get-Item "png/$na.png").LastAccessTime=((Get-Item "$na.webp").LastAccessTime)

    echo $na
    echo $file.FullName
}
(New-Object Media.SoundPlayer "C:\Windows\Media\notify.wav").Play();
pause
