#include "Skill.h"
#include "Player3D.h"
#include "Texture.h"
#include "Master.h"
#include "InputManager.h"
#include "Bait.h"
#include <cmath>

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

	// 選択完了後のカードが上昇・退出していく演出アニメーション
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

	// 選択されたカードが上空へ飛んで消える演出アニメーション
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

	// カード選択開始時に、カードが画面外から滑らかにスライドインするアニメーション
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
	
	SetMouseDispFlag(true); // スキル選択中はマウスポインタを表示
	Master::SelectSkill = true;
	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);

	mHoverSkill = 0;

	// スキルカードをホバーまたは左クリックした際のコライダー計算を行うラムダ関数
	auto ProcessSkill = [&](Texture* tex, int id) {
		if (tex == nullptr) return false;

		VECTOR pos = tex->GetPosition();
		int w = tex->GetWidth();
		int h = tex->GetHeight();

		float left = pos.x - (w / 2.0f);
		float right = pos.x + (w / 2.0f);
		float top = pos.y - (h / 2.0f);
		float bottom = pos.y + (h / 2.0f);

		if (mouseX >= left && mouseX <= right && mouseY >= top && mouseY <= bottom)
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
		// 速度アップ
		Status_S += 2.0f;
		mSelectedSkill = 1;
		mSelectPos = mpTexture->GetPosition();
		mSelectAnim = true;
		mFlash = true;
		mFlashAlpha = 255;
		Master::mpSoundManager->PlaySE(SoundManager::SE_SKILL_STATUS);
		SetMouseDispFlag(false);
	}
	else if (ProcessSkill(mpTexture2, 2))
	{
		// 餌（デコイにんじん）設置
		auto b = new Bait("Resource/3D/牛の餌/Carrot.mv1", mpParent->GetPosition());
		float scale = 5000.0f;
		b->mpModel->SetScale(VGet(scale, scale, scale));

		mSelectedSkill = 2;
		mSelectPos = mpTexture2->GetPosition();
		mSelectAnim = true;
		mFlash = true;
		mFlashAlpha = 255;
		Master::mpSoundManager->PlaySE(SoundManager::SE_SKILL_FOOD);
		SetMouseDispFlag(false);
	}
	else if (ProcessSkill(mpTexture3, 3))
	{
		// 吸引速度（攻撃力）アップ
		Status_A += 1.0f;
		mSelectedSkill = 3;
		mSelectPos = mpTexture3->GetPosition();
		mSelectAnim = true;
		mFlash = true;
		mFlashAlpha = 255;
		Master::mpSoundManager->PlaySE(SoundManager::SE_SKILL_STATUS);
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

