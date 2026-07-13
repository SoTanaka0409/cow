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
        kSeTornado,
        kSeKaminari,
        kSeTutorialChange,
        kSeBaitFinal,
    };

    SoundManager();
    ~SoundManager();

    void Initialize();
    void Finalize();
    void PlayBGM(SoundBgm bgm, bool IsTop = true);
    void PlaySE(SoundSe se);
    void LoadBGM(SoundBgm bgm, std::string filename);
    void LoadSE(SoundSe se, std::string filename);
    void StopBGM();
    void SetBGMVolume(int volume);
    void SetSEVolume(int volume);

    int GetMasterBGMVolume() const { return master_bgm_volume_; }
    int GetMasterSEVolume() const { return master_se_volume_; }

    void SetMasterBGMVolume(int volume);
    void SetMasterSEVolume(int volume);

private:
    int master_bgm_volume_ = 255;
    int master_se_volume_ = 255;

    SoundBgm now_playing_bgm_;
    SoundSe now_playing_se_;

    std::vector<std::pair<SoundBgm, int>> bgm_handle_list_;
    std::vector<std::pair<SoundSe, int>> se_handle_list_;
};