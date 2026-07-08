#pragma once
#include "Object3D.h"
#include "Model.h"
#include "dxlib.h"
#include "string"
#include "GameTimer.h"

// 迚帙ｒ蠑輔″蟇・○繧九◆繧√・鬢鯉ｼ・ait・峨が繝悶ず繧ｧ繧ｯ繝医ｒ蛻ｶ蠕｡縺吶ｋ繧ｯ繝ｩ繧ｹ
class Bait : public Object3D
{
public:
	/*
	 * @brief 鬢後が繝悶ず繧ｧ繧ｯ繝医ｒ驟咲ｽｮ縺励∵怏蜉ｹ譎る俣逕ｨ縺ｮ繧ｿ繧､繝槭・縺ｨ蜷ｸ蠑輔さ繝ｩ繧､繝繝ｼ繧定ｨｭ螳壹☆繧・
	 * [蜈･蜉嫋 filename: 繝｢繝・Ν縺ｮ繝輔ぃ繧､繝ｫ繝代せ, pos: 驟咲ｽｮ蠎ｧ讓・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] GameTimer繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ縺ｮ逕滓・縲√さ繝ｩ繧､繝繝ｼ繧ｵ繧､繧ｺ險ｭ螳・
	 */
	Bait(std::string filename, VECTOR pos);
	virtual ~Bait();

	void Update() override;
	void Draw() override;

	virtual void OnEnter(Collider* collider, Collider* check) override;

public:
	Model* model_;          // 鬢後・3D繝｢繝・Ν縺ｸ縺ｮ繝昴う繝ｳ繧ｿ

private:
	GameTimer* game_timer_;  // 譛牙柑譎る俣繧偵き繧ｦ繝ｳ繝医☆繧九ち繧､繝槭・繧ｪ繝悶ず繧ｧ繧ｯ繝医∈縺ｮ繝昴う繝ｳ繧ｿ
};
