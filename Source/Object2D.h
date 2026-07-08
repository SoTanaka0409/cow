#pragma once
#include <string>
#include <vector>
#include "Dxlib.h"

// 2D謠冗判繧定｡後≧繧ｪ繝悶ず繧ｧ繧ｯ繝医・蝓ｺ蠎輔け繝ｩ繧ｹ
class Object2D
{
public:
	// 繧ｪ繝悶ず繧ｧ繧ｯ繝医・隴伜挨逕ｨ繧ｿ繧ｰ
	enum Tag2D
	{
		kNone2d = 0,      // 險ｭ螳壹↑縺・
		kTag2dDino1 = 1,
		kTag2dDino2 = 100,
	};

public:
	/*
	 * @brief 2D繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ蛻晄悄蠎ｧ讓吶〒逕滓・縺励∫樟蝨ｨ縺ｮ繧｢繧ｯ繝・ぅ繝悶↑繧ｪ繝悶ず繧ｧ繧ｯ繝医・繝阪・繧ｸ繝｣繝ｼ縺ｫ閾ｪ蜍慕匳骭ｲ縺吶ｋ
	 * [蜈･蜉嫋 initPos: 蛻晄悄蠎ｧ讓・
	 * [蜃ｺ蜉嫋 縺ｪ縺・
	 * [蜑ｯ菴懃畑] 繧ｷ繝ｼ繝ｳ縺ｮObjectManager縺ｫ閾ｪ霄ｫ縺檎匳骭ｲ縺輔ｌ繧・
	 */
	Object2D(VECTOR initPos);

	virtual ~Object2D();

	virtual void Update();

	virtual void Draw();

public:
	// 繧ｲ繝・ち繝ｼ繝ｻ繧ｻ繝・ち繝ｼ鄒､
	void SetPosition(VECTOR pos) { position_ = pos; };
	VECTOR GetPosition() { return position_; }

	void SetRotation(VECTOR rot) { rotation_ = rot; }
	VECTOR GETRotation() { return rotation_; }

	void SetDeleteFlag(bool flag) { delete_flag_ = flag; }
	bool IsDeleteFlag() { return delete_flag_; }

	void SetDrawFlag(bool flag) { draw_flag_ = flag; }
	bool IsDrawFlag() { return draw_flag_; }

	void SetTag(Tag2D tag) { tag_ = tag; }
	Tag2D GetTag() { return tag_; }

protected:
	VECTOR position_;  // 逕ｻ髱｢荳翫・驟咲ｽｮ蠎ｧ讓・
	VECTOR rotation_;  // 蝗櫁ｻ｢隗貞ｺｦ

private:
	bool delete_flag_;  // 蜑企勁繝輔Λ繧ｰ・・rue縺ｧ繝槭ロ繝ｼ繧ｸ繝｣繝ｼ縺九ｉ遐ｴ譽・＆繧後ｋ・・
	Tag2D tag_;        // 繧ｪ繝悶ず繧ｧ繧ｯ繝郁ｭ伜挨繧ｿ繧ｰ
	bool draw_flag_;    // 謠冗判繧貞ｮ溯｡後☆繧九°縺ｩ縺・°縺ｮ繝輔Λ繧ｰ
};