$files = Get-ChildItem 'Source' -Include '*.cpp','*.h' -Recurse

$replacements = @(
  # GameManager
  @{ Old = 'mData';             New = 'data_' },
  @{ Old = 'mnType';            New = 'type_' },
  @{ Old = 'mCurrentPhase';     New = 'current_phase_' },
  @{ Old = 'Fadetimer';         New = 'fade_timer_' },
  @{ Old = 'Fadeflag';          New = 'fade_flag_' },
  @{ Old = 'm_PhaseTimer';      New = 'phase_timer_' },
  @{ Old = 'm_PhaseChangeCount';New = 'phase_change_count_' },

  # GameManager::GameStepType enum
  @{ Old = 'game_CowGet'; New = 'kCowGet' },
  @{ Old = 'game_final';  New = 'kFinal' },

  # GamePhase enum
  @{ Old = 'GamePhase::Normal';        New = 'GamePhase::kNormal' },
  @{ Old = 'GamePhase::MassSpawn';     New = 'GamePhase::kMassSpawn' },
  @{ Old = 'GamePhase::TornadoCrisis'; New = 'GamePhase::kTornadoCrisis' },

  # Rule
  @{ Old = 'mRuleGraph';       New = 'rule_graph_' },
  @{ Old = 'mFontHandle';      New = 'font_handle_' },
  @{ Old = 'mTitleFontHandle'; New = 'title_font_handle_' },
  @{ Old = 'mSelectedIndex';   New = 'selected_index_' },
  @{ Old = 'mPlaySeDelay';     New = 'play_se_delay_' },
  @{ Old = 'mSceneFrames';     New = 'scene_frames_' },

  # Rule MenuType enum
  @{ Old = 'MENU_BGM';  New = 'kMenuBgm' },
  @{ Old = 'MENU_SE';   New = 'kMenuSe' },
  @{ Old = 'MENU_BACK'; New = 'kMenuBack' },
  @{ Old = 'MENU_MAX';  New = 'kMenuMax' },

  # Combo
  @{ Old = 'comboCount';     New = 'combo_count_' },
  @{ Old = 'comboTimer';     New = 'combo_timer_' },
  @{ Old = 'comboMaxTime';   New = 'combo_max_time_' },
  @{ Old = 'comboImage';     New = 'combo_image_' },
  @{ Old = 'comboShow';      New = 'combo_show_' },
  @{ Old = 'comboShowTimer'; New = 'combo_show_timer_' },

  # Skill
  @{ Old = 'AddSkillFlag'; New = 'add_skill_flag_' },
  @{ Old = 'mpParent';     New = 'parent_' },
  @{ Old = 'mpTexture2';   New = 'texture2_' },
  @{ Old = 'mpTexture3';   New = 'texture3_' },
  @{ Old = 'mpTexture';    New = 'texture_' },
  @{ Old = 'StatusDate';   New = 'status_date_' },
  @{ Old = 'Status_A';     New = 'status_a_' },
  @{ Old = 'Status_S';     New = 'status_s_' },
  @{ Old = 'mHoverSkill';  New = 'hover_skill_' },
  @{ Old = 'mFlashAlpha';  New = 'flash_alpha_' },
  @{ Old = 'mFlash';       New = 'flash_' },
  @{ Old = 'mSelectAnim';  New = 'select_anim_' },
  @{ Old = 'mSelectedSkill';New = 'selected_skill_' },
  @{ Old = 'mSelectScale'; New = 'select_scale_' },
  @{ Old = 'mSelectPos';   New = 'select_pos_' },
  @{ Old = 'mPos1';        New = 'pos1_' },
  @{ Old = 'mPos2';        New = 'pos2_' },
  @{ Old = 'mPos3';        New = 'pos3_' },
  @{ Old = 'mOpenAnim';    New = 'open_anim_' },
  @{ Old = 'mCard1Y';      New = 'card1_y_' },
  @{ Old = 'mCard2Y';      New = 'card2_y_' },
  @{ Old = 'mCard3Y';      New = 'card3_y_' },

  # Skill SkillTag enum
  @{ Old = 'Skill_1'; New = 'kSkill1' },
  @{ Old = 'Skill_2'; New = 'kSkill2' },
  @{ Old = 'Skill_3'; New = 'kSkill3' },

  # Skill StatusTag enum
  @{ Old = 'Status_AttackSpeed'; New = 'kStatusAttackSpeed' },
  @{ Old = 'Status_Speed';       New = 'kStatusSpeed' },

  # Skill field
  @{ Old = 'SkillTag tag\b';  New = 'SkillTag tag_' }
)

$utf8bom = New-Object System.Text.UTF8Encoding $True
$count = 0

foreach ($file in $files) {
  $text = [System.IO.File]::ReadAllText($file.FullName, [System.Text.Encoding]::Default)
  $newText = $text

  foreach ($r in $replacements) {
    if ($r.Old.Contains('::') -or $r.Old.EndsWith('\b')) {
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
