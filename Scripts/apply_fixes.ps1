function Replace-InFile {
    param (
        [string]$Path,
        [string]$Find,
        [string]$Replace
    )
    $enc = [System.Text.Encoding]::GetEncoding("shift_jis")
    $content = [System.IO.File]::ReadAllText((Resolve-Path $Path).Path, $enc)
    $newContent = $content.Replace($Find, $Replace)
    [System.IO.File]::WriteAllText((Resolve-Path $Path).Path, $newContent, $enc)
}

$mainFind = @"
		if (InputManager::CheckDownKey(KEY_INPUT_F1))
		{
			Master::is_debug_camera_ = !Master::is_debug_camera_;
			if (Master::is_debug_camera_) {
				Master::debug_camera_->Initialize();
			}
		}
"@
$mainReplace = @"
		if (InputManager::CheckDownKey(KEY_INPUT_F1))
		{
			Master::is_debug_mode_ = !Master::is_debug_mode_;
			if (!Master::is_debug_mode_) {
				Master::is_debug_camera_ = false;
			}
		}
		if (Master::is_debug_mode_ && InputManager::CheckDownKey(KEY_INPUT_F2))
		{
			Master::is_debug_camera_ = !Master::is_debug_camera_;
			if (Master::is_debug_camera_) {
				Master::debug_camera_->Initialize();
			}
		}
"@
Replace-InFile -Path "Source\Main.cpp" -Find $mainFind -Replace $mainReplace

$colFind = @"
void ColliderManager::Draw()
{
}
"@
$colReplace = @"
void ColliderManager::Draw()
{
	if (!Master::is_debug_mode_) return;
	for (auto* col : collider_list_) {
		if (col != nullptr && !col->IsDeleteFlag()) {
			col->Draw();
		}
	}
}
"@
Replace-InFile -Path "Source\ColliderManager.cpp" -Find $colFind -Replace $colReplace

$gameFind = @"
	cow_manager_->Draw();

	if (game_manager_->GetGameTimer()
"@
$gameReplace = @"
	cow_manager_->Draw();
	ServiceLocator::GetColliderManager()->Draw();

	if (game_manager_->GetGameTimer()
"@
Replace-InFile -Path "Source\GameScene.cpp" -Find $gameFind -Replace $gameReplace

$tutFind = @"
void TutorialScene::Draw()
{
	Scene::Draw();
}
"@
$tutReplace = @"
void TutorialScene::Draw()
{
	Scene::Draw();
	ServiceLocator::GetColliderManager()->Draw();
}
"@
Replace-InFile -Path "Source\TutorialScene.cpp" -Find $tutFind -Replace $tutReplace

