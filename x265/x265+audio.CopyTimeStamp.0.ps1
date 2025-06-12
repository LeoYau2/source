start-process "echo 1.bat"
$ffmpeg_bat = "ffmpeg.bat"
#$input_container = Read-Host -Prompt 'input container'
$input_container = "mp4"
#$output_container = Read-Host -Prompt 'output container'
$output_container = "mp4"
$ffmpeg_params = "-t 0.1"
#$audio_format = Read-Host -Prompt 'audio format'
$change_audio_format = Read-Host -Prompt 'audio format(default:libopus)'
if($change_audio_format -eq "")
{
    $audio_format = "libopus"
}else
{
    $audio_format = $change_audio_format
}
echo $audio_format
$change_audio_bitrate = Read-Host -Prompt 'audio bitrate(default:128k)'
if($change_audio_bitrate -eq "")
{
    $audio_bitrate = "128k"
}else
{
    $audio_bitrate = $change_audio_bitrate
}
echo $audio_bitrate

$pixelFormat = "yuv420p"
$preset = "6"
#$qp = Read-Host -Prompt 'qp'
$qp = 20
$ffmpeg_params = "-t 0.1"

$dir = "$output_container"+"qp$qp"

$countAll = 0
$count = 0
foreach($file in Get-ChildItem -Filter *.$input_container)
{
    $countAll++
}
echo "$countAll $input_container files"
pause



echo "mp4 to $container"
echo "outdir $dir"
echo "pixel format $pixelFormat"
echo "preset $preset"
echo "qp $qp"
pause

mkdir $dir
foreach($file in Get-ChildItem -Filter *.$input_container)
{
    $na = [io.path]::GetFileNameWithoutExtension("$file")
    echo $ffmpeg_params
    #New-Item -Path ffmpeg.bat
    #echo "1" > ffmpeg.bat
    #echo "2" > ffmpeg.bat
    #echo "3" >> ffmpeg.bat
    #"4" >> ffmpeg.bat
    $bat_cotent = echo "ffmpeg -hide_banner -i "$file" -c:v libx265 "$ffmpeg_params" -qp $qp -c:a $audio_format -b:a $audio_bitrate "$dir\$na.qp$qp.$output_container""
    echo "ffmpeg -hide_banner -i "$file" -c:v libx265 "$ffmpeg_params" -qp $qp -c:a $audio_format -b:a $audio_bitrate "$dir\$na.qp$qp.$output_container"" > ffmpeg.bat -encoding utf8
    [System.IO.File]::WriteAllLines($ffmpeg_bat,$bat_cotent)
    echo "ffmpeg -hide_banner -i "$file" -c:v libx265 "$ffmpeg_params" -qp $qp -c:a $audio_format -b:a $audio_bitrate "$dir\$na.qp$qp.$output_container""
    pause
    .\ffmpeg.bat
    #Remove-Item ffmpeg.bat
    pause
    
    #ffmpeg -hide_banner -i "$file" -c:v libx265 -qp $qp -c:a $audio_format -b:a $audio_bitrate "$dir\$na.qp$qp.$output_container"
    #ffmpeg -i "$file" -pix_fmt $pixelForfmt -c:v libx265 -preset $preset -qp $qp -c:a copy "$dir\$na.qp$qp.$container"
    (Get-Item "$dir\$na.qp$qp.$output_container").CreationTime=((Get-Item $file).CreationTime)
    (Get-Item "$dir\$na.qp$qp.$output_container").LastWriteTime=((Get-Item $file).LastWriteTime)
    (Get-Item "$dir\$na.qp$qp.$output_container").LastAccessTime=((Get-Item $file).LastAccessTime)

    $count++
    echo $na
    echo $file.FullName
    $progress = [math]::round($count / $countAll * 100,3)
    echo "$count/$countAll progress $progress%"
    echo -------------------------------------------------------------
}
(New-Object Media.SoundPlayer "C:\Windows\Media\notify.wav").Play();
pause
