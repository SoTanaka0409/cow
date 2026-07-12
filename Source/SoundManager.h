#pragma once
#include<string>
#include<vector>

// ゲーム冁EGMとSEの二重再生防止めE��量�E一括制御を行うための管琁E��ラス
class SoundManager
{
public:
    // シーン遷移時�EBGM刁E��替え用ID
    enum SoundBgm
    {
        kBgmTitle = 0,
        kBgmGame,
        kBgmResult,
        kBgmTutorial,
        kBgmRule,
    };

    // �ꍇ��アクション効果音呼び出し用ID
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

    /*
     * ゲームプレイ中のロード遅延を防ぐため�E音源を初期化時に読み込む
     * [入力] �Ȃ�
     * [出力] �Ȃ�
     * [副作用] �ꍇ��ンドルリストにサウンドリソースを登録
     */
    void Initialize();

    /*
     * メモリリークを防ぐため�E全サウンドリソース解放
     * [入力] �Ȃ�
     * [出力] �Ȃ�
     * [副作用] BGM�ESEハンドルの削除とリスト�Eクリア
     */
    void Finalize();

    /*
     * BGMの再生と、同一曲の二重再生防止を制御する
     * [入力] bgm: 再生するBGMID, IsTop: trueで先頭から再生
     * [出力] �Ȃ�
     * [副作用] BGM再生開始、mnNowPlayingBgmの更新
     */
    void PlayBGM(SoundBgm bgm, bool IsTop = true);

    /*
     * 効果音をバチE��グラウンドで並列�E生すめE
     * [入力] se: 再生するSEのID
     * [出力] �Ȃ�
     * [副作用] SE再生開始、mnNowPlayingSeの更新
     */
    void PlaySE(SoundSe se);

    /*
     * 音源�E二重ロードを防ぎつつBGMをメモリに配置する
     * [入力] bgm: 登録ID, filename: ファイルパス
     * [出力] �Ȃ�
     * [副作用] ハンドル取得とリストへの追加
     */
    void LoadBGM(SoundBgm bgm, std::string filename);

    /*
     * 音源�E二重ロードを防ぎつつSEをメモリに配置する
     * [入力] se: 登録ID, filename: ファイルパス
     * [出力] �Ȃ�
     * [副作用] ハンドル取得とリストへの追加
     */
    void LoadSE(SoundSe se, std::string filename);

    /*
     * シーン終亁E��などに現在再生中のBGMを止める
     * [入力] �Ȃ�
     * [出力] �Ȃ�
     * [副作用] 再生中のBGMを停止
     */
    void StopBGM();

    /*
     * ロード済み全BGMに音量を一括適用する
     * [入力] volume: 音量値(0-255)
     * [出力] �Ȃ�
     * [副作用] ハンドルを通じたBGM音量変更
     */
    void SetBGMVolume(int volume);

    /*
     * ロード済み全SEに音量を一括適用する
     * [入力] volume: 音量値(0-255)
     * [出力] �Ȃ�
     * [副作用] ハンドルを通じたSE音量変更
     */
    void SetSEVolume(int volume);

    int GetMasterBGMVolume() const { return master_bgm_volume_; }
    int GetMasterSEVolume() const { return master_se_volume_; }

    /*
     * シスチE��設定などで一括変更されるBGMマスター音量を適用する
     * [入力] volume: 基準音釁E0-255)
     * [出力] �Ȃ�
     * [副作用] mMasterBGMVolume更新、�EBGMへ音量反映
     */
    void SetMasterBGMVolume(int volume);

    /*
     * シスチE��設定などで一括変更されるSEマスター音量を適用する
     * [入力] volume: 基準音釁E0-255)
     * [出力] �Ȃ�
     * [副作用] mMasterSEVolume更新、�ESEへ音量反映
     */
    void SetMasterSEVolume(int volume);

private:
    int master_bgm_volume_ = 255;
    int master_se_volume_ = 255;

    // 停止処琁E��二重再生防止の判定に用ぁE��状態保持
    SoundBgm now_playing_bgm_;
    SoundSe now_playing_se_;

    std::vector<std::pair<SoundBgm, int>> bgm_handle_list_;
    std::vector<std::pair<SoundSe, int>> se_handle_list_;
};