$enc = [System.Text.Encoding]::GetEncoding("shift_jis")

# 1. Revert AnimalMove.cpp
$path = "Source\AnimalMove.cpp"
$content = [System.IO.File]::ReadAllText($path, $enc)
$content = $content -replace "case DEATH_VACUUM:`r`n`tcase DEATH_BAIT:`r`n`t`tMaster::sound_manager_->PlaySE\(SoundManager::kSeCow\);`r`n`t`tMaster::effect_manager_->PlayCowDeathEffect\(position_\);`r`n`r`n`t`tif \(player != nullptr\)", "case DEATH_VACUUM:`r`n`tcase DEATH_BAIT:`r`n`t`tif (player != nullptr)"
[System.IO.File]::WriteAllText($path, $content, $enc)

# 2. Update CowMove.cpp
$path = "Source\CowMove.cpp"
$content = [System.IO.File]::ReadAllText($path, $enc)
$content = $content -replace "case DEATH_VACUUM:`r`n`t`tif \(player != nullptr\)", "case DEATH_VACUUM:`r`n`t`tMaster::sound_manager_->PlaySE(SoundManager::kSeCow);`r`n`t`tMaster::effect_manager_->PlayCowDeathEffect(position_);`r`n`t`tif (player != nullptr)"
$content = $content -replace "case DEATH_BAIT:`r`n`t`tif \(player != nullptr\)", "case DEATH_BAIT:`r`n`t`tMaster::sound_manager_->PlaySE(SoundManager::kSeCow);`r`n`t`tMaster::effect_manager_->PlayCowDeathEffect(position_);`r`n`t`tif (player != nullptr)"
[System.IO.File]::WriteAllText($path, $content, $enc)

