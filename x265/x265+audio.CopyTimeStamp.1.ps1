#$ffmpeg_bat = "ffmpeg.bat"
$change_input_container = Read-Host -Prompt 'input container(default:mkv)'
if($change_input_container -eq "")
{
    $input_container = "mkv"
}else
{
    $input_container = $change_input_container
}
echo $input_container
$change_output_container = Read-Host -Prompt 'output container(default:mkv)'
if($change_output_container -eq "")
{
    $output_container = "mkv"
}else
{
    $output_container = $change_output_container
}
echo $output_container
$ffmpeg_params = "-t 0.1"
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
#$pixelFormat = "yuv420p"
$change_preset = Read-Host -Prompt 'preset(default:5)'
if($change_preset -eq "")
{
    $preset = "5"
}else
{
    $preset = $change_preset
}
echo $preset
$change_qp = Read-Host -Prompt 'qp(default:30)'
if($change_qp -eq "")
{
    $qp = "30"
}else
{
    $qp = $change_qp
}
echo $qp

#$ffmpeg_params = "-t 0.1"

$dir = "$output_container"+"qp$qp"
$countAll = 0
$count = 0

Get-ChildItem -Filter *.$input_container

foreach($file in Get-ChildItem -Filter *.$input_container)
{
    $countAll++
}
echo "$countAll $input_container files"
pause

#echo "$input_container to $output_container"
#echo "outdir $dir"
#echo "pixel format $pixelFormat"
#echo "preset $preset"
#echo "qp $qp"
#pause

mkdir $dir
foreach($file in Get-ChildItem -Filter *.$input_container)
{
    $na = [io.path]::GetFileNameWithoutExtension("$file")
    #echo $ffmpeg_params
    echo "ffmpeg -hide_banner -i $file -c:v libx265 -preset $preset -qp $qp -c:a $audio_format -b:a $audio_bitrate $dir\$na.qp$qp.$output_container"
    #start-process "ffmpeg -hide_banner -i $file -c:v libx265 -t 0.1 -qp $qp -c:a $audio_format -b:a $audio_bitrate $dir\$na.qp$qp.$output_container"
    ffmpeg -hide_banner -i $file -c:v libx265 -preset $preset -qp $qp -c:a $audio_format -b:a $audio_bitrate $dir\$na.qp$qp.$output_container
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
