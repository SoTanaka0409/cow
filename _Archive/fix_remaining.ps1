$basePath = 'C:\Users\Admin\Desktop\progtram\Cow\Source'

$replacements = @(
    @{ File = 'Player3D.cpp'; Old = 'Utility::UI_VACUUM_W';  New = 'Utility::kUiVacuumW' },
    @{ File = 'Player3D.cpp'; Old = 'Utility::UI_VACUUM_H';  New = 'Utility::kUiVacuumH' },
    @{ File = 'Player3D.cpp'; Old = 'Utility::UI_BASE_X';    New = 'Utility::kUiBaseX'   },
    @{ File = 'Player3D.cpp'; Old = 'Utility::UI_VACUUM_Y';  New = 'Utility::kUiVacuumY' },
    @{ File = 'Player3D.cpp'; Old = 'Utility::SCREEN_WIDTH';  New = 'Utility::kScreenWidth'  },
    @{ File = 'Player3D.cpp'; Old = 'Utility::SCREEN_HEIGHT'; New = 'Utility::kScreenHeight' },
    @{ File = 'Fever.cpp'; Old = 'Utility::SCREEN_WIDTH';  New = 'Utility::kScreenWidth'  },
    @{ File = 'Fever.cpp'; Old = 'Utility::SCREEN_HEIGHT'; New = 'Utility::kScreenHeight' },
    @{ File = 'Fever.cpp'; Old = 'GameConstants::COW_GOLD.modelPath';    New = 'GameConstants::kCowGold.model_path'    },
    @{ File = 'Fever.cpp'; Old = 'GameConstants::COW_DEFAULT.modelPath'; New = 'GameConstants::kCowDefault.model_path' },
    @{ File = 'Fever.cpp'; Old = 'GameConstants::ANIMAL_SHEEP.modelPath'; New = 'GameConstants::kAnimalSheep.model_path' },
    @{ File = 'Main.cpp'; Old = 'Utility::SCREEN_WIDTH';  New = 'Utility::kScreenWidth'  },
    @{ File = 'Main.cpp'; Old = 'Utility::SCREEN_HEIGHT'; New = 'Utility::kScreenHeight' }
)

$filesUpdated = @{}
foreach ($r in $replacements) {
    $path = "$basePath\" + $r.File
    $content = [System.IO.File]::ReadAllText($path)
    $newContent = $content.Replace($r.Old, $r.New)
    if ($newContent -ne $content) {
        [System.IO.File]::WriteAllText($path, $newContent)
        if (-not $filesUpdated.ContainsKey($r.File)) {
            $filesUpdated[$r.File] = 0
        }
        $filesUpdated[$r.File]++
        Write-Host ("  " + $r.File + ": replaced '" + $r.Old + "'")
    }
}
Write-Host "Done."
