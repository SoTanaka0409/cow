#pragma once
#include <EffekseerForDXLib.h>
#include "DxLib.h"

/// @brief Effekseerリソースのライフサイクル管理と、3D空間上での座標同期（追従）をカプセル化するクラス
/// @details 破棄漏れによるVRAM枯渇を防ぐため、オブジェクトとエフェクトの生存期間を確実に同期させること
class EffekseerEffect
{
public:
	/// @brief 動的生成時の同期ロードはフレーム落ち（スパイク）の原因となるため、頻出エフェクトは事前ロードを検討すること
	/// @param filename ファイルパス
	/// @param initPos 初期座標
	/// @param kEffectSize 描画スケール [出力] なし [副作用] リソースの同期読み込み
	EffekseerEffect(const char* filename, VECTOR initPos, float kEffectSize);

	/// @brief VRAMリークを防ぐため、オブジェクト破棄時に確実にDxLib側のDeleteEffekseerEffectを呼ぶこと
	/// @details なし [出力] なし [副作用] エフェクトリソースの解放
	~EffekseerEffect();

	/// @brief コンストラクタ以外で遅延ロードを行うための処理。多重ロードによるメモリリークバグに注意すること
	/// @details なし [出力] なし [副作用] effect_resource_handle_の生成
	void Load();

	/// @brief 追従対象の移動から1フレーム遅れてエフェクトがズレるのを防ぐため、必ず対象の座標更新後に呼ぶこと
	/// @details なし [出力] なし [副作用] 再生中インスタンスのワールド行列更新
	void Update();

	/// @brief Effekseerの描画はマネージャー側で一括処理（UpdateEffekseer3D等）されるアーキテクチャのため、本関数は意図的に空実装とする
	/// @details なし [出力] なし [副作用] なし
	void Draw();

	/// @brief 毎フレーム呼び出しによるエフェクトの意図せぬ多重再生（白飛びや極端な処理落ち）を防ぐよう呼び出し側でガードすること
	/// @details なし [出力] なし [副作用] 再生ハンドルの取得と再生開始
	void Play();

	/// @brief シーン遷移時や対象キャラクター死亡時に、画面にエフェクトが不自然に残り続けるバグを防ぐため確実に呼ぶこと
	/// @details なし [出力] なし [副作用] 再生中インスタンスの破棄
	void Stop();

	void SetPosition(VECTOR pos) { play_pos_ = pos; }
	VECTOR GetPosition() { return play_pos_; }

	void SetRotation(VECTOR rot) { rotation_ = rot; }
	VECTOR GetRotation() { return rotation_; }

	void SetScale(VECTOR scale) { mvScale = scale; }
	VECTOR GetScale() { return mvScale; }

	void SetLoop(bool loop_value_) { LoopFlag = loop_value_; }

	/// @brief 戻り値-1(無効値)の判定。ワンショットエフェクト再生終了後の自動破棄処理のトリガーなどに使用する
	/// @details なし [出力] 再生中ならtrue [副作用] なし
	bool IsPlaying()
	{
		return IsEffekseer3DEffectPlaying(playingEffectHandle) != -1;
	}

private:
	/// @brief 再生中インスタンスの行列計算用パラメータ。直接操作は破綻を招くため必ずSet/Get関数を経由すること
	VECTOR play_pos_; ///< 座標や位置情報を管理する値
	VECTOR rotation_; ///< 移動や回転の計算に使用する値
	VECTOR mvScale;   ///< サイズや範囲の計算に使用する値

	const char* file_path_; ///< リソースのファイルパスを保持する値

	/// @brief 複数回のPlay呼び出しで使い回すためのマスターデータハンドル
	int effect_resource_handle_; ///< 描画やリソース管理に使用するハンドル

	/// @brief 座標更新やStop処理に必須となる、現在再生中の個別インスタンスハンドル
	int playingEffectHandle; ///< 描画やリソース管理に使用するハンドル

	/// @brief 生成時の基本スケール値。エフェクト側の元サイズが極端に大きい/小さい場合の補正に使用する
	float effectSize; ///< サイズや範囲の計算に使用する値

	/// @brief 旧再生周期管理の残骸（現状未使用）。別タスクにてクリーンアップ予定のため利用禁止
	const int EffectPlayInterval = 300; ///< 時間経過や処理間隔を管理するカウンター
	int PlayCount;                      ///< 時間経過や処理間隔を管理するカウンター

	/// @brief エフェクト作成ツール側のループ設定を、プログラム側から強制的に上書き制御するためのフラグ
	bool LoopFlag; ///< 状態の有効・無効を管理するフラグ
};
