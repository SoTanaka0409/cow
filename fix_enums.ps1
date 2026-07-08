$utf8bom = New-Object System.Text.UTF8Encoding $True

$f1 = 'Source\AnimalMove.cpp'
$t1 = Get-Content $f1 -Encoding Default -Raw
$t1 = $t1 -replace '\bAnimal_1\b', 'kAnimal1' -replace '\bAnimal_2\b', 'kAnimal2' -replace '\bAnimal_3\b', 'kAnimal3' -replace '\bAnimal_gold\b', 'kAnimalGold' -replace '\bAnimal_T\b', 'kAnimalT'
[System.IO.File]::WriteAllText($f1, $t1, $utf8bom)

$f2 = 'Source\CowMove.cpp'
$t2 = Get-Content $f2 -Encoding Default -Raw
$t2 = $t2 -replace '\bCow_1\b', 'kCow1' -replace '\bCow_2\b', 'kCow2' -replace '\bCow_3\b', 'kCow3' -replace '\bCow_gold\b', 'kCowGold' -replace '\bCow_T\b', 'kCowT'
[System.IO.File]::WriteAllText($f2, $t2, $utf8bom)

$f3 = 'Source\Cow_gold.cpp'
$t3 = Get-Content $f3 -Encoding Default -Raw
$t3 = $t3 -replace '\bfever\b', 'kFever' -replace '\bNofever\b', 'kNoFever'
[System.IO.File]::WriteAllText($f3, $t3, $utf8bom)

Write-Host 'Standalone enums fixed.'
