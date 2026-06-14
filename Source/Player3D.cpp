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
#include"GameTimer.h"
#include"GameManager.h"

/*
 * @brief コンストラクタ
 * @param filename モデルファイルのパス
 * @param initPos 初期配置座標
 */
Player3D::Player3D(std::string filename, VECTOR initPos)
	: Object3D(initPos)
	, mfVerticalAngle(0.0f)
	, mfHorizontalAngle(0.0f)
	, mfSpeed(35.0f)        // 基本移動速度
	, mfHp(0)
	, mfAttack_Speed(6.0f)  // 吸い込み（上昇）速度の基本値
	, CatchNowCount(0)
{
	// 状態フラグ・タイマーの初期化
	mIsStunned = false;
	mStunTimer = 0;
	mEffectTimer = 80;
	mfRadius = 100;

	// タグの設定
	SetTag(Object3D::Tag3D_player);

	// 各種コンポーネント・管理クラスの生成
	mpModel = new Model(filename, initPos, false);
	mpLevel = new Level(this);
	mpLevel->SetNextLevel();
	mpSkill = new Skill(this);
	mpCombo = new Combo();
	mpScore = new Score();

	// カメラの初期化
	Master::mpCamera->Initialize();

	// UI用グラフィックの読み込み
	mnLighGraph = LoadGraph("Resource/2D/green_beam_transparent.png");

	// 初期コライダーの設定（プレイヤーの足元から自身の位置を結ぶカプセル）
	mpCapsuleCollider->mvPosition = VGet(mvPosition.x, 0, mvPosition.z);
	mpCapsuleCollider->mvPosition2 = mvPosition;
	mpCapsuleCollider->mfRadius = mfRadius;

	// 吸い込みビームエフェクト（Effekseer）の設定
	mpBeam = new EffekseerEffect("Resource/3D/EFK/Beam.efk", mvPosition, 80.0f);
	mpBeam->SetRotation(VGet(DX_PI_F / -2.0f, 0.0f, 0.0f)); // 真下を向くように回転補正
	mpBeam->SetScale(VGet(1.0f, 1.0f, 4.0f));             // 縦長にスケールを伸ばす

	// スピードアップアイテム使用時のエフェクト設定
	mpSpeed = new EffekseerEffect("Resource/3D/EFK/UseSpItem.efk", VGet(0, 0, 0), 100.0f);
}

/*
 * @brief デストラクタ
 */
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

/*
 * @brief 更新処理
 */
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

	// 範囲内検知フラグを毎フレームリセット
	mIsCowInVacuumRange = false;

	// サブマネージャー・各UI関連の更新処理
	ManagerUpdate();

	// ゲーム終了、またはステージ外落下時の位置リセット（暫定で空中へ退避）
	if (Master::GameFinishFlag || mIsOutOfBounds)
	{
		mvPosition = VGet(0, 2000, 0);
		return;
	}

	// --- 通常時（スタンしていない場合）の行動更新 ---
	if (!mIsStunned)
	{
		MoveEx();          // 移動入力と壁衝突
		RotationByMove();  // 移動方向への旋回
		ColliderUpdate();  // 吸い込み範囲・エフェクト同期
		Play();            // マウス入力による吸い込み制御
	}
	// --- スタン中のフォールバック ---
	else
	{
		// スタン中の場合でも、コライダーの位置と大きさを非アクティブ状態（通常状態）に更新する必要がある。
		// そうしないと、吸い込み中にスタンした場合に、巨大な吸い込み判定（コライダー）がその場に残ったままになってしまう。
		mIsVacuumActive = false;
		ColliderUpdate();
	}

	// ステージ外に出ていないかのチェック
	ScreenOutCheck();
}

/*
 * @brief 吸い込みボタン（マウス入力）とゲージの増減管理
 */
void Player3D::Play()
{
	int mouseInput = GetMouseInput();

	// 左クリック長押し、かつゲージが残っており、フィーバータイム中でない場合
	if ((mouseInput & MOUSE_INPUT_LEFT) && mVacuumGauge > 0.0f && !Master::FeverFlag)
	{
		mIsVacuumActive = true;
		mVacuumGauge -= VACUUM_COST_PER_FRAME; // ゲージ消費

		if (mVacuumGauge < 0.0f) mVacuumGauge = 0.0f;
	}
	// ボタンを離している、またはゲージが空の場合（自動回復）
	else
	{
		mIsVacuumActive = false;

		float recoverySpeed = VACUUM_RECOVER_PER_FRAME;

		// ラストスパート処理：残り時間が60秒以下ならゲージの回復速度が2倍になる
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

	// フィーバータイム中はゲージに関係なく強制的に吸い込みが常時発動する
	if (Master::FeverFlag) mIsVacuumActive = true;
}

/*
 * @brief 吸い込み発動状態に応じたコライダー変形とエフェクトの同期
 */
void Player3D::ColliderUpdate()
{
	// --- 吸い込み発動中：コライダーを超巨大化 ---
	if (mIsVacuumActive)
	{
		// Y軸方向に大きな縦長のカプセルを作り、上空の牛まで判定が届くようにする
		mpCapsuleCollider->mvPosition = VGet(mvPosition.x, -1000, mvPosition.z);
		mpCapsuleCollider->mvPosition2 = VGet(mvPosition.x, 3000, mvPosition.z);
		mpCapsuleCollider->mfRadius = VACUUM_RADIUS; // 定数で定められた吸い込み半径へ拡張

		// 一定周期（80フレームごと）でEffekseerのビームエフェクトをループ再生
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
	// --- 通常時：吸い込み判定をリセット ---
	else
	{
		mpCapsuleCollider->mvPosition = VGet(mvPosition.x, -1000, mvPosition.z);
		mpCapsuleCollider->mvPosition2 = mvPosition;
		mpCapsuleCollider->mfRadius = 0.0f; // 半径を0にして接触判定を消す

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

/*
 * @brief ステージ外（画面外）への侵入チェックと復帰処理
 */
void Player3D::ScreenOutCheck()
{
	if (mvPosition.x > Utility::StageSize.x || mvPosition.x < -Utility::StageSize.x ||
		mvPosition.z > Utility::StageSize.z || mvPosition.z < -Utility::StageSize.z)
	{
		mIsOutOfBounds = true;

		// 救済措置：画面外でスペースキーを押すと中央上空にリスポーンする
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
 * @brief デバッグ用入力テスト
 */
void Player3D::test()
{
	if (InputManager::CheckDownKey(KEY_INPUT_5))
	{
		mpLevel->AddXp(20);         // 経験値付与
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

/*
 * @brief 描画（3D空間上の補助UI描画など）
 */
void Player3D::Draw()
{
	// --- 地面に吸い込み範囲を示すサークル（円）を描画する処理 ---
	const int DIV = 32; // 円の分割数（大きくするほど滑らかな真円になる）
	unsigned int color;

	// 範囲内にターゲット（牛）を捉えているかで円の色を変化させる
	if (mIsCowInVacuumRange == true)
	{
		color = GetColor(255, 0, 0); // 赤色（ロックオン状態）
	}
	else
	{
		color = GetColor(0, 255, 0); // 緑色（通常状態）
	}

	// 32分割した点をDrawLine3Dで順につなぎ、プレイヤーの足元（Y=0付近）に円を描く
	for (int i = 0; i < DIV; i++) {
		float angle1 = (float)i / DIV * DX_PI_F * 2.0f;
		float angle2 = (float)(i + 1) / DIV * DX_PI_F * 2.0f;

		VECTOR p1 = VAdd(mvPosition, VGet(cosf(angle1) * VACUUM_RADIUS, -mvPosition.y + 0.1f, sinf(angle1) * VACUUM_RADIUS));
		VECTOR p2 = VAdd(mvPosition, VGet(cosf(angle2) * VACUUM_RADIUS, -mvPosition.y + 0.1f, sinf(angle2) * VACUUM_RADIUS));

		DrawLine3D(p1, p2, color);
	}

	// 吸い込みゲージUI（2Dバー）の描画
	bar();
}

/*
 * @brief 移動入力処理（カメラの向きを基準にした3D移動 ＆ 壁とのスライド衝突判定）
 */
void Player3D::MoveEx()
{
	mvOldPosition = mvPosition; // 衝突前の現在座標を保存
	moveVec = VGet(0.0f, 0.0f, 0.0f);

	VECTOR UpMoveVector = VGet(0.0f, 0.0f, 0.0f);   // カメラ基準の前方ベクトル
	VECTOR leftMoveVector = VGet(0.0f, 0.0f, 0.0f); // カメラ基準の左方向ベクトル

	// --- カメラの視線ベクトルから移動方向を算出 ---
	{
		// 注視点からカメラ位置を引き算してカメラの向き（視線）を取得、大地の水平移動にするためYをリセット
		UpMoveVector = VSub(Master::mpCamera->GetLookAtPosition(), Master::mpCamera->GetPosition());
		UpMoveVector.y = 0.0f;

		// 前方ベクトルと世界のY上向きベクトルの外積（クロス積）をとり、直交する左向きベクトルを算出
		leftMoveVector = VCross(UpMoveVector, VGet(0.0f, 1.0f, 0.0f));
		leftMoveVector.y = 0.0f;

		// 単位ベクトル化（長さを1にする）
		UpMoveVector = VNorm(UpMoveVector);
		leftMoveVector = VNorm(leftMoveVector);
	}

	// WASDキー入力に応じて移動ベクトルを合成
	if (CheckHitKey(KEY_INPUT_A)) moveVec = VAdd(moveVec, leftMoveVector);
	if (CheckHitKey(KEY_INPUT_D)) moveVec = VAdd(moveVec, VScale(leftMoveVector, -1.0f));
	if (CheckHitKey(KEY_INPUT_W)) moveVec = VAdd(moveVec, UpMoveVector);
	if (CheckHitKey(KEY_INPUT_S)) moveVec = VAdd(moveVec, VScale(UpMoveVector, -1.0f));

	// いずれかのキーが押されて移動が発生している場合
	bool isMove = (moveVec.x != 0.0f || moveVec.z != 0.0f);
	if (isMove)
	{
		moveVec = VNorm(moveVec);
		mfTargetAngle = atan2f(moveVec.x, moveVec.z); // 移動ベクトルから目標の回転角度（ラジアン）を算出
		oldmoveVec = moveVec;

		// パッシブスキル等の上昇値を加味した現在の最終移動速度を取得して移動
		currentSpeed = Status(Status_Speed);
		mvPosition = VAdd(mvPosition, VScale(moveVec, currentSpeed));
	}

	// --- ポリゴン壁（Wall）との衝突判定およびすべり（壁ずり）処理 ---
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

				// プレイヤーのカプセルと壁（2つの三角形ポリゴン）の当たり判定
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

					// 法線との内積を用いて壁に沿って進むすべりベクトルを計算
					float a = VDot(VScale(moveVec, -1.0f), vertex.at(0).norm);
					slide = VAdd(moveVec, VScale(vertex.at(0).norm, a));

					// 1枚目の壁に衝突：位置を戻してから壁伝いの方向に速度を適用
					if (hitwall == true && hitwalls == false)
					{
						mvPosition = mvOldPosition;
						mvPosition = VAdd(mvPosition, VScale(slide, mfSpeed));
						hitwalls = true;
					}
					// 複数の壁の隙間に挟まれた場合：完全に進行を制限して埋まりを防止
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

/*
 * @brief スキル加算値を含めたプレイヤーの動的ステータス取得
 * @param id 取得したいステータスの種類（攻撃速度 / HP / 移動速度）
 */
float Player3D::Status(StatusID id)
{
	if (id == Status_AttackS)
	{
		// 基礎吸い込み速度 + スキルによる上昇値
		return mfAttack_Speed + mpSkill->GetStatusDate(Skill::Status_AttackSpeed);
	}
	if (id == Status_Hp)
	{
		return mfHp;
	}
	if (id == Status_Speed)
	{
		// 基礎移動速度 + スキルによる上昇値
		return mfSpeed + mpSkill->GetStatusDate(Skill::Status_Speed);
	}
	return 0.0f;
}

/*
 * @brief 移動方向へ滑らかにキャラクターの向きを旋回させる処理（線形補間風）
 */
void Player3D::RotationByMove()
{
	// 現在の角度と移動入力から求めた目標角度の差分を算出
	float subAngle = mfTargetAngle - mfAngle;

	// 角度の不連続点（-180度?180度の境界線）を跨いだ場合の最短ルート補正
	if (subAngle < -DX_PI_F) subAngle += DX_TWO_PI_F;
	if (subAngle > DX_PI_F)  subAngle -= DX_TWO_PI_F;

	// 毎フレーム一定の定数（ROTATE_SPEED）ずつ目標角度へと近づける
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

	// 補正された角度を適用
	mfAngle = mfTargetAngle - subAngle;

	// モデルの初期方向に合わせて180度（DX_PI_F）反転して反映
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
	int gaugeX = 50;
	int gaugeY = 700;

	// ゲージの背景（グレーの座布団）
	DrawBox(gaugeX, gaugeY, gaugeX + gaugeWidth, gaugeY + gaugeHeight, GetColor(100, 100, 100), TRUE);

	// 現在のゲージ量に応じた前面バーの横幅を計算
	int currentWidth = (int)((mVacuumGauge / VACUUM_GAUGE_MAX) * gaugeWidth);
	if (currentWidth < 0) currentWidth = 0;
	if (currentWidth > gaugeWidth) currentWidth = gaugeWidth;

	// 通常はシアン、ゲージが空の時は注意喚起のため赤色にする
	unsigned int gaugeColor = GetColor(0, 255, 255);
	if (mVacuumGauge <= 0.0f) gaugeColor = GetColor(255, 0, 0);

	DrawBox(gaugeX, gaugeY, gaugeX + currentWidth, gaugeY + gaugeHeight, gaugeColor, TRUE);

	// 白い外枠の描画とテキストによるパーセンテージ表記
	DrawBox(gaugeX, gaugeY, gaugeX + gaugeWidth, gaugeY + gaugeHeight, GetColor(255, 255, 255), FALSE);
	DrawFormatString(gaugeX, gaugeY - 30, GetColor(255, 255, 255), "Vacuum Gauge: %.1f%%", mVacuumGauge);
}

/*
 * @brief コライダー接触開始時のコールバック（牛やその他動物の吸引開始）
 */
void Player3D::OnEnter(Collider* collider, Collider* check)
{
	// --- 「牛」のコライダーがプレイヤーの吸い込み範囲に入った場合 ---
	if (collider == mpCapsuleCollider && check->mpParentObject->GetTag() == Tag3D_Cow)
	{
		CowMove* cow = dynamic_cast<CowMove*>(check->mpParentObject);
		if (cow->GetCurrentState() != STATE_VACUUM)
		{
			cow->IncreaseVacuumTimer();
			cow->ChangeStateToVacuum(); // 対象を吸引状態（上昇・回転）へ移行させる
		}
	}
	// --- 「その他動物」のコライダーがプレイヤーの吸い込み範囲に入った場合 ---
	if (collider == mpCapsuleCollider && check->mpParentObject->GetTag() == Tag3D_Animal)
	{
		AnimalMove* ani = dynamic_cast<AnimalMove*>(check->mpParentObject);
		if (ani->GetCurrentState() != STATE_VACUUM)
		{
			ani->IncreaseVacuumTimer();
			ani->ChangeStateToVacuum();
		}
	}
}

/*
 * @brief コライダー接触終了時のコールバック（吸引範囲外に逃げられた、またはボタンを離した際のクリーンアップ）
 */
void Player3D::OnExit(Collider* collider, Collider* check)
{
	// --- 牛が範囲外に出た、または吸い込みを中断した場合：地上へ戻す ---
	if (collider == mpCapsuleCollider && check->mpParentObject->GetTag() == Tag3D_Cow)
	{
		CowMove* cow = dynamic_cast<CowMove*>(check->mpParentObject);

		cow->ResetVacuumTimer();            // 蓄積タイマーをリセット
		cow->SetCurrentState(STATE_WALK); // 状態を通常の徘徊歩行に戻す

		VECTOR pos = cow->GetPosition();   // 現在の座標を取得（※必要に応じてY座標を接地させる処理を誘発）
		cow->SetPosition(pos);
	}
	// --- その他動物が範囲外に出た、または吸い込みを中断した場合 ---
	if (collider == mpCapsuleCollider && check->mpParentObject->GetTag() == Tag3D_Animal)
	{
		AnimalMove* ani = dynamic_cast<AnimalMove*>(check->mpParentObject);
		ani->SetCurrentState(STATE_WALK);
		ani->SetPosition(ani->GetPosition());
	}
}

/*
 * @brief コライダー接触継続中のコールバック（現在は未使用）
 */
void Player3D::OnTrigger(Collider* collider, Collider* check)
{
}

/*
 * @brief モデルのスケールを設定
 */
void Player3D::SetScale(float scale)
{
	mpModel->SetScale(scale);
}

/*
 * @brief 外部（敵の攻撃など）からスタン状態を付与される処理
 * @param stunTime スタンが持続するフレーム数
 */
void Player3D::ApplyStun(int stunTime)
{
	mIsStunned = true;
	mStunTimer = stunTime;
}

/*
 * @brief 移動速度アップなどのスキル発動時にエフェクトを再生する
 */
void Player3D::PlaySkillEffect()
{
	if (mpSpeed != nullptr)
	{
		mpSpeed->Play();
	}
}
