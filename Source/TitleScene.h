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
	int frameCount; // 演出アニメーション用のタイマー

	int titleGraphHandle;
	int newGame;
	int tutorial;
	int operationProcedures;

	int rankingTitleImage;
	int pointImg;

	SelectionManager* titleSelect;
	Texture* texture;
	Texture* texture2;
	Texture* texture3;

	int newGameX;
	int newGameY;
	int newGameW;
	int newGameH;

	bool isHoverNewGame;

	std::vector<TitleButton> buttons;

	// [入力] なし [出力] なし [副作用] 上位3名のスコアUIを描画
	void DrawRankingUI();

	int rankImage[3];

	int ufoGraphHandle;
	int ufoX, ufoY;
	int ufoW, ufoH;
	bool isDraggingUfo;
	int offsetX, offsetY;
	int cowVoiceTimer;
	float ufoVX, ufoVY;
	float ufoAngle;
	bool  isAutoPatrol;
	int autoPatrolTimer;
};
