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
	, verticalAngle(0.0f)
	, horizontalAngle(0.0f)
	, speed(35.0f)
	, hp(0)
	, attack_Speed(3.0f)
	, catchNowCount(0)
{
	isStunned = false;
	stunTimer = 0;
	effectTimer = 80;
	radius = 100;

	SetTag(Object3D::Tag3D_player);

	model = new Model(filename, initPos, false);
	level = new Level(this);
	level->SetNextLevel();
	skill = new Skill(this);
	combo = new Combo();
	score = new Score();

	Master::camera->Initialize();

	lighGraph = LoadGraph("Resource/2D/green_beam_transparent.png");

	capsuleCollider->mvPosition = VGet(mvPosition.x, 0, mvPosition.z);
	capsuleCollider->mvPosition2 = mvPosition;
	capsuleCollider->radius = radius;

	// Y軸方向にスケールを伸ばし真下に向ける
	beam = new EffekseerEffect("Resource/3D/EFK/Beam.efk", mvPosition, 80.0f);
	beam->SetRotation(VGet(DX_PI_F / -2.0f, 0.0f, 0.0f));
	beam->SetScale(VGet(1.0f, 1.0f, 4.0f));

	speedEffect = new EffekseerEffect("Resource/3D/EFK/UseSpItem.efk", VGet(0, 0, 0), 100.0f);
}

Player3D::~Player3D()
{
	if (model) { delete model; model = nullptr; }
	if (level) { delete level; level = nullptr; }
	if (skill) { delete skill; skill = nullptr; }
	if (combo) { delete combo; combo = nullptr; }
	if (score) { delete score; score = nullptr; }
	if (beam) { delete beam;  beam = nullptr; }
	if (speedEffect) { delete speedEffect; speedEffect = nullptr; }
}

void Player3D::Update()
{
	// デバッグカメラが有効な場合はプレイヤーの挙動をすべて停止
	if (Master::isDebugCamera) return;

	// --- スタン（気絶）状態の管理 ---
	if (isStunned)
	{
		stunTimer--;
		if (stunTimer <= 0)
		{
			isStunned = false;
		}
	}

	isCowInVacuumRange = false;

	ManagerUpdate();

	// ステージ外落下時などは復帰のために空中へ退避
	if (Master::gameFinishFlag || isOutOfBounds)
	{
		mvPosition = VGet(0, 2000, 0);
		return;
	}

	if (!isStunned)
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
		isVacuumActive = false;
		ColliderUpdate();
	}

	// ステージ外に出ていないかのチェック
	ScreenOutCheck();
}

void Player3D::Play()
{
	int mouseInput = GetMouseInput();

	if ((mouseInput & MOUSE_INPUT_LEFT) && vacuumGauge > 0.0f && !Master::feverFlag)
	{
		isVacuumActive = true;
		vacuumGauge -= VACUUM_COST_PER_FRAME;

		if (vacuumGauge < 0.0f) vacuumGauge = 0.0f;
	}
	else
	{
		isVacuumActive = false;

		float recoverySpeed = VACUUM_RECOVER_PER_FRAME;

		// ラストスパート時はゲージ回復速度を上げて難易度を緩和する
		if (Master::sceneManager && Master::sceneManager->GetCurrentScene() && ServiceLocator::GetGameManager())
		{
			auto timer = ServiceLocator::GetGameManager()->GetGameTimer();
			if (timer && timer->GetTime() <= 60)
			{
				recoverySpeed *= 2.0f;
			}
		}

		vacuumGauge += recoverySpeed;
		if (vacuumGauge > VACUUM_GAUGE_MAX) vacuumGauge = VACUUM_GAUGE_MAX;
	}

	// フィーバー状態の仕様を満たすため強制発動
	if (Master::feverFlag) isVacuumActive = true;
}

void Player3D::ColliderUpdate()
{
	if (isVacuumActive)
	{
		// 上空の牛まで判定が届くようにY軸方向にカプセルを広げる
		capsuleCollider->mvPosition = VGet(mvPosition.x, -1000, mvPosition.z);
		capsuleCollider->mvPosition2 = VGet(mvPosition.x, 3000, mvPosition.z);
		capsuleCollider->radius = VACUUM_RADIUS;

		effectTimer--;
		if (effectTimer <= 0)
		{
			if (beam != nullptr)
			{
				beam->Play();
			}
			effectTimer = 80;
		}
	}
	else
	{
		capsuleCollider->mvPosition = VGet(mvPosition.x, -1000, mvPosition.z);
		capsuleCollider->mvPosition2 = mvPosition;
		capsuleCollider->radius = 0.0f;

		if (beam != nullptr && beam->IsPlaying())
		{
			beam->Stop();
		}
		effectTimer = 0;
	}

	// 各種エフェクトの座標追従と更新処理
	if (beam != nullptr)
	{
		beam->SetPosition(mvPosition);
		beam->Update();
	}

	if (speedEffect != nullptr)
	{
		speedEffect->SetPosition(mvPosition);
		speedEffect->Update();
	}
}

void Player3D::ScreenOutCheck()
{
	if (mvPosition.x > Utility::StageSize.x || mvPosition.x < -Utility::StageSize.x ||
		mvPosition.z > Utility::StageSize.z || mvPosition.z < -Utility::StageSize.z)
	{
		isOutOfBounds = true;

		// 進行不能回避のためキー入力で復帰させる
		if (CheckHitKey(KEY_INPUT_SPACE))
		{
			SetPosition(VGet(0, 2000, 0));
			isOutOfBounds = false;
		}
	}
	else
	{
		isOutOfBounds = false;
	}
}

void Player3D::test()
{
	if (InputManager::CheckDownKey(KEY_INPUT_5))
	{
		level->AddXp(20);
		skill->SetSkillFlag(true); // スキルUI/効果の強制開放
	}
}

/*
 * @brief プレイヤーに紐づく各種サブシステムの一括描画・更新呼び出し
 */
void Player3D::ManagerUpdate()
{
	level->Draw();
	level->Update();
	model->Update();
	model->Draw();
	skill->Update();
	skill->Draw();
	combo->Draw();
	combo->Update();
	score->Draw();
	test();
}

void Player3D::Draw()
{
	const int DIV = 32;
	unsigned int color;

	// ロックオン状態に応じてサークルの色を変更する
	if (isCowInVacuumRange == true)
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
		UpMoveVector = VSub(Master::camera->GetLookAtPosition(), Master::camera->GetPosition());
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
		targetAngle = atan2f(moveVec.x, moveVec.z);
		oldmoveVec = moveVec;

		currentSpeed = Status(Status_Speed);
		mvPosition = VAdd(mvPosition, VScale(moveVec, currentSpeed * Master::GetDeltaTimeScaler()));
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
						mvPosition = VAdd(mvPosition, VScale(slide, speed * Master::GetDeltaTimeScaler()));
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

	model->SetPosition(mvPosition);
	model->SetRotation(mvRotation);
}

float Player3D::Status(StatusID id)
{
	if (id == Status_AttackS)
	{
		return attack_Speed + skill->GetStatusDate(Skill::Status_AttackSpeed);
	}
	if (id == Status_Hp)
	{
		return hp;
	}
	if (id == Status_Speed)
	{
		return speed + skill->GetStatusDate(Skill::Status_Speed);
	}
	return 0.0f;
}

void Player3D::RotationByMove()
{
	float subAngle = targetAngle - angle;

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

	angle = targetAngle - subAngle;

	mvRotation.y = angle + DX_PI_F;
	model->SetRotation(mvRotation);
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

	int currentWidth = (int)((vacuumGauge / VACUUM_GAUGE_MAX) * gaugeWidth);
	if (currentWidth < 0) currentWidth = 0;
	if (currentWidth > gaugeWidth) currentWidth = gaugeWidth;

	// ゲージが空の時は警告として赤色表示
	unsigned int gaugeColor = GetColor(0, 255, 255);
	if (vacuumGauge <= 0.0f) gaugeColor = GetColor(255, 0, 0);

	DrawBox(gaugeX, gaugeY, gaugeX + currentWidth, gaugeY + gaugeHeight, gaugeColor, TRUE);

	DrawBox(gaugeX, gaugeY, gaugeX + gaugeWidth, gaugeY + gaugeHeight, GetColor(255, 255, 255), FALSE);
	DrawFormatString(gaugeX, gaugeY - 30, GetColor(255, 255, 255), "Vacuum Gauge: %.1f%%", vacuumGauge);
}

void Player3D::OnEnter(Collider* collider, Collider* check)
{
	if (collider == capsuleCollider && check->parentObject->GetTag() == Tag3D_Cow)
	{
		CowMove* cow = dynamic_cast<CowMove*>(check->parentObject);
		if (cow->GetCurrentState() != STATE_VACUUM)
		{
			cow->SetTargetPlayer(this);
			cow->IncreaseVacuumTimer();
			cow->ChangeStateToVacuum();
		}
	}

	if (collider == capsuleCollider && check->parentObject->GetTag() == Tag3D_Animal)
	{
		AnimalMove* ani = dynamic_cast<AnimalMove*>(check->parentObject);
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
	if (collider == capsuleCollider && check->parentObject->GetTag() == Tag3D_Cow)
	{
		CowMove* cow = dynamic_cast<CowMove*>(check->parentObject);

		cow->SetTargetPlayer(nullptr);
		cow->ResetVacuumTimer();
		cow->SetCurrentState(STATE_WALK);
		cow->ChangeState(new StateWalk());

		VECTOR pos = cow->GetPosition();
		cow->SetPosition(pos);
	}

	if (collider == capsuleCollider && check->parentObject->GetTag() == Tag3D_Animal)
	{
		AnimalMove* ani = dynamic_cast<AnimalMove*>(check->parentObject);
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
	model->SetScale(scale);
}

void Player3D::ApplyStun(int stunTime)
{
	isStunned = true;
	stunTimer = stunTime;
}

void Player3D::PlaySkillEffect()
{
	if (speedEffect != nullptr)
	{
		speedEffect->Play();
	}
}
