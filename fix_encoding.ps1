$files = @(
    "Source\Scene3D.cpp",
    "Source\TutorialScene.cpp",
    "Source\Fever.cpp",
    "Source\CharacterState.cpp",
    "Source\CharacterState.h",
    "Source\GameConstants.h",
    "Source\AnimalMove.cpp",
    "Source\CowMove.cpp"
)

$utf8 = [System.Text.Encoding]::UTF8
$sjis = [System.Text.Encoding]::GetEncoding(932)

foreach ($f in $files) {
    if (Test-Path $f) {
        $content = [System.IO.File]::ReadAllText((Resolve-Path $f).Path, $utf8)
        [System.IO.File]::WriteAllText((Resolve-Path $f).Path, $content, $sjis)
        Write-Host "Fixed $f"
    }
}
