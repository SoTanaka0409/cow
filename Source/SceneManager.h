#pragma once
#include"Scene.h"

// 繧ｷ繝ｼ繝ｳ驕ｷ遘ｻ縺翫ｈ縺ｳ繝ｩ繧､繝輔し繧､繧ｯ繝ｫ繧堤ｮ｡逅・☆繧九け繝ｩ繧ｹ
class SceneManager
{
public:
	enum SCENE_TYPE
	{
		SCENE_NONE = 0,
		SCENE_TITLE,
		SCENE_RULE,
		SCENE_TUTORIAL,
		SCENE_LEVEL,
		SCENE_GAME,
		SCENE_3DHARD,
		SCENE_RESULT,
		SCENE_RESULTWIN,
		SCENE_NORMALRESULTSCENE,
		SCENE_LOADING,
		SCENE_3D
	};

public:
	SceneManager();
	~SceneManager();

	/*
	 * 蛻晄悄險ｭ螳壹ｒ陦後＞縲∵怙蛻昴・繧ｷ繝ｼ繝ｳ・医ち繧､繝医Ν・峨∈驕ｷ遘ｻ縺吶ｋ
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] mnNextSceneType縺ｮ譖ｴ譁ｰ縺翫ｈ縺ｳ繧ｷ繝ｼ繝ｳ蛻・ｊ譖ｿ縺・
	 */
	void Initialize();

	/*
	 * 迴ｾ蝨ｨ繧｢繧ｯ繝・ぅ繝悶↑繧ｷ繝ｼ繝ｳ縺ｮ謠冗判蜃ｦ逅・ｒ蜻ｼ縺ｳ蜃ｺ縺・
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 逕ｻ髱｢縺ｸ縺ｮ謠冗判蜃ｦ逅・
	 */
	void Draw();

	/*
	 * 迴ｾ蝨ｨ繧｢繧ｯ繝・ぅ繝悶↑繧ｷ繝ｼ繝ｳ縺ｮ譖ｴ譁ｰ蜃ｦ逅・ｒ蜻ｼ縺ｳ蜃ｺ縺・
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 迴ｾ蝨ｨ縺ｮ繧ｷ繝ｼ繝ｳ迥ｶ諷九・譖ｴ譁ｰ
	 */
	void Update();

	/*
	 * 繧｢繧ｯ繝・ぅ繝悶↑繧ｷ繝ｼ繝ｳ繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ螳牙・縺ｫ隗｣謾ｾ繝ｻ邨ゆｺ・☆繧・
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] mpCurrentScene縺ｮ繝｡繝｢繝ｪ隗｣謾ｾ
	 */
	void Finalize();

	/*
	 * 驕ｷ遘ｻ隕∵ｱゅ′縺ゅｋ蝣ｴ蜷医∝商縺・す繝ｼ繝ｳ繧堤ｴ譽・＠譁ｰ縺励＞繧ｷ繝ｼ繝ｳ繧堤函謌舌・蛻晄悄蛹悶☆繧・
	 * [蜈･蜉嫋 縺ｪ縺・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 繧ｷ繝ｼ繝ｳ繧ｪ繝悶ず繧ｧ繧ｯ繝医・繝｡繝｢繝ｪ隗｣謾ｾ縺ｨ譁ｰ隕冗函謌・
	 */
	void ChangeSceneIfNeeded();

	void SetNextScene(SCENE_TYPE next) { mnNextSceneType = next; }
	void SetSceneHard(bool Hard) { SceneHard = Hard; }
	bool GetSceneHard() { return SceneHard; }
	void SetSceneNormal(bool Normal) { SceneNormal = Normal; }
	bool GetSceneNormal() { return SceneNormal; }
	Scene* GetCurrentScene() { return mpCurrentScene; }
	SCENE_TYPE GetSceneType() const { return mnSceneType; }

private:
	SCENE_TYPE mnSceneType;
	SCENE_TYPE mnNextSceneType;
	Scene* mpCurrentScene;

	bool SceneHard;
	bool SceneNormal;
};
