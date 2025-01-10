#$OutputEncoding = [console]::InputEncoding = [console]::OutputEncoding = [Text.UTF8Encoding]::UTF8
#$OutputEncoding = New-Object -typename System.Text.UTF8Encoding

$qp_data = (0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 63) 
$speed_qp_size_data = (1.00000, 1.00000, 1.00000, 1.00000, 1.00000, 1.00000, 1.00000, 1.00000, 1.00000, 1.00000, 1.00000, 1.00000, 1.00000, 1.00000, 
                        1.00363, 1.01177, 1.01107, 1.01041, 1.00982, 1.00977, 1.01003, 1.01906, 1.01799, 1.01971, 1.02347, 1.03993, 1.06192, 1.06153, 
                        1.00709, 1.01806, 1.01745, 1.01611, 1.01388, 1.01438, 1.01318, 1.02051, 1.01886, 1.01645, 1.02079, 1.03563, 1.05510, 1.06008, 
                        1.01176, 1.02290, 1.02227, 1.02079, 1.01850, 1.01801, 1.01665, 1.02472, 1.02372, 1.01877, 1.02196, 1.03659, 1.05432, 1.05220, 
                        1.00991, 1.03832, 1.03809, 1.03638, 1.03462, 1.03352, 1.03231, 1.03828, 1.03456, 1.02921, 1.02422, 1.02822, 1.03660, 1.04083, 
                        1.01069, 1.04248, 1.04233, 1.04068, 1.03817, 1.03761, 1.03642, 1.04343, 1.03942, 1.03447, 1.02959, 1.03144, 1.03894, 1.03966, 
                        1.03510, 1.08062, 1.07746, 1.07440, 1.07226, 1.06723, 1.06507, 1.07208, 1.06845, 1.06237, 1.05792, 1.06492, 1.09579, 1.14027, 
                        1.03530, 1.14361, 1.14957, 1.15405, 1.16034, 1.17026, 1.18387, 1.20849, 1.22153, 1.22246, 1.20945, 1.21401, 1.26343, 1.30563, 
                        1.03692, 1.16324, 1.15958, 1.14920, 1.13950, 1.13511, 1.13402, 1.14354, 1.14448, 1.13932, 1.13394, 1.13187, 1.15284, 1.00408, 
                        1.11493, 1.40779, 1.37723, 1.35699, 1.34280, 1.32857, 1.31188, 1.30237, 1.27476, 1.23278, 1.18657, 1.15244, 1.15635, 1.01896)

function speed_qp_size {
    param (
        [int]$test_speed,
        [int]$target_speed,
        [float]$qp
    )
    for ($i = 0; $i -lt $qp_data.Count - 1; $i++) {
        if ($qp -le $qp_data[$i + 1]) {
            $x1 = $qp_data[$i]
            $y1 = $speed_qp_size_data[$test_speed*10+ [Math]::Ceiling($qp/5)]
            $y3 = $speed_qp_size_data[$target_speed*10+ [Math]::Ceiling($qp/5)]
            $x2 = $qp_data[$i + 1]
            $y2 = $speed_qp_size_data[$test_speed*10+ [Math]::Ceiling($qp/5) + 1]
            $y4 = $speed_qp_size_data[$target_speed*10+ [Math]::Ceiling($qp/5) + 1]
            #Write-Output "i: $i"
            #Write-Output "$x1 $x2 $y1 $y2"
            break
        }
    }
    $test_y = $y1 + ($y2 - $y1) * ($qp - $x1) / ($x2 - $x1)
    $target_y = $y3 + ($y4 - $y1) * ($qp - $x1) / ($x2 - $x1)
    $ratio = $test_y / $target_y
    return $ratio
}

function Func($x) {
    #return 5.48043369e-07 * [Math]::Pow($x, 4) -9.44538185e-05 * [Math]::Pow($x, 3) + 4.78633490e-03 * [Math]::Pow($x, 2) - 1.18258206e-01 * $x -6.65678822e-02
    #return 4.04498199e-07 * [Math]::Pow($x, 4) -7.54282987e-05 * [Math]::Pow($x, 3) + 3.92392666e-03 * [Math]::Pow($x, 2) -1.01654538e-01 * $x -1.90887412e-01
    #return -3.47332235e-08 * [Math]::Pow($x, 5) + 5.95288244e-06 * [Math]::Pow($x, 4) -3.86488810e-04 * [Math]::Pow($x, 3) + 1.10669698e-02 * [Math]::Pow($x, 2) -1.59793117e-01 * $x -1.07599299e-01
    return -3.31926605e-08 * [Math]::Pow($x, 5) + 5.70266000e-06 * [Math]::Pow($x, 4) -3.72482876e-04 * [Math]::Pow($x, 3) + 1.07570671e-02 * [Math]::Pow($x, 2) -1.57507930e-01 * $x -1.10503722e-01
}


function Derivative($x) {
    #return 5.48043369e-07 * 4 * [Math]::Pow($x, 3) -9.44538185e-05 * 3 * [Math]::Pow($x, 2) + 4.78633490e-03 * 2 * $x - 1.18258206e-01
    #return 4.04498199e-07 * 4 * [Math]::Pow($x, 3) -7.54282987e-05 * 3 * [Math]::Pow($x, 2) + 3.92392666e-03 * 2 * $x - 1.01654538e-01
    #return -3.47332235e-08 * 5 * [Math]::Pow($x, 4) + 5.95288244e-06 * 4 * [Math]::Pow($x, 3) -3.86488810e-04 * 3 * [Math]::Pow($x, 2) + 1.10669698e-02 * 2 * $x -1.59793117e-01
    return -3.31926605e-08 * 5 * [Math]::Pow($x, 4) + 5.70266000e-06 * 4 * [Math]::Pow($x, 3) -3.72482876e-04 * 3 * [Math]::Pow($x, 2) + 1.07570671e-02 * 2 * $x -1.57507930e-01
}

function InverseFunc($y) {
    $initialGuess = 36
    $maxIterations = 100
    $tolerance = 1e-6
    $x = $initialGuess
    for ($i = 0; $i -lt $maxIterations; $i++) {
        $fx = Func($x)
        if ([Math]::Abs($fx - $y) -lt $tolerance) {
            return $x
        }
        $dfx = Derivative($x)
        if ($dfx -eq 0) {
            Write-Error "Derivative is zero. Cannot continue."
            return $null
        }
        $x = $x - ($fx - $y) / $dfx
    }
    Write-Warning "Max iterations reached. Result may not be accurate."
    return $x
}

function estimate_qp {
    param ([string]$qp, [string]$size, [string]$target_size)
    $ln_size = Func([double]$qp)
    $y = [double]$ln_size + [math]::Log([double]::Parse($target_size) / [double]::Parse($size))
    $estimate_qp = InverseFunc($y)
    if ($estimate_qp -lt 0){
        $estimate_qp = 0
    }
    if ($estimate_qp -gt 63){
        $estimate_qp = 63
    }
    return [math]::Round($estimate_qp)
}


function estimate_qp_old {
    param ([string]$qp, [string]$size, [string]$target_size)
    $x = ([double]::Parse($qp) - 20) / 10
    $ln_size = -0.0295*[double]$x*[double]$x - 0.3652*[double]$x + 0.3427
    $y = [double]$ln_size + [math]::Log([double]::Parse($target_size) / [double]::Parse($size))
    $test = [math]::Pow(-0.118*$y+0.1738, 0.5)
    if (-0.118*$y+0.1738 -gt 0){
        $estimate_qp = -(0.3652 - [math]::Pow(-0.118*$y+0.1738, 0.5)) / 0.0059 + 20
    }
    else{
        $estimate_qp = 0
    }
    if ($estimate_qp -lt 0){
        $estimate_qp = 0
    }
    if ($estimate_qp -gt 63){
        $estimate_qp = 63
    }
    return [math]::Round($estimate_qp)
}

#$test = estimate_qp -qp 36 -size 57 -target_size 300
#echo "$test"

function process_files {
    param (
        [array]$files,
        [string]$dir,
        [string]$target_width,
        [string]$jobs,
        [string]$yuv,
        [string]$depth,
        [string]$range,
        [array]$qp_array,
        [string]$alphaQ,
        [string]$speed,
        [int]$maxConcurrency,
        [string]$stat
    )
    #$files = Get-ChildItem -Filter *.$input_container
    $countAll = $files.Count
    $count = 0

    $runspacePool = [runspacefactory]::CreateRunspacePool(1, $maxConcurrency)
    $runspacePool.Open()

    $startTime = Get-Date
    $runspaces = @()
    $processingFiles = @{}
    #foreach ($file in $files) {
    for ($i = 0; $i -lt $files.Length; $i++) {
        $file = $files[$i]
        $qp = $qp_array[$i]
        $na = [io.path]::GetFileNameWithoutExtension("$file")
        if ($stat -eq "final" -and [int]$target_width -eq 0){
            $arguments = "-j $jobs -y $yuv -d $depth -r $range --speed $speed --max $qp --min $qp --maxalpha $alphaQ --minalpha $alphaQ -s $speed `"$dir\$na.${target_width}.png`" -o `"$dir\$na.qp$qp.avif`""
        }
        else{
            $arguments = "-j $jobs -y $yuv -d $depth -r $range --speed $speed --max $qp --min $qp --maxalpha $alphaQ --minalpha $alphaQ -s $speed `"$dir\$na.${target_width}.png`" -o `"$dir\$na.${target_width}.qp$qp.avif`""
        }
        $ffmpeg_arguments = "-hide_banner -i `"$file`" -vf `"format=rgba,scale=${target_width}:-1`" -sws_flags area `"$dir\$na.${target_width}.png`""
        #echo $ffmpeg_arguments
        #Start-Process -FilePath "ffmpeg.exe" -ArgumentList $ffmpeg_arguments -PassThru -NoNewWindow
        $scriptBlock = {
            param ($target_width, $ffmpeg_arguments, $arguments, $file, $dir, $na, $qp ,$priorityClass, $speed)
            "$dir\$na.${target_width}.png"
            if (-not (Test-Path "$dir\$na.${target_width}.png")) {
                $process_resize = Start-Process -FilePath "ffmpeg.exe" -ArgumentList $ffmpeg_arguments -PassThru -WindowStyle Hidden
                $process_resize.PriorityClass = $priorityClass
                $process_resize.WaitForExit()
            }
            $process = Start-Process -FilePath "C:\Users\Leo\avifenc.exe" -ArgumentList $arguments -PassThru -WindowStyle Hidden
            $process.PriorityClass = $priorityClass
            $process.WaitForExit()

            (Get-Item "$dir\$na.$target_width.qp$qp.avif").CreationTime = ((Get-Item $file).CreationTime)
            (Get-Item "$dir\$na.$target_width.qp$qp.avif").LastWriteTime = ((Get-Item $file).LastWriteTime)
            (Get-Item "$dir\$na.$target_width.qp$qp.avif").LastAccessTime = ((Get-Item $file).LastAccessTime)

            $executionTime = [timespan]::FromMilliseconds($process.ExitTime - $process.StartTime)
            $executionTime = [timespan]::FromMilliseconds($process.ExitTime.Subtract($process.StartTime).TotalMilliseconds)  
            <#if (Test-Path "$dir\$na.${target_width}.png") {
                Remove-Item "$dir\$na.${target_width}.png" -Force
                Write-Output "delete: $dir\$na.${target_width}.png"
            } else {
                Write-Output "File not found: $dir\$na.${target_width}.png"
            }#>
            #$test_file = Get-Item "$dir\$na.${target_width}.qp$qp.avif"
            if ($stat -eq "final" -and $target_width -eq "0"){
                $test_file = Get-Item "$dir\$na.qp$qp.avif"
            }
            else{
                $test_file = Get-Item "$dir\$na.${target_width}.qp$qp.avif"
            }
            $test_size = [math]::Round($test_file.Length / 1KB)
            return @{
                ExecutionTime = $executionTime.ToString()
                FullPath = "$file"
                FileSize = $test_size
                qp = $qp
                speed = $speed
            }
        }

        $powershell = [powershell]::Create().AddScript($scriptBlock).AddArgument($target_width).AddArgument($ffmpeg_arguments).AddArgument($arguments).AddArgument($file.FullName).AddArgument($dir).AddArgument($na).AddArgument($qp).AddArgument($priorityClass).AddArgument($speed)
        #$currentThread = [System.Threading.Thread]::CurrentThread
        #$coreMask = [System.IntPtr]::new(50)
        #$currentThread.ProcessorAffinity = $coreMask
        $powershell.RunspacePool = $runspacePool

        $runspaces += [pscustomobject]@{
            PowerShell = $powershell
            Status = $powershell.BeginInvoke()
            File = $file.Name
            qp = $file.qp
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
        Write-Host ("qp: $($result.qp)")
        Write-Host ("speed: $($result.speed)")
        Write-Host ("FileSize: $($result.FileSize)kb")
        Write-Host ("Progress: $count/$countAll, $progress%")
        Write-Host ("Elapsed: $($timeDifference.ToString("hh\:mm\:ss")), ETA: $($ETA.ToString("hh\:mm\:ss"))")
        #Write-Host "Processing: $($processingFiles.Keys -join ', ')"
        Write-Host ("-----------------------------------------------")
    }

    $runspacePool.Close()
    $runspacePool.Dispose()
    $totalTime = New-TimeSpan -Start $startTime -End (Get-Date)
    #Write-Host ("Total processing time: $($totalTime.ToString("hh\:mm\:ss"))")
}

function size_sum {
    param (
        [array]$list,
        [array]$list_qp,
        [array]$id,
        [string]$dir,
        [string]$target_width,
        [string]$stat
    )
    for ($i = 0; $i -lt $id.Count; $i++) {
        $file = $list[$id[$i]]
        $na = [io.path]::GetFileNameWithoutExtension("$file")
        $last_qp = $list_qp[$id[$i]]
        if ([int]$target_width -eq 0 -and $stat -eq "final"){
            $test_file_path = "$dir\${na}.qp$last_qp.avif"
        }
        else{
            $test_file_path = "$dir\${na}.${target_width}.qp$last_qp.avif"
        }
        $test_file = Get-Item $test_file_path
        $test_size = $test_file.Length / 1KB
        $total_size += $test_size
    }
    $total_size = [Math]::Round($total_size, 1)
    $avg_size = [Math]::Round($total_size/$id.Count, 1)
    return @($total_size, $avg_size)
}



$input_container = 'png'

Get-ChildItem -Filter *.$input_container

$countAll = 0
$count = 0
foreach($file in Get-ChildItem -Filter *.$input_container)
{
    $countAll++
}
echo "$countAll $input_container files"

$cpuInfo = @(Get-CimInstance -ClassName Win32_Processor | Select-Object Name, NumberOfCores, NumberOfLogicalProcessors)
$threads = 0
foreach ($cpu in $cpuInfo) {
    Write-Output "CPU Model: $($cpu.Name)"
    Write-Output "$($cpu.NumberOfCores) Cores, $($cpu.NumberOfLogicalProcessors) Threads"
    $cores = $cores + $($cpu.NumberOfCores)
    $threads = $threads + $($cpu.NumberOfLogicalProcessors)
}

if ($($cpuInfo.Count -gt 1)){
    Write-Output "$($cpuInfo.Count) CPUs"
    Write-Output "Total $cores Cores, $threads Threads"
}

$coreMask = ('1' * $threads)

$change_coreMask = Read-Host -Prompt "Core Mask(default:$($coreMask)))"
if($change_coreMask -eq "")
{
    $coreMask = [Convert]::ToInt32($coreMask, 2)
}else
{
    $coreMask = [Convert]::ToInt32($change_coreMask, 2)
}
Write-Output "Core Mask: $coreMask"

$change_maxConcurrency = Read-Host -Prompt "maxConcurrency(default:$([math]::Round($threads / 3)))"
if($change_maxConcurrency -eq "")
{
    $maxConcurrency = $([math]::Round($threads / 3))
}else
{
    $maxConcurrency = $change_maxConcurrency
}

$change_target_width = Read-Host -Prompt 'target width(default:1440)'
if($change_target_width -eq '')
{
    $target_width = '1440'
}else
{
    $target_width = $change_target_width
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

$change_target_avg_size = Read-Host -Prompt 'target average size(kb):(default:300)'
if($change_target_avg_size -eq "")
{
    $target_avg_size = 300
}else
{
    $target_avg_size = $change_target_avg_size
}

$change_minsize_ratio = Read-Host -Prompt 'min size ratio:(default:0.5)'
if($change_minsize_ratio -eq "")
{
    $minsize_ratio = 0.5
    $minsize = [double]$target_avg_size * 0.5
}else
{
    $minsize_ratio = $change_minsize_ratio
    $minsize = [double]$target_avg_size * [double]$change_minsize_ratio
}
Write-Output "min size: $minsize kb"

$change_maxsize_ratio = Read-Host -Prompt 'max size ratio:(default:1.5)'
if($change_maxsize_ratio -eq "")
{
    $maxsize_ratio = 1.5
    $maxsize = [double]$target_avg_size * 1.5
}else
{
    $maxsize_ratio = $change_maxsize_ratio
    $maxsize = [double]$target_avg_size * [double]$change_maxsize_ratio
}
Write-Output "max size: $maxsize kb"

$change_max_ln_size_error = Read-Host -Prompt 'max ln(avg_size/target_size):(default:0.05)'
if($change_max_ln_size_error -eq "")
{
    $max_ln_size_error = 0.05
}else
{
    $max_ln_size_error = $change_max_ln_size_error
}

$change_inital_qp = Read-Host -Prompt 'inital test qp:(default:36)'
if($change_inital_qp -eq "")
{
    $inital_qp = 36
}else
{
    $inital_qp = $change_inital_qp
}

$change_min_qp = Read-Host -Prompt 'min_qp(default:30)'
if($change_min_qp -eq "")
{
    $min_qp = '30'
}else
{
    $min_qp = $change_min_qp
}

$change_max_qp = Read-Host -Prompt 'max_qp(default:63)'
if($change_min_qp -eq "")
{
    $max_qp = '63'
}else
{
    $max_qp = $change_max_qp
}

$change_test_speed = Read-Host -Prompt 'test speed(default:6)'
if($change_test_speed -eq '')
{
    $test_speed = '6'
}else
{
    $test_speed = $change_test_speed
}

$change_speed = Read-Host -Prompt 'final output speed(default:1)'
if($change_speed -eq '')
{
    $speed = '1'
}else
{
    $speed = $change_speed
}

$change_yuv = Read-Host -Prompt 'yuv(default:444)'
if($change_yuv -eq "")
{
    $yuv = '444'
}else
{
    $yuv = $change_yuv
}

$depth = 8
$range = "full"
$alphaQ = 0
$jobs = "all"

if ([int]$target_width -eq 0){
    $dir = "$target_avg_size"+"kb"
}
else{
    $dir = "$target_width"+"px_"+"$target_avg_size"+"kb"
}

echo "$input_container to avif"
echo "outdir $dir"
echo "yuv$yuv"
echo "qp $qp"
echo "depth $depth"
echo "speed $speed"
echo "alpha qp $alphaQ"
echo "range $range"
echo "jobs $jobs"

if (-not (Test-Path $dir)) {
    New-Item -ItemType Directory -Path $dir | Out-Null
}

if (Test-Path "$dir\log.txt") {
    Remove-Item "$dir\log.txt"
}


$list = [System.Collections.ArrayList]@()
$all_id = [System.Collections.ArrayList]@()
$remain = [System.Collections.ArrayList]@()
$remain_id = [System.Collections.ArrayList]@()
$remain_qp = [System.Collections.ArrayList]@()
$list_qp = [System.Collections.ArrayList]@()
$too_small = [System.Collections.ArrayList]@()
$too_small_id = [System.Collections.ArrayList]@()
$too_small_qp = [System.Collections.ArrayList]@()
$too_big = [System.Collections.ArrayList]@()
$too_big_id = [System.Collections.ArrayList]@()
$too_big_qp = [System.Collections.ArrayList]@()
$temp = [System.Collections.ArrayList]@()

$stat = "0"

$size_error = @()

$files = Get-ChildItem -Filter *.$input_container
for ($i = 0; $i -lt $files.Count; $i++) {
    $list.Add($files[$i]) > $null
    $all_id.Add($i) > $null
    $list_qp.Add($inital_qp) > $null
    $size_error += @(0) * 64
}
#write-output "$($size_error[200])"

pause
$startTime = Get-Date
$encode = process_files -files $list -dir $dir -target_width $target_width -jobs $jobs -yuv $yuv -depth $depth -range $range -qp_array $list_qp -alphaQ $alphaQ -speed $test_speed -maxConcurrency $maxConcurrency -stat $stat

<#$total_size = 0
for ($i = 0; $i -lt $list.Count; $i++) {
    $file = $list[$i]
    $na = [io.path]::GetFileNameWithoutExtension("$file")
    $last_qp = $list_qp[$i]
    $test_file_path = "$dir\${na}.${target_width}.qp$last_qp.avif"
    $test_file = Get-Item $test_file_path
    $test_size = $test_file.Length / 1KB
    $total_size += $test_size
}
$total_size = [Math]::Round($total_size, 1)
$avg_size = [Math]::Round($total_size/$list.Count, 1)#>
$count = $list.Count

$total_size, $avg_size = size_sum -list $list -list_qp $list_qp -id $all_id -dir $dir -target_width $target_width -stat $stat
Write-Output "count: $count, total size: $total_size kb, average size: $avg_size kb"

for ($i = 0; $i -lt $list.Count; $i++) {
    $file = $list[$i]
    $na = [io.path]::GetFileNameWithoutExtension("$file")
    $last_qp = $list_qp[$i]
    $test_file_path = "$dir\${na}.${target_width}.qp$last_qp.avif"
    $test_file = Get-Item $test_file_path
    $test_size = $test_file.Length / 1KB
    $total_size += $test_size
    $test = $avg_size * [double]$minsize_ratio
    #Write-Output "$avg_size $minsize_ratio"
    #Write-Output "$test_size -lt $test"
    
    if ($test_size -gt $avg_size * [double]$maxsize_ratio){
        $too_big_id.Add($i) > $null
    }
    elseif ($test_size -lt $avg_size * [double]$minsize_ratio){
        $too_small_id.Add($i) > $null
    }
    else {
        $remain_id.Add($i) > $null
    }
}

$output = "too_big_id: $too_big_id"
$output | Tee-Object -FilePath "$dir\log.txt" -Append | Write-Output
$output = "too_small_id: $too_small_id"
$output | Tee-Object -FilePath "$dir\log.txt" -Append | Write-Output
$output = "remain_id: $remain_id"
$output | Tee-Object -FilePath "$dir\log.txt" -Append | Write-Output
#pause

Write-Output "processing too big"

$temp = $too_big_id.Clone()
while ($temp.Count -gt 0){
    $temp = $too_big_id.Clone()
    $too_big_qp.Clear()
    $too_big.Clear()
    #$count = $too_big_id.Count
    for ($i = 0; $i -lt $too_big_id.Count; $i++) {
        Write-Output "temp: $temp"
        Write-Output "i = $i"
        $id = $too_big_id[$i]
        $file = $list[$id]
        Write-Output "file: $file, id:$id"
        $last_qp = $list_qp[$id]
        $na = [io.path]::GetFileNameWithoutExtension("$file")
        $test_file_path = "$dir\${na}.${target_width}.qp$last_qp.avif"
        Write-Output "test_file_path: $test_file_path"
        $test_file = Get-Item $test_file_path
        $test_size = [math]::Round($test_file.Length / 1KB, 1)
        Write-Output "test_size = $test_size"
        Write-Output "$test_size -gt $max_limit"
        Write-Output "list_qp :$list_qp"
        #if ($test_size -gt $max_limit -and $last_qp -lt $max_qp){
        write-output "$($test_size -lt [math]::Exp([math]::Log([double]$maxsize*$speed_size_ratio)-[double]$max_ln_size_error)), $($last_qp -gt $min_qp), $($test_size -gt [math]::Exp([math]::Log([double]$maxsize*$speed_size_ratio)+[double]$max_ln_size_error)), $($last_qp -lt $max_qp)"
        $speed_size_ratio = speed_qp_size -test_speed $test_speed -target_speed $speed -qp $last_qp
        #$target_size = [double]$maxsize * $speed_size_ratio
        if (($test_size -lt [math]::Exp([math]::Log([double]$maxsize*$speed_size_ratio)-[double]$max_ln_size_error) -and ($last_qp -gt $min_qp)) -or ($test_size -gt [math]::Exp([math]::Log([double]$maxsize*$speed_size_ratio)+[double]$max_ln_size_error) -and ($last_qp -lt $max_qp))){
            
            if ($test_size -lt [math]::Exp([math]::Log([double]$maxsize*$speed_size_ratio)-[double]$max_ln_size_error) -or $test_size -gt [math]::Exp([math]::Log([double]$maxsize*$speed_size_ratio)+[double]$max_ln_size_error)){
                $size_error[[int]$id*64+[int]$last_qp] = $test_size - [double]$maxsize*$speed_size_ratio
                Write-Output "size_error: $($size_error[[double]$id*64+[double]$last_qp])"
            }

            if (Test-Path $test_file_path) {
                Remove-Item -Path $test_file_path
            }
            Write-Output "estimate_qp = $last_qp $test_size $($maxsize*$speed_size_ratio)"
            $new_qp = estimate_qp -qp $last_qp -size $test_size -target_size $([double]$maxsize*$speed_size_ratio)

            while ($($size_error[[int]$id*64+[int]$new_qp]) -ne 0){
                if ($($size_error[[int]$id*64+[int]$new_qp]) -gt 0){
                    $new_qp = $new_qp + 1
                }elseif ($($size_error[[int]$id*64+[int]$new_qp]) -lt 0){
                    $new_qp = $new_qp - 1
                }
 
            }

            write-output "new_qp: $new_qp, id: $id, $([int]$id*64+[int]$new_qp), $($size_error[[int]$id*64+[int]$new_qp])  --------------------------------------------"
            for($j = 0; $j -lt $64; $j++){
                write-output "qp${j}: $($size_error[[int]$id*64+[int]$j])"
            }
            #pause

            if ([int]$new_qp -gt [int]$max_qp){
                $new_qp = $max_qp
            }
            if ([int]$new_qp -lt [int]$min_qp){
                $new_qp = $min_qp
            }
            Write-Output "new_qp = $new_qp"
            
            $list_qp[$id] = $new_qp
            $too_big_qp.Add($new_qp) > $null
        }
        else{
            $id = $too_big_id[$i]
            $temp.Remove($id)
        }
    }
    $too_big_id = $temp.Clone()
    Write-Output "too_big_id: $too_big_id"
    $too_big.Clear()
    for ($i = 0; $i -lt $too_big_id.Count; $i++) {
        $id = $too_big_id[$i]
        $file = $list[$id]
        $too_big.Add($file) > $null
    }
    Write-Output "too_big: $too_big"
    Write-Output "too_big_qp: $too_big_qp"
    Write-Output "list_qp: $list_qp"
    #pause
    $encode = process_files -files $too_big -dir $dir -target_width $target_width -jobs $jobs -yuv $yuv -depth $depth -range $range -qp $too_big_qp -alphaQ $alphaQ -speed $test_speed -maxConcurrency $maxConcurrency -stat $stat

}

Write-Output "processing too small"
#pause

$temp = $too_small_id.Clone()
while ($temp.Count -gt 0){
    $temp = $too_small_id.Clone()
    $too_small_qp.Clear()
    $too_small.Clear()
    #$count = $too_small_id.Count
    for ($i = 0; $i -lt $too_small_id.Count; $i++) {
        $id = $too_small_id[$i]
        $file = $list[$id]
        $last_qp = $list_qp[$id]
        $na = [io.path]::GetFileNameWithoutExtension("$file")
        $test_file_path = "$dir\${na}.${target_width}.qp$last_qp.avif"
        $test_file = Get-Item $test_file_path
        $test_size = [math]::Round($test_file.Length / 1KB, 1)
        $speed_size_ratio = speed_qp_size -test_speed $test_speed -target_speed $speed -qp $last_qp
        #$target_size = [double]$maxsize * $speed_size_ratio
        if (($test_size -lt [math]::Exp([math]::Log([double]$minsize*$speed_size_ratio)-[double]$max_ln_size_error) -and $last_qp -gt $min_qp) -or ($test_size -gt [math]::Exp([math]::Log([double]$minsize*$speed_size_ratio)+[double]$max_ln_size_error) -and $last_qp -lt $max_qp)){
            
            if ($test_size -lt [math]::Exp([math]::Log([double]$minsize*$speed_size_ratio)-[double]$max_ln_size_error) -or $test_size -gt [math]::Exp([math]::Log([double]$minsize*$speed_size_ratio)+[double]$max_ln_size_error)){
                $size_error[[int]$id*64+[int]$last_qp] = $test_size - [double]$minsize*$speed_size_ratio
                Write-Output "size_error: $($size_error[[double]$id*64+[double]$last_qp])"
            }

            if (Test-Path $test_file_path) {
                Remove-Item -Path $test_file_path
            }
            Write-Output "estimate_qp = $last_qp $test_size $($minsize*$speed_size_ratio)"
            $new_qp = estimate_qp -qp $last_qp -size $test_size -target_size $([double]$minsize*$speed_size_ratio)

            while ($($size_error[[int]$id*64+[int]$new_qp]) -ne 0){
                if ($($size_error[[int]$id*64+[int]$new_qp]) -gt 0){
                    $new_qp = $new_qp + 1
                }elseif ($($size_error[[int]$id*64+[int]$new_qp]) -lt 0){
                    $new_qp = $new_qp - 1
                }
                write-output "while new_qp: $new_qp, id: $id, $([int]$id*64+[int]$new_qp), $($size_error[[int]$id*64+[int]$new_qp])  --------------------------------------------"
                for($j = 0; $j -lt $64; $j++){
                    write-output "qp${j}: $($size_error[[int]$id*64+[int]$j])"
                }
                #pause
            }
            write-output "new_qp: $new_qp--------------------------------------------"
            for($j = 0; $j -lt $64; $j++){
                write-output "qp${j}: $($size_error[[int]$id*64+[int]$j])"
            }
            #pause

            if ([int]$new_qp -gt [int]$max_qp){
                $new_qp = $max_qp
            }
            if ([int]$new_qp -lt [int]$min_qp){
                $new_qp = $min_qp
            }
            Write-Output "new_qp = $new_qp"
            
            if ([int]$new_qp -gt [int]$max_qp){
                $new_qp = $max_qp
            }
            if ([int]$new_qp -lt [int]$min_qp){
                $new_qp = $min_qp
            }
            #Write-Output "new_qp = $new_qp"
            $list_qp[$id] = $new_qp
            $too_small_qp.Add($new_qp) > $null
            #Write-Output "list_qp :$list_qp"
            #pause
        }
        else{
            $id = $too_small_id[$i]
            $temp.Remove($id)
        }
    }
    $too_small_id = $temp.Clone()
    Write-Output "too_small_id: $too_small_id"
    $too_small.Clear()
    for ($i = 0; $i -lt $too_small_id.Count; $i++) {
        $id = $too_small_id[$i]
        $file = $list[$id]
        $too_small.Add($file) > $null
    }
    Write-Output "too_small: $too_small"
    Write-Output "too_small_qp: $too_small_qp"
    Write-Output "list_qp: $list_qp"
    $encode = process_files -files $too_small -dir $dir -target_width $target_width -jobs $jobs -yuv $yuv -depth $depth -range $range -qp $too_small_qp -alphaQ $alphaQ -speed $test_speed -maxConcurrency $maxConcurrency -stat $stat
}

Write-Output "remain_id: $remain_id"
#pause
$total_size = 0
$remain_size = 0
$count = 0
$last_qp = $inital_qp

for ($i = 0; $i -lt $remain_id.Count; $i++) {
    $file = $list[$remain_id[$i]]
    $remain.Add($file) > $null
}

$count = $remain_id.Count
$remain_size, $remain_avg_size = size_sum -list $list -list_qp $list_qp -id $remain_id -dir $dir -target_width $target_width -stat $stat
Write-Output "remain count: $count, total size: ${remain_size} kb, average size: $remain_avg_size kb"

Write-Output "remain: $remain" 
Write-Output "remain_id: $remain_id"

$count = $list.Count
$total_size, $avg_size = size_sum -list $list -list_qp $list_qp -id $all_id -dir $dir -target_width $target_width -stat $stat
Write-Output "total count: $count, total size: ${total_size} kb, average size: $avg_size kb"
Write-Output "list_qp: $list_qp"

$speed_size_ratio = speed_qp_size -test_speed $test_speed -target_speed $speed -qp $last_qp
while ([math]::Abs([math]::Log([double]$avg_size / ([double]$target_avg_size*$speed_size_ratio))) -gt [double]$max_ln_size_error -and $last_qp -gt $min_qp -and $last_qp -lt $max_qp){
    write-output "$([math]::Log([double]$avg_size / ([double]$target_avg_size*$speed_size_ratio))), $($last_qp -gt $min_qp -and $last_qp -lt $max_qp)"
    Write-Output " while $avg_size, $($target_avg_size*$speed_size_ratio)"
    Write-Output "list_qp: $list_qp"
    $remain_qp.Clear()
    #Write-Output "remain_id: $remain_id"
    $count = $remain_id.Count
    #Write-Output "count: $count"
    Write-Output "clear file"
    for ($i = 0; $i -lt $remain_id.Count; $i++) {
        $file = $list[$remain_id[$i]]
        #Write-Output "file: $file"
        $na = [io.path]::GetFileNameWithoutExtension("$file")
        $test_file_path = "$dir\${na}.${target_width}.qp$last_qp.avif"
        #Write-Output "test_file_path: $test_file_path"
        if (Test-Path $test_file_path) {
            while (Test-Path $test_file_path){
                Remove-Item -Path $test_file_path -Force
                Write-Output "remove: $test_file_path"
            }
        }
        else{
            Write-Output "can't find: $test_file_path"
        }
    }
    
    #Write-Output "while remain_id: $remain_id"
    #target_avg * all.count = ($total_size - $remain_size) + remain_target_avg_size * remain.count
    #(target_avg * all.count - ($total_size - $remain_size)) / remain.count = remain_target_avg_size
    $remain_target_avg_size = ([double]$target_avg_size * $($all_id.Count) - ($total_size - $remain_size)) / $($remain.Count)
    write-output "$remain_target_avg_size, $target_avg_size, $($all_id.Count), $total_size, $remain_size, $($remain.Count)"
    write-output "$last_qp, $avg_size, $([double]$remain_target_avg_size*$speed_size_ratio)"
    $last_qp = estimate_qp -qp $last_qp -size $remain_avg_size -target_size $([string]$([double]$remain_target_avg_size*$speed_size_ratio))
    write-output "$last_qp"
    #pause
    #$speed_size_ratio = speed_qp_size -test_speed $test_speed -target_speed $speed -qp $last_qp
    write-output "$last_qp, $avg_size, $([double]$remain_target_avg_size*$speed_size_ratio)"
    if ([int]$last_qp -gt [int]$max_qp){
        $last_qp = $max_qp
    }
    if ([int]$last_qp -lt [int]$min_qp){
        $last_qp = $min_qp
    }
    
    for ($i = 0; $i -lt $remain_id.Count; $i++) {
        $remain_qp.Add($last_qp) > $null
        $list_qp[$remain_id[$i]] = $last_qp
    }
    Write-Output "remain_id: $remain_id"
    Write-Output "last_qp: $last_qp"
    #pause
    $encode = process_files -files $remain -dir $dir -target_width $target_width -jobs $jobs -yuv $yuv -depth $depth -range $range -qp $remain_qp -alphaQ $alphaQ -speed $test_speed -maxConcurrency $maxConcurrency -stat $stat

    $count = $remain_id.Count
    $remain_size, $remain_avg_size = size_sum -list $list -list_qp $list_qp -id $remain_id -dir $dir -target_width $target_width -stat $stat
    Write-Output "remain count: $count, total size: ${remain_size} kb, average size: $remain_avg_size kb"
    #pause
    $count = $list.Count
    $total_size, $avg_size = size_sum -list $list -list_qp $list_qp -id $all_id -dir $dir -target_width $target_width -stat $stat
    Write-Output "total count: $count, total size: ${total_size} kb, average size: $avg_size kb"
    #pause
}

for ($i = 0; $i -lt $remain_id.Count; $i++) {
    $list_qp[$remain_id[$i]] = $last_qp
}
$output = "final qp: $list_qp"
$output | Tee-Object -FilePath "$dir\log.txt" -Append | Write-Output
#pause
$stat = "final"
if ([int]$target_width -eq 0){
    Remove-Item "$dir\*.avif"
}
$encode = process_files -files $list -dir $dir -target_width $target_width -jobs $jobs -yuv $yuv -depth $depth -range $range -qp $list_qp -alphaQ $alphaQ -speed $speed -maxConcurrency $maxConcurrency -stat $stat

Remove-Item "$dir\*.png"

if ($remain_id.Count -gt 0){
    $count = $remain_id.Count
    $total_size, $avg_size = size_sum -list $list -list_qp $list_qp -id $remain_id -dir $dir -target_width $target_width -stat $stat
    $output = "remain count: $count, total size: $total_size kb, average size: $avg_size kb"
    $output | Tee-Object -FilePath "$dir\log.txt" -Append | Write-Output
}

$count = $all_id.Count
$total_size, $avg_size = size_sum -list $list -list_qp $list_qp -id $all_id -dir $dir -target_width $target_width -stat $stat
$ratio = [math]::Round($avg_size / [double]$target_avg_size * 100, 1)
$output = "final count: $count, total size: $total_size kb, average size: $avg_size kb, target: $target_avg_size kb, ratio: $ratio %"
$output | Tee-Object -FilePath "$dir\log.txt" -Append | Write-Output
$currentTime = Get-Date
$timeDifference = New-TimeSpan -Start $startTime -End $currentTime
$output = "total time: $($timeDifference.ToString("hh\:mm\:ss"))"
$output | Tee-Object -FilePath "$dir\log.txt" -Append | Write-Output

(New-Object Media.SoundPlayer "C:\Windows\Media\notify.wav").Play();
pause
pause
pause