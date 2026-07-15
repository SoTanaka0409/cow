$enc932 = [System.Text.Encoding]::GetEncoding(932)
$encUtf8 = New-Object System.Text.UTF8Encoding $true
$sourceDir = "c:\Users\Admin\Desktop\progtram\Cow\Source"

$files = Get-ChildItem -Path $sourceDir -Include "*.h","*.cpp" -File | Where-Object { $_.Name -notlike "*.txt" }

$changedFiles = 0
foreach ($file in $files) {
    # Try UTF-8 with BOM first, then Shift-JIS (CP932)
    $rawBytes = [System.IO.File]::ReadAllBytes($file.FullName)
    if ($rawBytes.Length -ge 3 -and $rawBytes[0] -eq 0xEF -and $rawBytes[1] -eq 0xBB -and $rawBytes[2] -eq 0xBF) {
        $readEnc = $encUtf8
    } else {
        $readEnc = $enc932
    }

    $content = [System.IO.File]::ReadAllText($file.FullName, $readEnc)
    $newContent = $content

    # === Master:: static members (with prefix) ===
    $newContent = $newContent.Replace("Master::mpSceneManager",    "Master::scene_manager_")
    $newContent = $newContent.Replace("Master::mpSoundManager",    "Master::sound_manager_")
    $newContent = $newContent.Replace("Master::mpResourceManager", "Master::resource_manager_")
    $newContent = $newContent.Replace("Master::mpDebugCamera",     "Master::debug_camera_")
    $newContent = $newContent.Replace("Master::mpEffectManager",   "Master::effect_manager_")
    $newContent = $newContent.Replace("Master::mpScore",           "Master::score_")
    $newContent = $newContent.Replace("Master::mpLevel",           "Master::level_")
    $newContent = $newContent.Replace("Master::mbIsDebugCamera",   "Master::is_debug_camera_")
    $newContent = $newContent.Replace("Master::mfDeltaTime",       "Master::delta_time_")
    $newContent = $newContent.Replace("Master::mnTutorialcount",   "Master::tutorial_count_")
    $newContent = $newContent.Replace("Master::mnCaughtCowCount",  "Master::caught_cow_count_")
    $newContent = $newContent.Replace("Master::GameFinishFlag",    "Master::game_finish_flag_")
    $newContent = $newContent.Replace("Master::FeverFlag",         "Master::fever_flag_")
    $newContent = $newContent.Replace("Master::SelectSkill",       "Master::select_select_skill_")
    # fix double replacement
    $newContent = $newContent.Replace("Master::select_select_skill_", "Master::select_skill_")

    # === Master.h class body static declarations ===
    $newContent = $newContent.Replace("static SceneManager* mpSceneManager",     "static SceneManager* scene_manager_")
    $newContent = $newContent.Replace("static SoundManager* mpSoundManager",     "static SoundManager* sound_manager_")
    $newContent = $newContent.Replace("static ResourceManager* mpResourceManager","static ResourceManager* resource_manager_")
    $newContent = $newContent.Replace("static DebugCamera* mpDebugCamera",       "static DebugCamera* debug_camera_")
    $newContent = $newContent.Replace("static Score* mpScore",                   "static Score* score_")
    $newContent = $newContent.Replace("static Level* mpLevel",                   "static Level* level_")
    $newContent = $newContent.Replace("static EffectManager* mpEffectManager",   "static EffectManager* effect_manager_")
    $newContent = $newContent.Replace("static bool mbIsDebugCamera",             "static bool is_debug_camera_")
    $newContent = $newContent.Replace("static float mfDeltaTime",                "static float delta_time_")
    $newContent = $newContent.Replace("static int mnTutorialcount",              "static int tutorial_count_")
    $newContent = $newContent.Replace("static bool GameFinishFlag",              "static bool game_finish_flag_")
    $newContent = $newContent.Replace("static bool FeverFlag",                   "static bool fever_flag_")
    $newContent = $newContent.Replace("static int mnCaughtCowCount",             "static int caught_cow_count_")
    $newContent = $newContent.Replace("static bool SelectSkill",                 "static bool select_skill_")
    $newContent = $newContent.Replace("return mfDeltaTime * 60.0f",              "return delta_time_ * 60.0f")

    # === Main.cpp definitions ===
    $newContent = $newContent.Replace("SceneManager* Master::mpSceneManager",     "SceneManager* Master::scene_manager_")
    $newContent = $newContent.Replace("SoundManager* Master::mpSoundManager",     "SoundManager* Master::sound_manager_")
    $newContent = $newContent.Replace("ResourceManager* Master::mpResourceManager","ResourceManager* Master::resource_manager_")
    $newContent = $newContent.Replace("DebugCamera* Master::mpDebugCamera",       "DebugCamera* Master::debug_camera_")
    $newContent = $newContent.Replace("Score* Master::mpScore",                   "Score* Master::score_")
    $newContent = $newContent.Replace("Level* Master::mpLevel",                   "Level* Master::level_")
    $newContent = $newContent.Replace("EffectManager* Master::mpEffectManager",   "EffectManager* Master::effect_manager_")
    $newContent = $newContent.Replace("bool Master::mbIsDebugCamera",             "bool Master::is_debug_camera_")
    $newContent = $newContent.Replace("float Master::mfDeltaTime",                "float Master::delta_time_")
    $newContent = $newContent.Replace("int Master::mnTutorialcount",              "int Master::tutorial_count_")
    $newContent = $newContent.Replace("bool Master::GameFinishFlag",              "bool Master::game_finish_flag_")
    $newContent = $newContent.Replace("bool Master::FeverFlag",                   "bool Master::fever_flag_")
    $newContent = $newContent.Replace("int Master::mnCaughtCowCount",             "int Master::caught_cow_count_")
    $newContent = $newContent.Replace("bool Master::SelectSkill",                 "bool Master::select_skill_")

    # === CharacterMove members (must come before generic mf/mb to avoid conflicts) ===
    $newContent = $newContent.Replace("mpTargetPlayer",  "target_player_")
    $newContent = $newContent.Replace("mpCurrentState",  "current_state_")
    $newContent = $newContent.Replace("mfdeathTime",     "death_time_")
    $newContent = $newContent.Replace("mfScore",         "score_value_")
    $newContent = $newContent.Replace("mfXp",            "xp_value_member_") # temp to avoid conflict
    $newContent = $newContent.Replace("mbBaitFlag",      "bait_flag_")
    $newContent = $newContent.Replace("mbIsVisible",     "is_visible_")

    # === Player3D members ===
    $newContent = $newContent.Replace("mbVacuumFlag",         "vacuum_flag_")
    $newContent = $newContent.Replace("mIsOutOfBounds",       "is_out_of_bounds_")
    $newContent = $newContent.Replace("mIsCowInVacuumRange",  "is_cow_in_vacuum_range_")
    $newContent = $newContent.Replace("mVacuumGauge",         "vacuum_gauge_")
    $newContent = $newContent.Replace("mIsVacuumActive",      "is_vacuum_active_")
    $newContent = $newContent.Replace("CatchNowCount",        "catch_now_count_")
    $newContent = $newContent.Replace("currentSpeed",         "current_speed_")
    $newContent = $newContent.Replace("UpMoveVector",         "up_move_vector_")
    $newContent = $newContent.Replace("oldmoveVec",           "old_move_vec_")
    $newContent = $newContent.Replace("moveVec",              "move_vec_")
    $newContent = $newContent.Replace("hitPos",               "hit_pos_")
    $newContent = $newContent.Replace("mIsStunned",           "is_stunned_")
    $newContent = $newContent.Replace("mStunTimer",           "stun_timer_")
    $newContent = $newContent.Replace("mnLighGraph",          "light_graph_")
    $newContent = $newContent.Replace("mfAttack_Speed",       "attack_speed_")
    $newContent = $newContent.Replace("mfHp",                 "hp_")
    $newContent = $newContent.Replace("mpSkill",              "skill_")
    $newContent = $newContent.Replace("mpSpeed",              "speed_effect_")
    $newContent = $newContent.Replace("mpBeam",               "beam_effect_")
    # Player3D public members mpScore / mpLevel - these appear as field declarations and ->mpScore
    $newContent = $newContent.Replace("Score* mpScore",       "Score* score_")
    $newContent = $newContent.Replace("Level* mpLevel",       "Level* level_")
    $newContent = $newContent.Replace("Skill* mpSkill",       "Skill* skill_")
    # ->mpScore and ->mpLevel in cpp (after player-> )
    $newContent = $newContent.Replace("->mpScore",  "->score_")
    $newContent = $newContent.Replace("->mpLevel",  "->level_")
    $newContent = $newContent.Replace("->mpSkill",  "->skill_")

    # Player3D speed_ (must come after CharacterMove speed handling)
    $newContent = $newContent.Replace("mfSpeed",              "speed_")

    # === Camera members ===
    $newContent = $newContent.Replace("mfShakeTime",          "shake_time_total_")

    # === Model members ===
    $newContent = $newContent.Replace("mnHandle",               "handle_")
    $newContent = $newContent.Replace("mfScale",                "scale_")
    $newContent = $newContent.Replace("mnChangeTextureHandle",  "change_texture_handle_")

    # === ModelAnimation members ===
    $newContent = $newContent.Replace("mfAnimationTime",      "animation_time_")
    $newContent = $newContent.Replace("mfAnimationCount",     "animation_count_")
    $newContent = $newContent.Replace("mnAnimationIndex",     "animation_index_")
    $newContent = $newContent.Replace("mfOldAnimationTime",   "old_animation_time_")
    $newContent = $newContent.Replace("mnOldAnimationIndex",  "old_animation_index_")
    $newContent = $newContent.Replace("mfAnimBlendRate",      "anim_blend_rate_")
    $newContent = $newContent.Replace("mnLoopFinishState",    "loop_finish_state_")
    $newContent = $newContent.Replace("mbLoopFinish",         "loop_finish_")
    $newContent = $newContent.Replace("mbLoop",               "loop_")
    $newContent = $newContent.Replace("mnState",              "anim_state_")

    # === GameTimer members ===
    $newContent = $newContent.Replace("mbStopFlag", "stop_flag_")
    $newContent = $newContent.Replace("mbFlag",     "out_timer_flag_")

    # === Level members ===
    $newContent = $newContent.Replace("mnGaugeFrameGraph",    "gauge_frame_graph_")
    $newContent = $newContent.Replace("mfMaxXp",              "max_xp_")
    $newContent = $newContent.Replace("mfxp",                 "xp_value_member_") # same temp name
    $newContent = $newContent.Replace("mnNowLevel",           "now_level_")
    # Resolve temp name
    $newContent = $newContent.Replace("xp_value_member_",    "xp_")

    # === Texture members ===
    $newContent = $newContent.Replace("mnSizeX",     "size_x_")
    $newContent = $newContent.Replace("mnSizeY",     "size_y_")
    $newContent = $newContent.Replace("mnTransFlag", "trans_flag_")

    # === CreatureManager members ===
    $newContent = $newContent.Replace("mPools",      "pools_")
    $newContent = $newContent.Replace("mCreatures",  "creatures_")

    # === EffekseerEffect SetLoop ===
    $newContent = $newContent.Replace("bool mfLoop",         "bool loop")
    $newContent = $newContent.Replace("LoopFlag = mfLoop",   "LoopFlag = loop")

    if ($newContent -ne $content) {
        # Write back with same encoding
        [System.IO.File]::WriteAllText($file.FullName, $newContent, $readEnc)
        Write-Host "Modified: $($file.Name)"
        $changedFiles++
    }
}

Write-Host "`nTotal files modified: $changedFiles"
