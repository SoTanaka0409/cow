#include "Boss.h"
#include "Model.h"
#include "Master.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "Player3D.h"

Boss::Boss(std::string filename, VECTOR initPos)
	: Object3D(initPos)
	, count(0)
	, time(300)
{
	SetFontSize(50);
	auto p = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_player);
	Player3D* player = dynamic_cast<Player3D*>(p);
	mpModel = new Model(filename, initPos, true);
	mpModel->SetScale(20.0f);
	tag = StepTag::Step_Chat;

	if (player != nullptr)
	{
		MaxDamage = player->mpScore->GetScore();
	}
	else
	{
		MaxDamage = 1000.0f; // フォールバック初期値
	}
	mfHp = MaxDamage;
	mfMaxHp = MaxDamage;
}

Boss::~Boss()
{
	if (mpModel != nullptr)
	{
		delete mpModel;
		mpModel = nullptr;
	}
}

void Boss::Draw()
{
	mpModel->Draw();
}

void Boss::Update()
{
	mpModel->Update();
	
	if (tag == StepTag::Step_Chat)
	{
		count++;
		if (count >= time)
		{
			tag = StepTag::Step_Damage;
			count = 0;
		}
	}
	else if (tag == StepTag::Step_Damage)
	{
		if (MaxDamage >= 0.0f)
		{
			MaxDamage -= 1.0f;
			mfHp -= 1.0f;
		}
		else
		{
			tag = StepTag::Step_Finish;
			count = 0;
		}
	}
	Death();
}

void Boss::Death()
{
	if (tag == StepTag::Step_Finish)
	{
		DrawBox(0, 0, 2000, 150, GetColor(0, 0, 0), true);
		
		count++;
		if (count >= time)
		{
			Master::mpSceneManager->GetCurrentScene()->mpGameManager->GameNextStep(GameManager::game_final);
		}
	}
}

void Boss::Bar()
{
	VECTOR DrawHpBar3D = VAdd(mvPosition, VGet(0.0f, 250.0f, 0.0f));
	VECTOR DrawHpBarWorld = ConvWorldPosToScreenPos(DrawHpBar3D);

	int BarWidth = 400;
	int BarHeight = 40;

	if (mfHp <= 0.0f) mfHp = 0.0f;
	
	float HpBar = 0.0f;
	if (mfMaxHp > 0.0f)
	{
		HpBar = mfHp / mfMaxHp;
	}

	int HpBarX = DrawHpBarWorld.x - BarWidth / 2;
	int HpBarY = DrawHpBarWorld.y - BarHeight / 2;

	if (mfHp > 0.0f)
	{
		DrawBox(HpBarX, HpBarY, HpBarX + BarWidth, HpBarY + BarHeight, GetColor(255, 255, 255), FALSE);
		DrawBox(HpBarX, HpBarY, HpBarX + (int)(BarWidth * HpBar), HpBarY + BarHeight, GetColor(0, 255, 0), TRUE);
	}
}
