#pragma once
#include"Scene.h"
#include"Dxlib.h"
#include"Master.h"
#include"Thunder.h"
#include"CowManager.h"
#include"AnimalManager.h"

// ゲームメインの3Dアクションステージ画面を制御するクラス
class Scene3D : public Scene
{
public: 
	// ゲーム本編の各フェーズ（演出段階）の定義
	enum Phase
	{
		Normal,         // 通常状態
		TornadoCrisis,  // 竜巻が巨大化して暴れまわる段階
		MobSpawn,       // 牛などの生物が大量に出現する段階
	};	
	
public:
	Scene3D();
	~Scene3D();

	/*
	 * @brief 3Dステージシーンの初期設定（マップ、コライダー、プレイヤー、エネミー生成）を行う
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 各オブジェクトのnew、BGM再生開始、コライダー登録
	 */
	void Initialize() override;

	/*
	 * @brief シーン終了時のクリーンアップ処理（BGM停止など）
	 * [入力] なし
	 * [出力] なし
	 * [副作用] BGMの停止
	 */
	void Finalize() override;

	/*
	 * @brief バックバッファへのグリッドや各マネージャー、ゲームUIの描画処理
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 画面描画コマンドの実行
	 */
	void Draw() override;

	/*
	 * @brief ステージ全体のオブジェクト更新、制限時間判定、フェード更新を制御
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 各更新関数の呼び出し、制限時間終了時のリザルト遷移要求
	 */
	void Update() override;

	/*
	 * @brief ゲームフェーズに応じたカメラワークや竜巻の拡大、牛の降下処理などを実行
	 * [入力] なし
	 * [出力] なし
	 * [副作用] カメラ更新、Tatumaki状態変更、牛の新規スポーン
	 */
	void PhaseUpdate();

private:
	float warningRadius;       // 危険警告エリアの現在半径（未使用・リザーブ）
	float maxRadius;           // 警告エリアの最大判定半径（未使用・リザーブ）
	
	int mTatumakiSpawnTimer;   // 竜巻の出現制御用タイマー
	int mMassSpawnTimer;       // 大量出現時の定期スポーン間隔タイマー
	Phase mpPhase;             // 現在の演出フェーズ状態
	int mFontBackGraph;        // フェーズ警告テキストの背景に使用する透過画像ハンドル
};

