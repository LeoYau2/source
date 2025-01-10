$change_audio_bitrate = Read-Host -Prompt 'audio bitrate(default:128k)'
if($change_audio_bitrate -eq "")
{
    $audio_bitrate = "128k"
}else
{
    $audio_bitrate = $change_audio_bitrate
}
echo $audio_bitrate

$dir = "opus"+"$audio_bitrate"
$countAll = 0
$count = 0

Get-ChildItem -Filter *.m4a

foreach($file in Get-ChildItem -Filter *.m4a)
{
    $countAll++
}
echo "$countAll m4a"
pause

#echo "$input_container to $output_container"
#echo "outdir $dir"
#echo "pixel format $pixelFormat"
#echo "preset $preset"
#echo "qp $qp"
#pause

mkdir $dir
foreach($file in Get-ChildItem -Filter *.m4a)
{
    $na = [io.path]::GetFileNameWithoutExtension("$file")
    ffmpeg -hide_banner -i $file -c:a libopus -b:a $audio_bitrate $dir\$na.$audio_bitrate.opus
    #ffmpeg -i "$file" -pix_fmt $pixelForfmt -c:v libx265 -preset $preset -qp $qp -c:a copy "$dir\$na.qp$qp.$container"
    (Get-Item "$dir\$na.$audio_bitrate.opus").CreationTime=((Get-Item $file).CreationTime)
    (Get-Item "$dir\$na.$audio_bitrate.opus").LastWriteTime=((Get-Item $file).LastWriteTime)
    (Get-Item "$dir\$na.$audio_bitrate.opus").LastAccessTime=((Get-Item $file).LastAccessTime)

    $count++
    echo $na
    echo $file.FullName
    $progress = [math]::round($count / $countAll * 100,3)
    echo "$count/$countAll progress $progress%"
    echo -------------------------------------------------------------
}
(New-Object Media.SoundPlayer "C:\Windows\Media\notify.wav").Play();
pause
