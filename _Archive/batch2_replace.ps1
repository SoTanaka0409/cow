$files = Get-ChildItem 'Source' -Include '*.cpp','*.h' -Recurse

$replacements = @(
  @{ Old = 'mbIsPhaseCameraActive'; New = 'is_phase_camera_active_' },
  @{ Old = 'mvPosition';            New = 'position_' },
  @{ Old = 'mvLookAtPosition';      New = 'look_at_position_' },
  @{ Old = 'dir';                   New = 'dir_' },
  @{ Old = 'mpTarget';              New = 'target_' },
  @{ Old = 'ROTATE_SPEED';          New = 'kRotateSpeed' },
  @{ Old = 'mfTargetAngle';         New = 'target_angle_' },
  @{ Old = 'mfAngle';               New = 'angle_' },
  @{ Old = 'centerX';               New = 'center_x_' },
  @{ Old = 'centerY';               New = 'center_y_' },
  @{ Old = 'mouseX';                New = 'mouse_x_' },
  @{ Old = 'mouseY';                New = 'mouse_y_' },
  @{ Old = 'mMouseX';               New = 'current_mouse_x_' },
  @{ Old = 'mMouseY';               New = 'current_mouse_y_' },
  @{ Old = 'mPrevMouseX';           New = 'prev_mouse_x_' },
  @{ Old = 'mPrevMouseY';           New = 'prev_mouse_y_' },
  @{ Old = 'mnShakeTime';           New = 'shake_time_' },
  @{ Old = 'mnShakeTimeCount';      New = 'shake_time_count_' },
  @{ Old = 'mfShakeAngle';          New = 'shake_angle_' },
  @{ Old = 'mfShakeTimeCounter';    New = 'shake_time_counter_' },
  @{ Old = 'mfShakeWidth';          New = 'shake_width_' },
  @{ Old = 'mfShakeAngleSpeed';     New = 'shake_angle_speed_' },
  @{ Old = 'mfStepTime';            New = 'step_time_' },
  @{ Old = 'mvShakePosition';       New = 'shake_position_' },
  @{ Old = 'mfHorizontalAngle';     New = 'horizontal_angle_' },
  @{ Old = 'mfVerticalAngle';       New = 'vertical_angle_' },

  @{ Old = 'mDownBuffer';           New = 'down_buffer_' },

  @{ Old = 'SOUND_BGM';             New = 'SoundBgm' },
  @{ Old = 'SOUND_SE';              New = 'SoundSe' },
  @{ Old = 'BGM_TITLE';             New = 'kBgmTitle' },
  @{ Old = 'BGM_GAME';              New = 'kBgmGame' },
  @{ Old = 'BGM_RESULT';            New = 'kBgmResult' },
  @{ Old = 'BGM_TUTORIAL';          New = 'kBgmTutorial' },
  @{ Old = 'BGM_RULE';              New = 'kBgmRule' },
  @{ Old = 'SE_DECIDE';             New = 'kSeDecide' },
  @{ Old = 'SE_COW';                New = 'kSeCow' },
  @{ Old = 'SE_BEAM';               New = 'kSeBeam' },
  @{ Old = 'SE_SKILL_STATUS';       New = 'kSeSkillStatus' },
  @{ Old = 'SE_SKILL_FOOD';         New = 'kSeSkillFood' },
  @{ Old = 'SE_CHICKEN';            New = 'kSeChicken' },
  @{ Old = 'SE_TATUMAKI';           New = 'kSeTatumaki' },
  @{ Old = 'SE_KAMINARI';           New = 'kSeKaminari' },
  @{ Old = 'SE_TutorialChange';     New = 'kSeTutorialChange' },
  @{ Old = 'SE_BAITFINAL';          New = 'kSeBaitFinal' },
  @{ Old = 'mMasterBGMVolume';      New = 'master_bgm_volume_' },
  @{ Old = 'mMasterSEVolume';       New = 'master_se_volume_' },
  @{ Old = 'mnNowPlayingBgm';       New = 'now_playing_bgm_' },
  @{ Old = 'mnNowPlayingSe';        New = 'now_playing_se_' },
  @{ Old = 'mnBgmHandleList';       New = 'bgm_handle_list_' },
  @{ Old = 'mnSeHandleList';        New = 'se_handle_list_' },

  @{ Old = 'filePath';              New = 'file_path_' },
  @{ Old = 'divHandleList';         New = 'div_handle_list_' },
  @{ Old = 'divX';                  New = 'div_x_' },
  @{ Old = 'divY';                  New = 'div_y_' },
  @{ Old = 'allNum';                New = 'all_num_' },
  @{ Old = 'resourceMapList';       New = 'resource_map_list_' },
  @{ Old = 'graphicResourceMapList';New = 'graphic_resource_map_list_' },
  @{ Old = 'divGraphicResourceMapList'; New = 'div_graphic_resource_map_list_' },

  @{ Old = 'PlayPos';               New = 'play_pos_' },
  @{ Old = 'EffectParticleLimit';   New = 'kEffectParticleLimit' },
  @{ Old = 'EffectSize';            New = 'kEffectSize' },
  @{ Old = 'EffectPlayerInterval';  New = 'kEffectPlayerInterval' },
  @{ Old = 'EffectMoveSpeed';       New = 'kEffectMoveSpeed' },
  @{ Old = 'effectResourcehandle';  New = 'effect_resource_handle_' },
  @{ Old = 'playingEffecthandle';   New = 'playing_effect_handle_' }
)

$utf8bom = New-Object System.Text.UTF8Encoding $True
$count = 0

foreach ($file in $files) {
  $content = Get-Content $file.FullName -Encoding Default
  $text = $content -join "`r`n"
  $newText = $text

  foreach ($r in $replacements) {
    # -creplace specifies case-sensitive regex replacement
    # we use \b to match whole words so we don't accidentally replace substrings
    $regex = '\b' + $r.Old + '\b'
    $newText = $newText -creplace $regex, $r.New
  }

  if ($newText -ne $text) {
    [System.IO.File]::WriteAllText($file.FullName, $newText, $utf8bom)
    Write-Host ("Updated: " + $file.Name)
    $count++
  }
}
Write-Host ("Done. " + $count + " files updated.")
