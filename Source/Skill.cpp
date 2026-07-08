#include "Skill.h"
#include "Player3D.h"
#include "Texture.h"
#include "Master.h"
#include "InputManager.h"
#include "Bait.h"
#include <cmath>

void Skill::SetSkillFlag(bool flag)
{
	AddSkillFlag = flag;
	if (flag)
	{
		mpTexture->SetPosition(mPos1);
		mpTexture2->SetPosition(mPos2);
		mpTexture3->SetPosition(mPos3);

		mSelectScale = 1.0f;
		mSelectAnim = false;
		mSelectedSkill = 0;

		mOpenAnim = true;

		mCard1Y = 1400.0f;
		mCard2Y = 1500.0f;
		mCard3Y = 1600.0f;
	}
}

Skill::Skill(Object3D* parent)
	: Status_A(0.0f)
	, Status_S(0.0f)
	, tag(None)
	, AddSkillFlag(false)
	, mHoverSkill(0)
	, mSelectAnim(false)
	, mSelectedSkill(0)
	, mSelectScale(1.0f)
{
	this->mpParent = parent;

	mPos1 = VGet(200.0f, 450.0f, 0.0f);
	mPos2 = VGet(800.0f, 450.0f, 0.0f);
	mPos3 = VGet(1400.0f, 450.0f, 0.0f);

	mpTexture = new Texture("Resource/2D/Speed.png", mPos1, 300, 500, true);
	mpTexture2 = new Texture("Resource/2D/food.png", mPos2, 300, 500, true);
	mpTexture3 = new Texture("Resource/2D/Kyuusyuu.png", mPos3, 300, 500, true);
}

Skill::~Skill()
{
	if (mpTexture != nullptr)
	{
		delete mpTexture;
		mpTexture = nullptr;
	}
	if (mpTexture2 != nullptr)
	{
		delete mpTexture2;
		mpTexture2 = nullptr;
	}
	if (mpTexture3 != nullptr)
	{
		delete mpTexture3;
		mpTexture3 = nullptr;
	}
}

void Skill::Draw()
{
	if (!AddSkillFlag && !mSelectAnim) return;

	// 驕ｸ謚槫ｮ御ｺ・ｾ後・繧ｫ繝ｼ繝峨′荳頑・繝ｻ騾蜃ｺ縺励※縺・￥貍泌・繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ
	if (mSelectAnim)
	{
		if (mSelectedSkill == 1)
		{
			mpTexture->SetPosition(mSelectPos);
			mpTexture->Draw();
		}
		else if (mSelectedSkill == 2)
		{
			mpTexture2->SetPosition(mSelectPos);
			mpTexture2->Draw();
		}
		else if (mSelectedSkill == 3)
		{
			mpTexture3->SetPosition(mSelectPos);
			mpTexture3->Draw();
		}
		return;
	}

	mpTexture->Draw();
	mpTexture2->Draw();
	mpTexture3->Draw();
}

void Skill::Update()
{
	if (AddSkillFlag)
	{
		AddSkill();
	}

	if (!AddSkillFlag && !mSelectAnim)
	{
		Master::SelectSkill = false;
	}

	if (mFlash)
	{
		mFlashAlpha -= 20;
		if (mFlashAlpha <= 0)
		{
			mFlashAlpha = 0;
			mFlash = false;
		}
	}

	// 驕ｸ謚槭＆繧後◆繧ｫ繝ｼ繝峨′荳顔ｩｺ縺ｸ鬟帙ｓ縺ｧ豸医∴繧区ｼ泌・繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ
	if (mSelectAnim)
	{
		VECTOR target = VGet(mSelectPos.x, -400.0f, 0.0f);
		mSelectPos.y += (target.y - mSelectPos.y) * 0.08f;

		if (mSelectPos.y <= -350.0f)
		{
			mpTexture->SetPosition(mPos1);
			mpTexture2->SetPosition(mPos2);
			mpTexture3->SetPosition(mPos3);

			mSelectAnim = false;
			AddSkillFlag = false;
			Master::SelectSkill = false;
		}
	}

	// 繧ｫ繝ｼ繝蛾∈謚樣幕蟋区凾縺ｫ縲√き繝ｼ繝峨′逕ｻ髱｢螟悶°繧画ｻ代ｉ縺九↓繧ｹ繝ｩ繧､繝峨う繝ｳ縺吶ｋ繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ
	if (mOpenAnim)
	{
		mCard1Y += (450.0f - mCard1Y) * 0.15f;
		mCard2Y += (450.0f - mCard2Y) * 0.15f;
		mCard3Y += (450.0f - mCard3Y) * 0.15f;

		mpTexture->SetPosition(VGet(200.0f, mCard1Y, 0.0f));
		mpTexture2->SetPosition(VGet(800.0f, mCard2Y, 0.0f));
		mpTexture3->SetPosition(VGet(1400.0f, mCard3Y, 0.0f));

		if (std::abs(mCard1Y - 450.0f) < 1.0f)
		{
			mCard1Y = 450.0f;
			mCard2Y = 450.0f;
			mCard3Y = 450.0f;
			mOpenAnim = false;
		}
	}
}

void Skill::AddSkill()
{
	if (!AddSkillFlag) return;
	
	SetMouseDispFlag(true); // 繧ｹ繧ｭ繝ｫ驕ｸ謚樔ｸｭ縺ｯ繝槭え繧ｹ繝昴う繝ｳ繧ｿ繧定｡ｨ遉ｺ
	Master::SelectSkill = true;
	int mouse_x_, mouse_y_;
	GetMousePoint(&mouse_x_, &mouse_y_);

	mHoverSkill = 0;

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
			mHoverSkill = id;
			if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
			{
				return true;
			}
		}
		return false;
	};

	if (ProcessSkill(mpTexture, 1))
	{
		// 騾溷ｺｦ繧｢繝・・
		Status_S += 2.0f;
		mSelectedSkill = 1;
		mSelectPos = mpTexture->GetPosition();
		mSelectAnim = true;
		mFlash = true;
		mFlashAlpha = 255;
		Master::mpSoundManager->PlaySE(SoundManager::kSeSkillStatus);
		SetMouseDispFlag(false);
	}
	else if (ProcessSkill(mpTexture2, 2))
	{
		// 鬢鯉ｼ医ョ繧ｳ繧､縺ｫ繧薙§繧難ｼ芽ｨｭ鄂ｮ
		auto b = new Bait("Resource/3D/迚帙・鬢・Carrot.mv1", mpParent->GetPosition());
		float scale = 5000.0f;
		b->model_->SetScale(VGet(scale, scale, scale));

		mSelectedSkill = 2;
		mSelectPos = mpTexture2->GetPosition();
		mSelectAnim = true;
		mFlash = true;
		mFlashAlpha = 255;
		Master::mpSoundManager->PlaySE(SoundManager::kSeSkillFood);
		SetMouseDispFlag(false);
	}
	else if (ProcessSkill(mpTexture3, 3))
	{
		// 蜷ｸ蠑暮溷ｺｦ・域判謦・鴨・峨い繝・・
		Status_A += 1.0f;
		mSelectedSkill = 3;
		mSelectPos = mpTexture3->GetPosition();
		mSelectAnim = true;
		mFlash = true;
		mFlashAlpha = 255;
		Master::mpSoundManager->PlaySE(SoundManager::kSeSkillStatus);
		SetMouseDispFlag(false);
	}
}

float Skill::GetStatusDate(StatusTag tag)
{
	if (tag == Status_AttackSpeed)
	{
		return Status_A;
	}
	if (tag == Status_Speed)
	{
		return Status_S;
	}
	return 0.0f;
}