#pragma once
#include "Scene.h"
#include "ModelUtility.h"
#include "CowManager.h"
#include "Thunder.h"
#include "Texture.h"

class Camera;
class Combo;

// プレイヤーに移動やビーム吸引、コンボ、スキルなどの基本操作を解説するチュートリアルシーンクラス
class TutorialScene : public Scene
{
public:
	// チュートリアルの各進行ステップを表す定義
	enum State
	{
		STATE_MOVE,         // 1. 移動操作の解説ステップ
		STATE_BEAM,         // 2. 吸引ビームとゲージの解説ステップ
		STATE_COMBO_SCORE,  // 3. スコア・コンボ倍率の解説ステップ
		STATE_PHASE,        // 4. ゲーム内フェーズの解説ステップ
		STATE_SKILL,        // 5. ランダムスキルの解説ステップ
		STATE_FEVER,        // 6. フィーバーモードの体験ステップ
		STATE_END           // 終了処理とタイトルへ戻る待機ステップ
	};

	// フィーバー体験中のステップ状態定義
	enum State_fever
	{
		FEVER_1,            // ゴールド牛の出現・未吸引
		FEVER_2,            // フィーバー発動中
		FEVER_3,            // （リザーブ）
	};

	TutorialScene();
	virtual ~TutorialScene();

	/*
	 * @brief チュートリアル用のフォント読み込みや各ステップ説明用テクスチャの生成、プレイヤー配置を行う
	 * [入力] なし
	 * [出力] なし
	 * [副作用] テクスチャアセットのnew、プレイヤー・マップ障害物の生成、BGM再生
	 */
	void Initialize() override;

	/*
	 * @brief 現在のステップ(State)に応じた操作チェック、およびカメラ・フェード更新を行う
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 各ステップ更新関数の実行、フェードアルファ値更新
	 */
	void Update() override;

	/*
	 * @brief シーン描画および現在アクティブな操作説明テキスト等の描画処理
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 画面描画コマンドの実行
	 */
	void Draw() override;

	/*
	 * @brief チュートリアル終了時の後処理（BGM停止など）
	 * [入力] なし
	 * [出力] なし
	 * [副作用] BGM停止
	 */
	void Finalize() override;

	void SetCamera(Camera* camera);
	void OnCowSucked() { mIsCowSucked = true; } // 牛の回収成功を知らせるコールバック関数

private:
	/*
	 * @brief マップ境界沿いに並べる樹木の生成と配置
	 * [入力] なし
	 * [出力] なし
	 * [副作用] Object_Stage オブジェクト群の生成
	 */
	void CreateFences();

	/*
	 * @brief プレイ境界壁のインビジブルコライダーを生成し配置する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] Wall オブジェクトの生成
	 */
	void CreateWalls();

	// 各チュートリアル進行ステップに対応した状態更新ロジック群
	void UpdateStateMove();
	void UpdateStateBeam();
	void UpdateStateComboScore();
	void UpdateStatePhase();
	void UpdateStateSkill();
	void UpdateStateFever();
	void UpdateStateEnd();

	/*
	 * @brief 画面上部に操作指示などの案内テキストを描画する
	 * [入力] text: 表示する日本語文字列, yOffset: Y座標の調整値
	 * [出力] なし
	 * [副作用] 文字フォントの描画
	 */
	void DrawTutorialText(const char* text, int yOffset = 0);

	int COW_COUNT;                  // 牛の目標回収数（リザーブ）
	int fontHandle;                 // 操作指示表示用の日本語フォントハンドル
	int timerCount;                 // 一定時間経過待機などで汎用的に使用するタイマー

	State mState = STATE_MOVE;      // 現在進行中のチュートリアルステップ
	State_fever mfeverstate;        // フィーバー状態のサブステップ

	// 各操作説明に対応した画像（テクスチャ）ポインタ
	Texture* mpTexture;             // 移動説明用画像
	Texture* mpTexture2;            // ビーム説明用画像
	Texture* mpTexture3;            // コンボ説明用画像
	Texture* mpTexture4;            // フェーズ説明用画像
	Texture* mpTexture5;            // スキル説明用画像
	Texture* mpTexture6;            // フィーバー説明用画像

	int mouseX;                     // マウス座標（リザーブ）

	Thunder* thunder;               // 落雷用（リザーブ）
	Camera* mpCamera = nullptr;     // 3Dカメラオブジェクトへのポインタ
	Combo* mpCombo = nullptr;       // コンボ表示用（リザーブ）

	bool mIsCowSucked = false;      // 牛を1匹でも吸引したかどうかの判定フラグ
	bool CowDelete;
	bool mbSkillFlag{};             // スキル発動ボタンが押されたかを判定するフラグ
	bool mbfeverFlag{};
};

