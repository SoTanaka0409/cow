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
	mpModel = new Model(filename, pos, false);
	mpGameTimer = new GameTimer(pos, 5, GameTimer::Tag_NoGame);
	SetTag(Tag3D_Bait);
	
	mpCapsuleCollider->mvPosition = mvPosition;
	mpCapsuleCollider->mvPosition2 = VAdd(mvPosition, VGet(0.0f, 100.0f, 0.0f));
	mpCapsuleCollider->mfRadius = 1500.0f;
}

Bait::~Bait()
{
	if (mpModel != nullptr)
	{
		delete mpModel;
		mpModel = nullptr;
	}
	if (mpGameTimer != nullptr)
	{
		delete mpGameTimer;
		mpGameTimer = nullptr;
	}
}

void Bait::Draw()
{
	mpModel->Draw();
}

void Bait::Update()
{
	mpModel->Update();
	mpGameTimer->Update();

	if (mvPosition.y >= 0.0f)
	{
		mvPosition.y -= 40.0f;
	}
	mpCapsuleCollider->mvPosition = mvPosition;
	mpCapsuleCollider->mvPosition2 = VAdd(mvPosition, VGet(0.0f, 100.0f, 0.0f));

	if (mpGameTimer->OutTimerFlag())
	{
		const auto& c = ServiceLocator::GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Cow);
		for (int i = 0; i < c.size(); i++)
		{
			CowMove* cow = dynamic_cast<CowMove*>(c.at(i));
			if (cow != nullptr && cow->GetBaitFlag())
			{
				cow->KilledByBait();
				Master::mpSoundManager->PlaySE(SoundManager::SE_BAITFINAL);
			}
		}

		SetDeleteFlag(true);
		mpCapsuleCollider->SetDeleteFlag(true);
	}
	mpModel->SetPosition(mvPosition);
}

void Bait::OnEnter(Collider* collider, Collider* check)
{
	if (collider == mpCapsuleCollider && check->mpParentObject != nullptr)
	{
		if (check->mpParentObject->GetTag() == Tag3D_Cow)
		{
			CowMove* cow = dynamic_cast<CowMove*>(check->mpParentObject);
			if (mpGameTimer->OutTimerFlag() && cow != nullptr)
			{
				cow->KilledByBait();
			}
		}
	}
}
