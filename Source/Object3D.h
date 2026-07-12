#pragma once
#include "DxLib.h"
#include <string>

class Collider;
class CapsuleCollider;

// 3D遨ｺ髢薙↓驟咲ｽｮ縺輔ｌ繧九が繝悶ず繧ｧ繧ｯ繝医・蝓ｺ蠎輔け繝ｩ繧ｹ
class Object3D
{
public:
	// 繧ｪ繝悶ず繧ｧ繧ｯ繝医・隴伜挨逕ｨ繧ｿ繧ｰ
	enum Tag3D
	{
		kNone3d,
		kTag3dStage,
		kTag3dPlayer,
		kTag3dEnemy,
		kTag3dCow,
		kTag3dWall,
		kTag3dBait,
		kTag3dAnimal
	};

public:
	// 繧ｫ繝｡繝ｩ縺九ｉ縺ｮ霍晞屬繧定ｨｭ螳壹☆繧・
	void SetCameraDistance(float distance) { current_camera_distance_ = distance; }

	// 繧ｫ繝｡繝ｩ縺九ｉ縺ｮ霍晞屬繧貞渕貅悶↓驕縺・・ｼ・蛟､縺ｮ髯埼・ｼ峨〒繧ｽ繝ｼ繝医☆繧九◆繧√・豈碑ｼ・未謨ｰ
	struct CompareZOrder {
		bool operator()(Object3D* a, Object3D* b) const {
			return a->current_camera_distance_ > b->current_camera_distance_;
		}
	};

public:
	/*
	 * @brief 3D繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ蛻晄悄蠎ｧ讓吶〒逕滓・縺励∫樟蝨ｨ繧｢繧ｯ繝・ぅ繝悶↑繧ｷ繝ｼ繝ｳ縺ｮ繝槭ロ繝ｼ繧ｸ繝｣繝ｼ縺ｸ閾ｪ蜍慕匳骭ｲ縺吶ｋ
	 * [蜈･蜉嫋 initPos: 蛻晄悄蠎ｧ讓・
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] 繧ｷ繝ｼ繝ｳ縺ｮObjectManager縺ｫ閾ｪ霄ｫ縺檎匳骭ｲ縺輔ｌ繧九√ョ繝輔か繝ｫ繝医・繧ｫ繝励そ繝ｫ繧ｳ繝ｩ繧､繝繝ｼ縺檎函謌舌＆繧後ｋ
	 */
	Object3D(VECTOR initPos);

	virtual ~Object3D();

	virtual void Update();

	virtual void Draw();

	/*
	 * @brief 繧ｳ繝ｩ繧､繝繝ｼ蜷悟｣ｫ縺ｮ莠､蟾ｮ縺碁幕蟋九＠縺溽椪髢薙↓蜻ｼ縺ｰ繧後ｋ繧ｳ繝ｼ繝ｫ繝舌ャ繧ｯ髢｢謨ｰ
	 * [蜈･蜉嫋 collider: 閾ｪ霄ｫ縺ｮ繧ｳ繝ｩ繧､繝繝ｼ, check: 逶ｸ謇九・繧ｳ繝ｩ繧､繝繝ｼ
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] なし
	 */
	virtual void OnEnter(Collider* collider, Collider* check);

	/*
	 * @brief 繧ｳ繝ｩ繧､繝繝ｼ蜷悟｣ｫ縺御ｺ､蟾ｮ縺励※縺・ｋ髢薙∵ｯ弱ヵ繝ｬ繝ｼ繝蜻ｼ縺ｰ繧後ｋ繧ｳ繝ｼ繝ｫ繝舌ャ繧ｯ髢｢謨ｰ
	 * [蜈･蜉嫋 collider: 閾ｪ霄ｫ縺ｮ繧ｳ繝ｩ繧､繝繝ｼ, check: 逶ｸ謇九・繧ｳ繝ｩ繧､繝繝ｼ
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] なし
	 */
	virtual void OnTrigger(Collider* collider, Collider* check);

	/*
	 * @brief 繧ｳ繝ｩ繧､繝繝ｼ蜷悟｣ｫ縺ｮ莠､蟾ｮ縺檎ｵゆｺ・＠縺溽椪髢薙↓蜻ｼ縺ｰ繧後ｋ繧ｳ繝ｼ繝ｫ繝舌ャ繧ｯ髢｢謨ｰ
	 * [蜈･蜉嫋 collider: 閾ｪ霄ｫ縺ｮ繧ｳ繝ｩ繧､繝繝ｼ, check: 逶ｸ謇九・繧ｳ繝ｩ繧､繝繝ｼ
	 * [蜃ｺ蜉嫋 なし
	 * [蜑ｯ菴懃畑] なし
	 */
	virtual void OnExit(Collider* collider, Collider* check);

public:
	// 繧ｲ繝・ち繝ｼ・繧ｻ繝・ち繝ｼ鄒､
	void SetPosition(VECTOR pos) { position_ = pos; };
	VECTOR GetPosition() { return position_; }

	void SetOldPosition(VECTOR pos) { old_position_ = pos; }
	VECTOR GetOldPosition() { return old_position_; }

	void SetRotation(VECTOR rot) { rotation_ = rot; }
	VECTOR GETRotation() { return rotation_; }

	void SetDeleteFlag(bool flag) { delete_flag_ = flag; }
	bool IsDeleteFlag() { return delete_flag_; }

	void SetDrawFlag(bool flag) { draw_flag_ = flag; }
	bool IsDrawFlag() { return draw_flag_; }

	void SetTag(Tag3D tag) { tag_ = tag; }
	Tag3D GetTag() { return tag_; }

protected:
	VECTOR position_;                  // 迴ｾ蝨ｨ縺ｮ蠎ｧ讓・
	VECTOR rotation_;                  // 蝗櫁ｻ｢隗貞ｺｦ
	VECTOR old_position_;               // 1繝輔Ξ繝ｼ繝蜑阪・蠎ｧ讓・
	CapsuleCollider* capsule_collider_; // 謇譛峨☆繧九き繝励そ繝ｫ繧ｳ繝ｩ繧､繝繝ｼ
	float radius_;                     // 邁｡譏鍋噪縺ｪ陦晉ｪ∝濠蠕・

private:
	bool delete_flag_;                  // 蜑企勁繝輔Λ繧ｰ・・rue縺ｧ繝槭ロ繝ｼ繧ｸ繝｣繝ｼ縺九ｉ遐ｴ譽・＆繧後ｋ・・
	Tag3D tag_;                        // 繧ｪ繝悶ず繧ｧ繧ｯ繝郁ｭ伜挨繧ｿ繧ｰ
	bool draw_flag_;                    // 謠冗判繝輔Λ繧ｰ
	float current_camera_distance_;      // 繧ｫ繝｡繝ｩ縺九ｉ縺薙・繧ｪ繝悶ず繧ｧ繧ｯ繝医∪縺ｧ縺ｮ霍晞屬
};