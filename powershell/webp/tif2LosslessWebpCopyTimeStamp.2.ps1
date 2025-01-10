#$OutputEncoding = [console]::InputEncoding = [console]::OutputEncoding = [Text.UTF8Encoding]::UTF8
#$OutputEncoding = New-Object -typename System.Text.UTF8Encoding
Get-ChildItem -Filter *.tif

Foreach-Object {

}
$countAll = 0
$count = 0
foreach($file in Get-ChildItem -Filter *.tif)
{
    $countAll++
}
echo "$countAll tif files"
pause

mkdir webp
foreach($file in Get-ChildItem -Filter *.tif)
{
    $na = [io.path]::GetFileNameWithoutExtension("$file")
    ffmpeg -hide_banner -i "$na.tif" -c:v libwebp -lossless 1 "webp\$na.webp"
    (Get-Item "webp/$na.webp").CreationTime=((Get-Item $file).CreationTime)
    (Get-Item "webp/$na.webp").LastWriteTime=((Get-Item $file).LastWriteTime)
    (Get-Item "webp/$na.webp").LastAccessTime=((Get-Item $file).LastAccessTime)

    $count++
    echo -------------------------------------------------------------
    echo $na
    echo $file.FullName
    $progress = [math]::round($count / $countAll * 100,3)
    echo "$count/$countAll progress $progress%"
    echo -------------------------------------------------------------
}
(New-Object Media.SoundPlayer "C:\Windows\Media\notify.wav").Play();
pause
