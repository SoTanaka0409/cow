#pragma once
#include<string>
#include<vector>

// ゲーム内の音響（BGMおよび効果音）のロード・再生・音量制御を一括管理するクラス
class SoundManager
{
public:
	// 再生可能なBGMのID定義
	enum SOUND_BGM
	{
		BGM_TITLE = 0,   // タイトル画面
		BGM_GAME,        // ゲームメイン画面
		BGM_RESULT,      // リザルト（結果表示）画面
		BGM_TUTORIAL,    // チュートリアル画面
		BGM_RULE,        // 操作説明画面
	};

	// 再生可能な効果音（SE）のID定義
	enum SOUND_SE
	{
		SE_DECIDE = 0,        // メニュー等の決定音
		SE_COW,               // 牛の鳴き声（吸引時・通常時）
		SE_BEAM,              // UFOの吸引ビーム放射音
		SE_SKILL_STATUS,      // スキルによるステータスアップ時効果音
		SE_SKILL_FOOD,        // スキルによるエサ投下時の起動音
		SE_CHICKEN,           // ニワトリの鳴き声
		SE_TATUMAKI,          // スキル（竜巻）発動時の風切音
		SE_KAMINARI,          // スキル（落雷）発動時の雷鳴音
		SE_TutorialChange,    // チュートリアルの進行状況変化時の効果音
		SE_BAITFINAL,        // エサ投下完了時の効果音
	};

	SoundManager();
	~SoundManager();

	/*
	 * @brief 使用するすべてのBGM・SEアセットを一括でプリロードする
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 各サウンドリソースがロードされ、ハンドルリストに登録される
	 */
	void Initialize();

	/*
	 * @brief 読み込んだすべてのサウンドリソースをメモリから解放する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] BGM・SEハンドルがすべて削除され、各ハンドルリストがクリアされる
	 */
	void Finalize();

	/*
	 * @brief 指定されたBGMのループ再生を開始する（既に再生中の場合は二重再生を防止）
	 * [入力] bgm: 再生するBGMのID, IsTop: trueの場合は曲の先頭から再生する
	 * [出力] なし
	 * [副作用] 指定BGMの再生開始、mnNowPlayingBgmの更新
	 */
	void PlayBGM(SOUND_BGM bgm, bool IsTop = true);

	/*
	 * @brief 指定されたSEを単発（バックグラウンド）再生する
	 * [入力] se: 再生するSEのID
	 * [出力] なし
	 * [副作用] SEの再生開始、mnNowPlayingSeの更新
	 */
	void PlaySE(SOUND_SE se);

	/*
	 * @brief 個別にBGMアセットを読み込み、リストに登録する
	 * [入力] bgm: 登録ID, filename: ファイルパス
	 * [出力] なし
	 * [副作用] ロード成功時にBGMハンドルリストに登録
	 */
	void LoadBGM(SOUND_BGM bgm, std::string filename);

	/*
	 * @brief 個別にSEアセットを読み込み、リストに登録する
	 * [入力] se: 登録ID, filename: ファイルパス
	 * [出力] なし
	 * [副作用] ロード成功時にSEハンドルリストに登録
	 */
	void LoadSE(SOUND_SE se, std::string filename);

	/*
	 * @brief 現在再生中のBGMの再生を停止する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 再生中のBGMメモリの再生停止
	 */
	void StopBGM();

	/*
	 * @brief 読み込み済みすべてのBGMの再生音量を一時設定する
	 * [入力] volume: 音量値 (0 - 255)
	 * [出力] なし
	 * [副作用] 登録BGMの再生音量変更
	 */
	void SetBGMVolume(int volume);

	/*
	 * @brief 読み込み済みすべてのSEの再生音量を一時設定する
	 * [入力] volume: 音量値 (0 - 255)
	 * [出力] なし
	 * [副作用] 登録SEの再生音量変更
	 */
	void SetSEVolume(int volume);

	// 基準音量（マスター音量）のゲッター・セッター群
	int GetMasterBGMVolume() const { return mMasterBGMVolume; }
	int GetMasterSEVolume() const { return mMasterSEVolume; }
	void SetMasterBGMVolume(int volume);
	void SetMasterSEVolume(int volume);

private:
	int mMasterBGMVolume = 255;  // 全BGMの基準音量 (0 - 255)
	int mMasterSEVolume = 255;   // 全SEの基準音量 (0 - 255)

	SOUND_BGM mnNowPlayingBgm;   // 現在再生中のBGM ID (未再生時は-1)
	SOUND_SE mnNowPlayingSe;     // 直近で再生されたSE ID (未再生時は-1)

	std::vector<std::pair<SOUND_BGM, int>> mnBgmHandleList; // ロードされたBGMアセットのハンドルリスト
	std::vector<std::pair<SOUND_SE, int>> mnSeHandleList;   // ロードされたSEアセットのハンドルリスト
};

