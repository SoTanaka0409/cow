#pragma once
#include"Scene.h"
#include"Dxlib.h"
#include"Master.h"
#include"Thunder.h"
#include"CowManager.h"
#include"AnimalManager.h"

class Scene3D : public Scene
{
public: 
	enum Phase
	{
		kNormal,
		kTornadoCrisis,
		kMobSpawn,
	};	
	
public:
	Scene3D();
	~Scene3D();

	/*
	 * リソース確保と初期状態へのリセットを行うため
	 * [入力] なし
	 * [出力] なし
	 * [副作用] オブジェクト生成、BGM再生
	 */
	void Initialize() override;

	/*
	 * リソースリークを防ぎ、次のシーンへ安全に遷移するため
	 * [入力] なし
	 * [出力] なし
	 * [副作用] BGM停止、リソース解放
	 */
	void Finalize() override;

	/*
	 * 3D空間とUIを合成してプレイヤーに状況を視覚伝達するため
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 画面描画
	 */
	void Draw() override;

	/*
	 * 毎フレームのゲームロジックと状態遷移を処理するため
	 * [入力] なし
	 * [出力] なし
	 * [副作用] タイムアップ時にリザルトへ遷移
	 */
	void Update() override;

	/*
	 * 進行度に応じて難易度を段階的に引き上げるため
	 * [入力] なし
	 * [出力] なし
	 * [副作用] カメラ・竜巻状態変更、スポーン処理
	 */
	void PhaseUpdate();

private:
	void DrawGrid();
	void DrawPhaseUI();


	float warning_radius_;
	float max_radius_;
	
	int tatumaki_spawn_timer_;
	int mass_spawn_timer_;
	Phase phase_;
	int font_back_graph_;
};