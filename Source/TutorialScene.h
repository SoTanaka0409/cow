#pragma once
#include "Scene.h"
#include "ModelUtility.h"
#include "CowManager.h"
#include "Thunder.h"
#include "Texture.h"

class Camera;
class Combo;

// プレイヤーに基本操作とゲームシステムを解説するシーン
class TutorialScene : public Scene
{
public:
	// 進行フェーズを管理するステート定義
	enum State
	{
		STATE_MOVE,
		STATE_BEAM,
		STATE_COMBO_SCORE,
		STATE_PHASE,
		STATE_SKILL,
		STATE_FEVER,
		STATE_END
	};

	// フィーバーモード体験中のサブステート
	enum State_fever
	{
		FEVER_1,
		FEVER_2,
		FEVER_3,
	};

	TutorialScene();
	virtual ~TutorialScene();

	// [入力] なし [出力] なし [副作用] テクスチャアセット読込、オブジェクト配置、BGM再生
	void Initialize() override;

	// [入力] なし [出力] なし [副作用] ステートに基づく操作判定とフェード更新
	void Update() override;

	// [入力] なし [出力] なし [副作用] 画面にテキストと3D空間を描画
	void Draw() override;

	// [入力] なし [出力] なし [副作用] BGM停止など終了処理
	void Finalize() override;

	void SetCamera(Camera* camera);
	void OnCowSucked() { mIsCowSucked = true; } // 回収成功を外部から受け取るコールバック

private:
	// [入力] なし [出力] なし [副作用] 境界用オブジェクトを配置
	void CreateFences();

	// [入力] なし [出力] なし [副作用] 領域外判定用の透明壁を生成
	void CreateWalls();

	// 各ステートの更新ロジック
	void UpdateStateMove();
	void UpdateStateBeam();
	void UpdateStateComboScore();
	void UpdateStatePhase();
	void UpdateStateSkill();
	void UpdateStateFever();
	void UpdateStateEnd();

	// [入力] text:表示文字, yOffset:Y座標オフセット [出力] なし [副作用] 案内テキストの描画
	void DrawTutorialText(const char* text, int yOffset = 0);

	int COW_COUNT;
	int fontHandle; // 案内用フォント
	int timerCount; // 汎用待機タイマー

	State mState = STATE_MOVE;
	State_fever mfeverstate;

	Texture* mpTexture;
	Texture* mpTexture2;
	Texture* mpTexture3;
	Texture* mpTexture4;
	Texture* mpTexture5;
	Texture* mpTexture6;

	int mouseX;

	Thunder* thunder;
	Camera* mpCamera = nullptr;
	Combo* mpCombo = nullptr;

	bool mIsCowSucked = false;
	bool CowDelete;
	bool mbSkillFlag{};
	bool mbfeverFlag{};
};
