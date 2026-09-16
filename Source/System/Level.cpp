#include"Level.h"
#include"Master.h"
#include"Player3D.h"
Level::Level(Object3D* obj)
	: xp_(0.0f)
	, max_xp_(1.0f)
	, now_level_(1)
{
	this->parent_ = obj;
}
Level::~Level()
{
}
void Level::Draw()
{
	DrawBar();
	// レベル表x8eｦ: ゆづったるんごのような緑色で描画
	SetFontSize(18);
	DrawFormatString(Utility::kUiBaseX + 2, Utility::kUiLevelY - 26, GetColor(80, 110, 60), "Lv.%d", now_level_);
	DrawFormatString(Utility::kUiBaseX, Utility::kUiLevelY - 28, GetColor(200, 230, 160), "Lv.%d", now_level_);
	SetFontSize(16);
}
void Level::Update()
{
}
/// @brief 経験値を加算し、最大値に達した場合のレベルアップ（およびスキル選択要求）を行う
/// @param xp 加算する経験値
/// @details mfxpの加算、レベル値の増加、親オブジェクトがプレイヤーならスキル選択フラグを立てる
void Level::AddXp(float xp)
{
	xp_ += xp;
	if (xp_ >= max_xp_)
	{
		now_level_++;
		xp_ = 0;
		SetNextLevel();
		// プレイヤーの成長演出を進めるため、レベルアップ直後のスキル選択フラグを有効化する
		if (parent_->GetTag() == Object3D::kTag3dPlayer)
		{
			auto player = dynamic_cast<Player3D*>(parent_);
			player->skill_->SetSkillFlag(true);
		}
	}
}
/// @brief レベル増加に必要な目標経験値を設定する
/// @details max_xp_ の更新（レベルが高くなるほど必要経験値が増加）
void Level::SetNextLevel()
{
	max_xp_ = 50.0f * now_level_;
}
/// @brief 経験値の進行割合を画面上の進捗バーメーターとして描画する
/// @details 進捗背景、バー進行、全長縁の描画
void Level::DrawBar()
{
	float bar = xp_ / max_xp_;
	if (bar > 1.0f) bar = 1.0f;
	if (bar < 0.0f) bar = 0.0f;
	int x1 = static_cast<int>(pos.x);
	int y1 = static_cast<int>(pos.y);
	int x2 = static_cast<int>(pos.x + size.x);
	int y2 = static_cast<int>(pos.y + size.y);

	// やわらかなクリーム色の背景（貿場の塗の格示イメージ）
	DrawBox(x1 - 2, y1 - 2, x2 + 2, y2 + 2, GetColor(170, 190, 150), TRUE);
	DrawBox(x1, y1, x2, y2, GetColor(235, 245, 220), TRUE);

	if (bar > 0.0f)
	{
		int fillX = static_cast<int>(pos.x + size.x * bar);

		// のひたる貿場の縫のような緑色の結格
		DrawBox(x1 + 1, y1 + 2, fillX - 1, y2 - 2, GetColor(100, 190, 130), TRUE);

		// ぶわっとやわらかな輝き
		SetDrawBlendMode(DX_BLENDMODE_ADD, 80);
		DrawBox(x1, y1 + 1, fillX, y2 - 1, GetColor(180, 255, 200), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	// ゆったりした縫の外各
	DrawBox(x1 - 2, y1 - 2, x2 + 2, y2 + 2, GetColor(100, 150, 80), FALSE);

	// 革をやわらかくする小さな館の学
	DrawBox(x1 - 2, y1 - 2, x1 + 3, y1 + 3, GetColor(80, 130, 60), TRUE);
	DrawBox(x2 - 2, y1 - 2, x2 + 2, y1 + 3, GetColor(80, 130, 60), TRUE);
	DrawBox(x1 - 2, y2 - 2, x1 + 3, y2 + 2, GetColor(80, 130, 60), TRUE);
	DrawBox(x2 - 2, y2 - 2, x2 + 2, y2 + 2, GetColor(80, 130, 60), TRUE);
}
