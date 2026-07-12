#pragma once
#include "Model.h"
#include "Dxlib.h"
#include "Object3D.h"

// 繧ｹ繝・・繧ｸ蜀・↓驟咲ｽｮ縺輔ｌ繧句推遞ｮ髱咏噪3D繧ｪ繝悶ず繧ｧ繧ｯ繝茨ｼ磯囿螳ｳ迚ｩ繧・｣・｣ｾ繧ｪ繝悶ず繧ｧ繧ｯ繝医↑縺ｩ・峨ｒ蛻ｶ蠕｡縺吶ｋ繧ｯ繝ｩ繧ｹ
class Object_Stage : public Object3D
{
public:
	/*
	 * @brief 繧ｹ繝・・繧ｸ陬・｣ｾ繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ隱ｭ縺ｿ霎ｼ縺ｿ縲∽ｽ咲ｽｮ縲∵僑螟ｧ邇・∝屓霆｢隗貞ｺｦ繧定ｨｭ螳壹＠縺ｦ蛻晄悄蛹悶☆繧・
	 * [蜈･蜉嫋 filename: 繝｢繝・Ν縺ｮ繝輔ぃ繧､繝ｫ繝代せ, initPos: 蛻晄悄蠎ｧ讓・ scale: 諡｡螟ｧ邇・ angle: 蝗櫁ｻ｢隗貞ｺｦ
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] Model繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ縺悟虚逧・｢ｺ菫昴＆繧後ｋ
	 */
	Object_Stage(std::string filename, VECTOR initPos, float scale, VECTOR angle);
	virtual ~Object_Stage();

	void Update() override;
	void Draw() override;

	void SetScale(float scale);
	void SetRotation(VECTOR angle);
	void SetColor(float r, float g, float b, float a = 1.0f);

private:
	Model* model_;          // 蛻ｶ蠕｡蟇ｾ雎｡縺ｮ3D繝｢繝・Ν縺ｸ縺ｮ繝昴う繝ｳ繧ｿ
};
