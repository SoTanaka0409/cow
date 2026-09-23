#include "EffekseerEffect.h"
#include <EffekseerForDXLib.h>
#include "Master.h"

/// @brief 初期化処理を行う
/// @details filename (ファイルパス)
/// @details init_pos (初期座標)
/// @details kEffectSize (基本スケール)
/// @details 内部変数の初期化とエフェクトリソースの読み込み
EffekseerEffect::EffekseerEffect(const char* filename, VECTOR init_pos, float kEffectSize)
	: play_pos_(init_pos)
	, rotation_(VGet(0.0f, 0.0f, 0.0f))
	, effect_resource_handle_(-1)
	, file_path_(filename)
	, effectSize(kEffectSize)
	, PlayCount(0)
	, playingEffectHandle(-1)
	, LoopFlag(true)
	, mvScale(VGet(1.0f, 1.0f, 1.0f))
{
	Load();
}

/// @brief エフェクトリソースの解放
EffekseerEffect::~EffekseerEffect()
{
	// アプリケーション終了時やオブジェクト破棄時に、VRAM/RAMのメモリリークが発生するのを防ぐため明示的に破棄する
	DeleteEffekseerEffect(effect_resource_handle_);
}

/// @brief VRAMへのエフェクトデータ展開およびリソースハンドルの取得
void EffekseerEffect::Load()
{
	// 外部仕様依存: EffekseerForDXLibの仕様上、ベースサイズ(effectSize)は再生時ではなくロード時に確定させる必要がある
	effect_resource_handle_ = LoadEffekseerEffect(file_path_, effectSize);
}

/// @brief 再生中エフェクトのローカルトランスフォーム(座標・回転・スケール)の更新
void EffekseerEffect::Update()
{
	if (playingEffectHandle != -1)
	{
		// キャラクター等親オブジェクトの移動に対して、エフェクトが1フレーム遅れて描画される「位置ズレバグ」を防ぐため毎フレーム同期する
		SetPosPlayingEffekseer3DEffect(playingEffectHandle, play_pos_.x, play_pos_.y, play_pos_.z);
		SetRotationPlayingEffekseer3DEffect(playingEffectHandle, rotation_.x, rotation_.y, rotation_.z);
		SetScalePlayingEffekseer3DEffect(playingEffectHandle, mvScale.x, mvScale.y, mvScale.z);
	}
}

/// @brief 入力: なし
void EffekseerEffect::Draw()
{
	// 外部仕様依存: 本ライブラリにおける3Dエフェクトの描画はシステム側で一括処理されるため、個別オブジェクトからのDraw呼び出しは不要
}

/// @brief エフェクトの再生開始と再生ハンドルの保持
void EffekseerEffect::Play()
{
	playingEffectHandle = PlayEffekseer3DEffect(effect_resource_handle_);

	// 再生直後の1フレーム目にエフェクトが原点(0,0,0)で一瞬だけ描画されて瞬くバグを防ぐため、即座に座標を上書きする
	SetPosPlayingEffekseer3DEffect(playingEffectHandle, play_pos_.x, play_pos_.y, play_pos_.z);
}

/// @brief エフェクトの強制停止処理
void EffekseerEffect::Stop()
{
	// 無効なハンドルや既に自然消滅したエフェクトを停止しようとして、ライブラリ内部でクラッシュするのを防ぐための安全検証
	int NowPlayEffect = IsEffekseer3DEffectPlaying(playingEffectHandle);
	if (NowPlayEffect != -1)
	{
		StopEffekseer3DEffect(playingEffectHandle);
	}
}
