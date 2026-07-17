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
#include <cstring>
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

/*
 * プレイヤーの初期化処理
 * [入力] filename: モデルのファイルパス, initPos: 初期座標
 * [出力] なし
 * [副作用] 各種コンポーネントの生成と初期化
 */
Player3D::Player3D(std::string filename, VECTOR initPos)
	: Object3D(initPos)
	, vertical_angle_(0.0f)
	, horizontal_angle_(0.0f)
	, speed_(35.0f)
	, hp_(0)
	, attack_speed_(3.0f)
	, CatchNowCount(0)
	
{
	mIsStunned = false;
	mStunTimer = 0;
	effect_timer_ = 80;
	radius_ = 100;

	SetTag(Object3D::kTag3dPlayer);

	model_ = new Model(filename, initPos, false);
	level_manager_ = new Level(this);
	level_manager_->SetNextLevel();
	skill_ = new Skill(this);
	combo_ = new Combo();
	score_manager_ = new Score();

	Master::camera_->Initialize();

	light_graph_ = Master::resource_manager_->LoadGraphics("Resource/2D/PlayerCaptureBeam.png");
	gauge_frame_graph_ = Master::resource_manager_->LoadGraphics("Resource/2D/CowGaugeFrame.png");

	capsule_collider_->position_ = VGet(position_.x, 0, position_.z);
	capsule_collider_->position2_ = position_;
	capsule_collider_->radius_ = radius_;

	// プレイヤーから真下に向けて光線を出す表現にするため
	beam_ = new EffekseerEffect("Resource/3D/EFK/Beam.efk", position_, 80.0f);
	beam_->SetRotation(VGet(DX_PI_F / -2.0f, 0.0f, 0.0f));
	beam_->SetScale(VGet(1.0f, 1.0f, 4.0f));

	mpSpeed = new EffekseerEffect("Resource/3D/EFK/UseSpItem.efk", VGet(0, 0, 0), 100.0f);
}

/*
 * プレイヤーの終了処理
 * [入力] なし
 * [出力] なし
 * [副作用] 各種コンポーネントの破棄
 */
Player3D::~Player3D()
{
	Utility::SafeDelete(model_);
	Utility::SafeDelete(level_manager_);
	Utility::SafeDelete(skill_);
	Utility::SafeDelete(combo_);
	Utility::SafeDelete(score_manager_);
	Utility::SafeDelete(beam_);
	Utility::SafeDelete(mpSpeed);
}

/*
 * 毎フレームの更新処理
 * [入力] なし
 * [出力] なし
 * [副作用] 状態更新、移動処理、当たり判定処理の実行
 */
void Player3D::Update()
{
	// デバッグ時はカメラ操作に専念させるため
	if (Master::is_debug_camera_) return;

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

	// 落下時は安全な空中へ移動させ、画面外への永続的な逸脱を防ぐ
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
	else
	{
		// スタン中は意図しない吸い込みが発生しないようにする
		mIsVacuumActive = false;
		ColliderUpdate();
	}

	ScreenOutCheck();
}

/*
 * プレイヤーの入力処理と吸い込みゲージの管理
 * [入力] なし
 * [出力] なし
 * [副作用] mIsVacuumActiveとmVacuumGaugeの更新
 */
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

		// ラストスパート時はゲージ回復速度を上げ、プレイヤーを有利にする
		if (Master::scene_manager_ && Master::scene_manager_->GetCurrentScene() && ServiceLocator::GetGameManager())
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

	// フィーバー中は無制限に吸い込みを可能にするため
	if (Master::FeverFlag) mIsVacuumActive = true;
}



/*
 * コライダーとエフェクトの更新
 * [入力] なし
 * [出力] なし
 * [副作用] カプセルコライダーのサイズ変更、エフェクトの再生・停止
 */
void Player3D::ColliderUpdate()
{
	if (mIsVacuumActive)
	{
		// 空中の対象も吸い込めるように判定を上に伸ばす
		capsule_collider_->position_ = VGet(position_.x, -1000, position_.z);
		capsule_collider_->position2_ = VGet(position_.x, 3000, position_.z);
		capsule_collider_->radius_ = VACUUM_RADIUS;

		effect_timer_--;
		if (effect_timer_ <= 0)
		{
			if (beam_ != nullptr)
			{
				beam_->Play();
			}
			effect_timer_ = 80;
		}
	}
	else
	{
		capsule_collider_->position_ = VGet(position_.x, -1000, position_.z);
		capsule_collider_->position2_ = position_;
		capsule_collider_->radius_ = 0.0f;

		if (beam_ != nullptr && beam_->IsPlaying())
		{
			beam_->Stop();
		}
		effect_timer_ = 0;
	}

	if (beam_ != nullptr)
	{
		beam_->SetPosition(position_);
		beam_->Update();
	}

	if (mpSpeed != nullptr)
	{
		mpSpeed->SetPosition(position_);
		mpSpeed->Update();
	}
}

/*
 * 画面外への逸脱判定と復帰処理
 * [入力] なし
 * [出力] なし
 * [副作用] mIsOutOfBoundsの更新、座標のリセット
 */
void Player3D::ScreenOutCheck()
{
	if (position_.x > Utility::StageSize.x || position_.x < -Utility::StageSize.x ||
		position_.z > Utility::StageSize.z || position_.z < -Utility::StageSize.z)
	{
		mIsOutOfBounds = true;

		// 画面外にスタックした場合、手動で復帰できるようにする
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

/*
 * テスト機能の実行
 * [入力] なし
 * [出力] なし
 * [副作用] 経験値の追加、スキルUIの開放
 */
void Player3D::test()
{
	if (InputManager::CheckDownKey(KEY_INPUT_5))
	{
		level_manager_->AddXp(20);
		skill_->SetSkillFlag(true);
	}
}

/*
 * サブシステムの更新・描画の統括
 * [入力] なし
 * [出力] なし
 * [副作用] 各マネージャーオブジェクトの更新・描画
 */
void Player3D::ManagerUpdate()
{
	level_manager_->Draw();
	level_manager_->Update();
	model_->Update();
	skill_->Update();
	skill_->Draw();
	combo_->Draw();
	combo_->Update();
	score_manager_->Draw();
	test();
}

/*
 * プレイヤー固有の描画処理
 * [入力] なし
 * [出力] なし
 * [副作用] サークルやUIの画面出力
 */
void Player3D::Draw()
{
	if (model_ != nullptr)
	{
		model_->Draw();
	}

	const int DIV = 32;
	unsigned int color;

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

void Player3D::DrawShadowCaster()
{
	if (model_ != nullptr)
	{
		model_->Draw();
	}
}

/*
 * 移動処理と壁との衝突判定
 * [入力] なし
 * [出力] なし
 * [副作用] 座標の更新、移動ベクトルの計算
 */
void Player3D::MoveEx()
{
	old_position_ = position_;
	moveVec = VGet(0.0f, 0.0f, 0.0f);

	VECTOR UpMoveVector = VGet(0.0f, 0.0f, 0.0f);
	VECTOR leftMoveVector = VGet(0.0f, 0.0f, 0.0f);

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

		currentSpeed = Status(kStatusSpeed) * Master::GetDeltaTimeScaler();
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

					float a = VDot(VScale(moveVec, -1.0f), vertex.at(0).norm);
					slide = VAdd(moveVec, VScale(vertex.at(0).norm, a));

					if (hitwall == true && hitwalls == false)
					{
						position_ = old_position_;
						position_ = VAdd(position_, VScale(slide, speed_ * Master::GetDeltaTimeScaler()));
						hitwalls = true;
					}
					// 壁へのめり込みや挟まりを防ぐため、移動をキャンセルする
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

/*
 * 指定されたステータス値の取得
 * [入力] id: 取得したいステータスID
 * [出力] スキル補正を含めたステータス値
 * [副作用] なし
 */
float Player3D::Status(StatusID id)
{
	if (id == Status_AttackS)
	{
		return attack_speed_ + skill_->GetStatusDate(Skill::kStatusAttackSpeed);
	}
	if (id == Status_Hp)
	{
		return hp_;
	}
	if (id == kStatusSpeed)
	{
		return speed_ + skill_->GetStatusDate(Skill::kStatusSpeed);
	}
	return 0.0f;
}

/*
 * 移動方向への回転補間
 * [入力] なし
 * [出力] なし
 * [副作用] プレイヤーモデルの回転角度更新
 */
void Player3D::RotationByMove()
{
	float subAngle = target_angle_ - angle_;

	// 回転が不自然に逆回りしないように、最短ルートへ補正する
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
 * 吸い込みゲージのUI描画
 * [入力] なし
 * [出力] なし
 * [副作用] 画面上へのゲージ表示
 */
void Player3D::bar()
{
	int gaugeWidth = Utility::kUiVacuumW;
	int gaugeHeight = Utility::kUiVacuumH;
	int gaugeX = Utility::kUiBaseX;
	int gaugeY = Utility::kUiVacuumY;

	int x1 = gaugeX;
	int y1 = gaugeY;
	int x2 = gaugeX + gaugeWidth;
	int y2 = gaugeY + gaugeHeight;

	// --- Western Wood Background ---
	// 木目調の暗い茶色
	DrawBox(x1, y1, x2, y2, GetColor(60, 30, 15), TRUE);

	int currentWidth = (int)((mVacuumGauge / VACUUM_GAUGE_MAX) * gaugeWidth);
	if (currentWidth < 0) currentWidth = 0;

	
	if (currentWidth > 0)
	{
		int fillX = x1 + currentWidth;
		
		unsigned int coreColor = GetColor(200, 255, 255);
		unsigned int glowColor = GetColor(0, 255, 255);
		if (mVacuumGauge <= 0.0f) 
		{
			coreColor = GetColor(255, 100, 100);
			glowColor = GetColor(255, 0, 0);
		}

		// 内側の明るい芯（トラクタービームの中心）
		DrawBox(x1 + 2, y1 + 4, fillX - 2, y2 - 4, coreColor, TRUE);
		
		// 外側の発光（加算ブレンド）
		SetDrawBlendMode(DX_BLENDMODE_ADD, 160);
		DrawBox(x1, y1 + 1, fillX, y2 - 1, glowColor, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}


	// 紫色のエイリアン・ルーン風枠線
	DrawBox(x1 - 1, y1 - 1, x2 + 1, y2 + 1, GetColor(200, 0, 255), FALSE);
	
	// コーナーのシアンのアクセント（SF的なジョイント）
	DrawBox(x1 - 2, y1 - 2, x1 + 4, y1 + 4, GetColor(0, 255, 255), TRUE);
	DrawBox(x2 - 4, y1 - 2, x2 + 2, y1 + 4, GetColor(0, 255, 255), TRUE);
	DrawBox(x1 - 2, y2 - 4, x1 + 4, y2 + 2, GetColor(0, 255, 255), TRUE);
	DrawBox(x2 - 4, y2 - 4, x2 + 2, y2 + 2, GetColor(0, 255, 255), TRUE);

	// SF風テキスト表示（シアン色で表示）
	DrawFormatString(gaugeX, gaugeY - 30, GetColor(0, 255, 255), "TRACTOR BEAM: %.1f%%", mVacuumGauge);
}

/*
 * オブジェクトが範囲内に入った時の処理
 * [入力] collider: 自身のコライダー, check: 相手のコライダー
 * [出力] なし
 * [副作用] 対象オブジェクトの吸い込み状態への移行
 */
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

/*
 * オブジェクトが範囲外に出た時の処理
 * [入力] collider: 自身のコライダー, check: 相手のコライダー
 * [出力] なし
 * [副作用] 対象オブジェクトの通常状態への復帰
 */
void Player3D::OnExit(Collider* collider, Collider* check)
{
	// 吸い込み対象から外れた場合、通常の挙動へ復帰させる
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

/*
 * 範囲内にとどまっているオブジェクトへの処理
 * [入力] collider: 自身のコライダー, check: 相手のコライダー
 * [出力] なし
 * [副作用] なし
 */
void Player3D::OnTrigger(Collider* collider, Collider* check)
{
}

/*
 * スケールの設定
 * [入力] scale: 変更する倍率
 * [出力] なし
 * [副作用] モデルサイズの変更
 */
void Player3D::SetScale(float scale)
{
	model_->SetScale(scale);
}

/*
 * スタン状態の適用
 * [入力] stunTime: スタンさせるフレーム数
 * [出力] なし
 * [副作用] 状態異常フラグとタイマーのセット
 */
void Player3D::ApplyStun(int stunTime)
{
	mIsStunned = true;
	mStunTimer = stunTime;
}

/*
 * スキルエフェクトの再生
 * [入力] なし
 * [出力] なし
 * [副作用] エフェクトの再生開始
 */
void Player3D::PlaySkillEffect()
{
	if (mpSpeed != nullptr)
	{
		mpSpeed->Play();
	}
}
