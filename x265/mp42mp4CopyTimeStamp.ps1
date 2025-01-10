#$OutputEncoding = [console]::InputEncoding = [console]::OutputEncoding = [Text.UTF8Encoding]::UTF8
#$OutputEncoding = New-Object -typename System.Text.UTF8Encoding
Get-ChildItem -Filter *.mp4

Foreach-Object {
    
}

$countAll = 0
$count = 0
foreach($file in Get-ChildItem -Filter *.mp4)
{
    $countAll++
}
echo "$countAll mp4 files"
pause

$container = "mp4"
$pixelFormat = "yuv420p"
$preset = "6"
$qp = Read-Host -Prompt 'qp'
$ffmpeg_params="-t 1"

$dir = "$container"+"qp$qp"

echo "mp4 to $container"
echo "outdir $dir"
echo "pixel format $pixelFormat"
echo "preset $preset"
echo "qp $qp"
pause

mkdir $dir
foreach($file in Get-ChildItem -Filter *.mp4)
{
    $na = [io.path]::GetFileNameWithoutExtension("$file")
    #ffmpeg -i "$file" -pix_fmt $pixelForfmt -c:v libx265 -preset $preset -qp $qp -c:a copy "$dir\$na.qp$qp.$container"
    (Get-Item "$dir\$na.qp$qp.$container").CreationTime=((Get-Item $file).CreationTime)
    (Get-Item "$dir\$na.qp$qp.$container").LastWriteTime=((Get-Item $file).LastWriteTime)
    (Get-Item "$dir\$na.qp$qp.$container").LastAccessTime=((Get-Item $file).LastAccessTime)

    $count++
    echo $na
    echo $file.FullName
    $progress = [math]::round($count / $countAll * 100,3)
    echo "$count/$countAll progress $progress%"
    echo -------------------------------------------------------------
}
(New-Object Media.SoundPlayer "C:\Windows\Media\notify.wav").Play();
pause
