#$OutputEncoding = [console]::InputEncoding = [console]::OutputEncoding = [Text.UTF8Encoding]::UTF8
#$OutputEncoding = New-Object -typename System.Text.UTF8Encoding
Get-ChildItem -Filter *.jpg

Foreach-Object {
    
}

$countAll = 0
$count = 0
foreach($file in Get-ChildItem -Filter *.jpg)
{
    $countAll++
}
echo "$countAll jpg files"
pause

$yuv = 420
$preset = "placebo"
$quality = 38
$bitdepth = 8

$dir = "heif"+"q$quality"

echo "jpg to heif"
echo "outdir $dir"
echo "yuv$yuv"
echo "bitdepth $bitdepth"
echo "preset $preset"
echo "quality $quality"
pause

mkdir $dir
foreach($file in Get-ChildItem -Filter *.jpg)
{
    $na = [io.path]::GetFileNameWithoutExtension("$file")
    heif-enc "$file" -p chroma=$yuv -p preset=$preset -q $quality -b bitdepth -o "$dir\$na.q$quality.heif"
    (Get-Item "$dir\$na.q$quality.heif").CreationTime=((Get-Item $file).CreationTime)
    (Get-Item "$dir\$na.q$quality.heif").LastWriteTime=((Get-Item $file).LastWriteTime)
    #(Get-Item "$dir\$na.q$quality.heif").LastAccessTime=((Get-Item $file).LastAccessTime)

    $count++
    echo $na
    echo $file.FullName
    $progress = [math]::round($count / $countAll * 100,3)
    echo "$count/$countAll progress $progress%"
    echo -------------------------------------------------------------
}
pause
