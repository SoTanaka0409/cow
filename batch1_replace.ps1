$files = Get-ChildItem 'Source' -Include '*.cpp','*.h' -Recurse |
         Where-Object { $_.Name -notmatch '\.utf8\.txt$' }

$replacements = @(
  # Utility constants
  @{ Old = 'Utility::SCREEN_WIDTH';  New = 'Utility::kScreenWidth'  },
  @{ Old = 'Utility::SCREEN_HEIGHT'; New = 'Utility::kScreenHeight' },
  @{ Old = 'Utility::UI_BASE_X';     New = 'Utility::kUiBaseX'      },
  @{ Old = 'Utility::UI_DIGIT_X';    New = 'Utility::kUiDigitX'     },
  @{ Old = 'Utility::UI_PANEL_W';    New = 'Utility::kUiPanelW'     },
  @{ Old = 'Utility::UI_PANEL_H';    New = 'Utility::kUiPanelH'     },
  @{ Old = 'Utility::UI_TIMER_Y';    New = 'Utility::kUiTimerY'     },
  @{ Old = 'Utility::UI_SCORE_Y';    New = 'Utility::kUiScoreY'     },
  @{ Old = 'Utility::UI_COMBO_Y';    New = 'Utility::kUiComboY'     },
  @{ Old = 'Utility::UI_VACUUM_Y';   New = 'Utility::kUiVacuumY'    },
  @{ Old = 'Utility::UI_VACUUM_W';   New = 'Utility::kUiVacuumW'    },
  @{ Old = 'Utility::UI_VACUUM_H';   New = 'Utility::kUiVacuumH'    },
  @{ Old = 'Utility::UI_LEVEL_Y';    New = 'Utility::kUiLevelY'     },
  @{ Old = 'Utility::UI_LEVEL_W';    New = 'Utility::kUiLevelW'     },
  @{ Old = 'Utility::UI_LEVEL_H';    New = 'Utility::kUiLevelH'     },
  # GameConstants names + member
  @{ Old = 'GameConstants::COW_DEFAULT.modelPath';           New = 'GameConstants::kCowDefault.model_path'          },
  @{ Old = 'GameConstants::COW_2.modelPath';                 New = 'GameConstants::kCow2.model_path'                },
  @{ Old = 'GameConstants::COW_GOLD.modelPath';              New = 'GameConstants::kCowGold.model_path'             },
  @{ Old = 'GameConstants::COW_TUTORIAL.modelPath';          New = 'GameConstants::kCowTutorial.model_path'         },
  @{ Old = 'GameConstants::ANIMAL_SHEEP.modelPath';          New = 'GameConstants::kAnimalSheep.model_path'         },
  @{ Old = 'GameConstants::ANIMAL_CHICKEN.modelPath';        New = 'GameConstants::kAnimalChicken.model_path'       },
  @{ Old = 'GameConstants::ANIMAL_BEAR.modelPath';           New = 'GameConstants::kAnimalBear.model_path'          },
  @{ Old = 'GameConstants::COW_DEFAULT.deathTimeHeight';     New = 'GameConstants::kCowDefault.death_time_height'   },
  @{ Old = 'GameConstants::COW_TUTORIAL.deathTimeHeight';    New = 'GameConstants::kCowTutorial.death_time_height'  },
  @{ Old = 'GameConstants::COW_DEFAULT.score';               New = 'GameConstants::kCowDefault.score'               },
  @{ Old = 'GameConstants::COW_DEFAULT.xp';                  New = 'GameConstants::kCowDefault.xp'                  },
  @{ Old = 'GameConstants::ANIMAL_SHEEP.speed';              New = 'GameConstants::kAnimalSheep.speed'              },
  @{ Old = 'GameConstants::ANIMAL_SHEEP.score';              New = 'GameConstants::kAnimalSheep.score'              },
  @{ Old = 'GameConstants::ANIMAL_SHEEP.xp';                 New = 'GameConstants::kAnimalSheep.xp'                 },
  @{ Old = 'GameConstants::ANIMAL_SHEEP.deathTimeHeight';    New = 'GameConstants::kAnimalSheep.death_time_height'  }
)

$count = 0
foreach ($file in $files) {
  $content = [System.IO.File]::ReadAllText($file.FullName)
  $newContent = $content
  foreach ($r in $replacements) {
    $newContent = $newContent.Replace($r.Old, $r.New)
  }
  if ($newContent -ne $content) {
    [System.IO.File]::WriteAllText($file.FullName, $newContent)
    Write-Host ('Updated: ' + $file.Name)
    $count++
  }
}
Write-Host ('Done. ' + $count + ' files updated.')
