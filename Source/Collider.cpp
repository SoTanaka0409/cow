#include "Collider.h"
#include "Object3D.h"
#include "ColliderManager.h"
#include <cassert>

Collider::Collider(Object3D* parent)
	: parent_object_(parent)
	, position_(VGet(0.0f, 0.0f, 0.0f))
	, position2_(VGet(0.0f, 0.0f, 0.0f))
	, radius_(0.0f)
	, delete_flag_(false)
{
	assert(parent);
	ColliderManager::GetInstance()->AddCollider(this); // 逕滓・譎ゅ↓閾ｪ蜍輔〒繝槭ロ繝ｼ繧ｸ繝｣繝ｼ縺ｸ逋ｻ骭ｲ
}

Collider::~Collider()
{
	ColliderManager::GetInstance()->RemoveCollider(this); // 遐ｴ譽・凾縺ｫ閾ｪ蜍輔〒繝槭ロ繝ｼ繧ｸ繝｣繝ｼ縺九ｉ逋ｻ骭ｲ隗｣髯､
}

/*
 * @brief 蛻･縺ｮ繧ｳ繝ｩ繧､繝繝ｼ縺ｨ縺ｮ蟷ｾ菴募ｭｦ逧・↑莠､蟾ｮ迥ｶ諷九ｒ蜈・↓縲・←蛻・↑繧ｳ繝ｪ繧ｸ繝ｧ繝ｳ繧､繝吶Φ繝医ｒ騾夂衍縺吶ｋ
 * [蜈･蜉嫋 check: 蛻､螳壼ｯｾ雎｡縺ｮ逶ｸ謇九さ繝ｩ繧､繝繝ｼ, isHit: 蠖薙◆繧雁愛螳壹・莠､蟾ｮ險育ｮ礼ｵ先棡
 * [蜃ｺ蜉嫋 縺ｪ縺・
 * [蜑ｯ菴懃畑] mCollisionList縺ｮ謖ｿ蜈･繝ｻ蜑企勁縲＾nEnter / OnTrigger / OnExit 縺ｮ隕ｪ繧｢繧ｯ繧ｿ繝ｼ縺ｸ縺ｮ繧ｳ繝ｼ繝ｫ繝舌ャ繧ｯ騾夂衍
 */
void Collider::HitCheck(Collider* check, bool isHit)
{
	if (isHit)
	{
		// 譌｢縺ｫ蜑阪ヵ繝ｬ繝ｼ繝縺ｧ蜷後§逶ｸ謇九→陦晉ｪ√＠縺ｦ縺・◆縺九ｒ讀懃ｴ｢
		auto itr = std::find_if(
			collision_list_.begin(),
			collision_list_.end(),
			[&](Collider* col) { return col == check; }
		);

		if (itr != collision_list_.end())
		{
			// 蜑阪ヵ繝ｬ繝ｼ繝縺九ｉ陦晉ｪ√′邯咏ｶ壹＠縺ｦ縺・ｋ縺溘ａ縲＾nTrigger・域ｻ槫惠繧､繝吶Φ繝茨ｼ峨ｒ騾夂衍
			if (this->parent_object_ != nullptr)
			{
				parent_object_->OnTrigger(this, check);
			}
		}
		else
		{
			// 譁ｰ隕上・陦晉ｪ√′逋ｺ逕溘＠縺溘◆繧√√Μ繧ｹ繝医↓逋ｻ骭ｲ縺励※ OnEnter・磯幕蟋九う繝吶Φ繝茨ｼ峨ｒ騾夂衍
			collision_list_.push_back(check);
			if (this->parent_object_ != nullptr)
			{
				parent_object_->OnEnter(this, check);
			}
		}
	}
	else
	{
		// 陦晉ｪ√＠縺ｦ縺・↑縺・ｴ蜷医∝燕繝輔Ξ繝ｼ繝縺ｾ縺ｧ陦晉ｪ√＠縺ｦ縺・◆縺九・迥ｶ諷九ｒ繝√ぉ繝・け縺吶ｋ
		auto itr = std::find_if(
			collision_list_.begin(),
			collision_list_.end(),
			[&](Collider* col) { return col == check; }
		);

		if (itr != collision_list_.end())
		{
			// 陦晉ｪ√′蛻・ｌ縺滂ｼ磯屬閼ｱ縺励◆・臥椪髢薙・縺溘ａ縲＾nExit・育ｵゆｺ・う繝吶Φ繝茨ｼ峨ｒ騾夂衍縺励Μ繧ｹ繝医°繧蛾勁螟悶☆繧・
			if (this->parent_object_ != nullptr)
			{
				this->parent_object_->OnExit(this, check);
			}
			collision_list_.erase(itr);
		}
	}
}

void Collider::Draw()
{
}

void Collider::OnEnter()
{
}

void Collider::OnTrigger()
{
}

void Collider::OnExit()
{
}