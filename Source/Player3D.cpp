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
	mpCombo = new Combo();
	mpScore = new Score();

	Master::mpCamera->Initialize();

	mnLighGraph = Master::mpResourceManager->LoadGraphics("Resource/2D/green_beam_transparent.png");
	mnGaugeFrameGraph = Master::mpResourceManager->LoadGraphics("Resource/2D/cow_frame.png");

	capsule_collider_->position_ = VGet(position_.x, 0, position_.z);
	capsule_collider_->position2_ = position_;
	capsule_collider_->radius_ = radius_;

	// Y霆ｸ譁ｹ蜷代↓繧ｹ繧ｱ繝ｼ繝ｫ繧剃ｼｸ縺ｰ縺礼悄荳九↓蜷代￠繧・
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
	Utility::SafeDelete(mpCombo);
	Utility::SafeDelete(mpScore);
	Utility::SafeDelete(mpBeam);
	Utility::SafeDelete(mpSpeed);
}

void Player3D::Update()
{
	// 繝・ヰ繝・げ繧ｫ繝｡繝ｩ縺梧怏蜉ｹ縺ｪ蝣ｴ蜷医・繝励Ξ繧､繝､繝ｼ縺ｮ謖吝虚繧偵☆縺ｹ縺ｦ蛛懈ｭ｢
	if (Master::mbIsDebugCamera) return;

	// --- 繧ｹ繧ｿ繝ｳ・域ｰ礼ｵｶ・臥憾諷九・邂｡逅・---
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

	// 繧ｹ繝・・繧ｸ螟冶誠荳区凾縺ｪ縺ｩ縺ｯ蠕ｩ蟶ｰ縺ｮ縺溘ａ縺ｫ遨ｺ荳ｭ縺ｸ騾驕ｿ
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
	// --- 繧ｹ繧ｿ繝ｳ荳ｭ縺ｮ繝輔か繝ｼ繝ｫ繝舌ャ繧ｯ ---
	else
	{
		// 蜷ｸ縺・ｾｼ縺ｿ蛻､螳壹′谿九ｋ縺ｮ繧帝亟縺舌◆繧√さ繝ｩ繧､繝繝ｼ繧偵Μ繧ｻ繝・ヨ
		mIsVacuumActive = false;
		ColliderUpdate();
	}

	// 繧ｹ繝・・繧ｸ螟悶↓蜃ｺ縺ｦ縺・↑縺・°縺ｮ繝√ぉ繝・け
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

		// 繝ｩ繧ｹ繝医せ繝代・繝域凾縺ｯ繧ｲ繝ｼ繧ｸ蝗槫ｾｩ騾溷ｺｦ繧剃ｸ翫￡縺ｦ髮｣譏灘ｺｦ繧堤ｷｩ蜥後☆繧・
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

	// 繝輔ぅ繝ｼ繝舌・迥ｶ諷九・莉墓ｧ倥ｒ貅縺溘☆縺溘ａ蠑ｷ蛻ｶ逋ｺ蜍・
	if (Master::FeverFlag) mIsVacuumActive = true;
}

void Player3D::ColliderUpdate()
{
	if (mIsVacuumActive)
	{
		// 荳顔ｩｺ縺ｮ迚帙∪縺ｧ蛻､螳壹′螻翫￥繧医≧縺ｫY霆ｸ譁ｹ蜷代↓繧ｫ繝励そ繝ｫ繧貞ｺ・￡繧・
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

	// 蜷・ｨｮ繧ｨ繝輔ぉ繧ｯ繝医・蠎ｧ讓呵ｿｽ蠕薙→譖ｴ譁ｰ蜃ｦ逅・
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

		// 騾ｲ陦御ｸ崎・蝗樣∩縺ｮ縺溘ａ繧ｭ繝ｼ蜈･蜉帙〒蠕ｩ蟶ｰ縺輔○繧・
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
		mpSkill->SetSkillFlag(true); // 繧ｹ繧ｭ繝ｫUI/蜉ｹ譫懊・蠑ｷ蛻ｶ髢区叛
	}
}

/*
 * @brief 繝励Ξ繧､繝､繝ｼ縺ｫ邏舌▼縺丞推遞ｮ繧ｵ繝悶す繧ｹ繝・Β縺ｮ荳諡ｬ謠冗判繝ｻ譖ｴ譁ｰ蜻ｼ縺ｳ蜃ｺ縺・
 */
void Player3D::ManagerUpdate()
{
	mpLevel->Draw();
	mpLevel->Update();
	model_->Update();
	model_->Draw();
	mpSkill->Update();
	mpSkill->Draw();
	mpCombo->Draw();
	mpCombo->Update();
	mpScore->Draw();
	test();
}

void Player3D::Draw()
{
	const int DIV = 32;
	unsigned int color;

	// 繝ｭ繝・け繧ｪ繝ｳ迥ｶ諷九↓蠢懊§縺ｦ繧ｵ繝ｼ繧ｯ繝ｫ縺ｮ濶ｲ繧貞､画峩縺吶ｋ
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

	// 繧ｫ繝｡繝ｩ隕也せ繧貞渕貅悶→縺励◆遘ｻ蜍墓婿蜷代・邂怜・
	{
		UpMoveVector = VSub(Master::mpCamera->GetLookAtPosition(), Master::mpCamera->GetPosition());
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

		currentSpeed = Status(Status_Speed);
		position_ = VAdd(position_, VScale(moveVec, currentSpeed));
	}

	bool hitwall = false;
	bool hitwalls = false;
	const auto& walls = ServiceLocator::GetObjectManager()->GetObject3DListByTag(Object3D::kTag3dWall);
	if (!walls.empty())
	{
		for (int i = 0; i < walls.size(); i++)
		{
			Wall* wall = dynamic_cast<Wall*>(walls.at(i));
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

					// 豕慕ｷ壹°繧牙｣√★繧顔ｧｻ蜍慕畑繝吶け繝医Ν繧定ｨ育ｮ・
					float a = VDot(VScale(moveVec, -1.0f), vertex.at(0).norm);
					slide = VAdd(moveVec, VScale(vertex.at(0).norm, a));

					if (hitwall == true && hitwalls == false)
					{
						position_ = old_position_;
						position_ = VAdd(position_, VScale(slide, mfSpeed));
						hitwalls = true;
					}
					// 謖溘∪繧企亟豁｢縺ｮ縺溘ａ騾ｲ陦後ｒ蛻ｶ髯舌☆繧・
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
		return mfAttack_Speed + mpSkill->GetStatusDate(Skill::Status_AttackSpeed);
	}
	if (id == Status_Hp)
	{
		return mfHp;
	}
	if (id == Status_Speed)
	{
		return mfSpeed + mpSkill->GetStatusDate(Skill::Status_Speed);
	}
	return 0.0f;
}

void Player3D::RotationByMove()
{
	float subAngle = target_angle_ - angle_;

	// 隗貞ｺｦ縺ｮ蠅・阜邱壹ｒ霍ｨ縺・□蝣ｴ蜷医・譛遏ｭ繝ｫ繝ｼ繝郁｣懈ｭ｣
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
 * @brief 逕ｻ髱｢荳矩Κ縺ｫ陦ｨ遉ｺ縺吶ｋ蜷ｸ縺・ｾｼ縺ｿ繧ｲ繝ｼ繧ｸ・・D UI・峨・謠冗判
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

	// 繧ｲ繝ｼ繧ｸ縺檎ｩｺ縺ｮ譎ゅ・隴ｦ蜻翫→縺励※襍､濶ｲ陦ｨ遉ｺ
	unsigned int gaugeColor = GetColor(0, 255, 255);
	if (mVacuumGauge <= 0.0f) gaugeColor = GetColor(255, 0, 0);

	DrawBox(gaugeX, gaugeY, gaugeX + currentWidth, gaugeY + gaugeHeight, gaugeColor, TRUE);

	// 迚帶氛繝輔Ξ繝ｼ繝繧呈緒逕ｻ・亥・縺ｮ逋ｽ縺・棧邱・DrawBox 縺ｮ莉｣繧上ｊ・・
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
	// 蜷ｸ縺・ｾｼ縺ｿ荳ｭ譁ｭ譎ゅ・蟇ｾ雎｡繧貞慍荳翫〒縺ｮ蠕伜ｾ顔憾諷九↓謌ｻ縺・
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