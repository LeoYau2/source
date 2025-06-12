#$OutputEncoding = [console]::InputEncoding = [console]::OutputEncoding = [Text.UTF8Encoding]::UTF8
#$OutputEncoding = New-Object -typename System.Text.UTF8Encoding

function estimate_qp {
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

$test = estimate_qp -qp 36 -size 57 -target_size 300
echo "$test"

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
            param ($target_width, $ffmpeg_arguments, $arguments, $file, $dir, $na, $qp ,$priorityClass)
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
            $test_file = Get-Item "$dir\$na.${target_width}.qp$qp.avif"
            $test_size = [math]::Round($test_file.Length / 1KB)
            return @{
                ExecutionTime = $executionTime.ToString()
                FullPath = "$file"
                FileSize = $test_size
                qp = $qp
            }
        }

        $powershell = [powershell]::Create().AddScript($scriptBlock).AddArgument($target_width).AddArgument($ffmpeg_arguments).AddArgument($arguments).AddArgument($file.FullName).AddArgument($dir).AddArgument($na).AddArgument($qp).AddArgument($priorityClass)
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
        Write-Host ("FileSize: $($result.FileSize)kb")
        Write-Host ("Progress: $count/$countAll, $progress%")
        Write-Host ("Elapsed: $($timeDifference.ToString("hh\:mm\:ss")), ETA: $($ETA.ToString("hh\:mm\:ss"))")
        #Write-Host "Processing: $($processingFiles.Keys -join ', ')"
        Write-Host ("-----------------------------------------------")
    }

    $runspacePool.Close()
    $runspacePool.Dispose()
    $totalTime = New-TimeSpan -Start $startTime -End (Get-Date)
    Write-Host ("Total processing time: $($totalTime.ToString("hh\:mm\:ss"))")
}


$input_container = 'jpg'

Get-ChildItem -Filter *.$input_container

$countAll = 0
$count = 0
foreach($file in Get-ChildItem -Filter *.$input_container)
{
    $countAll++
}
echo "$countAll $input_container files"

$change_target_width = Read-Host -Prompt 'target width(default:1440)'
if($change_target_width -eq '')
{
    $target_width = '1440'
}else
{
    $target_width = $change_target_width
}

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

$change_target_avg_size = Read-Host -Prompt 'target average size(kb):(default:300)'
if($change_target_avg_size -eq "")
{
    $target_avg_size = 400
}else
{
    $target_avg_size = $change_target_avg_size
}

$change_minsize = Read-Host -Prompt 'min size(kb):(default:150)'
if($change_minsize -eq "")
{
    $minsize = 300
}else
{
    $minsize = $change_minsize
}

$change_maxsize = Read-Host -Prompt 'max size(kb):(default:400)'
if($change_maxsize -eq "")
{
    $maxsize = 550
}else
{
    $maxsize = $change_maxsize
}

$change_max_ln_size_error = Read-Host -Prompt 'max ln(avg_size/target_size):(default:0.1)'
if($change_max_ln_size_error -eq "")
{
    $max_ln_size_error = 0.03
}else
{
    $max_ln_size_error = $change_max_ln_size_error
}

$change_yuv = Read-Host -Prompt 'yuv(default:444)'
if($change_yuv -eq "")
{
    $yuv = '444'
}else
{
    $yuv = $change_yuv
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
    $speed = '4'
}else
{
    $speed = $change_speed
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

$change_inital_qp = Read-Host -Prompt 'inital test qp:(default:36)'
if($change_inital_qp -eq "")
{
    $inital_qp = 36
}else
{
    $inital_qp = $change_inital_qp
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

$max_limit = [math]::Exp([math]::Log([double]$maxsize)+[double]$max_ln_size_error)
$min_limit = [math]::Exp([math]::Log([double]$minsize)-[double]$max_ln_size_error)
Write-Output "min_limit: ${min_limit} kb"
Write-Output "max_limit: ${max_limit} kb"

if (-not (Test-Path $dir)) {
    New-Item -ItemType Directory -Path $dir | Out-Null
}

$list = [System.Collections.ArrayList]@()
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

$files = Get-ChildItem -Filter *.$input_container
for ($i = 0; $i -lt $files.Count; $i++) {
    $list.Add($files[$i]) > $null
    $remain_id.Add($i) > $null
    $list_qp.Add($inital_qp) > $null
    $too_small_id.Add($i) > $null
    $too_big_id.Add($i) > $null
}

pause
$encode = process_files -files $list -dir $dir -target_width $target_width -jobs $jobs -yuv $yuv -depth $depth -range $range -qp_array $list_qp -alphaQ $alphaQ -speed $test_speed -maxConcurrency $maxConcurrency -stat $stat
#pause

Write-Output "list: $list"
Write-Output "too_big_id: $too_big_id"
Write-Output "remain_id: $remain_id"
#pause
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
        $test_size = [math]::Round($test_file.Length / 1KB)
        Write-Output "test_size = $test_size"
        Write-Output "$test_size -gt $max_limit"
        Write-Output "list_qp :$list_qp"
        #pause
        if ($test_size -gt $max_limit -and $last_qp -lt $max_qp){
            $remain_id.Remove($id)
            Write-Output "remain_id remove $id"
            Write-Output "remain_id: $remain_id"
            if (Test-Path $test_file_path) {
                Remove-Item -Path $test_file_path
            }
            Write-Output "estimate_qp = $last_qp $test_size $minsize"
            $new_qp = estimate_qp -qp $last_qp -size $test_size -target_size $minsize
            if ([int]$new_qp -gt [int]$max_qp){
                $new_qp = $max_qp
            }
            Write-Output "new_qp = $new_qp"
            $list_qp[$id] = $new_qp
            $too_big_qp.Add($new_qp) > $null
            #pause
        }
        else{
            $id = $too_big_id[$i]
            $temp.Remove($id)
        }
        #pause
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
    #pause
}


Write-Output "too_small"

#pause
$temp = $too_small_id.Clone()
while ($temp.Count -gt 0){
    $temp = $too_small_id.Clone()
    $too_small_qp.Clear()
    $too_small.Clear()
    #$count = $too_small_id.Count
    for ($i = 0; $i -lt $too_small_id.Count; $i++) {
        Write-Output "temp: $temp"
        Write-Output "i = $i"
        $id = $too_small_id[$i]
        $file = $list[$id]
        Write-Output "file: $file, id:$id"
        $last_qp = $list_qp[$id]
        $na = [io.path]::GetFileNameWithoutExtension("$file")
        $test_file_path = "$dir\${na}.${target_width}.qp$last_qp.avif"
        Write-Output "test_file_path: $test_file_path"
        $test_file = Get-Item $test_file_path
        $test_size = [math]::Round($test_file.Length / 1KB)
        Write-Output "test_size = $test_size"
        if ($test_size -lt $min_limit -and $last_qp -gt $min_qp){
            $remain_id.Remove($id)
            Write-Output "remain_id remove $id"
            Write-Output "remain_id: $remain_id"
            if (Test-Path $test_file_path) {
                Remove-Item -Path $test_file_path
            }
            Write-Output "estimate_qp = $last_qp $test_size $minsize"
            $new_qp = estimate_qp -qp $last_qp -size $test_size -target_size $maxsize
            
            if ([int]$new_qp -lt [int]$min_qp){
                $new_qp = $min_qp
            }
            Write-Output "new_qp = $new_qp"
            $list_qp[$id] = $new_qp
            $too_small_qp.Add($new_qp) > $null
            Write-Output "list_qp :$list_qp"
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
    #pause
    $encode = process_files -files $too_small -dir $dir -target_width $target_width -jobs $jobs -yuv $yuv -depth $depth -range $range -qp $too_small_qp -alphaQ $alphaQ -speed $test_speed -maxConcurrency $maxConcurrency -stat $stat
    #pause
}

Write-Output "remain_id: $remain_id"
#pause
$total_size = 0
$count = 0
$last_qp = $inital_qp
<#for ($i = 0; $i -lt $remain_id.Count; $i++) {
        $id = $remain_id[$i]
        $file = $list[$id]
        $remain.Add($file) > $null
    }#>
for ($i = 0; $i -lt $remain_id.Count; $i++) {
    $file = $list[$remain_id[$i]]
    Write-Output "file: $file"
    $remain.Add($file) > $null
    $na = [io.path]::GetFileNameWithoutExtension("$file")
    $test_file_path = "$dir\${na}.${target_width}.qp$last_qp.avif"
    Write-Output "test_file_path: $test_file_path"
    $test_file = Get-Item $test_file_path
    $test_size = [math]::Round($test_file.Length / 1KB)
    Write-Output "file size: $test_size kb"
    $total_size += $test_size
    
}
$avg_size = [Math]::Round($total_size/$remain_id.Count)
Write-Output "total size: ${total_size} kb, average size: $avg_size kb"
Write-Output "remain: $remain" 
Write-Output "remain_id: $remain_id"
#pause

while ([math]::Abs([math]::Log([double]$avg_size / [double]$target_avg_size)) -gt [double]$max_ln_size_error -and $last_qp -gt $min_qp -and $last_qp -lt $max_qp){
    $remain_qp.Clear()
    Write-Output "remain_id: $remain_id"
    $count = $remain_id.Count
    Write-Output "count: $count"
    Write-Output "clear file"
    #pause
    for ($i = 0; $i -lt $remain_id.Count; $i++) {
        $file = $list[$remain_id[$i]]
        Write-Output "file: $file"
        $na = [io.path]::GetFileNameWithoutExtension("$file")
        $test_file_path = "$dir\${na}.${target_width}.qp$last_qp.avif"
        Write-Output "test_file_path: $test_file_path"
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
    #pause
    #Write-Output "while remain_id: $remain_id"
    $last_qp = estimate_qp -qp $last_qp -size $avg_size -target_size $target_avg_size
    if ([int]$last_qp -gt [int]$max_qp){
        $last_qp = $max_qp
    }
    if ([int]$last_qp -lt [int]$min_qp){
        $last_qp = $min_qp
    }
    
    
    for ($i = 0; $i -lt $count; $i++) {
        $remain_qp.Add($last_qp) > $null
    }
    Write-Output "qp: $last_qp"
    Write-Output "remain: $remain" 
    Write-Output "remain qp: $remain_qp" 
    #pause
    $encode = process_files -files $remain -dir $dir -target_width $target_width -jobs $jobs -yuv $yuv -depth $depth -range $range -qp $remain_qp -alphaQ $alphaQ -speed $test_speed -maxConcurrency $maxConcurrency -stat $stat

    $total_size = 0
    for ($i = 0; $i -lt $remain_id.Count; $i++) {
        $file = $list[$remain_id[$i]]
        $na = [io.path]::GetFileNameWithoutExtension("$file")
        $test_file_path = "$dir\${na}.${target_width}.qp$last_qp.avif"
        Write-Output "test_file_path: $test_file_path"
        $test_file = Get-Item $test_file_path
        $test_size = [math]::Round($test_file.Length / 1KB)
        Write-Output "file size: $test_size kb"
        $total_size += $test_size
    }
    $avg_size = [Math]::Round($total_size/$remain_id.Count)
    Write-Output "total size: ${total_size} kb, average size: $avg_size kb"
    
}

for ($i = 0; $i -lt $remain_id.Count; $i++) {
    $list_qp[$remain_id[$i]] = $last_qp
}
Write-Output "final qp: $list_qp"
#pause
$stat = "final"
if ([int]$target_width -eq 0){
    Remove-Item "$dir\*.avif"
}
$encode = process_files -files $list -dir $dir -target_width $target_width -jobs $jobs -yuv $yuv -depth $depth -range $range -qp $list_qp -alphaQ $alphaQ -speed $speed -maxConcurrency $maxConcurrency -stat $stat
Remove-Item "$dir\*.png"

$total_size = 0
if ($remain_id.Count -gt 0){
    for ($i = 0; $i -lt $remain_id.Count; $i++) {
        $file = $list[$remain_id[$i]]
        $na = [io.path]::GetFileNameWithoutExtension("$file")
        $last_qp = $list_qp[$remain_id[$i]]
        if ([int]$target_width -eq 0){
            $test_file_path = "$dir\${na}.qp$last_qp.avif"
        }
        else{
            $test_file_path = "$dir\${na}.${target_width}.qp$last_qp.avif"
        }
        $test_file = Get-Item $test_file_path
        $test_size = [math]::Round($test_file.Length / 1KB)
        $total_size += $test_size
    }
    $avg_size = [Math]::Round($total_size/$remain_id.Count)
    $count = $remain_id.Count
    Write-Output "final remain count: $count, total size: $total_size kb, average size: $avg_size kb"
}

$total_size = 0
for ($i = 0; $i -lt $list.Count; $i++) {
    $file = $list[$i]
    $na = [io.path]::GetFileNameWithoutExtension("$file")
    $last_qp = $list_qp[$i]
    if ([int]$target_width -eq 0){
        $test_file_path = "$dir\${na}.qp$last_qp.avif"
    }
    else{
        $test_file_path = "$dir\${na}.${target_width}.qp$last_qp.avif"
    }
    $test_file = Get-Item $test_file_path
    $test_size = [math]::Round($test_file.Length / 1KB)
    $total_size += $test_size
}
$avg_size = [Math]::Round($total_size/$list.Count)
$count = $list.Count
Write-Output "final count: $count, total size: $total_size kb, average size: $avg_size kb"

(New-Object Media.SoundPlayer "C:\Windows\Media\notify.wav").Play();
pause
