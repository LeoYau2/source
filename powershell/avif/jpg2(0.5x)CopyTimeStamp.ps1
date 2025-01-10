#$OutputEncoding = [console]::InputEncoding = [console]::OutputEncoding = [Text.UTF8Encoding]::UTF8
#$OutputEncoding = New-Object -typename System.Text.UTF8Encoding

$input_container = 'jpg'

Get-ChildItem -Filter *.$input_container

$countAll = 0
$count = 0
foreach($file in Get-ChildItem -Filter *.$input_container)
{
    $countAll++
}
echo "$countAll $input_container files"

$change_qp = Read-Host -Prompt 'qp(default:30)'
if($change_qp -eq '')
{
    $qp = '30'
}else
{
    $qp = $change_qp
}
$change_yuv = Read-Host -Prompt 'yuv(default:420)'
if($change_yuv -eq '')
{
    $yuv = '420'
}else
{
    $yuv = $change_yuv
}

$change_speed = Read-Host -Prompt 'speed(default:1)'
if($change_speed -eq '')
{
    $speed = '1'
}else
{
    $speed = $change_speed
}

$depth = 8
$range = "full"
$alphaQ = 0
$jobs = "all"

$dir = "(0.5x)avifyuv$yuv"+"qp$qp"

echo "$input_container to 0.5x avif"
echo "outdir $dir"
echo "yuv$yuv"
echo "qp $qp"
echo "depth $depth"
echo "speed $speed"
echo "alpha qp $alphaQ"
echo "range $range"
echo "jobs $jobs"
pause

mkdir $dir
foreach($file in Get-ChildItem -Filter *.$input_container)
{
    $na = [io.path]::GetFileNameWithoutExtension("$file")
    $arguments = "-hide_banner -i `"$file`" -vf `"format=rgba,scale=iw/2:ih/2`" -sws_flags bicubic `"$dir\$na.png`""
    echo $arguments
    Start-Process -FilePath "ffmpeg.exe" -ArgumentList $arguments -NoNewWindow
    echo processing $file.FullName
    $arguments = "-j $jobs -y $yuv -d $depth -r $range --speed $speed --max $qp --min $qp --maxalpha $alphaQ --minalpha $alphaQ -s $speed `"$dir\$na.png`" -o `"$dir\$na.(0.5x)qp$qp.avif`""
echo $arguments
    
    Remove-Item "$dir\$na.png"

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
