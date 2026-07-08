#pragma once
#include<string>
#include<vector>

// 繧ｲ繝ｼ繝蜀・GM縺ｨSE縺ｮ莠碁㍾蜀咲函髦ｲ豁｢繧・浹驥上・荳諡ｬ蛻ｶ蠕｡繧定｡後≧縺溘ａ縺ｮ邂｡逅・け繝ｩ繧ｹ
class SoundManager
{
public:
    // 繧ｷ繝ｼ繝ｳ驕ｷ遘ｻ譎ゅ・BGM蛻・ｊ譖ｿ縺育畑ID
    enum SoundBgm
    {
        kBgmTitle = 0,
        kBgmGame,
        kBgmResult,
        kBgmTutorial,
        kBgmRule,
    };

    // 蜷・ｨｮ繧｢繧ｯ繧ｷ繝ｧ繝ｳ蜉ｹ譫憺浹蜻ｼ縺ｳ蜃ｺ縺礼畑ID
    enum SoundSe
    {
        kSeDecide = 0,
        kSeCow,
        kSeBeam,
        kSeSkillStatus,
        kSeSkillFood,
        kSeChicken,
        kSeTatumaki,
        kSeKaminari,
        kSeTutorialChange,
        kSeBaitFinal,
    };

    SoundManager();
    ~SoundManager();

    /*
     * 繧ｲ繝ｼ繝繝励Ξ繧､荳ｭ縺ｮ繝ｭ繝ｼ繝蛾≦蟒ｶ繧帝亟縺舌◆繧∝・髻ｳ貅舌ｒ蛻晄悄蛹匁凾縺ｫ隱ｭ縺ｿ霎ｼ繧
     * [蜈･蜉嫋 縺ｪ縺・
     * [蜃ｺ蜉嫋 縺ｪ縺・
     * [蜑ｯ菴懃畑] 蜷・ワ繝ｳ繝峨Ν繝ｪ繧ｹ繝医↓繧ｵ繧ｦ繝ｳ繝峨Μ繧ｽ繝ｼ繧ｹ繧堤匳骭ｲ
     */
    void Initialize();

    /*
     * 繝｡繝｢繝ｪ繝ｪ繝ｼ繧ｯ繧帝亟縺舌◆繧√・蜈ｨ繧ｵ繧ｦ繝ｳ繝峨Μ繧ｽ繝ｼ繧ｹ隗｣謾ｾ
     * [蜈･蜉嫋 縺ｪ縺・
     * [蜃ｺ蜉嫋 縺ｪ縺・
     * [蜑ｯ菴懃畑] BGM繝ｻSE繝上Φ繝峨Ν縺ｮ蜑企勁縺ｨ繝ｪ繧ｹ繝医・繧ｯ繝ｪ繧｢
     */
    void Finalize();

    /*
     * BGM縺ｮ蜀咲函縺ｨ縲∝酔荳譖ｲ縺ｮ莠碁㍾蜀咲函髦ｲ豁｢繧貞宛蠕｡縺吶ｋ
     * [蜈･蜉嫋 bgm: 蜀咲函縺吶ｋBGMID, IsTop: true縺ｧ蜈磯ｭ縺九ｉ蜀咲函
     * [蜃ｺ蜉嫋 縺ｪ縺・
     * [蜑ｯ菴懃畑] BGM蜀咲函髢句ｧ九［nNowPlayingBgm縺ｮ譖ｴ譁ｰ
     */
    void PlayBGM(SoundBgm bgm, bool IsTop = true);

    /*
     * 蜉ｹ譫憺浹繧偵ヰ繝・け繧ｰ繝ｩ繧ｦ繝ｳ繝峨〒荳ｦ蛻怜・逕溘☆繧・
     * [蜈･蜉嫋 se: 蜀咲函縺吶ｋSE縺ｮID
     * [蜃ｺ蜉嫋 縺ｪ縺・
     * [蜑ｯ菴懃畑] SE蜀咲函髢句ｧ九［nNowPlayingSe縺ｮ譖ｴ譁ｰ
     */
    void PlaySE(SoundSe se);

    /*
     * 髻ｳ貅舌・莠碁㍾繝ｭ繝ｼ繝峨ｒ髦ｲ縺弱▽縺､BGM繧偵Γ繝｢繝ｪ縺ｫ驟咲ｽｮ縺吶ｋ
     * [蜈･蜉嫋 bgm: 逋ｻ骭ｲID, filename: 繝輔ぃ繧､繝ｫ繝代せ
     * [蜃ｺ蜉嫋 縺ｪ縺・
     * [蜑ｯ菴懃畑] 繝上Φ繝峨Ν蜿門ｾ励→繝ｪ繧ｹ繝医∈縺ｮ霑ｽ蜉
     */
    void LoadBGM(SoundBgm bgm, std::string filename);

    /*
     * 髻ｳ貅舌・莠碁㍾繝ｭ繝ｼ繝峨ｒ髦ｲ縺弱▽縺､SE繧偵Γ繝｢繝ｪ縺ｫ驟咲ｽｮ縺吶ｋ
     * [蜈･蜉嫋 se: 逋ｻ骭ｲID, filename: 繝輔ぃ繧､繝ｫ繝代せ
     * [蜃ｺ蜉嫋 縺ｪ縺・
     * [蜑ｯ菴懃畑] 繝上Φ繝峨Ν蜿門ｾ励→繝ｪ繧ｹ繝医∈縺ｮ霑ｽ蜉
     */
    void LoadSE(SoundSe se, std::string filename);

    /*
     * 繧ｷ繝ｼ繝ｳ邨ゆｺ・凾縺ｪ縺ｩ縺ｫ迴ｾ蝨ｨ蜀咲函荳ｭ縺ｮBGM繧呈ｭ｢繧√ｋ
     * [蜈･蜉嫋 縺ｪ縺・
     * [蜃ｺ蜉嫋 縺ｪ縺・
     * [蜑ｯ菴懃畑] 蜀咲函荳ｭ縺ｮBGM繧貞●豁｢
     */
    void StopBGM();

    /*
     * 繝ｭ繝ｼ繝画ｸ医∩蜈ｨBGM縺ｫ髻ｳ驥上ｒ荳諡ｬ驕ｩ逕ｨ縺吶ｋ
     * [蜈･蜉嫋 volume: 髻ｳ驥丞､(0-255)
     * [蜃ｺ蜉嫋 縺ｪ縺・
     * [蜑ｯ菴懃畑] 繝上Φ繝峨Ν繧帝壹§縺檻GM髻ｳ驥丞､画峩
     */
    void SetBGMVolume(int volume);

    /*
     * 繝ｭ繝ｼ繝画ｸ医∩蜈ｨSE縺ｫ髻ｳ驥上ｒ荳諡ｬ驕ｩ逕ｨ縺吶ｋ
     * [蜈･蜉嫋 volume: 髻ｳ驥丞､(0-255)
     * [蜃ｺ蜉嫋 縺ｪ縺・
     * [蜑ｯ菴懃畑] 繝上Φ繝峨Ν繧帝壹§縺欖E髻ｳ驥丞､画峩
     */
    void SetSEVolume(int volume);

    int GetMasterBGMVolume() const { return master_bgm_volume_; }
    int GetMasterSEVolume() const { return master_se_volume_; }

    /*
     * 繧ｷ繧ｹ繝・Β險ｭ螳壹↑縺ｩ縺ｧ荳諡ｬ螟画峩縺輔ｌ繧毅GM繝槭せ繧ｿ繝ｼ髻ｳ驥上ｒ驕ｩ逕ｨ縺吶ｋ
     * [蜈･蜉嫋 volume: 蝓ｺ貅夜浹驥・0-255)
     * [蜃ｺ蜉嫋 縺ｪ縺・
     * [蜑ｯ菴懃畑] mMasterBGMVolume譖ｴ譁ｰ縲∝・BGM縺ｸ髻ｳ驥丞渚譏
     */
    void SetMasterBGMVolume(int volume);

    /*
     * 繧ｷ繧ｹ繝・Β險ｭ螳壹↑縺ｩ縺ｧ荳諡ｬ螟画峩縺輔ｌ繧鬼E繝槭せ繧ｿ繝ｼ髻ｳ驥上ｒ驕ｩ逕ｨ縺吶ｋ
     * [蜈･蜉嫋 volume: 蝓ｺ貅夜浹驥・0-255)
     * [蜃ｺ蜉嫋 縺ｪ縺・
     * [蜑ｯ菴懃畑] mMasterSEVolume譖ｴ譁ｰ縲∝・SE縺ｸ髻ｳ驥丞渚譏
     */
    void SetMasterSEVolume(int volume);

private:
    int master_bgm_volume_ = 255;
    int master_se_volume_ = 255;

    // 蛛懈ｭ｢蜃ｦ逅・ｄ莠碁㍾蜀咲函髦ｲ豁｢縺ｮ蛻､螳壹↓逕ｨ縺・ｋ迥ｶ諷倶ｿ晄戟
    SoundBgm now_playing_bgm_;
    SoundSe now_playing_se_;

    std::vector<std::pair<SoundBgm, int>> bgm_handle_list_;
    std::vector<std::pair<SoundSe, int>> se_handle_list_;
};