$inputformat = "flac"
$outputformat = "flac"

New-Item -ItemType Directory -Path .\output -Force
$inputPath = ".\input"
$allFiles = Get-ChildItem -Path $inputPath -Recurse
#$allFiles = Get-ChildItem -Recurse

foreach ($file in $allFiles) {
    $relativePath = $file.FullName.Substring((Get-Location).Path.Length + 1)
    $relativePath = $relativePath -replace "^input\\", ""
    $outputPath = Join-Path -Path .\output -ChildPath $relativePath
    New-Item -ItemType Directory -Path (Split-Path $outputPath) -Force
    echo $file
    if ($file.Extension -eq '.flac') {
        & ffmpeg -hide_banner -i "$($file.FullName)" -map 0:0 -map 0:1? -c:a flac -compression_level 12 -c:v copy "$outputPath"
    } else {
        Copy-Item -Path $file.FullName -Destination $outputPath -Force
    }
}

(New-Object Media.SoundPlayer "C:\Windows\Media\notify.wav").Play();
pause
