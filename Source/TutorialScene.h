#pragma once
#include "Scene.h"
#include "ModelUtility.h"
#include "CowManager.h"
#include "Thunder.h"
#include "Texture.h"

class Camera;
class Combo;

// 繝励Ξ繧､繝､繝ｼ縺ｫ蝓ｺ譛ｬ謫堺ｽ懊→繧ｲ繝ｼ繝繧ｷ繧ｹ繝・Β繧定ｧ｣隱ｬ縺吶ｋ繧ｷ繝ｼ繝ｳ
class TutorialScene : public Scene
{
public:
	// 騾ｲ陦後ヵ繧ｧ繝ｼ繧ｺ繧堤ｮ｡逅・☆繧九せ繝・・繝亥ｮ夂ｾｩ
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

	// 繝輔ぅ繝ｼ繝舌・繝｢繝ｼ繝我ｽ馴ｨ謎ｸｭ縺ｮ繧ｵ繝悶せ繝・・繝・
	enum State_fever
	{
		FEVER_1,
		FEVER_2,
		FEVER_3,
	};

	TutorialScene();
	virtual ~TutorialScene();

	// [蜈･蜉嫋 縺ｪ縺・[蜃ｺ蜉嫋 縺ｪ縺・[蜑ｯ菴懃畑] 繝・け繧ｹ繝√Ε繧｢繧ｻ繝・ヨ隱ｭ霎ｼ縲√が繝悶ず繧ｧ繧ｯ繝磯・鄂ｮ縲。GM蜀咲函
	void Initialize() override;

	// [蜈･蜉嫋 縺ｪ縺・[蜃ｺ蜉嫋 縺ｪ縺・[蜑ｯ菴懃畑] 繧ｹ繝・・繝医↓蝓ｺ縺･縺乗桃菴懷愛螳壹→繝輔ぉ繝ｼ繝画峩譁ｰ
	void Update() override;

	// [蜈･蜉嫋 縺ｪ縺・[蜃ｺ蜉嫋 縺ｪ縺・[蜑ｯ菴懃畑] 逕ｻ髱｢縺ｫ繝・く繧ｹ繝医→3D遨ｺ髢薙ｒ謠冗判
	void Draw() override;

	// [蜈･蜉嫋 縺ｪ縺・[蜃ｺ蜉嫋 縺ｪ縺・[蜑ｯ菴懃畑] BGM蛛懈ｭ｢縺ｪ縺ｩ邨ゆｺ・・逅・
	void Finalize() override;

	void SetCamera(Camera* camera);
	void OnCowSucked() { mIsCowSucked = true; } // 蝗槫庶謌仙粥繧貞､夜Κ縺九ｉ蜿励￠蜿悶ｋ繧ｳ繝ｼ繝ｫ繝舌ャ繧ｯ

private:
	// [蜈･蜉嫋 縺ｪ縺・[蜃ｺ蜉嫋 縺ｪ縺・[蜑ｯ菴懃畑] 蠅・阜逕ｨ繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ驟咲ｽｮ
	void CreateFences();

	// [蜈･蜉嫋 縺ｪ縺・[蜃ｺ蜉嫋 縺ｪ縺・[蜑ｯ菴懃畑] 鬆伜沺螟門愛螳夂畑縺ｮ騾乗・螢√ｒ逕滓・
	void CreateWalls();

	// 蜷・せ繝・・繝医・譖ｴ譁ｰ繝ｭ繧ｸ繝・け
	void UpdateStateMove();
	void UpdateStateBeam();
	void UpdateStateComboScore();
	void UpdateStatePhase();
	void UpdateStateSkill();
	void UpdateStateFever();
	void UpdateStateEnd();

	// [蜈･蜉嫋 text:陦ｨ遉ｺ譁・ｭ・ yOffset:Y蠎ｧ讓吶が繝輔そ繝・ヨ [蜃ｺ蜉嫋 縺ｪ縺・[蜑ｯ菴懃畑] 譯亥・繝・く繧ｹ繝医・謠冗判
	void DrawTutorialText(const char* text, int yOffset = 0);

	int COW_COUNT;
	int fontHandle; // 譯亥・逕ｨ繝輔か繝ｳ繝・
	int timerCount; // 豎守畑蠕・ｩ溘ち繧､繝槭・

	State mState = STATE_MOVE;
	State_fever mfeverstate;

	Texture* mpTexture;
	Texture* mpTexture2;
	Texture* mpTexture3;
	Texture* mpTexture4;
	Texture* mpTexture5;
	Texture* mpTexture6;

	int mouse_x_;

	Thunder* thunder;
	Camera* mpCamera = nullptr;
	Combo* mpCombo = nullptr;

	bool mIsCowSucked = false;
	bool CowDelete;
	bool mbSkillFlag{};
	bool mbfeverFlag{};
};