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

$change_qp = Read-Host -Prompt 'qp(default:30)'
if($change_qp -eq "")
{
    $qp = '30'
}else
{
    $qp = $change_qp
}

$change_yuv = Read-Host -Prompt 'yuv(default:444)'
if($change_yuv -eq "")
{
    $yuv = '444'
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

$dir = "avifyuv$yuv"+"qp$qp"

echo "$input_container to avif"
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
$startTime = Get-Date
foreach($file in Get-ChildItem -Filter *.$input_container)
{
    $na = [io.path]::GetFileNameWithoutExtension("$file")
    #C:\Users\Leo\avifenc -j $jobs -y $yuv -d $depth -r $range --speed $speed --max $qp --min $qp --maxalpha $alphaQ --minalpha $alphaQ -s $speed "$file" -o "$dir\$na.qp$qp.avif"
    echo "processing $($file.FullName)"
    $arguments = "-j $jobs -y $yuv -d $depth -r $range --speed $speed --max $qp --min $qp --maxalpha $alphaQ --minalpha $alphaQ -s $speed `"$file`" -o `"$dir\$na.qp$qp.avif`""
    $scriptBlock = {
        $process = Start-Process -FilePath "C:\Users\Leo\avifenc.exe" -ArgumentList $arguments -PassThru -WindowStyle Hidden
        $process.PriorityClass = "Idle"
        Wait-Process -InputObject $process
    }
    $executionTime = Measure-Command -Expression $scriptBlock
    (Get-Item "$dir\$na.qp$qp.avif").CreationTime=((Get-Item $file).CreationTime)
    (Get-Item "$dir\$na.qp$qp.avif").LastWriteTime=((Get-Item $file).LastWriteTime)
    (Get-Item "$dir\$na.qp$qp.avif").LastAccessTime=((Get-Item $file).LastAccessTime)

    $count++
    $currentTime = Get-Date
    $timeDifference = New-TimeSpan -Start $startTime -End $currentTime
    $ETASeconds = $timeDifference.TotalSeconds / $count* ($countAll - $count)
    $ETA = New-TimeSpan -Seconds ([math]::Round($ETASeconds))
    #echo --------------------------------------------------------------------------
    echo $na
    Write-Host ("time: {0:F3} s" -f $executionTime.TotalSeconds)
    #echo $file.FullName
    $progress = [math]::round($count / $countAll * 100,3)
    echo "progress:$count/$countAll, $progress%"
    echo "elapsed: $($timeDifference.ToString("hh\:mm\:ss")), ETA: $($ETA.ToString("hh\:mm\:ss"))"
    echo ----------------------------------------------------------------------------
}
(New-Object Media.SoundPlayer "C:\Windows\Media\notify.wav").Play();
pause
