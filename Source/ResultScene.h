#pragma once
#include"dxlib.h"
#include"Scene.h"
#include"Texture.h"
#include"SelectionManager.h"
#include"vector"
#include "Score.h"

// 繝ｪ繧ｶ繝ｫ繝育判髱｢縺ｫ縺翫￠繧九・繧ｿ繝ｳ縺ｮUI迥ｶ諷九→謠冗判繝・・繧ｿ繧剃ｿ晄戟縺吶ｋ讒矩菴・
struct ResultButton
{
	SelectionManager::Title type;
	int graphHandle;
	int x, y;
	int w, h;
	bool isHover;
};

// 繧ｲ繝ｼ繝邨ゆｺ・凾縺ｫ譛邨ゅせ繧ｳ繧｢縺ｨ繝ｩ繝ｳ繧ｭ繝ｳ繧ｰ繧定｡ｨ遉ｺ縺吶ｋ繧ｷ繝ｼ繝ｳ
class ResultScene : public Scene
{
public:
	ResultScene();
	virtual ~ResultScene();

	// [蜈･蜉嫋 縺ｪ縺・[蜃ｺ蜉嫋 縺ｪ縺・[蜑ｯ菴懃畑] 譛譁ｰ繝ｩ繝ｳ繧ｭ繝ｳ繧ｰ縺ｮ繝ｭ繝ｼ繝峨→繝輔ぉ繝ｼ繝牙・譛溷喧
	void Initialize() override;

	// [蜈･蜉嫋 縺ｪ縺・[蜃ｺ蜉嫋 縺ｪ縺・[蜑ｯ菴懃畑] 閭梧勹縲√せ繧ｳ繧｢縲√Λ繝ｳ繧ｭ繝ｳ繧ｰUI縺ｮ謠冗判繧ｳ繝槭Φ繝牙ｮ溯｡・
	void Draw() override;

	// [蜈･蜉嫋 縺ｪ縺・[蜃ｺ蜉嫋 縺ｪ縺・[蜑ｯ菴懃畑] 荳螳壽凾髢鍋ｵ碁℃蠕後↓繧ｿ繧､繝医Ν逕ｻ髱｢縺ｸ縺ｮ驕ｷ遘ｻ繧定ｦ∵ｱ・
	void Update() override;

	// [蜈･蜉嫋 縺ｪ縺・[蜃ｺ蜉嫋 縺ｪ縺・[蜑ｯ菴懃畑] 菴ｿ逕ｨ縺励◆逕ｻ蜒上ワ繝ｳ繝峨Ν縺ｮ遐ｴ譽・→BGM蛛懈ｭ｢
	void Finalize() override;

private:
	SelectionManager* mnTitleSelect;
	Texture* mpTexture;
	Texture* mpTexture2;
	Texture* mpTexture3;

	int mNewGameX;
	int mNewGameY;
	int mNewGameW;
	int mNewGameH;

	bool mbIsHoverNewGame;

	std::vector<ResultButton> mButtons;
	int ResultGraphHandle;

	Score mScore;

	int rankingTitleImage;
	int rankImage[3];

	int yourScoreImage;
	int pointImage;

	// [蜈･蜉嫋 縺ｪ縺・[蜃ｺ蜉嫋 縺ｪ縺・[蜑ｯ菴懃畑] 荳贋ｽ・蜷阪・繧ｹ繧ｳ繧｢縺ｨ繝｡繝繝ｫ繧呈緒逕ｻ
	void DrawRankingUI();

	int yourScoreTextImg;
	int pointImg;

	int mFrameCount; // UI繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ蛻ｶ蠕｡逕ｨ繧ｿ繧､繝槭・
	int mnCount;     // 閾ｪ蜍暮・遘ｻ逕ｨ縺ｮ邨碁℃譎る俣繧ｿ繧､繝槭・
	bool mFlag;
};

