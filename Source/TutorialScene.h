#pragma once
#include "Scene.h"
#include "ModelUtility.h"
#include "CowManager.h"
#include "Thunder.h"
#include "Texture.h"

class Camera;
class Combo;

// プレイヤーに基本操作とゲームシスチE??を解説するシーン
class TutorialScene : public Scene
{
public:
	// 進行フェーズを管?E??るスチE?Eト定義
	enum State
	{
		kStateMove,
		kStateBeam,
		kStateComboScore,
		kStatePhase,
		kStateSkill,
		kStateFever,
		kStateEnd
	};

	// フィーバ?Eモード体験中のサブスチE?EチE
	enum State_fever
	{
		kFever1,
		kFever2,
		kFever3,
	};

	TutorialScene();
	virtual ~TutorialScene();

	// [入力] なし[出力] なし[副作用] チE??スチャアセチE??読込、オブジェクト?E置、BGM再生
	void Initialize() override;

	// [入力] なし[出力] なし[副作用] スチE?Eトに基づく操作判定とフェード更新
	void Update() override;

	// [入力] なし[出力] なし[副作用] 画面にチE??ストと3D空間を描画
	void Draw() override;

	// [入力] なし[出力] なし[副作用] BGM停止など終?E?E?E
	void Finalize() override;

	void SetCamera(Camera* camera);
	void OnCowSucked() { is_cow_sucked_ = true; } // 回収成功を外部から受け取るコールバック

private:
	// [入力] なし[出力] なし[副作用] ?E??用オブジェクトを配置
	void CreateFences();

	// [入力] なし[出力] なし[副作用] 領域外判定用の透?E壁を生?E
	void CreateWalls();

	// 吁E??チE?Eト?E更新ロジチE??
	void UpdateStateMove();
	void UpdateStateBeam();
	void UpdateStateComboScore();
	void UpdateStatePhase();
	void UpdateStateSkill();
	void UpdateStateFever();
	void UpdateStateEnd();

	// [入力] text:表示?E??E yOffset:Y座標オフセチE?? [出力] なし[副作用] 案?EチE??スト?E描画
	void DrawTutorialText(const char* text, int yOffset = 0);

	int cow_count_;
	int font_handle_; // 案?E用フォンチE
	int timer_count_; // 汎用征E??タイマ?E

	State state_ = kStateMove;
	State_fever fever_state_;

	Texture* texture_;
	Texture* texture2_;
	Texture* texture3_;
	Texture* texture4_;
	Texture* texture5_;
	Texture* texture6_;

	int mouse_x_;

	Thunder* thunder_;
	Camera* camera_ = nullptr;
	Combo* combo_ = nullptr;

	bool is_cow_sucked_ = false;
	bool cow_delete_;
	bool skill_flag_{};
	bool fever_flag_{};
};