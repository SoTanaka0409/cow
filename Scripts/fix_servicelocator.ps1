$enc = [System.Text.Encoding]::GetEncoding("shift_jis")

# GameScene.cpp
$path = "Source\GameScene.cpp"
$content = [System.IO.File]::ReadAllText($path, $enc)
$content = $content -replace 'ServiceLocator::GetColliderManager\(\)', 'ColliderManager::GetInstance()'
[System.IO.File]::WriteAllText($path, $content, $enc)

# TutorialScene.cpp
$path = "Source\TutorialScene.cpp"
$content = [System.IO.File]::ReadAllText($path, $enc)
$content = $content -replace 'ServiceLocator::GetColliderManager\(\)', 'ColliderManager::GetInstance()'
[System.IO.File]::WriteAllText($path, $content, $enc)

