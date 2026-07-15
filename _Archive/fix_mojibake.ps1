$files = @("AnimalMove.cpp", "Camera.cpp", "CowManager.cpp", "CowMove.cpp", "DebugCamera.cpp", "GameManager.cpp", "LoadingScene.cpp", "LoadingScene.h", "Main.cpp", "Master.h", "ObjectManager.cpp", "Player3D.cpp", "Player3D.h", "ResultScene.cpp", "ResultScene.h", "Rule.cpp", "Scene.cpp", "Scene.h", "Scene3D.cpp", "Scene3D.h", "SceneManager.cpp", "SceneManager.h", "ServiceLocator.cpp", "Tatumaki.cpp", "Texture.cpp", "Texture.h", "Thunder.cpp", "TitleScene.cpp", "TitleScene.h", "TutorialScene.cpp", "TutorialScene.h")
$utf8bom = New-Object System.Text.UTF8Encoding $True
foreach ($fname in $files) {
    $file = "Source\" + $fname
    $text = [System.IO.File]::ReadAllText($file, [System.Text.Encoding]::UTF8)
    $bytes = [System.Text.Encoding]::GetEncoding(932).GetBytes($text)
    $fixedText = [System.Text.Encoding]::UTF8.GetString($bytes)
    [System.IO.File]::WriteAllText($file, $fixedText, $utf8bom)
}
Write-Host "Mojibake Fixed!"
