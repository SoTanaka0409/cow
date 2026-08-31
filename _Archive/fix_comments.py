import codecs

input_manager_h = """#pragma once

// DxLibのキー入力をラップし、フレーム間の状態変化を管理する
class InputManager
{
public:
    InputManager();
    ~InputManager();

    /*
     * 連続入力を防ぐため、押し始めの1フレームのみを検知する
     * [入力] KeyCode: DxLibのキーコード
     * [出力] 1: 押下開始フレーム、0: それ以外
     * [副作用] mDownBufferを更新
     */
    static int CheckDownKey(int KeyCode);

    /*
     * 押し続けを許容する通常のキー入力判定
     * [入力] KeyCode: DxLibのキーコード
     * [出力] 1: 押下中、0: 非押下
     * [副作用] なし
     */
    static int CheckPressKey(int KeyCode);

private:
    // トリガー判定に必要な前フレームのキー状態を保持
    static int mDownBuffer[256];
};
"""

input_manager_cpp = """#include"InputManager.h"
#include"DxLib.h"

int InputManager::mDownBuffer[256] = { 0 };

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}

/*
 * 連続入力を防ぐため、押し始めの1フレームのみを検知する
 * [入力] KeyCode: DxLibのキーコード
 * [出力] 1: 押下開始フレーム、0: それ以外
 * [副作用] mDownBufferを更新
 */
int InputManager::CheckDownKey(int KeyCode)
{
    int result = 0;
    int keyState = CheckHitKey(KeyCode);
    
    // 長押しによる連続処理を防ぐためのトリガー判定
    if (mDownBuffer[KeyCode] == 0 && keyState == 1)
    {
        result = 1;
    }

    // 次フレームのトリガー判定用に状態を記録
    mDownBuffer[KeyCode] = keyState;
    return result;
}

/*
 * 押し続けを許容する通常のキー入力判定
 * [入力] KeyCode: DxLibのキーコード
 * [出力] 1: 押下中、0: 非押下
 * [副作用] なし
 */
int InputManager::CheckPressKey(int KeyCode)
{
    return CheckHitKey(KeyCode);
}
"""

sound_manager_h = """#pragma once
#include<string>
#include<vector>

// ゲーム内BGMとSEの二重再生防止や音量の一括制御を行うための管理クラス
class SoundManager
{
public:
    // シーン遷移時のBGM切り替え用ID
    enum SOUND_BGM
    {
        BGM_TITLE = 0,
        BGM_GAME,
        BGM_RESULT,
        BGM_TUTORIAL,
        BGM_RULE,
    };

    // 各種アクション効果音呼び出し用ID
    enum SOUND_SE
    {
        SE_DECIDE = 0,
        SE_COW,
        SE_BEAM,
        SE_SKILL_STATUS,
        SE_SKILL_FOOD,
        SE_CHICKEN,
        SE_TATUMAKI,
        SE_KAMINARI,
        SE_TutorialChange,
        SE_BAITFINAL,
    };

    SoundManager();
    ~SoundManager();

    /*
     * ゲームプレイ中のロード遅延を防ぐため全音源を初期化時に読み込む
     * [入力] なし
     * [出力] なし
     * [副作用] 各ハンドルリストにサウンドリソースを登録
     */
    void Initialize();

    /*
     * メモリリークを防ぐための全サウンドリソース解放
     * [入力] なし
     * [出力] なし
     * [副作用] BGM・SEハンドルの削除とリストのクリア
     */
    void Finalize();

    /*
     * BGMの再生と、同一曲の二重再生防止を制御する
     * [入力] bgm: 再生するBGMID, IsTop: trueで先頭から再生
     * [出力] なし
     * [副作用] BGM再生開始、mnNowPlayingBgmの更新
     */
    void PlayBGM(SOUND_BGM bgm, bool IsTop = true);

    /*
     * 効果音をバックグラウンドで並列再生する
     * [入力] se: 再生するSEのID
     * [出力] なし
     * [副作用] SE再生開始、mnNowPlayingSeの更新
     */
    void PlaySE(SOUND_SE se);

    /*
     * 音源の二重ロードを防ぎつつBGMをメモリに配置する
     * [入力] bgm: 登録ID, filename: ファイルパス
     * [出力] なし
     * [副作用] ハンドル取得とリストへの追加
     */
    void LoadBGM(SOUND_BGM bgm, std::string filename);

    /*
     * 音源の二重ロードを防ぎつつSEをメモリに配置する
     * [入力] se: 登録ID, filename: ファイルパス
     * [出力] なし
     * [副作用] ハンドル取得とリストへの追加
     */
    void LoadSE(SOUND_SE se, std::string filename);

    /*
     * シーン終了時などに現在再生中のBGMを止める
     * [入力] なし
     * [出力] なし
     * [副作用] 再生中のBGMを停止
     */
    void StopBGM();

    /*
     * ロード済み全BGMに音量を一括適用する
     * [入力] volume: 音量値(0-255)
     * [出力] なし
     * [副作用] ハンドルを通じたBGM音量変更
     */
    void SetBGMVolume(int volume);

    /*
     * ロード済み全SEに音量を一括適用する
     * [入力] volume: 音量値(0-255)
     * [出力] なし
     * [副作用] ハンドルを通じたSE音量変更
     */
    void SetSEVolume(int volume);

    int GetMasterBGMVolume() const { return mMasterBGMVolume; }
    int GetMasterSEVolume() const { return mMasterSEVolume; }

    /*
     * システム設定などで一括変更されるBGMマスター音量を適用する
     * [入力] volume: 基準音量(0-255)
     * [出力] なし
     * [副作用] mMasterBGMVolume更新、全BGMへ音量反映
     */
    void SetMasterBGMVolume(int volume);

    /*
     * システム設定などで一括変更されるSEマスター音量を適用する
     * [入力] volume: 基準音量(0-255)
     * [出力] なし
     * [副作用] mMasterSEVolume更新、全SEへ音量反映
     */
    void SetMasterSEVolume(int volume);

private:
    int mMasterBGMVolume = 255;
    int mMasterSEVolume = 255;

    // 停止処理や二重再生防止の判定に用いる状態保持
    SOUND_BGM mnNowPlayingBgm;
    SOUND_SE mnNowPlayingSe;

    std::vector<std::pair<SOUND_BGM, int>> mnBgmHandleList;
    std::vector<std::pair<SOUND_SE, int>> mnSeHandleList;
};
"""

sound_manager_cpp = """#include"SoundManager.h"
#include"DxLib.h"

SoundManager::SoundManager()
    : mnNowPlayingBgm((SOUND_BGM)-1)
    , mnNowPlayingSe((SOUND_SE)-1)
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
    for (auto it = mnBgmHandleList.begin(); it != mnBgmHandleList.end(); it++)
    {
        DeleteSoundMem(it->second);
    }
    mnBgmHandleList.clear();

    for (auto it = mnSeHandleList.begin(); it != mnSeHandleList.end(); it++)
    {
        DeleteSoundMem(it->second);
    }
    mnSeHandleList.clear();
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
    if (mnNowPlayingBgm == bgm && !isTop)
    {
        return;
    }

    for (auto it = mnBgmHandleList.begin(); it != mnBgmHandleList.end(); it++)
    {
        if (it->first == bgm)
        {
            // 設定されている最新のマスター音量で再生を開始する
            ChangeVolumeSoundMem(mMasterBGMVolume, it->second);
            PlaySoundMem(it->second, DX_PLAYTYPE_LOOP, isTop);
            mnNowPlayingBgm = bgm;
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
    for (auto it = mnSeHandleList.begin(); it != mnSeHandleList.end(); it++)
    {
        if (it->first == se)
        {
            // SEは複数同時に鳴る可能性があるため並列再生を指定
            PlaySoundMem(it->second, DX_PLAYTYPE_BACK);
            mnNowPlayingSe = se;
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
    for (auto it = mnBgmHandleList.begin(); it != mnBgmHandleList.end(); it++)
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
    ChangeVolumeSoundMem(mMasterBGMVolume, handle);
    mnBgmHandleList.push_back(std::pair<SOUND_BGM, int>(bgm, handle));
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
    for (auto it = mnSeHandleList.begin(); it != mnSeHandleList.end(); it++)
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
    ChangeVolumeSoundMem(mMasterSEVolume, handle);
    mnSeHandleList.push_back(std::pair<SOUND_SE, int>(se, handle));
}

/*
 * シーン終了時などに現在再生中のBGMを止める
 * [入力] なし
 * [出力] なし
 * [副作用] 再生中のBGMを停止
 */
void SoundManager::StopBGM()
{
    for (auto it = mnBgmHandleList.begin(); it != mnBgmHandleList.end(); it++)
    {
        if (it->first == mnNowPlayingBgm)
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
    for (auto it = mnBgmHandleList.begin(); it != mnBgmHandleList.end(); it++)
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
    for (auto it = mnSeHandleList.begin(); it != mnSeHandleList.end(); it++)
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
    mMasterBGMVolume = volume;
    SetBGMVolume(mMasterBGMVolume);
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
    mMasterSEVolume = volume;
    SetSEVolume(mMasterSEVolume);
}
"""

with codecs.open("InputManager.h", "w", "utf-8-sig") as f:
    f.write(input_manager_h)

with codecs.open("InputManager.cpp", "w", "utf-8-sig") as f:
    f.write(input_manager_cpp)

with codecs.open("SoundManager.h", "w", "utf-8-sig") as f:
    f.write(sound_manager_h)

with codecs.open("SoundManager.cpp", "w", "utf-8-sig") as f:
    f.write(sound_manager_cpp)
