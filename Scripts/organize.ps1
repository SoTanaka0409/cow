$ErrorActionPreference = "Stop"

$folders = @{
    "System" = @("Main", "GameManager", "Master", "GameConstants", "test_constants", "GameTimer", "Level", "Score", "Combo", "Fever", "Rule", "Utility", "ServiceLocator")
    "Scene" = @("Scene", "SceneManager", "TitleScene", "GameScene", "ResultScene", "LoadingScene", "TutorialScene")
    "Object" = @("Object2D", "Object3D", "ObjectManager")
    "Actor" = @("CharacterMove", "CharacterState", "Player3D")
    "Animal" = @("Animal", "AnimalMove", "AnimalManager", "CreatureManager", "Cow", "Cow_2", "CowMove", "CowManager", "GoldCow", "Cow_Tutorial")
    "Stage" = @("Stage", "StageLoader", "Object_Stage", "Floor", "Mountain", "Wall", "SkyBox", "Tornado", "Thunder", "Bait")
    "Camera" = @("Camera", "DebugCamera")
    "Collision" = @("Collider", "SphereCollider", "CapsuleCollider", "ColliderManager")
    "Graphics" = @("Model", "ModelAnimation", "ModelUtility", "Texture", "EffectManager", "EffekseerEffect")
    "UI_Input_Sound" = @("UIButton", "InputManager", "ResourceManager", "SoundManager", "SelectionManager", "Skill")
}

$vcxprojPath = "Project1.vcxproj"
$filtersPath = "Project1.vcxproj.filters"

$utf8Bom = New-Object System.Text.UTF8Encoding $true
$vcxprojContent = [System.IO.File]::ReadAllText($vcxprojPath, $utf8Bom)
$filtersContent = [System.IO.File]::ReadAllText($filtersPath, $utf8Bom)

$newFilters = @()

foreach ($folder in $folders.Keys) {
    $dir = "Source\$folder"
    if (-not (Test-Path $dir)) {
        New-Item -ItemType Directory -Path $dir | Out-Null
    }
    
    $guid1 = [guid]::NewGuid().ToString("B").ToUpper()
    $guid2 = [guid]::NewGuid().ToString("B").ToUpper()
    
    $newFilters += "    <Filter Include=`"ソース ファイル\$folder`">`r`n      <UniqueIdentifier>$guid1</UniqueIdentifier>`r`n    </Filter>"
    $newFilters += "    <Filter Include=`"ヘッダー ファイル\$folder`">`r`n      <UniqueIdentifier>$guid2</UniqueIdentifier>`r`n    </Filter>"
    
    foreach ($file in $folders[$folder]) {
        $cpp = "Source\$file.cpp"
        $h = "Source\$file.h"
        
        if (Test-Path $cpp) {
            Move-Item -Force $cpp "$dir\$file.cpp"
            
            $vcxprojContent = $vcxprojContent -replace "(?i)Include=`"Source\\$file\.cpp`"", "Include=`"Source\$folder\$file.cpp`""
            
            $filtersContent = $filtersContent -replace "(?i)Include=`"Source\\$file\.cpp`"", "Include=`"Source\$folder\$file.cpp`""
            $filtersContent = [regex]::Replace($filtersContent, "(?i)(<ClCompile Include=`"Source\\$folder\\$file\.cpp`">\s*<Filter>)ソース ファイル(</Filter>)", "`${1}ソース ファイル\$folder`$2")
        }
        if (Test-Path $h) {
            Move-Item -Force $h "$dir\$file.h"
            
            $vcxprojContent = $vcxprojContent -replace "(?i)Include=`"Source\\$file\.h`"", "Include=`"Source\$folder\$file.h`""
            
            $filtersContent = $filtersContent -replace "(?i)Include=`"Source\\$file\.h`"", "Include=`"Source\$folder\$file.h`""
            $filtersContent = [regex]::Replace($filtersContent, "(?i)(<ClInclude Include=`"Source\\$folder\\$file\.h`">\s*<Filter>)ヘッダー ファイル(</Filter>)", "`${1}ヘッダー ファイル\$folder`$2")
        }
    }
}

# Insert new filters into the filters file before the first closing </ItemGroup>
$filtersContent = [regex]::Replace($filtersContent, "(?s)(<ItemGroup>.*?</ItemGroup>)", {
    param($match)
    if ($match.Value -match "ソース ファイル") {
        return $match.Value -replace "</ItemGroup>", (($newFilters -join "`r`n") + "`r`n  </ItemGroup>")
    }
    return $match.Value
}, 1) # Only replace the first match which defines the filters

[System.IO.File]::WriteAllText($vcxprojPath, $vcxprojContent, $utf8Bom)
[System.IO.File]::WriteAllText($filtersPath, $filtersContent, $utf8Bom)
Write-Host "Migration complete!"
