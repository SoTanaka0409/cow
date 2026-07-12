#include "Skill.h"
#include "Player3D.h"
#include "Texture.h"
#include "Master.h"
#include "InputManager.h"
#include "Bait.h"
#include <cmath>

void Skill::SetSkillFlag(bool flag)
{
	add_skill_flag_ = flag;
	if (flag)
	{
		texture_->SetPosition(pos1_);
		texture2_->SetPosition(pos2_);
		texture3_->SetPosition(pos3_);

		select_scale_ = 1.0f;
		select_anim_ = false;
		selected_skill_ = 0;

		open_anim_ = true;

		card1_y_ = 1400.0f;
		card2_y_ = 1500.0f;
		card3_y_ = 1600.0f;
	}
}

Skill::Skill(Object3D* parent)
	: status_a_(0.0f)
	, status_s_(0.0f)
	, tag_(None)
	, add_skill_flag_(false)
	, hover_skill_(0)
	, select_anim_(false)
	, selected_skill_(0)
	, select_scale_(1.0f)
{
	this->parent_ = parent;

	pos1_ = VGet(200.0f, 450.0f, 0.0f);
	pos2_ = VGet(800.0f, 450.0f, 0.0f);
	pos3_ = VGet(1400.0f, 450.0f, 0.0f);

	texture_ = new Texture("Resource/2D/Speed.png", pos1_, 300, 500, true);
	texture2_ = new Texture("Resource/2D/food.png", pos2_, 300, 500, true);
	texture3_ = new Texture("Resource/2D/Kyuusyuu.png", pos3_, 300, 500, true);
}

Skill::~Skill()
{
	if (texture_ != nullptr)
	{
		delete texture_;
		texture_ = nullptr;
	}
	if (texture2_ != nullptr)
	{
		delete texture2_;
		texture2_ = nullptr;
	}
	if (texture3_ != nullptr)
	{
		delete texture3_;
		texture3_ = nullptr;
	}
}

void Skill::Draw()
{
	if (!add_skill_flag_ && !select_anim_) return;

	// 驕ｸ謚槫ｮ御ｺ・ｾ後・繧ｫ繝ｼ繝峨′荳頑・・騾蜃ｺ縺励※縺・￥貍泌・繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ
	if (select_anim_)
	{
		if (selected_skill_ == 1)
		{
			texture_->SetPosition(select_pos_);
			texture_->Draw();
		}
		else if (selected_skill_ == 2)
		{
			texture2_->SetPosition(select_pos_);
			texture2_->Draw();
		}
		else if (selected_skill_ == 3)
		{
			texture3_->SetPosition(select_pos_);
			texture3_->Draw();
		}
		return;
	}

	texture_->Draw();
	texture2_->Draw();
	texture3_->Draw();
}

void Skill::Update()
{
	if (add_skill_flag_)
	{
		AddSkill();
	}

	if (!add_skill_flag_ && !select_anim_)
	{
		Master::SelectSkill = false;
	}

	if (flash_)
	{
		flash_alpha_ -= 20;
		if (flash_alpha_ <= 0)
		{
			flash_alpha_ = 0;
			flash_ = false;
		}
	}

	// 驕ｸ謚槭＆繧後◆繧ｫ繝ｼ繝峨′荳顔ｩｺ縺ｸ鬟帙ｓ縺ｧ豸医∴繧区ｼ泌・繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ
	if (select_anim_)
	{
		VECTOR target = VGet(select_pos_.x, -400.0f, 0.0f);
		select_pos_.y += (target.y - select_pos_.y) * 0.08f;

		if (select_pos_.y <= -350.0f)
		{
			texture_->SetPosition(pos1_);
			texture2_->SetPosition(pos2_);
			texture3_->SetPosition(pos3_);

			select_anim_ = false;
			add_skill_flag_ = false;
			Master::SelectSkill = false;
		}
	}

	// 繧ｫ繝ｼ繝蛾∈謚樣幕蟋区凾縺ｫ縲√き繝ｼ繝峨′逕ｻ髱｢螟悶°繧画ｻ代ｉ縺九↓繧ｹ繝ｩ繧､繝峨う繝ｳ縺吶ｋ繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ
	if (open_anim_)
	{
		card1_y_ += (450.0f - card1_y_) * 0.15f;
		card2_y_ += (450.0f - card2_y_) * 0.15f;
		card3_y_ += (450.0f - card3_y_) * 0.15f;

		texture_->SetPosition(VGet(200.0f, card1_y_, 0.0f));
		texture2_->SetPosition(VGet(800.0f, card2_y_, 0.0f));
		texture3_->SetPosition(VGet(1400.0f, card3_y_, 0.0f));

		if (std::abs(card1_y_ - 450.0f) < 1.0f)
		{
			card1_y_ = 450.0f;
			card2_y_ = 450.0f;
			card3_y_ = 450.0f;
			open_anim_ = false;
		}
	}
}

void Skill::AddSkill()
{
	if (!add_skill_flag_) return;
	
	SetMouseDispFlag(true); // 繧ｹ繧ｭ繝ｫ驕ｸ謚樔ｸｭ縺ｯ繝槭え繧ｹ繝昴う繝ｳ繧ｿ繧定｡ｨ遉ｺ
	Master::SelectSkill = true;
	int mouse_x_, mouse_y_;
	GetMousePoint(&mouse_x_, &mouse_y_);

	hover_skill_ = 0;

	// 繧ｹ繧ｭ繝ｫ繧ｫ繝ｼ繝峨ｒ繝帙ヰ繝ｼ縺ｾ縺溘・蟾ｦ繧ｯ繝ｪ繝・け縺励◆髫帙・繧ｳ繝ｩ繧､繝繝ｼ險育ｮ励ｒ陦後≧繝ｩ繝繝髢｢謨ｰ
	auto ProcessSkill = [&](Texture* tex, int id) {
		if (tex == nullptr) return false;

		VECTOR pos = tex->GetPosition();
		int w = tex->GetWidth();
		int h = tex->GetHeight();

		float left = pos.x - (w / 2.0f);
		float right = pos.x + (w / 2.0f);
		float top = pos.y - (h / 2.0f);
		float bottom = pos.y + (h / 2.0f);

		if (mouse_x_ >= left && mouse_x_ <= right && mouse_y_ >= top && mouse_y_ <= bottom)
		{
			hover_skill_ = id;
			if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
			{
				return true;
			}
		}
		return false;
	};

	if (ProcessSkill(texture_, 1))
	{
		// 騾溷ｺｦ繧｢繝・・
		status_s_ += 2.0f;
		selected_skill_ = 1;
		select_pos_ = texture_->GetPosition();
		select_anim_ = true;
		flash_ = true;
		flash_alpha_ = 255;
		Master::mpSoundManager->PlaySE(SoundManager::kSeSkillStatus);
		SetMouseDispFlag(false);
	}
	else if (ProcessSkill(texture2_, 2))
	{
		// 鬢鯉ｼ医ョ繧ｳ繧､縺ｫ繧薙§繧難ｼ芽ｨｭ鄂ｮ
		auto b = new Bait("Resource/3D/迚帙・鬢・Carrot.mv1", parent_->GetPosition());
		float scale = 5000.0f;
		b->model_->SetScale(VGet(scale, scale, scale));

		selected_skill_ = 2;
		select_pos_ = texture2_->GetPosition();
		select_anim_ = true;
		flash_ = true;
		flash_alpha_ = 255;
		Master::mpSoundManager->PlaySE(SoundManager::kSeSkillFood);
		SetMouseDispFlag(false);
	}
	else if (ProcessSkill(texture3_, 3))
	{
		// 蜷ｸ蠑暮溷ｺｦ・域判謦・鴨・峨い繝・・
		status_a_ += 1.0f;
		selected_skill_ = 3;
		select_pos_ = texture3_->GetPosition();
		select_anim_ = true;
		flash_ = true;
		flash_alpha_ = 255;
		Master::mpSoundManager->PlaySE(SoundManager::kSeSkillStatus);
		SetMouseDispFlag(false);
	}
}

float Skill::GetStatusDate(StatusTag tag)
{
	if (tag == kStatusAttackSpeed)
	{
		return status_a_;
	}
	if (tag == kStatusSpeed)
	{
		return status_s_;
	}
	return 0.0f;
}