#pragma once
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

    int GetMasterBGMVolume() const { return masterBGMVolume; }
    int GetMasterSEVolume() const { return masterSEVolume; }

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
    int masterBGMVolume = 255;
    int masterSEVolume = 255;

    // 停止処理や二重再生防止の判定に用いる状態保持
    SOUND_BGM nowPlayingBgm;
    SOUND_SE nowPlayingSe;

    std::vector<std::pair<SOUND_BGM, int>> bgmHandleList;
    std::vector<std::pair<SOUND_SE, int>> seHandleList;
};
