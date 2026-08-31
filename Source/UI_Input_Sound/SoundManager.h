#pragma once
#include<string>
#include<vector>

/// @brief 設計ルール：重複ロードの防止や音量の一括制御、および同フレーム内でのSE暴発による爆音化を防ぐための音響管理クラス
class SoundManager
{
public:
	enum SoundBgm
	{
		kBgmTitle = 0,
		kBgmGame,
		kBgmResult,
		kBgmTutorial,
		kBgmRule,
	};

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

	/// @brief 全BGM・SEアセットの事前読み込み、および初期マスターボリュームの適用
	void Initialize();

	/// @brief DxLibのサウンドメモリハンドルをすべて一括解放
	void Finalize();

	/// @param bgm 再生対象のID
	/// @param IsTop 最初からループ再生するか（falseでレジューム）
	/// @brief 指定BGMの再生開始、現在再生中のBGM状態の更新
	void PlayBGM(SoundBgm bgm, bool IsTop = true);

	/// @param se 再生対象のID
	/// @brief 指定SEの単発再生（同一SEが重複した場合は多重再生）
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
	/// @brief 仕様制約：DxLibの最大音量値（255）を基準とした、ユーザー設定用のシステム音量基準値
	int master_bgm_volume_ = 255; ///< 音量やサウンド再生状態を管理する値
	int master_se_volume_ = 255;  ///< 音量やサウンド再生状態を管理する値

	/// @brief 同じBGMを重ねて再生しようとした際のリスタート（音途切れ）を検知・防御するための現在再生ID
	SoundBgm now_playing_bgm_; ///< 音量やサウンド再生状態を管理する値
	SoundSe now_playing_se_;   ///< 音量やサウンド再生状態を管理する値

	/// @brief シーン切り替え時の同期ズレ（プチノイズ）を防ぐため、IDとDxLibハンドルをペアでキャッシュ管理
	std::vector<std::pair<SoundBgm, int>> bgm_handle_list_; ///< 音量やサウンド再生状態を管理する値
	std::vector<std::pair<SoundSe, int>> se_handle_list_;   ///< 音量やサウンド再生状態を管理する値
};
