Add-Type -TypeDefinition @"
using System;
using System.IO;
using System.Text;
using System.Text.RegularExpressions;
using System.Collections.Generic;

public class Refactor {
    public static void Run(string dir) {
        var files = Directory.GetFiles(dir, "*.*", SearchOption.AllDirectories);
        
        Dictionary<string, string> rep = new Dictionary<string, string>();
        rep.Add(@"\bmpSceneManager\b", "scene_manager_");
        rep.Add(@"\bmpSoundManager\b", "sound_manager_");
        rep.Add(@"\bmpEffectManager\b", "effect_manager_");
        rep.Add(@"\bmpResourceManager\b", "resource_manager_");
        rep.Add(@"\bmpScore\b", "score_manager_");
        rep.Add(@"\bmpLevel\b", "level_manager_");
        rep.Add(@"\bmpTargetPlayer\b", "target_player_");
        rep.Add(@"\bmpCurrentState\b", "current_state_");
        rep.Add(@"\bmpDebugCamera\b", "debug_camera_");
        rep.Add(@"\bmbIsDebugCamera\b", "is_debug_camera_");
        rep.Add(@"\bmbIsVisible\b", "is_visible_");
        rep.Add(@"\bmbVacuumFlag\b", "vacuum_flag_");
        rep.Add(@"\bmbBaitFlag\b", "bait_flag_");
        rep.Add(@"\bmbStopFlag\b", "stop_flag_");
        rep.Add(@"\bmbFlag\b", "flag_");
        rep.Add(@"\bmbLoop\b", "is_loop_");
        rep.Add(@"\bmbLoopFinish\b", "is_loop_finish_");
        rep.Add(@"\bmfDeltaTime\b", "delta_time_");
        rep.Add(@"\bmfSpeed\b", "speed_");
        rep.Add(@"\bmfHp\b", "hp_");
        rep.Add(@"\bmfScore\b", "score_");
        rep.Add(@"\bmfXp\b", "xp_");
        rep.Add(@"\bmfxp\b", "xp_");
        rep.Add(@"\bmfMaxXp\b", "max_xp_");
        rep.Add(@"\bmfAttack_Speed\b", "attack_speed_");
        rep.Add(@"\bmfAnimationCount\b", "animation_count_");
        rep.Add(@"\bmfAnimationTime\b", "animation_time_");
        rep.Add(@"\bmfAnimBlendRate\b", "anim_blend_rate_");
        rep.Add(@"\bmfLoop\b", "loop_value_");
        rep.Add(@"\bmfOldAnimationTime\b", "old_animation_time_");
        rep.Add(@"\bmfScale\b", "scale_");
        rep.Add(@"\bmfShakeTime\b", "shake_time_");
        rep.Add(@"\bmnTutorialcount\b", "tutorial_count_");
        rep.Add(@"\bmnCaughtCowCount\b", "caught_cow_count_");
        rep.Add(@"\bmnNowLevel\b", "now_level_");
        rep.Add(@"\bmnAnimationIndex\b", "animation_index_");
        rep.Add(@"\bmnChangeTextureHandle\b", "change_texture_handle_");
        rep.Add(@"\bmnGaugeFrameGraph\b", "gauge_frame_graph_");
        rep.Add(@"\bmnHandle\b", "handle_");
        rep.Add(@"\bmnLighGraph\b", "light_graph_");
        rep.Add(@"\bmnLoopFinishState\b", "loop_finish_state_");
        rep.Add(@"\bmnOldAnimationIndex\b", "old_animation_index_");
        rep.Add(@"\bmnSizeX\b", "size_x_");
        rep.Add(@"\bmnSizeY\b", "size_y_");
        rep.Add(@"\bmnState\b", "state_");
        rep.Add(@"\bmnTransFlag\b", "trans_flag_");
        rep.Add(@"\bmpBeam\b", "beam_");
        rep.Add(@"\bmPools\b", "pools_");
        rep.Add(@"\bmpSkill\b", "skill_");

        foreach (var f in files) {
            if (!f.EndsWith(".h") && !f.EndsWith(".cpp")) continue;
            
            byte[] bytes = File.ReadAllBytes(f);
            
            // Detect encoding: simple heuristic
            Encoding enc = Encoding.GetEncoding("shift_jis"); // Default
            if (bytes.Length >= 3 && bytes[0] == 0xEF && bytes[1] == 0xBB && bytes[2] == 0xBF) {
                enc = new UTF8Encoding(true);
            }
            
            string content = enc.GetString(bytes);
            if (enc is UTF8Encoding && bytes.Length >= 3) {
                content = enc.GetString(bytes, 3, bytes.Length - 3);
            }
            
            bool changed = false;
            foreach (var kvp in rep) {
                if (Regex.IsMatch(content, kvp.Key)) {
                    content = Regex.Replace(content, kvp.Key, kvp.Value);
                    changed = true;
                }
            }
            
            if (changed) {
                if (enc is UTF8Encoding) {
                    byte[] bom = { 0xEF, 0xBB, 0xBF };
                    byte[] newBytes = enc.GetBytes(content);
                    using (FileStream fs = new FileStream(f, FileMode.Create, FileAccess.Write)) {
                        fs.Write(bom, 0, bom.Length);
                        fs.Write(newBytes, 0, newBytes.Length);
                    }
                } else {
                    File.WriteAllText(f, content, enc);
                }
                Console.WriteLine("Updated: " + f);
            }
        }
    }
}
"@
[Refactor]::Run("c:\Users\Admin\Desktop\progtram\Cow\Source")
