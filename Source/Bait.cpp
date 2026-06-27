#include "ServiceLocator.h"
#include "Bait.h"
#include "Model.h"
#include "CapsuleCollider.h"
#include "GameTimer.h"
#include "CowMove.h"
#include "AnimalMove.h"
#include "Master.h"
#include "SceneManager.h"
#include "ObjectManager.h"

Bait::Bait(std::string filename, VECTOR pos)
	: Object3D(pos)
{
	model = new Model(filename, pos, false);
	gameTimer = new GameTimer(pos, 5, GameTimer::Tag_NoGame);
	SetTag(Tag3D_Bait);
	
	capsuleCollider->mvPosition = mvPosition;
	capsuleCollider->mvPosition2 = VAdd(mvPosition, VGet(0.0f, 100.0f, 0.0f));
	capsuleCollider->radius = 1500.0f;
}

Bait::~Bait()
{
	if (model != nullptr)
	{
		delete model;
		model = nullptr;
	}
	if (gameTimer != nullptr)
	{
		delete gameTimer;
		gameTimer = nullptr;
	}
}

void Bait::Draw()
{
	model->Draw();
}

void Bait::Update()
{
	model->Update();
	gameTimer->Update();

	if (mvPosition.y >= 0.0f)
	{
		mvPosition.y -= 40.0f;
	}
	capsuleCollider->mvPosition = mvPosition;
	capsuleCollider->mvPosition2 = VAdd(mvPosition, VGet(0.0f, 100.0f, 0.0f));

	if (gameTimer->OutTimerFlag())
	{
		const auto& c = ServiceLocator::GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Cow);
		for (int i = 0; i < c.size(); i++)
		{
			CowMove* cow = dynamic_cast<CowMove*>(c.at(i));
			if (cow != nullptr && cow->GetBaitFlag())
			{
				cow->KilledByBait();
				Master::soundManager->PlaySE(SoundManager::SE_BAITFINAL);
			}
		}

		SetDeleteFlag(true);
		capsuleCollider->SetDeleteFlag(true);
	}
	model->SetPosition(mvPosition);
}

void Bait::OnEnter(Collider* collider, Collider* check)
{
	if (collider == capsuleCollider && check->parentObject != nullptr)
	{
		if (check->parentObject->GetTag() == Tag3D_Cow)
		{
			CowMove* cow = dynamic_cast<CowMove*>(check->parentObject);
			if (gameTimer->OutTimerFlag() && cow != nullptr)
			{
				cow->KilledByBait();
			}
		}
	}
}
