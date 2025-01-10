#$OutputEncoding = [console]::InputEncoding = [console]::OutputEncoding = [Text.UTF8Encoding]::UTF8
#$OutputEncoding = New-Object -typename System.Text.UTF8Encoding
Get-ChildItem -Filter *.wav

$inputformat = "aif"
$outputformat = "flac"

Foreach-Object {

}
$countAll = 0
$count = 0
foreach($file in Get-ChildItem -Filter *.$inputformat)
{
    $countAll++
}
echo "$countAll $inputformat files"
pause

mkdir flac
foreach($file in Get-ChildItem -Filter *.$inputformat)
{
    $na = [io.path]::GetFileNameWithoutExtension("$file")
    ffmpeg -hide_banner -i "$na.$inputformat" -map 0:0 -map 0:1? -c:a $outputformat -compression_level 12 -c:v copy "$outputformat\$na.$outputformat"
    (Get-Item "$outputformat/$na.$outputformat").CreationTime=((Get-Item $file).CreationTime)
    (Get-Item "$outputformat/$na.$outputformat").LastWriteTime=((Get-Item $file).LastWriteTime)
    (Get-Item "$outputformat/$na.$outputformat").LastAccessTime=((Get-Item $file).LastAccessTime)

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
