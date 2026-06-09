#include"SoundManager.h"
#include"DxLib.h"

SoundManager::SoundManager()
	: mnNowPlayingBgm((SOUND_BGM)-1)
	, mnNowPlayingSe((SOUND_SE)-1)
{
}

SoundManager::~SoundManager()
{
	Finalize(); // オブジェクト破棄時のリソース解放漏れを防止
}

/*
 * @brief すべてのBGMおよび効果音アセットをプリロードする
 * [入力] なし
 * [出力] なし
 * [副作用] BGM・SEファイルをディスクからメモリにロードしリストへ登録
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
}

/*
 * @brief 読み込んだすべてのサウンドハンドルを解放する
 * [入力] なし
 * [出力] なし
 * [副作用] ロードされた全ハンドルの削除とリストのクリア
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
 * @brief 指定されたBGMのループ再生を開始する
 * [入力] bgm: 曲ID, isTop: 先頭から再生するか
 * [出力] なし
 * [副作用] BGMの再生、音量の適用、現在再生中曲IDの更新
 */
void SoundManager::PlayBGM(SOUND_BGM bgm, bool isTop)
{
	// 同一曲が既に再生中で、かつ最初から再生し直す指示がない場合は処理をスキップ
	if (mnNowPlayingBgm == bgm && !isTop)
	{
		return;
	}

	for (auto it = mnBgmHandleList.begin(); it != mnBgmHandleList.end(); it++)
	{
		if (it->first == bgm)
		{
			ChangeVolumeSoundMem(mMasterBGMVolume, it->second); // 最新のマスターボリュームを反映
			PlaySoundMem(it->second, DX_PLAYTYPE_LOOP, isTop);
			mnNowPlayingBgm = bgm;
			break;
		}
	}
}

/*
 * @brief 効果音（SE）をバックグラウンドで単発再生する
 * [入力] se: 効果音ID
 * [出力] なし
 * [副作用] SEの再生、現在再生中SEのID更新
 */
void SoundManager::PlaySE(SOUND_SE se)
{
	for (auto it = mnSeHandleList.begin(); it != mnSeHandleList.end(); it++)
	{
		if (it->first == se)
		{
			PlaySoundMem(it->second, DX_PLAYTYPE_BACK);
			mnNowPlayingSe = se;
			break;
		}
	}
}

/*
 * @brief ディスクからBGMアセットを読み込みリストへ登録する
 * [入力] bgm: BGMのID, filename: ファイルパス
 * [出力] なし
 * [副作用] ロード成功時にBGMリストへ登録、初期音量を設定
 */
void SoundManager::LoadBGM(SOUND_BGM bgm, std::string filename)
{
	// 二重読み込み防止のための重複チェック
	for (auto it = mnBgmHandleList.begin(); it != mnBgmHandleList.end(); it++)
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
	ChangeVolumeSoundMem(mMasterBGMVolume, handle);
	mnBgmHandleList.push_back(std::pair<SOUND_BGM, int>(bgm, handle));
}

/*
 * @brief ディスクから効果音アセットを読み込みリストへ登録する
 * [入力] se: 効果音ID, filename: ファイルパス
 * [出力] なし
 * [副作用] ロード成功時にSEリストへ登録、初期音量を設定
 */
void SoundManager::LoadSE(SOUND_SE se, std::string filename)
{
	for (auto it = mnSeHandleList.begin(); it != mnSeHandleList.end(); it++)
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
	ChangeVolumeSoundMem(mMasterSEVolume, handle);
	mnSeHandleList.push_back(std::pair<SOUND_SE, int>(se, handle));
}

/*
 * @brief 現在再生されているBGMを停止する
 * [入力] なし
 * [出力] なし
 * [副作用] BGMの停止処理
 */
void SoundManager::StopBGM()
{
	for (auto it = mnBgmHandleList.begin(); it != mnBgmHandleList.end(); it++)
	{
		if (it->first == mnNowPlayingBgm)
		{
			if (CheckSoundMem(it->second))
			{
				StopSoundMem(it->second);
				break;
			}
		}
	}
}

/*
 * @brief すべての登録済みBGMの音量を変更する
 * [入力] volume: 新しい音量設定値 (0 - 255)
 * [出力] なし
 * [副作用] 登録全BGMのボリューム変更
 */
void SoundManager::SetBGMVolume(int volume)
{
	for (auto it = mnBgmHandleList.begin(); it != mnBgmHandleList.end(); it++)
	{
		ChangeVolumeSoundMem(volume, it->second);
	}
}

/*
 * @brief すべての登録済みSEの音量を変更する
 * [入力] volume: 新しい音量設定値 (0 - 255)
 * [出力] なし
 * [副作用] 登録全SEのボリューム変更
 */
void SoundManager::SetSEVolume(int volume)
{
	for (auto it = mnSeHandleList.begin(); it != mnSeHandleList.end(); it++)
	{
		ChangeVolumeSoundMem(volume, it->second);
	}
}

/*
 * @brief 全BGMのマスターボリュームを設定する
 * [入力] volume: 設定する基準音量 (0 - 255)
 * [出力] なし
 * [副作用] mMasterBGMVolumeの更新および全BGMの音量同期
 */
void SoundManager::SetMasterBGMVolume(int volume)
{
	if (volume < 0) volume = 0;
	if (volume > 255) volume = 255;
	mMasterBGMVolume = volume;
	SetBGMVolume(mMasterBGMVolume);
}

/*
 * @brief 全SEのマスターボリュームを設定する
 * [入力] volume: 設定する基準音量 (0 - 255)
 * [出力] なし
 * [副作用] mMasterSEVolumeの更新および全SEの音量同期
 */
void SoundManager::SetMasterSEVolume(int volume)
{
	if (volume < 0) volume = 0;
	if (volume > 255) volume = 255;
	mMasterSEVolume = volume;
	SetSEVolume(mMasterSEVolume);
}
