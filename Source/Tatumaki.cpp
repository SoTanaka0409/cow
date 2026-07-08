#include "ServiceLocator.h"
#include "Tatumaki.h"
#include "Player3D.h"
#include "CapsuleCollider.h"
#include "SphereCollider.h"
#include "Master.h"
#include "SceneManager.h"
#include "Scene.h"
#include "ObjectManager.h"

Tatumaki::Tatumaki(VECTOR pos)
	: Object3D(pos)
{
	tatumaki_spawn_timer_ = 0;
	pos_ = pos;
	move_timer_ = 0;
	speed_ = 5.0f;
	velocity_ = VGet(0, 0, 0);

	effect_timer_ = 360;

	is_crisis_ = false;
	current_scale_ratio_ = 0.5f; 
	current_radius_ = 400.0f;
	capsule_collider_->radius_ = current_radius_;

	tatu_ = new EffekseerEffect("Resource/3D/EFK/Tatumaki2.efk", pos_, 200.0f);

	// 蛻晄悄迥ｶ諷九〒逋ｺ逕溽峩蠕後↓繝励Ξ繧､繝､繝ｼ繧貞叉豁ｻ縺輔○縺ｪ縺・◆繧√√せ繧ｱ繝ｼ繝ｫ繧堤ｵ槭▲縺ｦ逕滓・縺吶ｋ
	tatu_->SetScale(VGet(1.0f * current_scale_ratio_, 1.4f * current_scale_ratio_, 1.0f * current_scale_ratio_));
}

Tatumaki::~Tatumaki()
{
	// 繝｡繝｢繝ｪ繝ｪ繝ｼ繧ｯ蝗樣∩縺ｮ縺溘ａ縲∝､夜Κ繝ｪ繧ｽ繝ｼ繧ｹ縺ｧ縺ゅｋEffekseerEffect繧呈・遉ｺ逧・↓隗｣謾ｾ縺吶ｋ
	if (tatu_ != nullptr)
	{
		delete tatu_;
		tatu_ = nullptr;
	}
}

void Tatumaki::Update()
{
	// 繧ｵ繧､繧ｺ諤･螟峨↓繧医ｋ隕冶ｦ夂噪驕募柱諢溘ｒ髦ｲ縺舌◆繧√∫岼讓吶し繧､繧ｺ縺ｫ蜷代￠縺ｦLerp縺ｧ蠕舌・↓陬憺俣縺吶ｋ
	float targetScale = is_crisis_ ? 1.0f : 0.5f;
	float targetRadius = is_crisis_ ? 800.0f : 400.0f;
	
	current_scale_ratio_ += (targetScale - current_scale_ratio_) * 0.05f;
	current_radius_ += (targetRadius - current_radius_) * 0.05f;
	
	tatu_->SetScale(VGet(1.0f * current_scale_ratio_, 1.4f * current_scale_ratio_, 1.0f * current_scale_ratio_));
	capsule_collider_->radius_ = current_radius_;

	auto players = ServiceLocator::GetPlayers();
	Player3D* p = nullptr;
	float minDistSq = -1.0f;

	// 繝槭Ν繝√・繝ｬ繧､迺ｰ蠅・ｸ九↓縺翫＞縺ｦ縲∫判髱｢螟悶・驕縺・・繝ｬ繧､繝､繝ｼ繧剃ｸ崎・辟ｶ縺ｫ霑ｽ霍｡縺吶ｋ縺ｮ繧帝亟縺舌◆繧∵怙遏ｭ霍晞屬縺ｮ蟇ｾ雎｡繧呈､懃ｴ｢縺吶ｋ
	for (auto player : players)
	{
		VECTOR diff = VSub(player->GetPosition(), pos_);
		diff.y = 0; // 鬮倅ｽ主ｷｮ縺ｫ繧医ｋ霑ｽ霍｡蟇ｾ雎｡縺ｮ繝悶Ξ繧帝亟縺舌◆繧√々Z蟷ｳ髱｢縺ｮ縺ｿ縺ｧ霍晞屬險育ｮ励ｒ陦後≧
		float distSq = VSquareSize(diff);
		if (minDistSq < 0 || distSq < minDistSq)
		{
			minDistSq = distSq;
			p = player;
		}
	}

	// 霑ｽ蠕灘ｯｾ雎｡縺悟ｭ伜惠縺吶ｋ蝣ｴ蜷医・縺ｿ縲√・繝ｼ繝溘Φ繧ｰ蜃ｦ逅・ｒ螳溯｡後＠繝励Ξ繧､繝､繝ｼ縺ｫ蜷代°縺｣縺ｦ繝吶け繝医Ν繧貞髄縺代ｋ
	if (p != nullptr)
	{
		VECTOR playerPos = p->GetPosition();
		VECTOR targetDir = VSub(playerPos, pos_);
		targetDir.y = 0; 

		// 螳悟・縺ｫ驥阪↑縺｣縺滄圀縺ｮ繧ｼ繝ｭ髯､邂・VNorm)繧ｨ繝ｩ繝ｼ繧帝亟縺舌◆繧√・髢ｾ蛟､繝√ぉ繝・け
		if (VSize(targetDir) > 0.1f)
		{
			targetDir = VNorm(targetDir);

			// 譌句屓蜊雁ｾ・ｒ螟ｧ縺阪￥縺励※蝗樣∩蜿ｯ閭ｽ縺ｪ驕翫・繧呈戟縺溘○繧九◆繧√∝刈邂励☆繧句髄縺阪・繝吶け繝医Ν繧貞ｼｱ繧√ｋ
			float homingStrength = 0.001f;
			velocity_ = VAdd(velocity_, VScale(targetDir, homingStrength));
			velocity_ = VNorm(velocity_);
		}
	}

	pos_ = VAdd(pos_, VScale(velocity_, speed_));
	SetPosition(pos_);

	// 繧ｫ繝励そ繝ｫ繧ｳ繝ｩ繧､繝繝ｼ繧堤ｸｦ縺ｫ髟ｷ縺丞叙繧翫√ず繝｣繝ｳ繝嶺ｸｭ縺ｮ繝励Ξ繧､繝､繝ｼ縺ｫ繧ょｽ薙◆繧雁愛螳壹ｒ驕ｩ逕ｨ縺輔○繧・
	capsule_collider_->position_ = VSub(position_, VGet(0, 2000, 0));
	capsule_collider_->position2_ = VAdd(position_, VGet(0, 2000, 0));

	// 遶懷ｷｻ縺後せ繝・・繧ｸ螟悶∈豸亥､ｱ縺励√ご繝ｼ繝騾ｲ陦後↓謾ｯ髫懊ｒ縺阪◆縺吶・繧帝亟縺舌◆繧√・隕九∴縺ｪ縺・｣・
	float limit = 5000.0f;
	if (pos_.x < -limit || pos_.x > limit) { velocity_.x *= -1; }
	if (pos_.z < -limit || pos_.z > limit) { velocity_.z *= -1; }

	effect_timer_--;
	if (effect_timer_ <= 0)
	{
		if (tatu_ != nullptr) 
		{ 
			tatu_->Play(); 
			
			// 繝代ヵ繧ｩ繝ｼ繝槭Φ繧ｹ譛驕ｩ蛹悶・縺溘ａ縲√・繝ｬ繧､繝､繝ｼ縺九ｉ髮｢繧後◆菴咲ｽｮ縺ｧ縺ｮSE蜀咲函繧堤怐逡･縺吶ｋ
			if (p != nullptr)
			{
				VECTOR diff = VSub(p->GetPosition(), pos_);
				if (VSquareSize(diff) < 3000.0f * 3000.0f)
				{
					Master::mpSoundManager->PlaySE(SoundManager::kSeTatumaki);
				}
			}
		}
		effect_timer_ = 360;
	}

	// 繧ｨ繝輔ぉ繧ｯ繝医・蠎ｧ讓吶ｒ繧ｪ繝悶ず繧ｧ繧ｯ繝医↓霑ｽ蠕薙＆縺帙∵緒逕ｻ諠・ｱ縺ｮ蜷梧悄繧貞叙繧・
	if (tatu_ != nullptr)
	{
		tatu_->SetPosition(pos_);
		tatu_->Update();
	}
}

void Tatumaki::Draw()
{
}

void Tatumaki::OnEnter(Collider* collider, Collider* check)
{
	// 遶懷ｷｻ謗･隗ｦ譎ゅ√・繝ｬ繧､繝､繝ｼ繧偵Λ繝ｳ繝繝縺ｪ菴咲ｽｮ縺ｸ繝ｯ繝ｼ繝励＆縺帙ｋ・医・繝翫Ν繝・ぅ莉墓ｧ假ｼ・
	if (collider == capsule_collider_ && check->parent_object_->GetTag() == kTag3dPlayer)
	{
		Player3D* Player = dynamic_cast<Player3D*>(check->parent_object_);

		float rangeLimit = 4000.0f;
		float warpX = (float)GetRand((int)rangeLimit * 2) - rangeLimit;
		float warpZ = (float)GetRand((int)rangeLimit * 2) - rangeLimit;

		Player->SetPosition(VGet(warpX, 2000.0f, warpZ));

		// 繝励Ξ繧､繝､繝ｼ縺ｸ縺ｮ隕冶ｦ夂噪繝輔ぅ繝ｼ繝峨ヰ繝・け縺ｨ縺励※逕ｻ髱｢繧呈ｿ縺励￥謠ｺ繧峨☆
		Master::mpCamera->SetupShake(20.0f, 35.0f, 30.0f);
	}
}

void Tatumaki::OnTrigger(Collider* collider, Collider* check)
{
}

void Tatumaki::OnExit(Collider* collider, Collider* check)
{
}