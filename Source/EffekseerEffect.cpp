#include "EffekseerEffect.h"
#include <EffekseerForDXLib.h>
#include "Master.h"

/*
 * 入力: filename (ファイルパス), initPos (初期座標), kEffectSize (基本スケール)
 * 出力: なし
 * 副作用: 内部変数の初期化とエフェクトリソースの読み込み
 */
EffekseerEffect::EffekseerEffect(const char* filename, VECTOR initPos, float kEffectSize)
	: play_pos_(initPos)
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

/*
 * 入力: なし
 * 出力: なし
 * 副作用: エフェクトリソースの解放
 */
EffekseerEffect::~EffekseerEffect()
{
	// アプリケーション終了時やオブジェクト破棄時に、VRAM/RAMのメモリリークが発生するのを防ぐため明示的に破棄する
	DeleteEffekseerEffect(effect_resource_handle_);
}

/*
 * 入力: なし
 * 出力: なし
 * 副作用: VRAMへのエフェクトデータ展開およびリソースハンドルの取得
 */
void EffekseerEffect::Load()
{
	// 外部仕様依存: EffekseerForDXLibの仕様上、ベースサイズ(effectSize)は再生時ではなくロード時に確定させる必要がある
	effect_resource_handle_ = LoadEffekseerEffect(file_path_, effectSize);
}

/*
 * 入力: なし
 * 出力: なし
 * 副作用: 再生中エフェクトのローカルトランスフォーム(座標・回転・スケール)の更新
 */
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

/*
 * 入力: なし
 * 出力: なし
 * 副作用: なし
 */
void EffekseerEffect::Draw()
{
	// 外部仕様依存: 本ライブラリにおける3Dエフェクトの描画はシステム側で一括処理されるため、個別オブジェクトからのDraw呼び出しは不要
}

/*
 * 入力: なし
 * 出力: なし
 * 副作用: エフェクトの再生開始と再生ハンドルの保持
 */
void EffekseerEffect::Play()
{
	playingEffectHandle = PlayEffekseer3DEffect(effect_resource_handle_);

	// 再生直後の1フレーム目にエフェクトが原点(0,0,0)で一瞬だけ描画されて瞬くバグを防ぐため、即座に座標を上書きする
	SetPosPlayingEffekseer3DEffect(playingEffectHandle, play_pos_.x, play_pos_.y, play_pos_.z);
}

/*
 * 入力: なし
 * 出力: なし
 * 副作用: エフェクトの強制停止処理
 */
void EffekseerEffect::Stop()
{
	// 無効なハンドルや既に自然消滅したエフェクトを停止しようとして、ライブラリ内部でクラッシュするのを防ぐための安全検証
	int NowPlayEffect = IsEffekseer3DEffectPlaying(playingEffectHandle);
	if (NowPlayEffect != -1)
	{
		StopEffekseer3DEffect(playingEffectHandle);
	}
}
