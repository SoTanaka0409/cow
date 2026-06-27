#include"SoundManager.h"
#include"DxLib.h"

SoundManager::SoundManager()
    : nowPlayingBgm((SOUND_BGM)-1)
    , nowPlayingSe((SOUND_SE)-1)
{
}

SoundManager::~SoundManager()
{
    // オブジェクト破棄時のメモリリークを確実に防ぐため明示的に解放
    Finalize();
}

/*
 * ゲームプレイ中のロード遅延を防ぐため全音源を初期化時に読み込む
 * [入力] なし
 * [出力] なし
 * [副作用] 各ハンドルリストにサウンドリソースを登録
 */
void SoundManager::Initialize()
{
    LoadBGM(SOUND_BGM::BGM_TITLE, "Resource/BGM/hajimetenootsukai.mp3");
    LoadBGM(SOUND_BGM::BGM_GAME, "Resource/BGM/natsuyasuminotanken.mp3");
    LoadBGM(SOUND_BGM::BGM_RESULT, "Resource/BGM/retrogamecenter3.mp3");
    LoadBGM(SOUND_BGM::BGM_TUTORIAL, "Resource/BGM/MusMus-BGM-103.mp3");
    LoadBGM(SOUND_BGM::BGM_RULE, "Resource/BGM/MusMus-BGM-158.mp3");

    LoadSE(SOUND_SE::SE_DECIDE, "Resource/SE/決定ボタンを押す42.mp3");
    LoadSE(SOUND_SE::SE_COW, "Resource/SE/牛の鳴き声.mp3");
    LoadSE(SOUND_SE::SE_CHICKEN, "Resource/SE/MusMus-BGM-084");
    LoadSE(SOUND_SE::SE_TATUMAKI, "Resource/SE/天候・風一吹き（しらけ表現）.mp3");
    LoadSE(SOUND_SE::SE_KAMINARI, "Resource/SE/天候・雷01.mp3");
    LoadSE(SOUND_SE::SE_TutorialChange, "Resource/SE/シーン切り替え2.mp3");
    LoadSE(SOUND_SE::SE_BEAM, "Resource/SE/HP吸収魔法1.mp3");
    LoadSE(SOUND_SE::SE_SKILL_STATUS, "Resource/SE/ステータス上昇魔法2.mp3");
    LoadSE(SOUND_SE::SE_SKILL_FOOD, "Resource/SE/ロボット起動2.mp3");
    LoadSE(SOUND_SE::SE_BAITFINAL, "Resource/SE/高速移動.mp3");
}

/*
 * メモリリークを防ぐための全サウンドリソース解放
 * [入力] なし
 * [出力] なし
 * [副作用] BGM・SEハンドルの削除とリストのクリア
 */
void SoundManager::Finalize()
{
    for (auto it = bgmHandleList.begin(); it != bgmHandleList.end(); it++)
    {
        DeleteSoundMem(it->second);
    }
    bgmHandleList.clear();

    for (auto it = seHandleList.begin(); it != seHandleList.end(); it++)
    {
        DeleteSoundMem(it->second);
    }
    seHandleList.clear();
}

/*
 * BGMの再生と、同一曲の二重再生防止を制御する
 * [入力] bgm: 再生するBGMID, isTop: 先頭から再生するか
 * [出力] なし
 * [副作用] BGM再生開始、mnNowPlayingBgmの更新
 */
void SoundManager::PlayBGM(SOUND_BGM bgm, bool isTop)
{
    // 同一曲が指定された場合、曲の不自然な再開を防ぐため処理をスキップ
    if (nowPlayingBgm == bgm && !isTop)
    {
        return;
    }

    for (auto it = bgmHandleList.begin(); it != bgmHandleList.end(); it++)
    {
        if (it->first == bgm)
        {
            // 設定されている最新のマスター音量で再生を開始する
            ChangeVolumeSoundMem(masterBGMVolume, it->second);
            PlaySoundMem(it->second, DX_PLAYTYPE_LOOP, isTop);
            nowPlayingBgm = bgm;
            break;
        }
    }
}

/*
 * 効果音をバックグラウンドで並列再生する
 * [入力] se: 再生するSEのID
 * [出力] なし
 * [副作用] SE再生開始、mnNowPlayingSeの更新
 */
void SoundManager::PlaySE(SOUND_SE se)
{
    for (auto it = seHandleList.begin(); it != seHandleList.end(); it++)
    {
        if (it->first == se)
        {
            // SEは複数同時に鳴る可能性があるため並列再生を指定
            PlaySoundMem(it->second, DX_PLAYTYPE_BACK);
            nowPlayingSe = se;
            break;
        }
    }
}

/*
 * 音源の二重ロードを防ぎつつBGMをメモリに配置する
 * [入力] bgm: 登録ID, filename: ファイルパス
 * [出力] なし
 * [副作用] ハンドル取得とリストへの追加
 */
void SoundManager::LoadBGM(SOUND_BGM bgm, std::string filename)
{
    // 無駄なメモリ消費を防ぐため、既に登録済みの場合はスキップ
    for (auto it = bgmHandleList.begin(); it != bgmHandleList.end(); it++)
    {
        if (it->first == bgm)
        {
            return;
        }
    }

    int handle = LoadSoundMem(filename.c_str());
    // 存在しないファイルパス等による無効なハンドル登録を防ぐ
    if (handle == -1)
    {
        return;
    }
    ChangeVolumeSoundMem(masterBGMVolume, handle);
    bgmHandleList.push_back(std::pair<SOUND_BGM, int>(bgm, handle));
}

/*
 * 音源の二重ロードを防ぎつつSEをメモリに配置する
 * [入力] se: 登録ID, filename: ファイルパス
 * [出力] なし
 * [副作用] ハンドル取得とリストへの追加
 */
void SoundManager::LoadSE(SOUND_SE se, std::string filename)
{
    // 無駄なメモリ消費を防ぐため、既に登録済みの場合はスキップ
    for (auto it = seHandleList.begin(); it != seHandleList.end(); it++)
    {
        if (it->first == se)
        {
            return;
        }
    }

    int handle = LoadSoundMem(filename.c_str());
    // 存在しないファイルパス等による無効なハンドル登録を防ぐ
    if (handle == -1)
    {
        return;
    }
    ChangeVolumeSoundMem(masterSEVolume, handle);
    seHandleList.push_back(std::pair<SOUND_SE, int>(se, handle));
}

/*
 * シーン終了時などに現在再生中のBGMを止める
 * [入力] なし
 * [出力] なし
 * [副作用] 再生中のBGMを停止
 */
void SoundManager::StopBGM()
{
    for (auto it = bgmHandleList.begin(); it != bgmHandleList.end(); it++)
    {
        if (it->first == nowPlayingBgm)
        {
            // DxLib側で既に停止している場合のエラーを回避
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
 * [出力] なし
 * [副作用] ハンドルを通じたBGM音量変更
 */
void SoundManager::SetBGMVolume(int volume)
{
    for (auto it = bgmHandleList.begin(); it != bgmHandleList.end(); it++)
    {
        ChangeVolumeSoundMem(volume, it->second);
    }
}

/*
 * ロード済み全SEに音量を一括適用する
 * [入力] volume: 音量値(0-255)
 * [出力] なし
 * [副作用] ハンドルを通じたSE音量変更
 */
void SoundManager::SetSEVolume(int volume)
{
    for (auto it = seHandleList.begin(); it != seHandleList.end(); it++)
    {
        ChangeVolumeSoundMem(volume, it->second);
    }
}

/*
 * システム設定などで一括変更されるBGMマスター音量を適用する
 * [入力] volume: 基準音量(0-255)
 * [出力] なし
 * [副作用] mMasterBGMVolume更新、全BGMへ音量反映
 */
void SoundManager::SetMasterBGMVolume(int volume)
{
    // DxLibの音量指定範囲(0-255)外の値を防ぐためのクランプ
    if (volume < 0) volume = 0;
    if (volume > 255) volume = 255;
    masterBGMVolume = volume;
    SetBGMVolume(masterBGMVolume);
}

/*
 * システム設定などで一括変更されるSEマスター音量を適用する
 * [入力] volume: 基準音量(0-255)
 * [出力] なし
 * [副作用] mMasterSEVolume更新、全SEへ音量反映
 */
void SoundManager::SetMasterSEVolume(int volume)
{
    // DxLibの音量指定範囲(0-255)外の値を防ぐためのクランプ
    if (volume < 0) volume = 0;
    if (volume > 255) volume = 255;
    masterSEVolume = volume;
    SetSEVolume(masterSEVolume);
}
