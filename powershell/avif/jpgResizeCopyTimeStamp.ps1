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

#$width = Read-Host -Prompt 'input width'
#$height = Read-Host -Prompt 'input height'
$change_target_width = Read-Host -Prompt 'target width(default:1440)'
if($change_target_width -eq '')
{
    $target_width = '1440'
}else
{
    $target_width = $change_target_width
}
#$height = [Math]::Round([double]$target_width*[double]$height/[double]$width)
#echo "height = $height"

$depth = 8
$range = "full"
$alphaQ = 0
$jobs = "all"

$dir = "$target_width"

echo "$input_container to $target_width*$height"

pause

mkdir $dir
foreach($file in Get-ChildItem -Filter *.$input_container)
{
    $na = [io.path]::GetFileNameWithoutExtension("$file")
    $arguments = "-hide_banner -i `"$file`" -vf `"format=rgba,scale=${target_width}:-1`" -sws_flags area `"$dir\$na.${target_width}.png`""
    echo $arguments
    Start-Process -FilePath "ffmpeg.exe" -ArgumentList $arguments -NoNewWindow
    echo processing $file.FullName
    $arguments = "-j $jobs -y $yuv -d $depth -r $range --speed $speed --max $qp --min $qp --maxalpha $alphaQ --minalpha $alphaQ -s $speed `"$dir\$na.png`" -o `"$dir\$na.(0.5x)qp$qp.avif`""
echo $arguments
    
    #Remove-Item "$dir\$na.png"

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
