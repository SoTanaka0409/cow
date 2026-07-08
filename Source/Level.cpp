#include"Level.h"
#include"Master.h"
#include"Player3D.h"

Level::Level(Object3D* obj)
	: mfxp(0.0f)
	, mfMaxXp(1.0f)
	, mnNowLevel(1)
{
	this->mpParent = obj;
}

Level::~Level()
{
}

void Level::Draw()
{
	DrawBar();
	DrawFormatString(Utility::kUiBaseX, Utility::kUiLevelY - 35, GetColor(255, 255, 255), "Level : %d", mnNowLevel);
}

void Level::Update()
{
}

/*
 * @brief 経験値を加算し、最大値に達した場合のレベルアップ（およびスキル選択要求）を行う
 * [入力] xp: 加算する経験値
 * [出力] なし
 * [備考] mfxpの加算、レベル値の増加、親オブジェクトがプレイヤーならスキル選択フラグを立てる
 */
void Level::AddXp(float xp)
{
	mfxp += xp;

	if (mfxp >= mfMaxXp)
	{
		mnNowLevel++;
		mfxp = 0;
		SetNextLevel();
		
		// プレイヤーの成長演出を進めるため、レベルアップ直後のスキル選択フラグを有効化する
		if (mpParent->GetTag() == Object3D::kTag3dPlayer)
		{
			auto player = dynamic_cast<Player3D*>(mpParent);
			player->mpSkill->SetSkillFlag(true);
		}
	}
}

/*
 * @brief レベル増加に必要な目標経験値を設定する
 * [入力] なし
 * [出力] なし
 * [備考] mfMaxXp の更新（レベルが高くなるほど必要経験値が増加）
 */
void Level::SetNextLevel()
{
	mfMaxXp = 50.0f * mnNowLevel;
}

/*
 * @brief 経験値の進行割合を画面上の進捗バーメーターとして描画する
 * [入力] なし
 * [出力] なし
 * [備考] 進捗背景、バー進行、全長縁の描画
 */
void Level::DrawBar()
{
	float bar = mfxp / mfMaxXp;
	if (bar > 1.0f) bar = 1.0f;
	if (bar < 0.0f) bar = 0.0f;

	// 視覚幅上端のため背景を黒で塗りつぶす
	DrawBox(static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(pos.x + size.x), static_cast<int>(pos.y + size.y), GetColor(0, 0, 0), TRUE);
	
	// 進行度を表示するため割合に応じて緑色のバーを一定長させる
	if (bar > 0.0f)
	{
		DrawBox(static_cast<int>(pos.x + 1), static_cast<int>(pos.y + 1),
			static_cast<int>(pos.x + size.x * bar - 1),
			static_cast<int>(pos.y + size.y - 1),
			GetColor(0, 255, 0), TRUE);
	}
	
	// バーの全長縁を表示するための枠線を描画
	DrawBox(static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(pos.x + size.x), static_cast<int>(pos.y + size.y), GetColor(255, 255, 255), FALSE);
}