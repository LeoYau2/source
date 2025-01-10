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

$change_maxConcurrency = Read-Host -Prompt 'maxConcurrency(default:4)'
if($change_maxConcurrency -eq "")
{
    $maxConcurrency = 4
}else
{
    $maxConcurrency = $change_maxConcurrency
}

$change_priority = Read-Host -Prompt 'priority:4,6,8,10,13,24(default:4)'
if($change_priority -eq "")
{
    $priority = 4
}else
{
    $priority = $change_priority
}

$priorityClass = switch ($priority) {
    4 { "Idle" }
    6 { "BelowNormal" }
    8 { "Normal" }
    10 { "AboveNormal" }
    13 { "High" }
    24 { "RealTime" }
    Default { throw "Invalid priority value" }
}
echo $priorityClass

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

if (-not (Test-Path $dir)) {
    New-Item -ItemType Directory -Path $dir | Out-Null
}

$files = Get-ChildItem -Filter *.$input_container
$countAll = $files.Count
$count = 0

$runspacePool = [runspacefactory]::CreateRunspacePool(1, $maxConcurrency)
$runspacePool.Open()

$startTime = Get-Date
$runspaces = @()
$processingFiles = @{}
foreach ($file in $files) {
    $na = [io.path]::GetFileNameWithoutExtension("$file")
    $arguments = "-j $jobs -y $yuv -d $depth -r $range --speed $speed --max $qp --min $qp --maxalpha $alphaQ --minalpha $alphaQ -s $speed `"$file`" -o `"$dir\$na.qp$qp.avif`""

    $scriptBlock = {
        param ($arguments, $file, $dir, $na, $qp ,$priorityClass)

        $process = Start-Process -FilePath "C:\Users\Leo\avifenc.exe" -ArgumentList $arguments -PassThru -WindowStyle Hidden
        $process.PriorityClass = $priorityClass
        $process.WaitForExit()

        (Get-Item "$dir\$na.qp$qp.avif").CreationTime = ((Get-Item $file).CreationTime)
        (Get-Item "$dir\$na.qp$qp.avif").LastWriteTime = ((Get-Item $file).LastWriteTime)
        (Get-Item "$dir\$na.qp$qp.avif").LastAccessTime = ((Get-Item $file).LastAccessTime)

        $executionTime = [timespan]::FromMilliseconds($process.ExitTime - $process.StartTime)
        $executionTime = [timespan]::FromMilliseconds($process.ExitTime.Subtract($process.StartTime).TotalMilliseconds)
        return @{
            ExecutionTime = $executionTime.ToString()
            FullPath = "$file"
        }
    }

    $powershell = [powershell]::Create().AddScript($scriptBlock).AddArgument($arguments).AddArgument($file.FullName).AddArgument($dir).AddArgument($na).AddArgument($qp).AddArgument($priorityClass)
    $powershell.RunspacePool = $runspacePool

    $runspaces += [pscustomobject]@{
        PowerShell = $powershell
        Status = $powershell.BeginInvoke()
        File = $file.Name
    }
    $processingFiles[$file.Name] = $true
    #Write-Host "Processing: $($processingFiles.Keys -join ', ')"
}

foreach ($runspace in $runspaces) {
    $result = $runspace.PowerShell.EndInvoke($runspace.Status)
    #$runspace.PowerShell.EndInvoke($runspace.Status)
    $runspace.PowerShell.Dispose()

    $count++
    $currentTime = Get-Date
    $timeDifference = New-TimeSpan -Start $startTime -End $currentTime
    $ETASeconds = $timeDifference.TotalSeconds / $count * ($countAll - $count)
    $ETA = New-TimeSpan -Seconds ([math]::Round($ETASeconds))

    $progress = [math]::round($count / $countAll * 100, 3)
    $processingFiles.Remove($runspace.File)
    Write-Host ("$($result.FullPath)")
    Write-Host ("Progress: $count/$countAll, $progress%")
    Write-Host ("Elapsed: $($timeDifference.ToString("hh\:mm\:ss")), ETA: $($ETA.ToString("hh\:mm\:ss"))")
    #Write-Host "Processing: $($processingFiles.Keys -join ', ')"
    Write-Host ("-----------------------------------------------")
}

$runspacePool.Close()
$runspacePool.Dispose()
$totalTime = New-TimeSpan -Start $startTime -End (Get-Date)
Write-Host ("Total processing time: $($totalTime.ToString("hh\:mm\:ss"))")
(New-Object Media.SoundPlayer "C:\Windows\Media\notify.wav").Play();
pause
