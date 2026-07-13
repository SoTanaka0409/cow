#include "ServiceLocator.h"
#include"Player3D.h"
#include"Model.h"
#include"ModelAnimation.h"
#include"Master.h"
#include"InputManager.h"
#include"SceneManager.h"
#include"ObjectManager.h"
#include"Scene3D.h"
#include"Utility.h"
#include<string>
#include <iostream>
#include <cstring> // strcmp, strncmp
#include"stage.h"
#include"Camera.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include"Level.h"
#include"CowMove.h"
#include"Skill.h"
#include"Wall.h"
#include"Combo.h"
#include"Score.h"
#include"AnimalMove.h"
#include"CharacterState.h"
#include"GameTimer.h"
#include"GameManager.h"

Player3D::Player3D(std::string filename, VECTOR initPos)
	: Object3D(initPos)
	, vertical_angle_(0.0f)
	, horizontal_angle_(0.0f)
	, mfSpeed(35.0f)
	, mfHp(0)
	, mfAttack_Speed(3.0f)
	, CatchNowCount(0)
{
	mIsStunned = false;
	mStunTimer = 0;
	effect_timer_ = 80;
	radius_ = 100;

	SetTag(Object3D::kTag3dPlayer);

	model_ = new Model(filename, initPos, false);
	mpLevel = new Level(this);
	mpLevel->SetNextLevel();
	mpSkill = new Skill(this);
	combo_ = new Combo();
	mpScore = new Score();

	Master::camera_->Initialize();

	mnLighGraph = Master::mpResourceManager->LoadGraphics("Resource/2D/green_beam_transparent.png");
	mnGaugeFrameGraph = Master::mpResourceManager->LoadGraphics("Resource/2D/cow_frame.png");

	capsule_collider_->position_ = VGet(position_.x, 0, position_.z);
	capsule_collider_->position2_ = position_;
	capsule_collider_->radius_ = radius_;

	// Y軸方向にスケールを伸ばし真下に向けめE
	mpBeam = new EffekseerEffect("Resource/3D/EFK/Beam.efk", position_, 80.0f);
	mpBeam->SetRotation(VGet(DX_PI_F / -2.0f, 0.0f, 0.0f));
	mpBeam->SetScale(VGet(1.0f, 1.0f, 4.0f));

	mpSpeed = new EffekseerEffect("Resource/3D/EFK/UseSpItem.efk", VGet(0, 0, 0), 100.0f);
}

Player3D::~Player3D()
{
	Utility::SafeDelete(model_);
	Utility::SafeDelete(mpLevel);
	Utility::SafeDelete(mpSkill);
	Utility::SafeDelete(combo_);
	Utility::SafeDelete(mpScore);
	Utility::SafeDelete(mpBeam);
	Utility::SafeDelete(mpSpeed);
}

void Player3D::Update()
{
	// チE??チE??カメラが有効な場合?Eプレイヤーの挙動をすべて停止
	if (Master::mbIsDebugCamera) return;

	// --- スタン?E?気絶?E?状態?E管?E---
	if (mIsStunned)
	{
		mStunTimer--;
		if (mStunTimer <= 0)
		{
			mIsStunned = false;
		}
	}

	mIsCowInVacuumRange = false;

	ManagerUpdate();

	// スチE?Eジ外落下時などは復帰のために空中へ退避
	if (Master::GameFinishFlag || mIsOutOfBounds)
	{
		position_ = VGet(0, 2000, 0);
		return;
	}

	if (!mIsStunned)
	{
		MoveEx();
		RotationByMove();
		ColliderUpdate();
		Play();
	}
	// --- スタン中のフォールバック ---
	else
	{
		// 吸ぁE??み判定が残るのを防ぐためコライダーをリセチE??
		mIsVacuumActive = false;
		ColliderUpdate();
	}

	// スチE?Eジ外に出てぁE??ぁE??のチェチE??
	ScreenOutCheck();
}

void Player3D::Play()
{
	int mouseInput = GetMouseInput();

	if ((mouseInput & MOUSE_INPUT_LEFT) && mVacuumGauge > 0.0f && !Master::FeverFlag)
	{
		mIsVacuumActive = true;
		mVacuumGauge -= VACUUM_COST_PER_FRAME;

		if (mVacuumGauge < 0.0f) mVacuumGauge = 0.0f;
	}
	else
	{
		mIsVacuumActive = false;

		float recoverySpeed = VACUUM_RECOVER_PER_FRAME;

		// ラストスパ?Eト時はゲージ回復速度を上げて難易度を緩和すめE
		if (Master::mpSceneManager && Master::mpSceneManager->GetCurrentScene() && ServiceLocator::GetGameManager())
		{
			auto timer = ServiceLocator::GetGameManager()->GetGameTimer();
			if (timer && timer->GetTime() <= 60)
			{
				recoverySpeed *= 2.0f;
			}
		}

		mVacuumGauge += recoverySpeed;
		if (mVacuumGauge > VACUUM_GAUGE_MAX) mVacuumGauge = VACUUM_GAUGE_MAX;
	}

	// フィーバ?E状態?E仕様を満たすため強制発勁E
	if (Master::FeverFlag) mIsVacuumActive = true;
}

void Player3D::ColliderUpdate()
{
	if (mIsVacuumActive)
	{
		// 上空の牛まで判定が届くようにY軸方向にカプセルを庁E??めE
		capsule_collider_->position_ = VGet(position_.x, -1000, position_.z);
		capsule_collider_->position2_ = VGet(position_.x, 3000, position_.z);
		capsule_collider_->radius_ = VACUUM_RADIUS;

		effect_timer_--;
		if (effect_timer_ <= 0)
		{
			if (mpBeam != nullptr)
			{
				mpBeam->Play();
			}
			effect_timer_ = 80;
		}
	}
	else
	{
		capsule_collider_->position_ = VGet(position_.x, -1000, position_.z);
		capsule_collider_->position2_ = position_;
		capsule_collider_->radius_ = 0.0f;

		if (mpBeam != nullptr && mpBeam->IsPlaying())
		{
			mpBeam->Stop();
		}
		effect_timer_ = 0;
	}

	// 吁E??エフェクト?E座標追従と更新処?E
	if (mpBeam != nullptr)
	{
		mpBeam->SetPosition(position_);
		mpBeam->Update();
	}

	if (mpSpeed != nullptr)
	{
		mpSpeed->SetPosition(position_);
		mpSpeed->Update();
	}
}

void Player3D::ScreenOutCheck()
{
	if (position_.x > Utility::StageSize.x || position_.x < -Utility::StageSize.x ||
		position_.z > Utility::StageSize.z || position_.z < -Utility::StageSize.z)
	{
		mIsOutOfBounds = true;

		// 進行不?E回避のためキー入力で復帰させめE
		if (CheckHitKey(KEY_INPUT_SPACE))
		{
			SetPosition(VGet(0, 2000, 0));
			mIsOutOfBounds = false;
		}
	}
	else
	{
		mIsOutOfBounds = false;
	}
}

void Player3D::test()
{
	if (InputManager::CheckDownKey(KEY_INPUT_5))
	{
		mpLevel->AddXp(20);
		mpSkill->SetSkillFlag(true); // スキルUI/効果?E強制開放
	}
}

/*
 * @brief プレイヤーに紐づく各種サブシスチE??の一括描画・更新呼び出ぁE
 */
void Player3D::ManagerUpdate()
{
	mpLevel->Draw();
	mpLevel->Update();
	model_->Update();
	model_->Draw();
	mpSkill->Update();
	mpSkill->Draw();
	combo_->Draw();
	combo_->Update();
	mpScore->Draw();
	test();
}

void Player3D::Draw()
{
	const int DIV = 32;
	unsigned int color;

	// ロチE??オン状態に応じてサークルの色を変更する
	if (mIsCowInVacuumRange == true)
	{
		color = GetColor(255, 0, 0);
	}
	else
	{
		color = GetColor(0, 255, 0);
	}

	for (int i = 0; i < DIV; i++) {
		float angle1 = (float)i / DIV * DX_PI_F * 2.0f;
		float angle2 = (float)(i + 1) / DIV * DX_PI_F * 2.0f;

		VECTOR p1 = VAdd(position_, VGet(cosf(angle1) * VACUUM_RADIUS, -position_.y + 0.1f, sinf(angle1) * VACUUM_RADIUS));
		VECTOR p2 = VAdd(position_, VGet(cosf(angle2) * VACUUM_RADIUS, -position_.y + 0.1f, sinf(angle2) * VACUUM_RADIUS));

		DrawLine3D(p1, p2, color);
	}

	bar();
}

void Player3D::MoveEx()
{
	old_position_ = position_;
	moveVec = VGet(0.0f, 0.0f, 0.0f);

	VECTOR UpMoveVector = VGet(0.0f, 0.0f, 0.0f);
	VECTOR leftMoveVector = VGet(0.0f, 0.0f, 0.0f);

	// カメラ視点を基準とした移動方向?E算?E
	{
		UpMoveVector = VSub(Master::camera_->GetLookAtPosition(), Master::camera_->GetPosition());
		UpMoveVector.y = 0.0f;

		leftMoveVector = VCross(UpMoveVector, VGet(0.0f, 1.0f, 0.0f));
		leftMoveVector.y = 0.0f;

		UpMoveVector = VNorm(UpMoveVector);
		leftMoveVector = VNorm(leftMoveVector);
	}

	if (CheckHitKey(KEY_INPUT_A)) moveVec = VAdd(moveVec, leftMoveVector);
	if (CheckHitKey(KEY_INPUT_D)) moveVec = VAdd(moveVec, VScale(leftMoveVector, -1.0f));
	if (CheckHitKey(KEY_INPUT_W)) moveVec = VAdd(moveVec, UpMoveVector);
	if (CheckHitKey(KEY_INPUT_S)) moveVec = VAdd(moveVec, VScale(UpMoveVector, -1.0f));

	bool isMove = (moveVec.x != 0.0f || moveVec.z != 0.0f);
	if (isMove)
	{
		moveVec = VNorm(moveVec);
		target_angle_ = atan2f(moveVec.x, moveVec.z);
		oldmoveVec = moveVec;

		currentSpeed = Status(kStatusSpeed);
		position_ = VAdd(position_, VScale(moveVec, currentSpeed));
	}

	bool hitwall = false;
	bool hitwalls = false;
	const auto& walls = ServiceLocator::GetObjectManager()->GetObject3DListByTag(Object3D::kTag3dWall);
	if (!walls.empty())
	{
		for (auto& w : walls)
		{
			Wall* wall = dynamic_cast<Wall*>(w);
			if (wall != nullptr)
			{
				std::vector<VERTEX3D> vertex = wall->GetVertex();

				if (HitCheck_Capsule_Triangle(
					position_, VAdd(position_, VGet(0.0f, 200.0f, 0.0f)), 80.0f,
					vertex.at(0).pos, vertex.at(1).pos, vertex.at(2).pos) ||
					HitCheck_Capsule_Triangle(
						position_, VAdd(position_, VGet(0.0f, 200.0f, 0.0f)), 80.0f,
						vertex.at(3).pos, vertex.at(1).pos, vertex.at(2).pos)
					)
				{
					hitwall = true;
					VECTOR slide = VGet(0.0f, 0.0f, 0.0f);

					// 法線から壁ずり移動用ベクトルを計?E
					float a = VDot(VScale(moveVec, -1.0f), vertex.at(0).norm);
					slide = VAdd(moveVec, VScale(vertex.at(0).norm, a));

					if (hitwall == true && hitwalls == false)
					{
						position_ = old_position_;
						position_ = VAdd(position_, VScale(slide, mfSpeed));
						hitwalls = true;
					}
					// 挟まり防止のため進行を制限すめE
					else if (hitwalls == true)
					{
						position_ = old_position_;
					}
				}
			}
		}
	}

	model_->SetPosition(position_);
	model_->SetRotation(rotation_);
}

float Player3D::Status(StatusID id)
{
	if (id == Status_AttackS)
	{
		return mfAttack_Speed + mpSkill->GetStatusDate(Skill::kStatusAttackSpeed);
	}
	if (id == Status_Hp)
	{
		return mfHp;
	}
	if (id == kStatusSpeed)
	{
		return mfSpeed + mpSkill->GetStatusDate(Skill::kStatusSpeed);
	}
	return 0.0f;
}

void Player3D::RotationByMove()
{
	float subAngle = target_angle_ - angle_;

	// 角度の?E??線を跨ぁE??場合?E最短ルート補正
	if (subAngle < -DX_PI_F) subAngle += DX_TWO_PI_F;
	if (subAngle > DX_PI_F)  subAngle -= DX_TWO_PI_F;

	if (subAngle > 0.0f)
	{
		subAngle -= kRotateSpeed;
		if (subAngle < 0.0f) subAngle = 0.0f;
	}
	else if (subAngle < 0.0f)
	{
		subAngle += kRotateSpeed;
		if (subAngle > 0.0f) subAngle = 0.0f;
	}

	angle_ = target_angle_ - subAngle;

	rotation_.y = angle_ + DX_PI_F;
	model_->SetRotation(rotation_);
}

/*
 * @brief 画面下部に表示する吸ぁE??みゲージ?E?ED UI?E??E描画
 */
void Player3D::bar()
{
	int gaugeWidth = Utility::kUiVacuumW;
	int gaugeHeight = Utility::kUiVacuumH;
	int gaugeX = Utility::kUiBaseX;
	int gaugeY = Utility::kUiVacuumY;

	DrawBox(gaugeX, gaugeY, gaugeX + gaugeWidth, gaugeY + gaugeHeight, GetColor(100, 100, 100), TRUE);

	int currentWidth = (int)((mVacuumGauge / VACUUM_GAUGE_MAX) * gaugeWidth);
	if (currentWidth < 0) currentWidth = 0;
	if (currentWidth > gaugeWidth) currentWidth = gaugeWidth;

	// ゲージが空の時?E警告として赤色表示
	unsigned int gaugeColor = GetColor(0, 255, 255);
	if (mVacuumGauge <= 0.0f) gaugeColor = GetColor(255, 0, 0);

	DrawBox(gaugeX, gaugeY, gaugeX + currentWidth, gaugeY + gaugeHeight, gaugeColor, TRUE);

	// 牛柄フレームを描画?E??Eの白ぁE???EDrawBox の代わり?E?E
	DrawExtendGraph(gaugeX - 10, gaugeY - 10, gaugeX + gaugeWidth + 10, gaugeY + gaugeHeight + 10, mnGaugeFrameGraph, TRUE);

	DrawFormatString(gaugeX, gaugeY - 30, GetColor(255, 255, 255), "Vacuum Gauge: %.1f%%", mVacuumGauge);
}

void Player3D::OnEnter(Collider* collider, Collider* check)
{
	if (collider == capsule_collider_ && check->parent_object_->GetTag() == kTag3dCow)
	{
		CowMove* cow = dynamic_cast<CowMove*>(check->parent_object_);
		if (cow->GetCurrentState() != STATE_VACUUM)
		{
			cow->SetTargetPlayer(this);
			cow->IncreaseVacuumTimer();
			cow->ChangeStateToVacuum();
		}
	}

	if (collider == capsule_collider_ && check->parent_object_->GetTag() == kTag3dAnimal)
	{
		AnimalMove* ani = dynamic_cast<AnimalMove*>(check->parent_object_);
		if (ani->GetCurrentState() != STATE_VACUUM)
		{
			ani->SetTargetPlayer(this);
			ani->IncreaseVacuumTimer();
			ani->ChangeStateToVacuum();
		}
	}
}

void Player3D::OnExit(Collider* collider, Collider* check)
{
	// 吸ぁE??み中断時?E対象を地上での徘徊状態に戻ぁE
	if (collider == capsule_collider_ && check->parent_object_->GetTag() == kTag3dCow)
	{
		CowMove* cow = dynamic_cast<CowMove*>(check->parent_object_);

		cow->SetTargetPlayer(nullptr);
		cow->ResetVacuumTimer();
		cow->SetCurrentState(STATE_WALK);
		cow->ChangeState(new StateWalk());

		VECTOR pos = cow->GetPosition();
		cow->SetPosition(pos);
	}

	if (collider == capsule_collider_ && check->parent_object_->GetTag() == kTag3dAnimal)
	{
		AnimalMove* ani = dynamic_cast<AnimalMove*>(check->parent_object_);
		ani->SetTargetPlayer(nullptr);
		ani->SetCurrentState(STATE_WALK);
		ani->ChangeState(new StateWalk());
		ani->SetPosition(ani->GetPosition());
	}
}

void Player3D::OnTrigger(Collider* collider, Collider* check)
{
}

void Player3D::SetScale(float scale)
{
	model_->SetScale(scale);
}

void Player3D::ApplyStun(int stunTime)
{
	mIsStunned = true;
	mStunTimer = stunTime;
}

void Player3D::PlaySkillEffect()
{
	if (mpSpeed != nullptr)
	{
		mpSpeed->Play();
	}
}