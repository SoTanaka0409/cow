$enc = [System.Text.Encoding]::GetEncoding("shift_jis")

# Main.cpp
$path = "Source\Main.cpp"
$content = [System.IO.File]::ReadAllText($path, $enc)
$pattern = 'if \(InputManager::CheckDownKey\(KEY_INPUT_F1\)\)\s*\{\s*Master::is_debug_camera_ = !Master::is_debug_camera_;\s*if \(Master::is_debug_camera_\) \{\s*Master::debug_camera_->Initialize\(\);\s*\}\s*\}'
$replace = "if (InputManager::CheckDownKey(KEY_INPUT_F1))`r`n`t`t{`r`n`t`t`tMaster::is_debug_mode_ = !Master::is_debug_mode_;`r`n`t`t`tif (!Master::is_debug_mode_) {`r`n`t`t`t`tMaster::is_debug_camera_ = false;`r`n`t`t`t}`r`n`t`t}`r`n`r`n`t`tif (Master::is_debug_mode_ && InputManager::CheckDownKey(KEY_INPUT_F2))`r`n`t`t{`r`n`t`t`tMaster::is_debug_camera_ = !Master::is_debug_camera_;`r`n`t`t`tif (Master::is_debug_camera_) {`r`n`t`t`t`tMaster::debug_camera_->Initialize();`r`n`t`t`t}`r`n`t`t}"
$content = $content -replace $pattern, $replace
[System.IO.File]::WriteAllText($path, $content, $enc)

# ColliderManager.cpp
$path = "Source\ColliderManager.cpp"
$content = [System.IO.File]::ReadAllText($path, $enc)
$pattern = 'void ColliderManager::Draw\(\)\s*\{\s*\}'
$replace = "void ColliderManager::Draw()`r`n{`r`n`tif (!Master::is_debug_mode_) return;`r`n`tfor (auto* col : collider_list_) {`r`n`t`tif (col != nullptr && !col->IsDeleteFlag()) {`r`n`t`t`tcol->Draw();`r`n`t`t}`r`n`t}`r`n}"
$content = $content -replace $pattern, $replace
[System.IO.File]::WriteAllText($path, $content, $enc)

# GameScene.cpp
$path = "Source\GameScene.cpp"
$content = [System.IO.File]::ReadAllText($path, $enc)
$pattern = 'cow_manager_->Draw\(\);\s*if \(game_manager_->GetGameTimer\(\)'
$replace = "cow_manager_->Draw();`r`n`tServiceLocator::GetColliderManager()->Draw();`r`n`r`n`tif (game_manager_->GetGameTimer()"
$content = $content -replace $pattern, $replace
[System.IO.File]::WriteAllText($path, $content, $enc)

# TutorialScene.cpp
$path = "Source\TutorialScene.cpp"
$content = [System.IO.File]::ReadAllText($path, $enc)
$pattern = 'void TutorialScene::Draw\(\)\s*\{\s*Scene::Draw\(\);\s*\}'
$replace = "void TutorialScene::Draw()`r`n{`r`n`tScene::Draw();`r`n`tServiceLocator::GetColliderManager()->Draw();`r`n}"
$content = $content -replace $pattern, $replace
[System.IO.File]::WriteAllText($path, $content, $enc)

