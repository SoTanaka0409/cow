#include"SoundManager.h"
#include"DxLib.h"

SoundManager::SoundManager()
	: now_playing_bgm_((SoundBgm)-1)
	, now_playing_se_((SoundSe)-1)
{
}

SoundManager::~SoundManager()
{
	Finalize();
}

/*
 * 入力: なし
 * 出力: なし
 * 副作用: 全サウンドリソースのメモリへのロードおよびハンドルリストへの登録
 */
void SoundManager::Initialize()
{
	// ゲームプレイ中の動的ロードによるスパイク(画面の瞬間的なフリーズ)を防ぐため、起動時に全てVRAM/RAMへ確保する
	LoadBGM(SoundBgm::kBgmTitle, "Resource/BGM/hajimetenootsukai.mp3");
	LoadBGM(SoundBgm::kBgmGame, "Resource/BGM/natsuyasuminotanken.mp3");
	LoadBGM(SoundBgm::kBgmResult, "Resource/BGM/retrogamecenter3.mp3");
	LoadBGM(SoundBgm::kBgmTutorial, "Resource/BGM/MusMus-BGM-103.mp3");
	LoadBGM(SoundBgm::kBgmRule, "Resource/BGM/MusMus-BGM-158.mp3");

	LoadSE(SoundSe::kSeDecide, "Resource/SE/豎ｺ螳壹・繧ｿ繝ｳ繧呈款縺・2.mp3");
	LoadSE(SoundSe::kSeCow, "Resource/SE/迚帙・魑ｴ縺榊｣ｰ.mp3");
	LoadSE(SoundSe::kSeChicken, "Resource/SE/MusMus-BGM-084");
	LoadSE(SoundSe::kSeTornado, "Resource/SE/螟ｩ蛟吶・鬚ｨ荳€蜷ｹ縺搾ｼ医＠繧峨￠陦ｨ迴ｾ・・mp3");
	LoadSE(SoundSe::kSeKaminari, "Resource/SE/螟ｩ蛟吶・髮ｷ01.mp3");
	LoadSE(SoundSe::kSeTutorialChange, "Resource/SE/繧ｷ繝ｼ繝ｳ蛻・ｊ譖ｿ縺・.mp3");
	LoadSE(SoundSe::kSeBeam, "Resource/SE/HP蜷ｸ蜿朱ｭ疲ｳ・.mp3");
	LoadSE(SoundSe::kSeSkillStatus, "Resource/SE/繧ｹ繝・・繧ｿ繧ｹ荳頑・鬲疲ｳ・.mp3");
	LoadSE(SoundSe::kSeSkillFood, "Resource/SE/繝ｭ繝懊ャ繝郁ｵｷ蜍・.mp3");
	LoadSE(SoundSe::kSeBaitFinal, "Resource/SE/鬮倬€溽ｧｻ蜍・mp3");
}

/*
 * 入力: なし
 * 出力: なし
 * 副作用: 全BGM・SEハンドルの破棄およびリストのクリア
 */
void SoundManager::Finalize()
{
	// アプリ終了時にサウンドデバイスやメモリのリークが発生し、OS側でプロセスが残留するバグを防ぐため明示的に破棄する
	for (auto& pair : bgm_handle_list_)
	{
		DeleteSoundMem(pair.second);
	}
	bgm_handle_list_.clear();

	for (auto& pair : se_handle_list_)
	{
		DeleteSoundMem(pair.second);
	}
	se_handle_list_.clear();
}

/*
 * 入力: bgm (再生対象のBGMID), isTop (先頭から再生するか)
 * 出力: なし
 * 副作用: BGMのループ再生開始および現在再生中BGMの更新
 */
void SoundManager::PlayBGM(SoundBgm bgm, bool isTop)
{
	// 同一シーンへの連続遷移などで同じBGMが要求された際、イントロから再スタートしてブツ切れになるのを防ぐ
	if (now_playing_bgm_ == bgm && !isTop)
	{
		return;
	}

	for (auto it = bgm_handle_list_.begin(); it != bgm_handle_list_.end(); it++)
	{
		if (it->first == bgm)
		{
			ChangeVolumeSoundMem(master_bgm_volume_, it->second);
			PlaySoundMem(it->second, DX_PLAYTYPE_LOOP, isTop);
			now_playing_bgm_ = bgm;
			break;
		}
	}
}

/*
 * 入力: se (再生対象のSEID)
 * 出力: なし
 * 副作用: SEの非同期(バックグラウンド)再生開始
 */
void SoundManager::PlaySE(SoundSe se)
{
	for (auto it = se_handle_list_.begin(); it != se_handle_list_.end(); it++)
	{
		if (it->first == se)
		{
			// 外部仕様依存: メインスレッドの処理をブロックしてゲームが停止しないよう、DX_PLAYTYPE_BACKで非同期再生する
			PlaySoundMem(it->second, DX_PLAYTYPE_BACK);
			now_playing_se_ = se;
			break;
		}
	}
}

/*
 * 入力: bgm (登録するBGMID), filename (ファイルパス)
 * 出力: なし
 * 副作用: ファイルのロード、初期音量の適用、およびリストへの追加
 */
void SoundManager::LoadBGM(SoundBgm bgm, std::string filename)
{
	// 既にロード済みのリソースを再読み込みしてメモリを無駄に消費し、ハンドルの管理が破綻するバグを防ぐ
	for (auto it = bgm_handle_list_.begin(); it != bgm_handle_list_.end(); it++)
	{
		if (it->first == bgm)
		{
			return;
		}
	}

	int handle = LoadSoundMem(filename.c_str());
	if (handle == -1)
	{
		return;
	}

	// 再生時に爆音で鳴るのを防ぐため、ロード直後に現在のマスターボリュームを適用しておく
	ChangeVolumeSoundMem(master_bgm_volume_, handle);
	bgm_handle_list_.push_back(std::pair<SoundBgm, int>(bgm, handle));
}

/*
 * 入力: se (登録するSEID), filename (ファイルパス)
 * 出力: なし
 * 副作用: ファイルのロード、初期音量の適用、およびリストへの追加
 */
void SoundManager::LoadSE(SoundSe se, std::string filename)
{
	// LoadBGMと同様、二重ロードによるメモリリークと管理破綻の防止
	for (auto it = se_handle_list_.begin(); it != se_handle_list_.end(); it++)
	{
		if (it->first == se)
		{
			return;
		}
	}

	int handle = LoadSoundMem(filename.c_str());
	if (handle == -1)
	{
		return;
	}
	ChangeVolumeSoundMem(master_se_volume_, handle);
	se_handle_list_.push_back(std::pair<SoundSe, int>(se, handle));
}

/*
 * 入力: なし
 * 出力: なし
 * 副作用: 再生中BGMの停止処理
 */
void SoundManager::StopBGM()
{
	for (auto it = bgm_handle_list_.begin(); it != bgm_handle_list_.end(); it++)
	{
		if (it->first == now_playing_bgm_)
		{
			// 既に自然終了した音源や無効なハンドルに対して停止命令を出し、DxLib内部でクラッシュするのを防ぐ
			if (CheckSoundMem(it->second))
			{
				StopSoundMem(it->second);
				break;
			}
		}
	}
}

/*
 * 入力: volume (設定する音量値 0-255)
 * 出力: なし
 * 副作用: ロード済み全BGMへの音量適用
 */
void SoundManager::SetBGMVolume(int volume)
{
	for (auto it = bgm_handle_list_.begin(); it != bgm_handle_list_.end(); it++)
	{
		ChangeVolumeSoundMem(volume, it->second);
	}
}

/*
 * 入力: volume (設定する音量値 0-255)
 * 出力: なし
 * 副作用: ロード済み全SEへの音量適用
 */
void SoundManager::SetSEVolume(int volume)
{
	for (auto it = se_handle_list_.begin(); it != se_handle_list_.end(); it++)
	{
		ChangeVolumeSoundMem(volume, it->second);
	}
}

/*
 * 入力: volume (マスターBGM音量 0-255)
 * 出力: なし
 * 副作用: マスター値の更新および全BGM音量の再設定
 */
void SoundManager::SetMasterBGMVolume(int volume)
{
	if (volume < 0) volume = 0;
	if (volume > 255) volume = 255;
	master_bgm_volume_ = volume;

	// 設定画面等で音量が変更された際、現在鳴っているBGMと今後鳴るBGMの両方に即座に反映させる
	SetBGMVolume(master_bgm_volume_);
}

/*
 * 入力: volume (マスターSE音量 0-255)
 * 出力: なし
 * 副作用: マスター値の更新および全SE音量の再設定
 */
void SoundManager::SetMasterSEVolume(int volume)
{
	if (volume < 0) volume = 0;
	if (volume > 255) volume = 255;
	master_se_volume_ = volume;
	SetSEVolume(master_se_volume_);
}