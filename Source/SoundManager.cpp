#include"SoundManager.h"
#include"DxLib.h"

SoundManager::SoundManager()
    : now_playing_bgm_((SoundBgm)-1)
    , now_playing_se_((SoundSe)-1)
{
}

SoundManager::~SoundManager()
{
    // 繧ｪ繝悶ず繧ｧ繧ｯ繝育ｴ譽・凾縺ｮ繝｡繝｢繝ｪ繝ｪ繝ｼ繧ｯ繧堤｢ｺ螳溘↓髦ｲ縺舌◆繧∵・遉ｺ逧・↓隗｣謾ｾ
    Finalize();
}

/*
 * 繧ｲ繝ｼ繝繝励Ξ繧､荳ｭ縺ｮ繝ｭ繝ｼ繝蛾≦蟒ｶ繧帝亟縺舌◆繧∝・髻ｳ貅舌ｒ蛻晄悄蛹匁凾縺ｫ隱ｭ縺ｿ霎ｼ繧
 * [蜈･蜉嫋 縺ｪ縺・
 * [蜃ｺ蜉嫋 縺ｪ縺・
 * [蜑ｯ菴懃畑] 蜷・ワ繝ｳ繝峨Ν繝ｪ繧ｹ繝医↓繧ｵ繧ｦ繝ｳ繝峨Μ繧ｽ繝ｼ繧ｹ繧堤匳骭ｲ
 */
void SoundManager::Initialize()
{
    LoadBGM(SoundBgm::kBgmTitle, "Resource/BGM/hajimetenootsukai.mp3");
    LoadBGM(SoundBgm::kBgmGame, "Resource/BGM/natsuyasuminotanken.mp3");
    LoadBGM(SoundBgm::kBgmResult, "Resource/BGM/retrogamecenter3.mp3");
    LoadBGM(SoundBgm::kBgmTutorial, "Resource/BGM/MusMus-BGM-103.mp3");
    LoadBGM(SoundBgm::kBgmRule, "Resource/BGM/MusMus-BGM-158.mp3");

    LoadSE(SoundSe::kSeDecide, "Resource/SE/豎ｺ螳壹・繧ｿ繝ｳ繧呈款縺・2.mp3");
    LoadSE(SoundSe::kSeCow, "Resource/SE/迚帙・魑ｴ縺榊｣ｰ.mp3");
    LoadSE(SoundSe::kSeChicken, "Resource/SE/MusMus-BGM-084");
    LoadSE(SoundSe::kSeTatumaki, "Resource/SE/螟ｩ蛟吶・鬚ｨ荳蜷ｹ縺搾ｼ医＠繧峨￠陦ｨ迴ｾ・・mp3");
    LoadSE(SoundSe::kSeKaminari, "Resource/SE/螟ｩ蛟吶・髮ｷ01.mp3");
    LoadSE(SoundSe::kSeTutorialChange, "Resource/SE/繧ｷ繝ｼ繝ｳ蛻・ｊ譖ｿ縺・.mp3");
    LoadSE(SoundSe::kSeBeam, "Resource/SE/HP蜷ｸ蜿朱ｭ疲ｳ・.mp3");
    LoadSE(SoundSe::kSeSkillStatus, "Resource/SE/繧ｹ繝・・繧ｿ繧ｹ荳頑・鬲疲ｳ・.mp3");
    LoadSE(SoundSe::kSeSkillFood, "Resource/SE/繝ｭ繝懊ャ繝郁ｵｷ蜍・.mp3");
    LoadSE(SoundSe::kSeBaitFinal, "Resource/SE/鬮倬溽ｧｻ蜍・mp3");
}

/*
 * 繝｡繝｢繝ｪ繝ｪ繝ｼ繧ｯ繧帝亟縺舌◆繧√・蜈ｨ繧ｵ繧ｦ繝ｳ繝峨Μ繧ｽ繝ｼ繧ｹ隗｣謾ｾ
 * [蜈･蜉嫋 縺ｪ縺・
 * [蜃ｺ蜉嫋 縺ｪ縺・
 * [蜑ｯ菴懃畑] BGM繝ｻSE繝上Φ繝峨Ν縺ｮ蜑企勁縺ｨ繝ｪ繧ｹ繝医・繧ｯ繝ｪ繧｢
 */
void SoundManager::Finalize()
{
    for (auto it = bgm_handle_list_.begin(); it != bgm_handle_list_.end(); it++)
    {
        DeleteSoundMem(it->second);
    }
    bgm_handle_list_.clear();

    for (auto it = se_handle_list_.begin(); it != se_handle_list_.end(); it++)
    {
        DeleteSoundMem(it->second);
    }
    se_handle_list_.clear();
}

/*
 * BGM縺ｮ蜀咲函縺ｨ縲∝酔荳譖ｲ縺ｮ莠碁㍾蜀咲函髦ｲ豁｢繧貞宛蠕｡縺吶ｋ
 * [蜈･蜉嫋 bgm: 蜀咲函縺吶ｋBGMID, isTop: 蜈磯ｭ縺九ｉ蜀咲函縺吶ｋ縺・
 * [蜃ｺ蜉嫋 縺ｪ縺・
 * [蜑ｯ菴懃畑] BGM蜀咲函髢句ｧ九［nNowPlayingBgm縺ｮ譖ｴ譁ｰ
 */
void SoundManager::PlayBGM(SoundBgm bgm, bool isTop)
{
    // 蜷御ｸ譖ｲ縺梧欠螳壹＆繧後◆蝣ｴ蜷医∵峇縺ｮ荳崎・辟ｶ縺ｪ蜀埼幕繧帝亟縺舌◆繧∝・逅・ｒ繧ｹ繧ｭ繝・・
    if (now_playing_bgm_ == bgm && !isTop)
    {
        return;
    }

    for (auto it = bgm_handle_list_.begin(); it != bgm_handle_list_.end(); it++)
    {
        if (it->first == bgm)
        {
            // 險ｭ螳壹＆繧後※縺・ｋ譛譁ｰ縺ｮ繝槭せ繧ｿ繝ｼ髻ｳ驥上〒蜀咲函繧帝幕蟋九☆繧・
            ChangeVolumeSoundMem(master_bgm_volume_, it->second);
            PlaySoundMem(it->second, DX_PLAYTYPE_LOOP, isTop);
            now_playing_bgm_ = bgm;
            break;
        }
    }
}

/*
 * 蜉ｹ譫憺浹繧偵ヰ繝・け繧ｰ繝ｩ繧ｦ繝ｳ繝峨〒荳ｦ蛻怜・逕溘☆繧・
 * [蜈･蜉嫋 se: 蜀咲函縺吶ｋSE縺ｮID
 * [蜃ｺ蜉嫋 縺ｪ縺・
 * [蜑ｯ菴懃畑] SE蜀咲函髢句ｧ九［nNowPlayingSe縺ｮ譖ｴ譁ｰ
 */
void SoundManager::PlaySE(SoundSe se)
{
    for (auto it = se_handle_list_.begin(); it != se_handle_list_.end(); it++)
    {
        if (it->first == se)
        {
            // SE縺ｯ隍・焚蜷梧凾縺ｫ魑ｴ繧句庄閭ｽ諤ｧ縺後≠繧九◆繧∽ｸｦ蛻怜・逕溘ｒ謖・ｮ・
            PlaySoundMem(it->second, DX_PLAYTYPE_BACK);
            now_playing_se_ = se;
            break;
        }
    }
}

/*
 * 髻ｳ貅舌・莠碁㍾繝ｭ繝ｼ繝峨ｒ髦ｲ縺弱▽縺､BGM繧偵Γ繝｢繝ｪ縺ｫ驟咲ｽｮ縺吶ｋ
 * [蜈･蜉嫋 bgm: 逋ｻ骭ｲID, filename: 繝輔ぃ繧､繝ｫ繝代せ
 * [蜃ｺ蜉嫋 縺ｪ縺・
 * [蜑ｯ菴懃畑] 繝上Φ繝峨Ν蜿門ｾ励→繝ｪ繧ｹ繝医∈縺ｮ霑ｽ蜉
 */
void SoundManager::LoadBGM(SoundBgm bgm, std::string filename)
{
    // 辟｡鬧・↑繝｡繝｢繝ｪ豸郁ｲｻ繧帝亟縺舌◆繧√∵里縺ｫ逋ｻ骭ｲ貂医∩縺ｮ蝣ｴ蜷医・繧ｹ繧ｭ繝・・
    for (auto it = bgm_handle_list_.begin(); it != bgm_handle_list_.end(); it++)
    {
        if (it->first == bgm)
        {
            return;
        }
    }

    int handle = LoadSoundMem(filename.c_str());
    // 蟄伜惠縺励↑縺・ヵ繧｡繧､繝ｫ繝代せ遲峨↓繧医ｋ辟｡蜉ｹ縺ｪ繝上Φ繝峨Ν逋ｻ骭ｲ繧帝亟縺・
    if (handle == -1)
    {
        return;
    }
    ChangeVolumeSoundMem(master_bgm_volume_, handle);
    bgm_handle_list_.push_back(std::pair<SoundBgm, int>(bgm, handle));
}

/*
 * 髻ｳ貅舌・莠碁㍾繝ｭ繝ｼ繝峨ｒ髦ｲ縺弱▽縺､SE繧偵Γ繝｢繝ｪ縺ｫ驟咲ｽｮ縺吶ｋ
 * [蜈･蜉嫋 se: 逋ｻ骭ｲID, filename: 繝輔ぃ繧､繝ｫ繝代せ
 * [蜃ｺ蜉嫋 縺ｪ縺・
 * [蜑ｯ菴懃畑] 繝上Φ繝峨Ν蜿門ｾ励→繝ｪ繧ｹ繝医∈縺ｮ霑ｽ蜉
 */
void SoundManager::LoadSE(SoundSe se, std::string filename)
{
    // 辟｡鬧・↑繝｡繝｢繝ｪ豸郁ｲｻ繧帝亟縺舌◆繧√∵里縺ｫ逋ｻ骭ｲ貂医∩縺ｮ蝣ｴ蜷医・繧ｹ繧ｭ繝・・
    for (auto it = se_handle_list_.begin(); it != se_handle_list_.end(); it++)
    {
        if (it->first == se)
        {
            return;
        }
    }

    int handle = LoadSoundMem(filename.c_str());
    // 蟄伜惠縺励↑縺・ヵ繧｡繧､繝ｫ繝代せ遲峨↓繧医ｋ辟｡蜉ｹ縺ｪ繝上Φ繝峨Ν逋ｻ骭ｲ繧帝亟縺・
    if (handle == -1)
    {
        return;
    }
    ChangeVolumeSoundMem(master_se_volume_, handle);
    se_handle_list_.push_back(std::pair<SoundSe, int>(se, handle));
}

/*
 * 繧ｷ繝ｼ繝ｳ邨ゆｺ・凾縺ｪ縺ｩ縺ｫ迴ｾ蝨ｨ蜀咲函荳ｭ縺ｮBGM繧呈ｭ｢繧√ｋ
 * [蜈･蜉嫋 縺ｪ縺・
 * [蜃ｺ蜉嫋 縺ｪ縺・
 * [蜑ｯ菴懃畑] 蜀咲函荳ｭ縺ｮBGM繧貞●豁｢
 */
void SoundManager::StopBGM()
{
    for (auto it = bgm_handle_list_.begin(); it != bgm_handle_list_.end(); it++)
    {
        if (it->first == now_playing_bgm_)
        {
            // DxLib蛛ｴ縺ｧ譌｢縺ｫ蛛懈ｭ｢縺励※縺・ｋ蝣ｴ蜷医・繧ｨ繝ｩ繝ｼ繧貞屓驕ｿ
            if (CheckSoundMem(it->second))
            {
                StopSoundMem(it->second);
                break;
            }
        }
    }
}

/*
 * 繝ｭ繝ｼ繝画ｸ医∩蜈ｨBGM縺ｫ髻ｳ驥上ｒ荳諡ｬ驕ｩ逕ｨ縺吶ｋ
 * [蜈･蜉嫋 volume: 髻ｳ驥丞､(0-255)
 * [蜃ｺ蜉嫋 縺ｪ縺・
 * [蜑ｯ菴懃畑] 繝上Φ繝峨Ν繧帝壹§縺檻GM髻ｳ驥丞､画峩
 */
void SoundManager::SetBGMVolume(int volume)
{
    for (auto it = bgm_handle_list_.begin(); it != bgm_handle_list_.end(); it++)
    {
        ChangeVolumeSoundMem(volume, it->second);
    }
}

/*
 * 繝ｭ繝ｼ繝画ｸ医∩蜈ｨSE縺ｫ髻ｳ驥上ｒ荳諡ｬ驕ｩ逕ｨ縺吶ｋ
 * [蜈･蜉嫋 volume: 髻ｳ驥丞､(0-255)
 * [蜃ｺ蜉嫋 縺ｪ縺・
 * [蜑ｯ菴懃畑] 繝上Φ繝峨Ν繧帝壹§縺欖E髻ｳ驥丞､画峩
 */
void SoundManager::SetSEVolume(int volume)
{
    for (auto it = se_handle_list_.begin(); it != se_handle_list_.end(); it++)
    {
        ChangeVolumeSoundMem(volume, it->second);
    }
}

/*
 * 繧ｷ繧ｹ繝・Β險ｭ螳壹↑縺ｩ縺ｧ荳諡ｬ螟画峩縺輔ｌ繧毅GM繝槭せ繧ｿ繝ｼ髻ｳ驥上ｒ驕ｩ逕ｨ縺吶ｋ
 * [蜈･蜉嫋 volume: 蝓ｺ貅夜浹驥・0-255)
 * [蜃ｺ蜉嫋 縺ｪ縺・
 * [蜑ｯ菴懃畑] mMasterBGMVolume譖ｴ譁ｰ縲∝・BGM縺ｸ髻ｳ驥丞渚譏
 */
void SoundManager::SetMasterBGMVolume(int volume)
{
    // DxLib縺ｮ髻ｳ驥乗欠螳夂ｯ・峇(0-255)螟悶・蛟､繧帝亟縺舌◆繧√・繧ｯ繝ｩ繝ｳ繝・
    if (volume < 0) volume = 0;
    if (volume > 255) volume = 255;
    master_bgm_volume_ = volume;
    SetBGMVolume(master_bgm_volume_);
}

/*
 * 繧ｷ繧ｹ繝・Β險ｭ螳壹↑縺ｩ縺ｧ荳諡ｬ螟画峩縺輔ｌ繧鬼E繝槭せ繧ｿ繝ｼ髻ｳ驥上ｒ驕ｩ逕ｨ縺吶ｋ
 * [蜈･蜉嫋 volume: 蝓ｺ貅夜浹驥・0-255)
 * [蜃ｺ蜉嫋 縺ｪ縺・
 * [蜑ｯ菴懃畑] mMasterSEVolume譖ｴ譁ｰ縲∝・SE縺ｸ髻ｳ驥丞渚譏
 */
void SoundManager::SetMasterSEVolume(int volume)
{
    // DxLib縺ｮ髻ｳ驥乗欠螳夂ｯ・峇(0-255)螟悶・蛟､繧帝亟縺舌◆繧√・繧ｯ繝ｩ繝ｳ繝・
    if (volume < 0) volume = 0;
    if (volume > 255) volume = 255;
    master_se_volume_ = volume;
    SetSEVolume(master_se_volume_);
}