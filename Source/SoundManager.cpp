#include"SoundManager.h"
#include"DxLib.h"

SoundManager::SoundManager()
    : now_playing_bgm_((SoundBgm)-1)
    , now_playing_se_((SoundSe)-1)
{
}

SoundManager::~SoundManager()
{
    // オブジェクト破棁E��のメモリリークを確実に防ぐため�E示皁E��解放
    Finalize();
}

/*
 * ゲームプレイ中のロード遅延を防ぐため�E音源を初期化時に読み込む
 * [入力] �Ȃ�
 * [出力] �Ȃ�
 * [副作用] �ꍇ��ンドルリストにサウンドリソースを登録
 */
void SoundManager::Initialize()
{
    LoadBGM(SoundBgm::kBgmTitle, "Resource/BGM/hajimetenootsukai.mp3");
    LoadBGM(SoundBgm::kBgmGame, "Resource/BGM/natsuyasuminotanken.mp3");
    LoadBGM(SoundBgm::kBgmResult, "Resource/BGM/retrogamecenter3.mp3");
    LoadBGM(SoundBgm::kBgmTutorial, "Resource/BGM/MusMus-BGM-103.mp3");
    LoadBGM(SoundBgm::kBgmRule, "Resource/BGM/MusMus-BGM-158.mp3");

    LoadSE(SoundSe::kSeDecide, "Resource/SE/����{�^��������42.mp3");
    LoadSE(SoundSe::kSeCow, "Resource/SE/���̖���.mp3");
    LoadSE(SoundSe::kSeChicken, "Resource/SE/MusMus-BGM-084");
    LoadSE(SoundSe::kSeTornado, "Resource/SE/�V��E���ꐁ���i���炯�\���j.mp3");
    LoadSE(SoundSe::kSeKaminari, "Resource/SE/�V��E��01.mp3");
    LoadSE(SoundSe::kSeTutorialChange, "Resource/SE/�V�[���؂�ւ�2.mp3");
    LoadSE(SoundSe::kSeBeam, "Resource/SE/HP�z�����@1.mp3");
    LoadSE(SoundSe::kSeSkillStatus, "Resource/SE/�X�e�[�^�X�㏸���@2.mp3");
    LoadSE(SoundSe::kSeSkillFood, "Resource/SE/���{�b�g�N��2.mp3");
    LoadSE(SoundSe::kSeBaitFinal, "Resource/SE/�����ړ�.mp3");
}

/*
 * メモリリークを防ぐため�E全サウンドリソース解放
 * [入力] �Ȃ�
 * [出力] �Ȃ�
 * [副作用] BGM�ESEハンドルの削除とリスト�Eクリア
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
 * BGMの再生と、同一曲の二重再生防止を制御する
 * [入力] bgm: 再生するBGMID, isTop: 先頭から再生するぁE
 * [出力] �Ȃ�
 * [副作用] BGM再生開始、mnNowPlayingBgmの更新
 */
void SoundManager::PlayBGM(SoundBgm bgm, bool isTop)
{
    // 同一曲が指定された場合、曲の不�E然な再開を防ぐため�E琁E��スキチE�E
    if (now_playing_bgm_ == bgm && !isTop)
    {
        return;
    }

    for (auto it = bgm_handle_list_.begin(); it != bgm_handle_list_.end(); it++)
    {
        if (it->first == bgm)
        {
            // 設定されてぁE��最新のマスター音量で再生を開始すめE
            ChangeVolumeSoundMem(master_bgm_volume_, it->second);
            PlaySoundMem(it->second, DX_PLAYTYPE_LOOP, isTop);
            now_playing_bgm_ = bgm;
            break;
        }
    }
}

/*
 * 効果音をバチE��グラウンドで並列�E生すめE
 * [入力] se: 再生するSEのID
 * [出力] �Ȃ�
 * [副作用] SE再生開始、mnNowPlayingSeの更新
 */
void SoundManager::PlaySE(SoundSe se)
{
    for (auto it = se_handle_list_.begin(); it != se_handle_list_.end(); it++)
    {
        if (it->first == se)
        {
            // SEは褁E��同時に鳴る可能性があるため並列�E生を持E��E
            PlaySoundMem(it->second, DX_PLAYTYPE_BACK);
            now_playing_se_ = se;
            break;
        }
    }
}

/*
 * 音源�E二重ロードを防ぎつつBGMをメモリに配置する
 * [入力] bgm: 登録ID, filename: ファイルパス
 * [出力] �Ȃ�
 * [副作用] ハンドル取得とリストへの追加
 */
void SoundManager::LoadBGM(SoundBgm bgm, std::string filename)
{
    // 無駁E��メモリ消費を防ぐため、既に登録済みの場合�EスキチE�E
    for (auto it = bgm_handle_list_.begin(); it != bgm_handle_list_.end(); it++)
    {
        if (it->first == bgm)
        {
            return;
        }
    }

    int handle = LoadSoundMem(filename.c_str());
    // 存在しなぁE��ァイルパス等による無効なハンドル登録を防ぁE
    if (handle == -1)
    {
        return;
    }
    ChangeVolumeSoundMem(master_bgm_volume_, handle);
    bgm_handle_list_.push_back(std::pair<SoundBgm, int>(bgm, handle));
}

/*
 * 音源�E二重ロードを防ぎつつSEをメモリに配置する
 * [入力] se: 登録ID, filename: ファイルパス
 * [出力] �Ȃ�
 * [副作用] ハンドル取得とリストへの追加
 */
void SoundManager::LoadSE(SoundSe se, std::string filename)
{
    // 無駁E��メモリ消費を防ぐため、既に登録済みの場合�EスキチE�E
    for (auto it = se_handle_list_.begin(); it != se_handle_list_.end(); it++)
    {
        if (it->first == se)
        {
            return;
        }
    }

    int handle = LoadSoundMem(filename.c_str());
    // 存在しなぁE��ァイルパス等による無効なハンドル登録を防ぁE
    if (handle == -1)
    {
        return;
    }
    ChangeVolumeSoundMem(master_se_volume_, handle);
    se_handle_list_.push_back(std::pair<SoundSe, int>(se, handle));
}

/*
 * シーン終亁E��などに現在再生中のBGMを止める
 * [入力] �Ȃ�
 * [出力] �Ȃ�
 * [副作用] 再生中のBGMを停止
 */
void SoundManager::StopBGM()
{
    for (auto it = bgm_handle_list_.begin(); it != bgm_handle_list_.end(); it++)
    {
        if (it->first == now_playing_bgm_)
        {
            // DxLib側で既に停止してぁE��場合�Eエラーを回避
            if (CheckSoundMem(it->second))
            {
                StopSoundMem(it->second);
                break;
            }
        }
    }
}

/*
 * ロード済み全BGMに音量を一括適用する
 * [入力] volume: 音量値(0-255)
 * [出力] �Ȃ�
 * [副作用] ハンドルを通じたBGM音量変更
 */
void SoundManager::SetBGMVolume(int volume)
{
    for (auto it = bgm_handle_list_.begin(); it != bgm_handle_list_.end(); it++)
    {
        ChangeVolumeSoundMem(volume, it->second);
    }
}

/*
 * ロード済み全SEに音量を一括適用する
 * [入力] volume: 音量値(0-255)
 * [出力] �Ȃ�
 * [副作用] ハンドルを通じたSE音量変更
 */
void SoundManager::SetSEVolume(int volume)
{
    for (auto it = se_handle_list_.begin(); it != se_handle_list_.end(); it++)
    {
        ChangeVolumeSoundMem(volume, it->second);
    }
}

/*
 * シスチE��設定などで一括変更されるBGMマスター音量を適用する
 * [入力] volume: 基準音釁E0-255)
 * [出力] �Ȃ�
 * [副作用] mMasterBGMVolume更新、�EBGMへ音量反映
 */
void SoundManager::SetMasterBGMVolume(int volume)
{
    // DxLibの音量指定篁E��(0-255)外�E値を防ぐため�EクランチE
    if (volume < 0) volume = 0;
    if (volume > 255) volume = 255;
    master_bgm_volume_ = volume;
    SetBGMVolume(master_bgm_volume_);
}

/*
 * シスチE��設定などで一括変更されるSEマスター音量を適用する
 * [入力] volume: 基準音釁E0-255)
 * [出力] �Ȃ�
 * [副作用] mMasterSEVolume更新、�ESEへ音量反映
 */
void SoundManager::SetMasterSEVolume(int volume)
{
    // DxLibの音量指定篁E��(0-255)外�E値を防ぐため�EクランチE
    if (volume < 0) volume = 0;
    if (volume > 255) volume = 255;
    master_se_volume_ = volume;
    SetSEVolume(master_se_volume_);
}