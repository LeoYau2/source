#$OutputEncoding = [console]::InputEncoding = [console]::OutputEncoding = [Text.UTF8Encoding]::UTF8
#$OutputEncoding = New-Object -typename System.Text.UTF8Encoding

$input_container = 'png'

Get-ChildItem -Filter *.$input_container

$countAll = 0
$count = 0
foreach($file in Get-ChildItem -Filter *.$input_container)
{
    $countAll++
}
echo "$countAll $input_container files"

$change_size_multiplier = Read-Host -Prompt 'size_multiplier(default:.5)'
if($change_multiplier -eq '')
{
    $size_multiplier = '.5'
}else
{
    $size_multiplier = $change_size_multiplier
}

$change_qp = Read-Host -Prompt 'qp(default:20)'
if($change_qp -eq '')
{
    $qp = '20'
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

$dir = "(0$size_multiplier"+"x)avifyuv$yuv"+"qp$qp"

echo "$input_container to 0$size_multiplier"+"x avif"
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
    ffmpeg -hide_banner -i "$file" -vf "format=rgba,scale=iw*$size_multiplier2:ih*$size_multiplier" -sws_flags bicubic "$dir\$na.png"
    C:\Users\Leo\avifenc -j $jobs -y $yuv -d $depth -r $range --speed $speed --max $qp --min $qp --maxalpha $alphaQ --minalpha $alphaQ -s $speed "$dir\$na.png" -o "$dir\$na.qp$qp.avif"
    (Get-Item "$dir\$na.qp$qp.avif").CreationTime=((Get-Item $file).CreationTime)
    (Get-Item "$dir\$na.qp$qp.avif").LastWriteTime=((Get-Item $file).LastWriteTime)
    (Get-Item "$dir\$na.qp$qp.avif").LastAccessTime=((Get-Item $file).LastAccessTime)
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
