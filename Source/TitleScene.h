#pragma once
#include "Scene.h"
#include"Texture.h"
#include "SelectionManager.h"
#include <vector>
#include "Score.h"
#include "SceneManager.h"

// UI迥ｶ諷狗ｮ｡逅・→謠冗判逕ｨ縺ｮ繝・・繧ｿ繧偵∪縺ｨ繧√ｋ縺溘ａ縺ｮ讒矩菴・
struct TitleButton
{
	SelectionManager::Title type;
	int graphHandle;
	int x, y;
	int w, h;
	bool isHover;
};

// 繧｢繝励Μ繧ｱ繝ｼ繧ｷ繝ｧ繝ｳ襍ｷ蜍墓凾縺ｮ蛻晄悄逕ｻ髱｢
class TitleScene : public Scene
{
public:
	TitleScene();
	virtual ~TitleScene();

	// [蜈･蜉嫋 縺ｪ縺・[蜃ｺ蜉嫋 縺ｪ縺・[蜑ｯ菴懃畑] 繧｢繧ｻ繝・ヨ蛻晄悄蛹悶√せ繧ｳ繧｢隱ｭ縺ｿ霎ｼ縺ｿ縲。GM蜀咲函
	virtual void Initialize() override;

	// [蜈･蜉嫋 縺ｪ縺・[蜃ｺ蜉嫋 縺ｪ縺・[蜑ｯ菴懃畑] 繝槭え繧ｹ蜈･蜉帙ｄUI迥ｶ諷九√ヵ繧ｧ繝ｼ繝画峩譁ｰ
	virtual void Update() override;

	// [蜈･蜉嫋 縺ｪ縺・[蜃ｺ蜉嫋 縺ｪ縺・[蜑ｯ菴懃畑] 閭梧勹繧ФI縲ゞFO縺ｮ謠冗判繧ｳ繝槭Φ繝臥匱陦・
	virtual void Draw() override;

	// [蜈･蜉嫋 縺ｪ縺・[蜃ｺ蜉嫋 縺ｪ縺・[蜑ｯ菴懃畑] 繝ｪ繧ｽ繝ｼ繧ｹ縺ｮ隗｣謾ｾ縲。GM蛛懈ｭ｢
	virtual void Finalize() override;

private:
	int mFrameCount; // 貍泌・繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ逕ｨ縺ｮ繧ｿ繧､繝槭・

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

	// [蜈･蜉嫋 縺ｪ縺・[蜃ｺ蜉嫋 縺ｪ縺・[蜑ｯ菴懃畑] 荳贋ｽ・蜷阪・繧ｹ繧ｳ繧｢UI繧呈緒逕ｻ
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
