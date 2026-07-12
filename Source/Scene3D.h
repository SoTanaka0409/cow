#pragma once
#include"Scene.h"
#include"Dxlib.h"
#include"Master.h"
#include"Thunder.h"
#include"CowManager.h"
#include"AnimalManager.h"

// 3DアクションスチE?Eジ管?E
class Scene3D : public Scene
{
public: 
	// ゲーム進行?Eフェーズ
	enum Phase
	{
		kNormal,         // 通常
		kTornadoCrisis,  // 竜巻巨大匁E
		kMobSpawn,       // 生物大量スポ?Eン
	};	
	
public:
	Scene3D();
	~Scene3D();

	/*
	 * @brief スチE?Eジ初期匁E
	 * [入力] なし[出力] なし[副作用] オブジェクト生成、BGM再生
	 */
	void Initialize() override;

	/*
	 * @brief シーン終?E?E?E
	 * [入力] なし[出力] なし[副作用] BGM停止
	 */
	void Finalize() override;

	/*
	 * @brief スチE?Eジ描画
	 * [入力] なし[出力] なし[副作用] 画面描画
	 */
	void Draw() override;

	/*
	 * @brief スチE?Eジ更新
	 * [入力] なし[出力] なし[副作用] タイムアチE?E時?Eリザルト?E移
	 */
	void Update() override;

	/*
	 * @brief フェーズ進行による環?E??化
	 * [入力] なし[出力] なし[副作用] カメラ・竜巻状態変更、スポ?Eン処?E
	 */
	void PhaseUpdate();

private:
	float warning_radius_;       // 警告エリア現在半征E(予?E
	float max_radius_;           // 警告エリア最大半征E(予?E
	
	int tatumaki_spawn_timer_;   // 竜巻出現タイマ?E
	int mass_spawn_timer_;       // 大量?E現の間隔タイマ?E
	Phase phase_;             // 現在のフェーズ
	int font_back_graph_;        // 警告テキスト背景用ハンドル
};