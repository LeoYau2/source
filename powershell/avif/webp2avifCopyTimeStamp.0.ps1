#$OutputEncoding = [console]::InputEncoding = [console]::OutputEncoding = [Text.UTF8Encoding]::UTF8
#$OutputEncoding = New-Object -typename System.Text.UTF8Encoding

$input_container = 'webp'

Get-ChildItem -Filter *.$input_container

$countAll = 0
$count = 0
foreach($file in Get-ChildItem -Filter *.$input_container)
{
    $countAll++
}
echo "$countAll $input_container files"

$change_size_multiplier = Read-Host -Prompt 'size_multiplier(default:1)'
if($change_multiplier -eq '')
{
    $size_multiplier = ''
}else
{
    $size_multiplier = $change_size_multiplier
}

$change_yuv = Read-Host -Prompt 'yuv(default:420)'
if($change_yuv -eq '')
{
    $yuv = '420'
}else
{
    $yuv = $change_yuv
}

$change_qp = Read-Host -Prompt 'qp(default:20)'
if($change_qp -eq '')
{
    $qp = '20'
}else
{
    $qp = $change_qp
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

$dir = "avifyuv$yuv"+"qp$qp"
if($size_multiplier -eq '')
{
    $dir = "avifyuv$yuv"+"qp$qp"
}else
{
    $dir = "($size_multiplier"+"x)avifyuv$yuv"+"qp$qp"
}

echo "$input_container to avif"
echo "outdir $dir"
echo "yuv$yuv"
echo "qp $qp"
echo "speed $speed"
echo "depth $depth"
echo "range $range"
echo "alpha qp $alphaQ"
echo "jobs $jobs"
pause

mkdir $dir
foreach($file in Get-ChildItem -Filter *.$input_container)
{
    $na = [io.path]::GetFileNameWithoutExtension("$file")
        if($size_multiplier -eq '1')
    {
        ffmpeg -hide_banner -i "$file" "$na.png"
    }else
    {
        ffmpeg -hide_banner -i "$file" -vf "format=rgba,scale=iw*$size_multiplyer:ih*$size_multiplyer" -sws_flags bicubic "$na.png"
    }
    C:\Users\Leo\avifenc -j $jobs -y $yuv -d $depth -r $range --max $qp --min $qp --maxalpha $alphaQ --minalpha $alphaQ -s $speed "$na.png" -o "$dir\$na.qp$qp.avif"
    Remove-Item "$na.png"
    (Get-Item "$dir\$na.qp$qp.avif").CreationTime=((Get-Item $file).CreationTime)
    (Get-Item "$dir\$na.qp$qp.avif").LastWriteTime=((Get-Item $file).LastWriteTime)
    (Get-Item "$dir\$na.qp$qp.avif").LastAccessTime=((Get-Item $file).LastAccessTime)
    
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
