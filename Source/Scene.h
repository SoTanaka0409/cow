#pragma once
#include"DxLib.h"
#include"GameManager.h"
#include"CowManager.h"
#include"Fever.h"
#include"AnimalManager.h"

class ObjectManager;
class ColliderManager;

// 蜷・ご繝ｼ繝繧ｷ繝ｼ繝ｳ縺ｮ謚ｽ雎｡蝓ｺ蠎輔け繝ｩ繧ｹ
class Scene
{
public:
	// 繝輔ぉ繝ｼ繝臥憾諷・
	enum SceneFade
	{
		SceneFade_In,   // 證苓ｻ｢縺九ｉ譏手ｻ｢
		SceneFade_Out,  // 譏手ｻ｢縺九ｉ證苓ｻ｢
		SceneFade_None, // 繝輔ぉ繝ｼ繝峨↑縺・
		SceneFade_Load, // 繝ｭ繝ｼ繝我ｸｭ
	};
	
public:
	Scene();
	virtual ~Scene();

	/*
	 * @brief 繧ｷ繝ｼ繝ｳ蛻晄悄蛹・
	 * [蜈･蜉嫋 縺ｪ縺・[蜃ｺ蜉嫋 縺ｪ縺・[蜑ｯ菴懃畑] 繝ｪ繧ｽ繝ｼ繧ｹ蛻晄悄蛹・
	 */
	virtual void Initialize() = 0;

	/*
	 * @brief 繝輔ぉ繝ｼ繝画緒逕ｻ
	 * [蜈･蜉嫋 fade: 繝輔ぉ繝ｼ繝臥憾諷・[蜃ｺ蜉嫋 縺ｪ縺・[蜑ｯ菴懃畑] 逕ｻ髱｢謠冗判縲√い繝ｫ繝輔ぃ蛟､譖ｴ譁ｰ
	 */
	virtual void Fade(SceneFade fade);

	/*
	 * @brief 繧ｷ繝ｼ繝ｳ蜀・ｦ∫ｴ縺ｮ譖ｴ譁ｰ
	 * [蜈･蜉嫋 縺ｪ縺・[蜃ｺ蜉嫋 縺ｪ縺・[蜑ｯ菴懃畑] 蜷・・繝阪・繧ｸ繝｣繝ｼ縺ｮUpdate蜻ｼ縺ｳ蜃ｺ縺・
	 */
	virtual void Update();

	/*
	 * @brief 繧ｷ繝ｼ繝ｳ蜀・ｦ∫ｴ縺ｮ謠冗判
	 * [蜈･蜉嫋 縺ｪ縺・[蜃ｺ蜉嫋 縺ｪ縺・[蜑ｯ菴懃畑] 蜷・・繝阪・繧ｸ繝｣繝ｼ縺ｮDraw蜻ｼ縺ｳ蜃ｺ縺・
	 */
	virtual void Draw();

	/*
	 * @brief 繧ｷ繝ｼ繝ｳ邨ゆｺ・・逅・
	 * [蜈･蜉嫋 縺ｪ縺・[蜃ｺ蜉嫋 縺ｪ縺・[蜑ｯ菴懃畑] 繧ｪ繝悶ず繧ｧ繧ｯ繝育ｭ峨・隗｣謾ｾ
	 */
	virtual void Finalize() = 0;

	// 繧ｲ繝・ち繝ｼ繝ｻ繧ｻ繝・ち繝ｼ鄒､
	ObjectManager* GetObjectManager() { return mpObjectManager; }
	ColliderManager* GetCollisionManager() { return mpColliderManager; }
	float GetFadeAlpha() const { return mfFadeAlpha; }
	void SetFadeAlpha(float alpha) { mfFadeAlpha = alpha; }
	
	SceneFade mFadeState = SceneFade_None; // 繝輔ぉ繝ｼ繝臥憾諷・
	int mNextScene = 0;                    // 谺｡縺ｮ繧ｷ繝ｼ繝ｳID

	Fever* mpFever;                        // 繝輔ぅ繝ｼ繝舌・邂｡逅・
	AnimalManager* mpAnimalManager;        // 繧｢繝九・繝ｫ邂｡逅・
	CowManager* mpCowManager;              // 迚帷ｮ｡逅・
	GameManager* mpGameManager;            // 繧ｲ繝ｼ繝騾ｲ陦檎ｮ｡逅・

private:
	ObjectManager* mpObjectManager;        // 繧ｪ繝悶ず繧ｧ繧ｯ繝育ｮ｡逅・
	ColliderManager* mpColliderManager;    // 繧ｳ繝ｩ繧､繝繝ｼ邂｡逅・
	float mfFadeAlpha;                     // 繝輔ぉ繝ｼ繝我ｸ埼乗・蠎ｦ (0-255)
	float mfFadeSpeed;                     // 繝輔ぉ繝ｼ繝蛾溷ｺｦ
};
