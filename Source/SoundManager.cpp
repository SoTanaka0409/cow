#include "SoundManager.h"
#include "DxLib.h"

SoundManager::SoundManager()
	: now_playing_bgm_((SoundBgm)-1)
	, now_playing_se_((SoundSe)-1)
{
}

SoundManager::~SoundManager()
{
	Finalize();
}

// 入力：なし
// 出力：なし
// 副作用：全サウンドデータの動的メモリ確保、および管理リストへの登録
void SoundManager::Initialize()
{
	// パフォーマンス理由：ゲーム中のBGMロードによる一瞬のフレーム落ち（スパイク）を防ぐため、ゲーム起動時に先行ロードする
	LoadBGM(SoundBgm::kBgmTitle, "Resource/BGM/シーン/BGM_タイトル.mp3");
	LoadBGM(SoundBgm::kBgmGame, "Resource/BGM/シーン/BGM_ゲーム中.mp3");
	LoadBGM(SoundBgm::kBgmResult, "Resource/BGM/シーン/BGM_リザルト.mp3");
	LoadBGM(SoundBgm::kBgmTutorial, "Resource/BGM/シーン/BGM_チュートリアル.mp3");
	LoadBGM(SoundBgm::kBgmRule, "Resource/BGM/シーン/BGM_ルール説明.mp3");

	LoadSE(SoundSe::kSeDecide, "Resource/SE/UI/効果音_決定.mp3");
	LoadSE(SoundSe::kSeCow, "Resource/SE/牛/効果音_牛の鳴き声.mp3");
	LoadSE(SoundSe::kSeChicken, "Resource/SE/牛/効果音_牛の鳴き声.mp3");
	LoadSE(SoundSe::kSeTornado, "Resource/SE/天候/効果音_竜巻.mp3");
	LoadSE(SoundSe::kSeKaminari, "Resource/SE/天候/効果音_雷.mp3");
	LoadSE(SoundSe::kSeTutorialChange, "Resource/SE/UI/効果音_画面切り替え.mp3");
	LoadSE(SoundSe::kSeBeam, "Resource/SE/プレイヤー/効果音_吸い込み.mp3");
	LoadSE(SoundSe::kSeSkillStatus, "Resource/SE/スキル/効果音_ステータス上昇.mp3");
	LoadSE(SoundSe::kSeSkillFood, "Resource/SE/スキル/効果音_牛の餌スキル.mp3");
	LoadSE(SoundSe::kSeBaitFinal, "Resource/SE/スキル/効果音_高速移動.mp3");
}

// 入力：なし
// 出力：なし
// 副作用：DxLib内の全サウンドメモリハンドルの破棄、および管理リストの全消去
void SoundManager::Finalize()
{
	// アプリ終了時にBGM・SEのハンドル解放漏れが発生し、OS上にゾンビプロセスが残留するメモリリークバグを防止する
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

// 入力：bgm=再生対象のBGMID, isTop=再生位置を先頭に戻すかのフラグ
// 出力：なし
// 副作用：指定BGMのループ再生開始、および現在再生中BGM状態の更新
void SoundManager::PlayBGM(SoundBgm bgm, bool isTop)
{
	// 同一シーンへの遷移や連続呼び出し時に、再生中のBGMが最初から再生し直されて曲が途切れるのを防ぐ
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

// 入力：se=再生対象のSEID
// 出力：なし
// 副作用：SEの再生開始、現在再生中SE状態の更新
void SoundManager::PlaySE(SoundSe se)
{
	for (auto it = se_handle_list_.begin(); it != se_handle_list_.end(); it++)
	{
		if (it->first == se)
		{
			// 外部仕様依存：メインスレッドの描画処理をブロックさせず、ゲームの処理落ちを防ぐため非同期（バックグラウンド）で流す
			PlaySoundMem(it->second, DX_PLAYTYPE_BACK);
			now_playing_se_ = se;
			break;
		}
	}
}

// 入力：bgm=ロード対象のBGMID, filename=ファイルパス
// 出力：なし
// 副作用：DxLibへのサウンドリソース登録、およびロード済み配列への登録
void SoundManager::LoadBGM(SoundBgm bgm, std::string filename)
{
	// 二重ロードによる同一リソースの重複登録と、無駄なサウンドメモリ消費を防ぐための事前確認
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

	// ロード直後の1フレーム目に、マスター音量設定を無視した「等倍の爆音」が一瞬再生されるバグを防止する
	ChangeVolumeSoundMem(master_bgm_volume_, handle);
	bgm_handle_list_.push_back(std::pair<SoundBgm, int>(bgm, handle));
}

// 入力：se=ロード対象のSEID, filename=ファイルパス
// 出力：なし
// 副作用：DxLibへのサウンドリソース登録、およびロード済み配列への登録
void SoundManager::LoadSE(SoundSe se, std::string filename)
{
	// BGMと同様、二重ロードによるサウンドメモリのリークおよびハンドル管理の重複バグを回避する
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

// 入力：なし
// 出力：なし
// 副作用：現在再生中のBGMのサウンド停止
void SoundManager::StopBGM()
{
	for (auto it = bgm_handle_list_.begin(); it != bgm_handle_list_.end(); it++)
	{
		if (it->first == now_playing_bgm_)
		{
			// バグ回避：既に自然終了している、または無効なアセットハンドルに対して二重に停止命令を出してクラッシュするのを防ぐ
			if (CheckSoundMem(it->second))
			{
				StopSoundMem(it->second);
				break;
			}
		}
	}
}

// 入力：volume=変更先の音量値（0-255）
// 出力：なし
// 副作用：登録されている全BGMのシステム音量の書き換え
void SoundManager::SetBGMVolume(int volume)
{
	for (auto it = bgm_handle_list_.begin(); it != bgm_handle_list_.end(); it++)
	{
		ChangeVolumeSoundMem(volume, it->second);
	}
}

// 入力：volume=変更先の音量値（0-255）
// 出力：なし
// 副作用：登録されている全SEのシステム音量の書き換え
void SoundManager::SetSEVolume(int volume)
{
	for (auto it = se_handle_list_.begin(); it != se_handle_list_.end(); it++)
	{
		ChangeVolumeSoundMem(volume, it->second);
	}
}

// 入力：volume=設定するマスター音量（0-255）
// 出力：なし
// 副作用：基準音量値の変更、および登録されているすべてのBGM音量のリアルタイム反映更新
void SoundManager::SetMasterBGMVolume(int volume)
{
	// 外部仕様依存：DxLibの ChangeVolumeSoundMem 引数上限（0?255）を越える値を弾くための安全クランプ
	if (volume < 0) volume = 0;
	if (volume > 255) volume = 255;
	master_bgm_volume_ = volume;

	// 設定画面のスライダー操作を、現在流れているBGMと次回シーン遷移後のBGMの両方に即時反映させる
	SetBGMVolume(master_bgm_volume_);
}

// 入力：volume=設定するマスター音量（0-255）
// 出力：なし
// 副作用：基準音量値の変更、および登録されているすべてのSE音量のリアルタイム反映更新
void SoundManager::SetMasterSEVolume(int volume)
{
	// BGMと同様、DxLibの規定音量範囲（0?255）から逸脱して予期せぬ内部バグが起きるのを防ぐ安全クランプ
	if (volume < 0) volume = 0;
	if (volume > 255) volume = 255;
	master_se_volume_ = volume;
	SetSEVolume(master_se_volume_);
}
