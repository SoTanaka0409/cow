$files = Get-ChildItem 'Source' -Include '*.cpp','*.h' -Recurse

$replacements = @(
  # Scene Manager & Scene Types
  @{ Old = '\bSCENE_NONE\b'; New = 'kSceneNone' },
  @{ Old = '\bSCENE_TITLE\b'; New = 'kSceneTitle' },
  @{ Old = '\bSCENE_RULE\b'; New = 'kSceneRule' },
  @{ Old = '\bSCENE_TUTORIAL\b'; New = 'kSceneTutorial' },
  @{ Old = '\bSCENE_LEVEL\b'; New = 'kSceneLevel' },
  @{ Old = '\bSCENE_GAME\b'; New = 'kSceneGame' },
  @{ Old = '\bSCENE_3DHARD\b'; New = 'kScene3DHard' },
  @{ Old = '\bSCENE_RESULT\b'; New = 'kSceneResult' },
  @{ Old = '\bSCENE_RESULTWIN\b'; New = 'kSceneResultWin' },
  @{ Old = '\bSCENE_NORMALRESULTSCENE\b'; New = 'kSceneNormalResult' },
  @{ Old = '\bSCENE_LOADING\b'; New = 'kSceneLoading' },
  @{ Old = '\bSCENE_3D\b'; New = 'kScene3D' },
  @{ Old = '\bmnSceneType\b'; New = 'scene_type_' },
  @{ Old = '\bmnNextSceneType\b'; New = 'next_scene_type_' },
  @{ Old = '\bmpCurrentScene\b'; New = 'current_scene_' },
  @{ Old = '\bSceneHard\b'; New = 'scene_hard_' },
  @{ Old = '\bSceneNormal\b'; New = 'scene_normal_' },

  # Scene
  @{ Old = '\bSceneFade_In\b'; New = 'kSceneFadeIn' },
  @{ Old = '\bSceneFade_Out\b'; New = 'kSceneFadeOut' },
  @{ Old = '\bSceneFade_None\b'; New = 'kSceneFadeNone' },
  @{ Old = '\bSceneFade_Load\b'; New = 'kSceneFadeLoad' },
  @{ Old = '\bmFadeState\b'; New = 'fade_state_' },
  @{ Old = '\bmNextScene\b'; New = 'next_scene_' },
  @{ Old = '\bmpFever\b'; New = 'fever_' },
  @{ Old = '\bmpAnimalManager\b'; New = 'animal_manager_' },
  @{ Old = '\bmpCowManager\b'; New = 'cow_manager_' },
  @{ Old = '\bmpGameManager\b'; New = 'game_manager_' },
  @{ Old = '\bmpObjectManager\b'; New = 'object_manager_' },
  @{ Old = '\bmpColliderManager\b'; New = 'collider_manager_' },
  @{ Old = '\bmfFadeAlpha\b'; New = 'fade_alpha_' },
  @{ Old = '\bmfFadeSpeed\b'; New = 'fade_speed_' },

  # Scene3D Phase enum and vars
  @{ Old = '\bwarningRadius\b'; New = 'warning_radius_' },
  @{ Old = '\bmaxRadius\b'; New = 'max_radius_' },
  @{ Old = '\bmMassSpawnTimer\b'; New = 'mass_spawn_timer_' },
  @{ Old = '\bmpPhase\b'; New = 'phase_' },
  @{ Old = '\bmFontBackGraph\b'; New = 'font_back_graph_' },

  # LoadingScene
  @{ Old = '\bmLoadingTimer\b'; New = 'loading_timer_' },
  @{ Old = '\bmbLoadStarted\b'; New = 'load_started_' },

  # TitleScene
  @{ Old = '\bmFrameCount\b'; New = 'frame_count_' },
  @{ Old = '\bmnTitleGraphHandle\b'; New = 'title_graph_handle_' },
  @{ Old = '\bmnNewGame\b'; New = 'new_game_' },
  @{ Old = '\bmnTutorial\b'; New = 'tutorial_' },
  @{ Old = '\bmnOperationProcedures\b'; New = 'operation_procedures_' },
  @{ Old = '\brankingTitleImage\b'; New = 'ranking_title_image_' },
  @{ Old = '\bpointImg\b'; New = 'point_img_' },
  @{ Old = '\bmnTitleSelect\b'; New = 'title_select_' },
  @{ Old = '\bmNewGameX\b'; New = 'new_game_x_' },
  @{ Old = '\bmNewGameY\b'; New = 'new_game_y_' },
  @{ Old = '\bmNewGameW\b'; New = 'new_game_w_' },
  @{ Old = '\bmNewGameH\b'; New = 'new_game_h_' },
  @{ Old = '\bmbIsHoverNewGame\b'; New = 'is_hover_new_game_' },
  @{ Old = '\bmButtons\b'; New = 'buttons_' },
  @{ Old = '\brankImage\b'; New = 'rank_image_' },
  @{ Old = '\bmnUfoGraphHandle\b'; New = 'ufo_graph_handle_' },
  @{ Old = '\bmUfoX\b'; New = 'ufo_x_' },
  @{ Old = '\bmUfoY\b'; New = 'ufo_y_' },
  @{ Old = '\bmUfoW\b'; New = 'ufo_w_' },
  @{ Old = '\bmUfoH\b'; New = 'ufo_h_' },
  @{ Old = '\bmbIsDraggingUfo\b'; New = 'is_dragging_ufo_' },
  @{ Old = '\bmOffsetX\b'; New = 'offset_x_' },
  @{ Old = '\bmOffsetY\b'; New = 'offset_y_' },
  @{ Old = '\bmCowVoiceTimer\b'; New = 'cow_voice_timer_' },
  @{ Old = '\bmUfoVX\b'; New = 'ufo_vx_' },
  @{ Old = '\bmUfoVY\b'; New = 'ufo_vy_' },
  @{ Old = '\bmUfoAngle\b'; New = 'ufo_angle_' },
  @{ Old = '\bmIsAutoPatrol\b'; New = 'is_auto_patrol_' },
  @{ Old = '\bmAutoPatrolTimer\b'; New = 'auto_patrol_timer_' },

  # TitleButton / ResultButton struct members
  @{ Old = '\bgraphHandle\b'; New = 'graph_handle' },
  @{ Old = '\bisHover\b'; New = 'is_hover' },

  # ResultScene
  @{ Old = '\bResultGraphHandle\b'; New = 'result_graph_handle_' },
  @{ Old = '\bmScore\b'; New = 'score_' },
  @{ Old = '\byourScoreImage\b'; New = 'your_score_image_' },
  @{ Old = '\bpointImage\b'; New = 'point_image_' },
  @{ Old = '\byourScoreTextImg\b'; New = 'your_score_text_img_' },
  @{ Old = '\bmnCount\b'; New = 'count_' },
  @{ Old = '\bmFlag\b'; New = 'flag_' },

  # TutorialScene
  @{ Old = '\bSTATE_MOVE\b'; New = 'kStateMove' },
  @{ Old = '\bSTATE_BEAM\b'; New = 'kStateBeam' },
  @{ Old = '\bSTATE_COMBO_SCORE\b'; New = 'kStateComboScore' },
  @{ Old = '\bSTATE_PHASE\b'; New = 'kStatePhase' },
  @{ Old = '\bSTATE_SKILL\b'; New = 'kStateSkill' },
  @{ Old = '\bSTATE_FEVER\b'; New = 'kStateFever' },
  @{ Old = '\bSTATE_END\b'; New = 'kStateEnd' },
  @{ Old = '\bFEVER_1\b'; New = 'kFever1' },
  @{ Old = '\bFEVER_2\b'; New = 'kFever2' },
  @{ Old = '\bFEVER_3\b'; New = 'kFever3' },
  @{ Old = '\bCOW_COUNT\b'; New = 'cow_count_' },
  @{ Old = '\bfontHandle\b'; New = 'font_handle_' },
  @{ Old = '\btimerCount\b'; New = 'timer_count_' },
  @{ Old = '\bmfeverstate\b'; New = 'fever_state_' },
  @{ Old = '\bmpTexture4\b'; New = 'texture4_' },
  @{ Old = '\bmpTexture5\b'; New = 'texture5_' },
  @{ Old = '\bmpTexture6\b'; New = 'texture6_' },
  @{ Old = '\bthunder\b'; New = 'thunder_' },
  @{ Old = '\bmpCamera\b'; New = 'camera_' },
  @{ Old = '\bmpCombo\b'; New = 'combo_' },
  @{ Old = '\bmIsCowSucked\b'; New = 'is_cow_sucked_' },
  @{ Old = '\bCowDelete\b'; New = 'cow_delete_' },
  @{ Old = '\bmbSkillFlag\b'; New = 'skill_flag_' },
  @{ Old = '\bmbfeverFlag\b'; New = 'fever_flag_' }
)

$utf8bom = New-Object System.Text.UTF8Encoding $True
$count = 0

foreach ($file in $files) {
  $text = [System.IO.File]::ReadAllText($file.FullName, [System.Text.Encoding]::Default)
  $newText = $text

  foreach ($r in $replacements) {
    if ($r.Old.StartsWith('\b')) {
      $newText = $newText -creplace $r.Old, $r.New
    } else {
      $regex = '\b' + [regex]::Escape($r.Old) + '\b'
      $newText = $newText -creplace $regex, $r.New
    }
  }

  if ($newText -ne $text) {
    [System.IO.File]::WriteAllText($file.FullName, $newText, $utf8bom)
    Write-Host ("Updated: " + $file.Name)
    $count++
  }
}
Write-Host ("Done. " + $count + " files updated.")

# Scene3D.h Phase enum definition specific fix
$f1 = 'Source\Scene3D.h'
$t1 = Get-Content $f1 -Encoding Default -Raw
$t1 = $t1 -replace '(?m)^\s*Normal,', "`t`tkNormal,"
$t1 = $t1 -replace '(?m)^\s*TornadoCrisis,', "`t`tkTornadoCrisis,"
$t1 = $t1 -replace '(?m)^\s*MobSpawn,', "`t`tkMobSpawn,"
[System.IO.File]::WriteAllText($f1, $t1, $utf8bom)
