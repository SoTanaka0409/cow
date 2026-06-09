#pragma once
#include "dxlib.h"
#include "Object3D.h"
#include "Model.h"

// ゲーム本編のボスキャラクター（巨大円盤など）を制御するクラス
class Boss : public Object3D
{
public:
	// ボス戦のフェーズ（演出）段階の定義
	enum StepTag
	{
		Step_Chat,      // 会話・登場演出段階
		Step_Damage,    // プレイヤーからの吸引ダメージを受ける段階
		Step_Finish     // 撃破演出・ゲームクリア段階
	};

public:
	/*
	 * @brief ボスアセットを読み込み、初期座標、ステータスを初期化する
	 * [入力] filename: モデルのファイルパス, initPos: 初期座標
	 * [出力] なし
	 * [副作用] モデルのロード、Hp/MaxHpの初期化、登場フェーズ設定
	 */
	Boss(std::string filename, VECTOR initPos);
	virtual ~Boss();

	void Draw() override;
	void Update() override;

	/*
	 * @brief ボスの頭上に現在の残りHPゲージを表示する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 画面上にボスのHPバーを描画
	 */
	void Bar();

	/*
	 * @brief ボス撃破時の消滅演出と、ゲーム本編のクリアシーケンスの起動を処理する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] ゲームマネージャーに最終クリアフェーズ(game_final)を要求する
	 */
	void Death();

private:
	Model* mpModel;          // ボスモデル制御用クラスのポインタ
	float mfHp;             // 現在のHP
	float mfMaxHp;          // 最大HP（初期HP）
	StepTag tag;            // 現在のフェーズ
	int count;              // 演出用フレームタイマーカウンター
	int time;               // 演出の切り替えインターバル（フレーム数）
	float MaxDamage;        // プレイヤーのハイスコアから算出する総被ダメージ上限
};
