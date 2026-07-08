$utf8bom = New-Object System.Text.UTF8Encoding $True

$f1 = 'Source\Score.h'
$t1 = Get-Content $f1 -Encoding Default -Raw
$t1 = $t1 -replace 'char name\[64\];', 'char name_[64];'
[System.IO.File]::WriteAllText($f1, $t1, $utf8bom)

$f2 = 'Source\Score.cpp'
$t2 = Get-Content $f2 -Encoding Default -Raw
$t2 = $t2 -replace '\.name\b', '.name_'
[System.IO.File]::WriteAllText($f2, $t2, $utf8bom)

$f3 = 'Source\ResultScene.cpp'
$t3 = Get-Content $f3 -Encoding Default -Raw
$t3 = $t3 -replace '\.score\b', '.score_' -replace '\.name\b', '.name_'
[System.IO.File]::WriteAllText($f3, $t3, $utf8bom)

$f4 = 'Source\TitleScene.cpp'
$t4 = Get-Content $f4 -Encoding Default -Raw
$t4 = $t4 -replace '\.score\b', '.score_' -replace '\.name\b', '.name_'
[System.IO.File]::WriteAllText($f4, $t4, $utf8bom)

Write-Host 'Fixed RankData members.'
