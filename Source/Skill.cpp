#include "Skill.h"
#include "Player3D.h"
#include "Texture.h"
#include "Master.h"
#include "InputManager.h"
#include "Bait.h"
#include <cmath>

/*
 * スキル選択画面の表示状態を切り替えるため。
 * [入力] flag: スキル画面を表示するかどうかのフラグ
 * [出力] なし
 * [副作用] add_skill_flag_等のUI関連メンバ変数が初期化される
 */
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

/*
 * スキルシステムの初期化とUIテクスチャの準備を行うため。
 * [入力] parent: スキルを保持する親オブジェクト(プレイヤー等)のポインタ
 * [出力] なし
 * [副作用] 各種メンバ変数の初期化、テクスチャの動的メモリ確保
 */
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

	texture_ = new Texture("Resource/2D/スキル/スキル画像_移動速度.png", pos1_, 300, 500, true);
	texture2_ = new Texture("Resource/2D/スキル/スキル画像_牛の餌.png", pos2_, 300, 500, true);
	texture3_ = new Texture("Resource/2D/スキル/スキル画像_吸い込み.png", pos3_, 300, 500, true);
}

/*
 * 動的確保したテクスチャのメモリリークを防ぐため。
 * [入力] なし
 * [出力] なし
 * [副作用] UIテクスチャのメモリが解放される
 */
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

/*
 * スキル選択中や決定時のUIを画面に描画するため。
 * [入力] なし
 * [出力] なし
 * [副作用] 画面にスキルカードが描画される
 */
void Skill::Draw()
{
	if (!add_skill_flag_ && !select_anim_) return;

	// 選択したスキルをプレイヤーに視覚的に強く印象付けるため。
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

/*
 * スキルUIのアニメーションや選択状態を毎フレーム更新するため。
 * [入力] なし
 * [出力] なし
 * [副作用] アニメーション進行に伴い、UI座標やフラグが変化する
 */
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

	UpdateFlashAnimation();
	UpdateSelectAnimation();
	UpdateOpenAnimation();
}

void Skill::UpdateFlashAnimation()
{
	if (flash_)
	{
		flash_alpha_ -= 20;
		if (flash_alpha_ <= 0)
		{
			flash_alpha_ = 0;
			flash_ = false;
		}
	}
}

void Skill::UpdateSelectAnimation()
{
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
}

void Skill::UpdateOpenAnimation()
{
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

/*
 * プレイヤーの入力に応じて対応するスキル効果を付与するため。
 * [入力] なし
 * [出力] なし
 * [副作用] マウス座標の取得、ステータスの上昇や餌の生成が発生する
 */
void Skill::AddSkill()
{
	if (!add_skill_flag_) return;

	SetMouseDispFlag(true);
	Master::SelectSkill = true;
	int mouse_x_, mouse_y_;
	GetMousePoint(&mouse_x_, &mouse_y_);

	hover_skill_ = 0;

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
		status_s_ += 2.0f;
		selected_skill_ = 1;
		select_pos_ = texture_->GetPosition();
		select_anim_ = true;
		flash_ = true;
		flash_alpha_ = 255;
		Master::sound_manager_->PlaySE(SoundManager::kSeSkillStatus);
		SetMouseDispFlag(false);
	}
	else if (ProcessSkill(texture2_, 2))
	{
		auto b = new Bait("Resource/3D/小物/牛の餌/にんじん.mv1", parent_->GetPosition());
		float scale = 5000.0f;
		b->model_->SetScale(VGet(scale, scale, scale));

		selected_skill_ = 2;
		select_pos_ = texture2_->GetPosition();
		select_anim_ = true;
		flash_ = true;
		flash_alpha_ = 255;
		Master::sound_manager_->PlaySE(SoundManager::kSeSkillFood);
		SetMouseDispFlag(false);
	}
	else if (ProcessSkill(texture3_, 3))
	{
		status_a_ += 1.0f;
		selected_skill_ = 3;
		select_pos_ = texture3_->GetPosition();
		select_anim_ = true;
		flash_ = true;
		flash_alpha_ = 255;
		Master::sound_manager_->PlaySE(SoundManager::kSeSkillStatus);
		SetMouseDispFlag(false);
	}
}

/*
 * 他のクラスから現在の強化状態を参照できるようにするため。
 * [入力] tag: 取得したいステータスの種類
 * [出力] 対象ステータスの現在値
 * [副作用] なし
 */
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
