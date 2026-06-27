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
	, hoverSkill(0)
	, selectAnim(false)
	, selectedSkill(0)
	, selectScale(1.0f)
{
	this->parent = parent;

	pos1 = VGet(200.0f, 450.0f, 0.0f);
	pos2 = VGet(800.0f, 450.0f, 0.0f);
	pos3 = VGet(1400.0f, 450.0f, 0.0f);

	texture = new Texture("Resource/2D/Speed.png", pos1, 300, 500, true);
	texture2 = new Texture("Resource/2D/food.png", pos2, 300, 500, true);
	texture3 = new Texture("Resource/2D/Kyuusyuu.png", pos3, 300, 500, true);
}

Skill::~Skill()
{
	if (texture != nullptr)
	{
		delete texture;
		texture = nullptr;
	}
	if (texture2 != nullptr)
	{
		delete texture2;
		texture2 = nullptr;
	}
	if (texture3 != nullptr)
	{
		delete texture3;
		texture3 = nullptr;
	}
}

void Skill::Draw()
{
	if (!AddSkillFlag && !selectAnim) return;

	// 選択完了後のカードが上昇・退出していく演出アニメーション
	if (selectAnim)
	{
		if (selectedSkill == 1)
		{
			texture->SetPosition(selectPos);
			texture->Draw();
		}
		else if (selectedSkill == 2)
		{
			texture2->SetPosition(selectPos);
			texture2->Draw();
		}
		else if (selectedSkill == 3)
		{
			texture3->SetPosition(selectPos);
			texture3->Draw();
		}
		return;
	}

	texture->Draw();
	texture2->Draw();
	texture3->Draw();
}

void Skill::Update()
{
	if (AddSkillFlag)
	{
		AddSkill();
	}

	if (!AddSkillFlag && !selectAnim)
	{
		Master::selectSkill = false;
	}

	if (flash)
	{
		flashAlpha -= 20;
		if (flashAlpha <= 0)
		{
			flashAlpha = 0;
			flash = false;
		}
	}

	// 選択されたカードが上空へ飛んで消える演出アニメーション
	if (selectAnim)
	{
		VECTOR target = VGet(selectPos.x, -400.0f, 0.0f);
		selectPos.y += (target.y - selectPos.y) * 0.08f;

		if (selectPos.y <= -350.0f)
		{
			texture->SetPosition(pos1);
			texture2->SetPosition(pos2);
			texture3->SetPosition(pos3);

			selectAnim = false;
			AddSkillFlag = false;
			Master::selectSkill = false;
		}
	}

	// カード選択開始時に、カードが画面外から滑らかにスライドインするアニメーション
	if (openAnim)
	{
		card1Y += (450.0f - card1Y) * 0.15f;
		card2Y += (450.0f - card2Y) * 0.15f;
		card3Y += (450.0f - card3Y) * 0.15f;

		texture->SetPosition(VGet(200.0f, card1Y, 0.0f));
		texture2->SetPosition(VGet(800.0f, card2Y, 0.0f));
		texture3->SetPosition(VGet(1400.0f, card3Y, 0.0f));

		if (std::abs(card1Y - 450.0f) < 1.0f)
		{
			card1Y = 450.0f;
			card2Y = 450.0f;
			card3Y = 450.0f;
			openAnim = false;
		}
	}
}

void Skill::AddSkill()
{
	if (!AddSkillFlag) return;
	
	SetMouseDispFlag(true); // スキル選択中はマウスポインタを表示
	Master::selectSkill = true;
	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);

	hoverSkill = 0;

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
			hoverSkill = id;
			if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
			{
				return true;
			}
		}
		return false;
	};

	if (ProcessSkill(texture, 1))
	{
		// 速度アップ
		Status_S += 2.0f;
		selectedSkill = 1;
		selectPos = texture->GetPosition();
		selectAnim = true;
		flash = true;
		flashAlpha = 255;
		Master::soundManager->PlaySE(SoundManager::SE_SKILL_STATUS);
		SetMouseDispFlag(false);
	}
	else if (ProcessSkill(texture2, 2))
	{
		// 餌（デコイにんじん）設置
		auto b = new Bait("Resource/3D/牛の餌/Carrot.mv1", parent->GetPosition());
		float scale = 5000.0f;
		b->model->SetScale(VGet(scale, scale, scale));

		selectedSkill = 2;
		selectPos = texture2->GetPosition();
		selectAnim = true;
		flash = true;
		flashAlpha = 255;
		Master::soundManager->PlaySE(SoundManager::SE_SKILL_FOOD);
		SetMouseDispFlag(false);
	}
	else if (ProcessSkill(texture3, 3))
	{
		// 吸引速度（攻撃力）アップ
		Status_A += 1.0f;
		selectedSkill = 3;
		selectPos = texture3->GetPosition();
		selectAnim = true;
		flash = true;
		flashAlpha = 255;
		Master::soundManager->PlaySE(SoundManager::SE_SKILL_STATUS);
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

