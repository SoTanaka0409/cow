#pragma once
#include"Scene.h"
#include"Dxlib.h"
#include"Master.h"
#include"Thunder.h"
#include"CowManager.h"
#include"AnimalManager.h"

// 3Dアクションステージ管理
class Scene3D : public Scene
{
public: 
	// ゲーム進行のフェーズ
	enum Phase
	{
		Normal,         // 通常
		TornadoCrisis,  // 竜巻巨大化
		MobSpawn,       // 生物大量スポーン
	};	
	
public:
	Scene3D();
	~Scene3D();

	/*
	 * @brief ステージ初期化
	 * [入力] なし [出力] なし [副作用] オブジェクト生成、BGM再生
	 */
	void Initialize() override;

	/*
	 * @brief シーン終了処理
	 * [入力] なし [出力] なし [副作用] BGM停止
	 */
	void Finalize() override;

	/*
	 * @brief ステージ描画
	 * [入力] なし [出力] なし [副作用] 画面描画
	 */
	void Draw() override;

	/*
	 * @brief ステージ更新
	 * [入力] なし [出力] なし [副作用] タイムアップ時のリザルト遷移
	 */
	void Update() override;

	/*
	 * @brief フェーズ進行による環境変化
	 * [入力] なし [出力] なし [副作用] カメラ・竜巻状態変更、スポーン処理
	 */
	void PhaseUpdate();

private:
	float warningRadius;       // 警告エリア現在半径 (予約)
	float maxRadius;           // 警告エリア最大半径 (予約)
	
	int mTatumakiSpawnTimer;   // 竜巻出現タイマー
	int mMassSpawnTimer;       // 大量出現の間隔タイマー
	Phase mpPhase;             // 現在のフェーズ
	int mFontBackGraph;        // 警告テキスト背景用ハンドル
};
