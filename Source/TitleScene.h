#pragma once
#include "Scene.h"
#include"Texture.h"
#include "SelectionManager.h"
#include <vector>
#include "Score.h"
#include "SceneManager.h"

// UI状態管理と描画用のデータをまとめるための構造体
struct TitleButton
{
	SelectionManager::Title type;
	int graphHandle;
	int x, y;
	int w, h;
	bool isHover;
};

// アプリケーション起動時の初期画面
class TitleScene : public Scene
{
public:
	TitleScene();
	virtual ~TitleScene();

	// [入力] なし [出力] なし [副作用] アセット初期化、スコア読み込み、BGM再生
	virtual void Initialize() override;

	// [入力] なし [出力] なし [副作用] マウス入力やUI状態、フェード更新
	virtual void Update() override;

	// [入力] なし [出力] なし [副作用] 背景やUI、UFOの描画コマンド発行
	virtual void Draw() override;

	// [入力] なし [出力] なし [副作用] リソースの解放、BGM停止
	virtual void Finalize() override;

private:
	int mFrameCount; // 演出アニメーション用のタイマー

	int mnTitleGraphHandle;
	int mnNewGame;
	int mnTutorial;
	int mnOperationProcedures;

	int rankingTitleImage;
	int pointImg;

	SelectionManager* mnTitleSelect;
	Texture* mpTexture;
	Texture* mpTexture2;
	Texture* mpTexture3;

	int mNewGameX;
	int mNewGameY;
	int mNewGameW;
	int mNewGameH;

	bool mbIsHoverNewGame;

	std::vector<TitleButton> mButtons;

	// [入力] なし [出力] なし [副作用] 上位3名のスコアUIを描画
	void DrawRankingUI();

	int rankImage[3];

	int mnUfoGraphHandle;
	int mUfoX, mUfoY;
	int mUfoW, mUfoH;
	bool mbIsDraggingUfo;
	int mOffsetX, mOffsetY;
	int mCowVoiceTimer;
	float mUfoVX, mUfoVY;
	float mUfoAngle;
	bool  mIsAutoPatrol;
	int mAutoPatrolTimer;
};
