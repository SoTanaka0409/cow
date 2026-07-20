$enc = [System.Text.Encoding]::GetEncoding("shift_jis")

# 1. Update EffectManager.h
$path = "Source\EffectManager.h"
$content = [System.IO.File]::ReadAllText($path, $enc)
$content = $content -replace 'void Draw\(\);', "void Draw();`r`n`r`n`tvoid PlayCowDeathEffect(VECTOR pos);"
$content = $content -replace 'int playing_effect_handle_;', "int playing_effect_handle_;`r`n`tint cow_death_effect_handle_;"
[System.IO.File]::WriteAllText($path, $content, $enc)

# 2. Update EffectManager.cpp
$path = "Source\EffectManager.cpp"
$content = [System.IO.File]::ReadAllText($path, $enc)
$content = $content -replace 'EffectManager::~EffectManager\(\)\r?\n\{', "EffectManager::~EffectManager()`r`n{`r`n`tDeleteEffekseerEffect(cow_death_effect_handle_);"
$content = $content -replace 'SetWriteZBuffer3D\(TRUE\);\r?\n\}', "SetWriteZBuffer3D(TRUE);`r`n`tcow_death_effect_handle_ = LoadEffekseerEffect(`"Resource/3D/エフェクト/牛吸い込み.efk`");`r`n}"
$content += "`r`nvoid EffectManager::PlayCowDeathEffect(VECTOR pos)`r`n{`r`n`tint playing_handle = PlayEffekseer3DEffect(cow_death_effect_handle_);`r`n`tSetPosPlayingEffekseer3DEffect(playing_handle, pos.x, pos.y, pos.z);`r`n`tSetScalePlayingEffekseer3DEffect(playing_handle, 1.0f, 1.0f, 1.0f);`r`n}`r`n"
[System.IO.File]::WriteAllText($path, $content, $enc)

# 3. Update AnimalMove.cpp
$path = "Source\AnimalMove.cpp"
$content = [System.IO.File]::ReadAllText($path, $enc)
$content = $content -replace 'case DEATH_VACUUM:\r?\n\tcase DEATH_BAIT:\r?\n\t\tif \(player != nullptr\)', "case DEATH_VACUUM:`r`n`tcase DEATH_BAIT:`r`n`t`tMaster::sound_manager_->PlaySE(SoundManager::kSeCow);`r`n`t`tMaster::effect_manager_->PlayCowDeathEffect(position_);`r`n`r`n`t`tif (player != nullptr)"
[System.IO.File]::WriteAllText($path, $content, $enc)

