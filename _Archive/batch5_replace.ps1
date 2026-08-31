$files = Get-ChildItem 'Source' -Include '*.cpp','*.h' -Recurse

$replacements = @(
  # Tatumaki
  @{ Old = 'mPos'; New = 'pos_' },
  @{ Old = 'mVelocity'; New = 'velocity_' },
  @{ Old = 'mMoveTimer'; New = 'move_timer_' },
  @{ Old = 'mSpeed'; New = 'speed_' },
  @{ Old = 'Tatu'; New = 'tatu_' },
  @{ Old = 'mIsCrisis'; New = 'is_crisis_' },
  @{ Old = 'mCurrentScaleRatio'; New = 'current_scale_ratio_' },
  @{ Old = 'mCurrentRadius'; New = 'current_radius_' },
  @{ Old = 'mTatumakiSpawnTimer'; New = 'tatumaki_spawn_timer_' },

  # Fever
  @{ Old = 'dropTime'; New = 'drop_time_' },
  @{ Old = 'DropCount'; New = 'drop_count_' },
  @{ Old = 'mGauge'; New = 'gauge_' },
  @{ Old = 'mTimer'; New = 'timer_' },
  @{ Old = 'mIsFever'; New = 'is_fever_' },
  @{ Old = 'playerStatus'; New = 'player_status_' },
  @{ Old = 'mpFeverPlayer'; New = 'fever_player_' },

  # Bait
  @{ Old = 'mpModel'; New = 'model_' },
  @{ Old = 'mpGameTimer'; New = 'game_timer_' },

  # Score
  @{ Old = 'inputHandle'; New = 'input_handle_' },
  @{ Old = 'playerName'; New = 'player_name_' },
  @{ Old = 'nameInputMode'; New = 'name_input_mode_' },
  @{ Old = 'scoreImage'; New = 'score_image_' },
  @{ Old = 'numberImg'; New = 'number_img_' },
  @{ Old = 'minusImg'; New = 'minus_img_' },
  @{ Old = 'nameIndex'; New = 'name_index_' },
  @{ Old = 'scoreTextImage'; New = 'score_text_image_' },
  @{ Old = 'resultScore'; New = 'result_score_' }
)

$utf8bom = New-Object System.Text.UTF8Encoding $True
$count = 0

foreach ($file in $files) {
  $content = Get-Content $file.FullName -Encoding Default
  $text = $content -join "`r`n"
  $newText = $text

  foreach ($r in $replacements) {
    $regex = '\b' + $r.Old + '\b'
    $newText = $newText -creplace $regex, $r.New
  }
  
  # score is too generic to be replaced with regex safely without colliding with GetScore() or Score()
  if ($file.Name -eq 'Score.h' -or $file.Name -eq 'Score.cpp') {
    $newText = $newText -creplace '\bscore\b', 'score_'
    $newText = $newText -creplace '\branking\b', 'ranking_'
  }

  if ($newText -ne $text) {
    [System.IO.File]::WriteAllText($file.FullName, $newText, $utf8bom)
    Write-Host ("Updated: " + $file.Name)
    $count++
  }
}
Write-Host ("Done. " + $count + " files updated.")
