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
	, mfVerticalAngle(0.0f)
	, mfHorizontalAngle(0.0f)
	, mfSpeed(35.0f)
	, mfHp(0)
	, mfAttack_Speed(3.0f)
	, CatchNowCount(0)
{
	mIsStunned = false;
	mStunTimer = 0;
	mEffectTimer = 80;
	mfRadius = 100;

	SetTag(Object3D::Tag3D_player);

	mpModel = new Model(filename, initPos, false);
	mpLevel = new Level(this);
	mpLevel->SetNextLevel();
	mpSkill = new Skill(this);
	mpCombo = new Combo();
	mpScore = new Score();

	Master::mpCamera->Initialize();

	mnLighGraph = Master::mpResourceManager->LoadGraphics("Resource/2D/green_beam_transparent.png");

	mpCapsuleCollider->mvPosition = VGet(mvPosition.x, 0, mvPosition.z);
	mpCapsuleCollider->mvPosition2 = mvPosition;
	mpCapsuleCollider->mfRadius = mfRadius;

	// Y軸方向にスケールを伸ばし真下に向ける
	mpBeam = new EffekseerEffect("Resource/3D/EFK/Beam.efk", mvPosition, 80.0f);
	mpBeam->SetRotation(VGet(DX_PI_F / -2.0f, 0.0f, 0.0f));
	mpBeam->SetScale(VGet(1.0f, 1.0f, 4.0f));

	mpSpeed = new EffekseerEffect("Resource/3D/EFK/UseSpItem.efk", VGet(0, 0, 0), 100.0f);
}

Player3D::~Player3D()
{
	if (mpModel) { delete mpModel; mpModel = nullptr; }
	if (mpLevel) { delete mpLevel; mpLevel = nullptr; }
	if (mpSkill) { delete mpSkill; mpSkill = nullptr; }
	if (mpCombo) { delete mpCombo; mpCombo = nullptr; }
	if (mpScore) { delete mpScore; mpScore = nullptr; }
	if (mpBeam) { delete mpBeam;  mpBeam = nullptr; }
	if (mpSpeed) { delete mpSpeed; mpSpeed = nullptr; }
}

void Player3D::Update()
{
	// デバッグカメラが有効な場合はプレイヤーの挙動をすべて停止
	if (Master::mbIsDebugCamera) return;

	// --- スタン（気絶）状態の管理 ---
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

	// ステージ外落下時などは復帰のために空中へ退避
	if (Master::GameFinishFlag || mIsOutOfBounds)
	{
		mvPosition = VGet(0, 2000, 0);
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
		// 吸い込み判定が残るのを防ぐためコライダーをリセット
		mIsVacuumActive = false;
		ColliderUpdate();
	}

	// ステージ外に出ていないかのチェック
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

		// ラストスパート時はゲージ回復速度を上げて難易度を緩和する
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

	// フィーバー状態の仕様を満たすため強制発動
	if (Master::FeverFlag) mIsVacuumActive = true;
}

void Player3D::ColliderUpdate()
{
	if (mIsVacuumActive)
	{
		// 上空の牛まで判定が届くようにY軸方向にカプセルを広げる
		mpCapsuleCollider->mvPosition = VGet(mvPosition.x, -1000, mvPosition.z);
		mpCapsuleCollider->mvPosition2 = VGet(mvPosition.x, 3000, mvPosition.z);
		mpCapsuleCollider->mfRadius = VACUUM_RADIUS;

		mEffectTimer--;
		if (mEffectTimer <= 0)
		{
			if (mpBeam != nullptr)
			{
				mpBeam->Play();
			}
			mEffectTimer = 80;
		}
	}
	else
	{
		mpCapsuleCollider->mvPosition = VGet(mvPosition.x, -1000, mvPosition.z);
		mpCapsuleCollider->mvPosition2 = mvPosition;
		mpCapsuleCollider->mfRadius = 0.0f;

		if (mpBeam != nullptr && mpBeam->IsPlaying())
		{
			mpBeam->Stop();
		}
		mEffectTimer = 0;
	}

	// 各種エフェクトの座標追従と更新処理
	if (mpBeam != nullptr)
	{
		mpBeam->SetPosition(mvPosition);
		mpBeam->Update();
	}

	if (mpSpeed != nullptr)
	{
		mpSpeed->SetPosition(mvPosition);
		mpSpeed->Update();
	}
}

void Player3D::ScreenOutCheck()
{
	if (mvPosition.x > Utility::StageSize.x || mvPosition.x < -Utility::StageSize.x ||
		mvPosition.z > Utility::StageSize.z || mvPosition.z < -Utility::StageSize.z)
	{
		mIsOutOfBounds = true;

		// 進行不能回避のためキー入力で復帰させる
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
		mpSkill->SetSkillFlag(true); // スキルUI/効果の強制開放
	}
}

/*
 * @brief プレイヤーに紐づく各種サブシステムの一括描画・更新呼び出し
 */
void Player3D::ManagerUpdate()
{
	mpLevel->Draw();
	mpLevel->Update();
	mpModel->Update();
	mpModel->Draw();
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

	// ロックオン状態に応じてサークルの色を変更する
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

		VECTOR p1 = VAdd(mvPosition, VGet(cosf(angle1) * VACUUM_RADIUS, -mvPosition.y + 0.1f, sinf(angle1) * VACUUM_RADIUS));
		VECTOR p2 = VAdd(mvPosition, VGet(cosf(angle2) * VACUUM_RADIUS, -mvPosition.y + 0.1f, sinf(angle2) * VACUUM_RADIUS));

		DrawLine3D(p1, p2, color);
	}

	bar();
}

void Player3D::MoveEx()
{
	mvOldPosition = mvPosition;
	moveVec = VGet(0.0f, 0.0f, 0.0f);

	VECTOR UpMoveVector = VGet(0.0f, 0.0f, 0.0f);
	VECTOR leftMoveVector = VGet(0.0f, 0.0f, 0.0f);

	// カメラ視点を基準とした移動方向の算出
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
		mfTargetAngle = atan2f(moveVec.x, moveVec.z);
		oldmoveVec = moveVec;

		currentSpeed = Status(Status_Speed);
		mvPosition = VAdd(mvPosition, VScale(moveVec, currentSpeed));
	}

	bool hitwall = false;
	bool hitwalls = false;
	const auto& walls = ServiceLocator::GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Wall);
	if (!walls.empty())
	{
		for (int i = 0; i < walls.size(); i++)
		{
			Wall* wall = dynamic_cast<Wall*>(walls.at(i));
			if (wall != nullptr)
			{
				std::vector<VERTEX3D> vertex = wall->GetVertex();

				if (HitCheck_Capsule_Triangle(
					mvPosition, VAdd(mvPosition, VGet(0.0f, 200.0f, 0.0f)), 80.0f,
					vertex.at(0).pos, vertex.at(1).pos, vertex.at(2).pos) ||
					HitCheck_Capsule_Triangle(
						mvPosition, VAdd(mvPosition, VGet(0.0f, 200.0f, 0.0f)), 80.0f,
						vertex.at(3).pos, vertex.at(1).pos, vertex.at(2).pos)
					)
				{
					hitwall = true;
					VECTOR slide = VGet(0.0f, 0.0f, 0.0f);

					// 法線から壁ずり移動用ベクトルを計算
					float a = VDot(VScale(moveVec, -1.0f), vertex.at(0).norm);
					slide = VAdd(moveVec, VScale(vertex.at(0).norm, a));

					if (hitwall == true && hitwalls == false)
					{
						mvPosition = mvOldPosition;
						mvPosition = VAdd(mvPosition, VScale(slide, mfSpeed));
						hitwalls = true;
					}
					// 挟まり防止のため進行を制限する
					else if (hitwalls == true)
					{
						mvPosition = mvOldPosition;
					}
				}
			}
		}
	}

	mpModel->SetPosition(mvPosition);
	mpModel->SetRotation(mvRotation);
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
	float subAngle = mfTargetAngle - mfAngle;

	// 角度の境界線を跨いだ場合の最短ルート補正
	if (subAngle < -DX_PI_F) subAngle += DX_TWO_PI_F;
	if (subAngle > DX_PI_F)  subAngle -= DX_TWO_PI_F;

	if (subAngle > 0.0f)
	{
		subAngle -= ROTATE_SPEED;
		if (subAngle < 0.0f) subAngle = 0.0f;
	}
	else if (subAngle < 0.0f)
	{
		subAngle += ROTATE_SPEED;
		if (subAngle > 0.0f) subAngle = 0.0f;
	}

	mfAngle = mfTargetAngle - subAngle;

	mvRotation.y = mfAngle + DX_PI_F;
	mpModel->SetRotation(mvRotation);
}

/*
 * @brief 画面下部に表示する吸い込みゲージ（2D UI）の描画
 */
void Player3D::bar()
{
	int gaugeWidth = 300;
	int gaugeHeight = 50;
	int gaugeX = 20;
	int gaugeY = 700;

	DrawBox(gaugeX, gaugeY, gaugeX + gaugeWidth, gaugeY + gaugeHeight, GetColor(100, 100, 100), TRUE);

	int currentWidth = (int)((mVacuumGauge / VACUUM_GAUGE_MAX) * gaugeWidth);
	if (currentWidth < 0) currentWidth = 0;
	if (currentWidth > gaugeWidth) currentWidth = gaugeWidth;

	// ゲージが空の時は警告として赤色表示
	unsigned int gaugeColor = GetColor(0, 255, 255);
	if (mVacuumGauge <= 0.0f) gaugeColor = GetColor(255, 0, 0);

	DrawBox(gaugeX, gaugeY, gaugeX + currentWidth, gaugeY + gaugeHeight, gaugeColor, TRUE);

	DrawBox(gaugeX, gaugeY, gaugeX + gaugeWidth, gaugeY + gaugeHeight, GetColor(255, 255, 255), FALSE);
	DrawFormatString(gaugeX, gaugeY - 30, GetColor(255, 255, 255), "Vacuum Gauge: %.1f%%", mVacuumGauge);
}

void Player3D::OnEnter(Collider* collider, Collider* check)
{
	if (collider == mpCapsuleCollider && check->mpParentObject->GetTag() == Tag3D_Cow)
	{
		CowMove* cow = dynamic_cast<CowMove*>(check->mpParentObject);
		if (cow->GetCurrentState() != STATE_VACUUM)
		{
			cow->SetTargetPlayer(this);
			cow->IncreaseVacuumTimer();
			cow->ChangeStateToVacuum();
		}
	}

	if (collider == mpCapsuleCollider && check->mpParentObject->GetTag() == Tag3D_Animal)
	{
		AnimalMove* ani = dynamic_cast<AnimalMove*>(check->mpParentObject);
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
	// 吸い込み中断時は対象を地上での徘徊状態に戻す
	if (collider == mpCapsuleCollider && check->mpParentObject->GetTag() == Tag3D_Cow)
	{
		CowMove* cow = dynamic_cast<CowMove*>(check->mpParentObject);

		cow->SetTargetPlayer(nullptr);
		cow->ResetVacuumTimer();
		cow->SetCurrentState(STATE_WALK);
		cow->ChangeState(new StateWalk());

		VECTOR pos = cow->GetPosition();
		cow->SetPosition(pos);
	}

	if (collider == mpCapsuleCollider && check->mpParentObject->GetTag() == Tag3D_Animal)
	{
		AnimalMove* ani = dynamic_cast<AnimalMove*>(check->mpParentObject);
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
	mpModel->SetScale(scale);
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
