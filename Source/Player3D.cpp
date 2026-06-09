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
//#include"Effect.h"
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
#include"Boss.h"
#include"AnimalMove.h"
#include"GameTimer.h"
#include"GameManager.h"
// �����A�j���[�V������g�p���邩�̐ݒ��ǉ��i�f�t�H���g��false�i�g�p���Ȃ��j�j
Player3D::Player3D(std::string filename, VECTOR initPos)
	:Object3D(initPos)
	, mfVerticalAngle(0.0f)
	, mfHorizontalAngle(0.0f)
	, mfSpeed(35.0f)//speed
	, mfHp(0)
	, mfAttack_Speed(6.0f)
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
	//mpCamerafa = Master::mpCamerafa;

	mnLighGraph = LoadGraph("Resource/2D/green_beam_transparent.png");

	mpCapsuleCollider->mvPosition = VGet(mvPosition.x, 0, mvPosition.z);
	mpCapsuleCollider->mvPosition2 = mvPosition;
	mpCapsuleCollider->mfRadius = mfRadius;
	//mpTarget = new Enemy3D("", VGet(20000, 0, 0), 0, 0, 0, 0, 0);

	//mpCamera->ResetCameraPlayer(true);
	 // �R���C�_�[����
	//mpCapsuleCollider = new CapsuleCollider(this, mvPosition, VAdd(mvPosition, VGet(0.0f, 60.0f, 0.0f)), 60.0f);



	//WeaponHitPos = VAdd(mpModel->GetAttachmentPosition(), VGet(0.0f, 50.0f, 0.0f));

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
	if (mpBeam) { delete mpBeam; mpBeam = nullptr; }
	if (mpSpeed) { delete mpSpeed; mpSpeed = nullptr; }
}

void Player3D::Update()
{
	if (Master::mbIsDebugCamera) return; // �f�o�b�O�J�����N�����͏����𖳎�����
	// --- �X�^����Ԃ̊Ǘ� ---
	if (mIsStunned)
	{
		mStunTimer--;
		if (mStunTimer <= 0)
		{
			mIsStunned = false;
		}
		// ������ return �����A���́u�X�V�E�`�揈���v�֐i�ނ悤�ɂ��܂�
	}

	mIsCowInVacuumRange = false;

	// �}�l�[�W���[�̍X�V�i�����s��Ȃ��ƃ��f�����\���E�X�V����܂���j
	ManagerUpdate();

	if (Master::GameFinishFlag)
	{
		mvPosition = VGet(0, 2000, 0);
		return;
	}
	if (mIsOutOfBounds)
	{
		mvPosition = VGet(0, 2000, 0);
	}
	
	

	if (!mIsStunned) // X^ĂȂ
	{
		//ړ
		MoveEx();
		//]
		RotationByMove();

		ColliderUpdate();
		Play();

	}
	else
	{
		// スタン中の場合でも、コライダーの位置と大きさを非アクティブ状態（通常状態）に更新する必要がある。
		// そうしないと、吸い込み中にスタンした場合に、巨大な吸い込み判定（コライダー）がその場に残ったままになってしまう。
		mIsVacuumActive = false;
		ColliderUpdate();
	}


	ScreenOutCheck();

	

	



	
}

void Player3D::Play()
{
	
	int mouseInput = GetMouseInput();
	if ((mouseInput & MOUSE_INPUT_LEFT) && mVacuumGauge > 0.0f&&!Master::FeverFlag)
	{
		mIsVacuumActive = true;
		
		mVacuumGauge -= VACUUM_COST_PER_FRAME;
		
		
		if (mVacuumGauge < 0.0f) mVacuumGauge = 0.0f;
	}
	else
	{
		mIsVacuumActive = false;
		
		float recoverySpeed = VACUUM_RECOVER_PER_FRAME;
		if (Master::mpSceneManager && Master::mpSceneManager->GetCurrentScene() && Master::mpSceneManager->GetCurrentScene()->mpGameManager)
		{
			auto timer = Master::mpSceneManager->GetCurrentScene()->mpGameManager->GetGameTimer();
			if (timer && timer->GetTime() <= 60)
			{
				recoverySpeed *= 2.0f;
			}
		}

		mVacuumGauge += recoverySpeed;
		if (mVacuumGauge > VACUUM_GAUGE_MAX) mVacuumGauge = VACUUM_GAUGE_MAX;
	}
	if (Master::FeverFlag)mIsVacuumActive = true;//feverちゅうなら
	

}
void Player3D::ColliderUpdate()
{
	if (mIsVacuumActive)
	{
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

	if (mpBeam != nullptr)
	{
		mpBeam->SetPosition(mvPosition);
		// 2. ���̌�ɃG�t�F�N�g�̍X�V������s��
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
	if (mvPosition.x >Utility::StageSize.x  || mvPosition.x < -Utility::StageSize.x||
		mvPosition.z > Utility::StageSize.z || mvPosition.z < -Utility::StageSize.z)
	{
		mIsOutOfBounds = true;

		// �Z�{�^���i�����Space�L�[��z��j�Ŗ߂�
		if (CheckHitKey(KEY_INPUT_SPACE))
		{
			SetPosition(VGet(0, 2000, 0)); // �����֕��A
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
		// �e�X�g�p�F5�L�[���������X�L���I���ʂ�o��
		mpSkill->SetSkillFlag(true);
	}
}

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


	//���f���̕`��

	/*DrawSphere3D(
		mvPosition,
		60.0f,
		8,
		GetColor(255, 255, 255),
		GetColor(255, 255, 255),
		false

	);*/

	

	// --- �n�ʂɉ~��`�悷�鏈�� ---
	const int DIV = 32;             // �~����������ĕ`�����i���������قǊ��炩�ɂȂ�܂��j
	// ���ύX�F�t���O��true�Ȃ�ԁAfalse�Ȃ�΂ɂ���
	unsigned int color;
	if (mIsCowInVacuumRange == true)
	{
		color = GetColor(255, 0, 0); // �ԐF
	}
	else
	{
		color = GetColor(0, 255, 0); // �ΐF
	}

	for (int i = 0; i < DIV; i++) {
		// ���݂̓_�Ǝ��̓_�̊p�x��v�Z (���W�A��)
		float angle1 = (float)i / DIV * DX_PI_F * 2.0f;
		float angle2 = (float)(i + 1) / DIV * DX_PI_F * 2.0f;

		// ���ύX�FRADIUS�̑���ɗp�ӂ��� VACUUM_RADIUS ��g���܂�
		VECTOR p1 = VAdd(mvPosition, VGet(cosf(angle1) * VACUUM_RADIUS, -mvPosition.y + 0.1f, sinf(angle1) * VACUUM_RADIUS));
		VECTOR p2 = VAdd(mvPosition, VGet(cosf(angle2) * VACUUM_RADIUS, -mvPosition.y + 0.1f, sinf(angle2) * VACUUM_RADIUS));

		// 2�_�Ԃ���łȂ�
		DrawLine3D(p1, p2, color);
	}

	//DrawBillboard3D(
	//	VSub(mvPosition, VGet(0.0f, 520.0f, 0.0f)),      // Pos: vC[̈ʒu
	//	0.5f, 0.5f,      // cx, cy: 摜̊_i^񒆁j
	//	450,          // Size: 傫
	//	0.0f,            // Angle: ]
	//	mnLighGraph,     // GrHandle: ǂݍ񂾉摜̔ԍ
	//	true             // TransFlag: Lɂ邩
	//);

	// Draw Vacuum Gauge UI
	bar();//吸い込みゲージの描画
}



//ړXe[WƂ̓蔻
void Player3D::MoveEx()
{
	
	//�ړ�������
	mvOldPosition = mvPosition;//�O��̍��W���U�ێ�
	moveVec = VGet(0.0f, 0.0f, 0.0f);//�ړ�����
	VECTOR UpMoveVector = VGet(0.0f, 0.0f, 0.0f);//�J�����̏�����x�N�g��
	VECTOR leftMoveVector = VGet(0.0f, 0.0f, 0.0f);//�J�����̍������x�N�g��
	//VECTOR DownMoveVector = VGet(0.0f, 0.0f, 0.0f);//�J�����̉������x�N�g��
	//VECTOR RightMoveVector = VGet(0.0f, 0.0f, 0.0f);//�J�����̉E�����x�N�g��


	//�J�����̌�������ړ��x�N�g������߂�
	{
		//������ւ̈ړ��x�N�g���́A�J�������_��������y�����𔲂�����̂Ƃ���
		UpMoveVector = VSub(Master::mpCamera->GetLookAtPosition(), Master::mpCamera->GetPosition());
		UpMoveVector.y = 0.0f;

		//�������ւ̈ړ��x�N�g�����A������̈ړ��x�N�g���ƁAY���̃v���X�����ւ̃x�N�g���ɐ����ȕ����i�O�ρj
		leftMoveVector = VCross(UpMoveVector, VGet(0.0f, 1.0f, 0.0f));
		leftMoveVector.y = 0.0f;


		//�ړ��x�N�g���͈ړ��ʂ�������Ȃ��̂ŁA���K�����Ă���(�x�N�g���̒�����P�ɂ��邱��)
		UpMoveVector = VNorm(UpMoveVector);
		leftMoveVector = VNorm(leftMoveVector);
	}

	//VECTOR moveVec = VGet(0.0f, 0.0f, 0.0f);//�ړ�����

	if (CheckHitKey(KEY_INPUT_A))//�������ւ̈ڑ�
	{
		moveVec = VAdd(moveVec, leftMoveVector);
	}
	if (CheckHitKey(KEY_INPUT_D))//�E�����ւ̈ڑ�
	{
		//veVec.x = 1.0f;
		moveVec = VAdd(moveVec, VScale(leftMoveVector, -1.0f));
		//moveVec = VSub(moveVec, leftMoveVector);

	}
	if (CheckHitKey(KEY_INPUT_W))//�������ւ̈ڑ�
	{
		//veVec.z = 1.0;
		moveVec = VAdd(moveVec, UpMoveVector);
	}
	if (CheckHitKey(KEY_INPUT_S))//��O�����ւ̈ڑ�
	{
		//moveVec.z = -1.0;
		moveVec = VAdd(moveVec, VScale(UpMoveVector, -1.0f));
		//moveVec = VSub(moveVec, UpMoveVector);

	}
	//�ړ����Ă����Ԃł����
	bool isMove = (moveVec.x != 0.0f || moveVec.z != 0.0f);
	if (isMove)
	{
		moveVec = VNorm(moveVec);
		mfTargetAngle = atan2f(moveVec.x, moveVec.z);
		oldmoveVec = moveVec;

		// �y�������|�C���g�I�z
		// mfSpeed�i��{�l�j�𒼐ڎg�킸�AStatus�֐�����
		// �u��{�X�s�[�h + �X�L���㏸���v����݂̑��x�Ƃ��Ď擾���܂�
		currentSpeed = Status(Status_Speed);

	
		// ���̑��x��g���Ĉړ�
		mvPosition = VAdd(mvPosition, VScale(moveVec, currentSpeed));
	}
	//�W�����v�J�n�̃A�j������Ȃ��̂ł����
	



	//

	///////////////////////�ǂ̓����蔻��////////////////////////////
	bool hitwall = false;
	bool hitwalls = false;
	const auto& walls = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Wall);
	if (!walls.empty())
	{
		// �������Ă���ǂ𒲂ׂ�
		// hint: ����̏����ł�1���̕ǂ����ŏI�I�ɔ��肳��Ȃ��̂ŁA2���i�ȏ�j�ɓ������Ă����ꍇ�̏�����l����
		for (int i = 0; i < walls.size(); i++)
		{
			Wall* wall = dynamic_cast<Wall*>(walls.at(i));
			if (wall != nullptr)
			{
				std::vector<VERTEX3D> vertex = wall->GetVertex();

				// �v���C���[���ނ悤�ȃJ�v�Z���^�̔���ƁA�ǂ̎O�p�`�|���S���Ƃ̓����蔻���s��
				if (HitCheck_Capsule_Triangle(
					mvPosition,
					VAdd(mvPosition, VGet(0.0f, 200.0f, 0.0f)),
					80.0f,
					vertex.at(0).pos, vertex.at(1).pos, vertex.at(2).pos) ||
					HitCheck_Capsule_Triangle(
						mvPosition,
						VAdd(mvPosition, VGet(0.0f, 200.0f, 0.0f)),
						80.0f,
						vertex.at(3).pos, vertex.at(1).pos, vertex.at(2).pos)
					)
				{
					
					hitwall = true;
					// �ǂɉ����čs���悤�ȃx�N�g����擾
					VECTOR slide = VGet(0.0f, 0.0f, 0.0f);  // �ǉ����x�N�g��
					float a = VDot(VScale(moveVec, -1.0f), vertex.at(0).norm);  // �ړ������x�N�g���̔��΃x�N�g���ƁA�ǂ̖@���Ƃ̓�ς���߂�
					slide = VAdd(moveVec, VScale(vertex.at(0).norm, a));    // �ǉ����x�N�g����v�Z

					if (hitwall == true && hitwalls == false)
					{
						mvPosition = mvOldPosition;
						mvPosition = VAdd(mvPosition, VScale(slide, mfSpeed));
						hitwalls = true;
						//hitwall = false;
					}
					else if (hitwalls == true)
					{
						mvPosition = mvOldPosition;
					}
				}
			}
		}

	}

	//////////////////////////////////////////////////////







	mpModel->SetPosition(mvPosition);
	mpModel->SetRotation(mvRotation);

}



float Player3D::Status(StatusID id)
{
	if (id == Status_AttackS)
	{
		// ��b�U�����x + �X�L���ŏ㏸������
		return mfAttack_Speed + mpSkill->GetStatusDate(Skill::Status_AttackSpeed);
	}
	if (id == Status_Hp)
	{
		return mfHp;
	}
	if (id == Status_Speed)
	{
		// �v���C���[���g�� mfSpeed �ɁA�X�L���� Status_S �𑫂��ĕԂ�
		return mfSpeed + mpSkill->GetStatusDate(Skill::Status_Speed);
	}
	return 0.0f;
}



void Player3D::RotationByMove()
{

	//////�������Ăق����p�x��Y�o
	//Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag_3D_Camera)
	//���݂̉�]�l����ڕW�̉�]�l�̍�������߂�
	float subAngle = mfTargetAngle - mfAngle;



	//����������炠������̍����P�W�O�x�ȏ�i�ȉ��j�ɂȂ邱�Ƃ��Ȃ��͂��Ȃ̂�
	//���̒l���P�W�O�ȏ�i�ȉ��j�ɂȂ��Ă����狸������
	if (subAngle < -DX_PI_F)
	{
		subAngle += DX_TWO_PI_F;
	}
	if (subAngle > DX_PI_F)
	{
		subAngle -= DX_TWO_PI_F;
	}

	//�p�x�̍�������X�ɂO�ɋ߂Â���
	if (subAngle > 0.0f)
	{
		subAngle -= ROTATE_SPEED;
		if (subAngle < 0.0f)
		{
			subAngle = 0.0f;
		}
	}
	else if (subAngle < 0.0f)
	{
		subAngle += ROTATE_SPEED;
		if (subAngle > 0.0f)
		{
			subAngle = 0.0f;
		}
	}

	//(400, 560, GetColor(0, 0, 0), "subAngle::%f",subAngle);

	//�������Ăق����p�x��Y�o
	mfAngle = mfTargetAngle - subAngle;

	//DrawFormatString(400, 600, GetColor(0, 0, 0), "mfAngle::%f", subAngle);

	//��]�l��ݒ�
	mvRotation.y = mfAngle + DX_PI_F;
	//���f���ɓ`����
	mpModel->SetRotation(mvRotation);
	//mfAngle = mfTargetAngle - Master::mpCamera->GetLookCamera();

	////��]�l��ݒ�
	//mvRotation.y = mpCamera->GetlookAtPosition().y; //mfAngle + DX_PI_F;
	////���f���ɓ`����
	//mpModel->SetRotation(mvRotation);


}





void Player3D::bar()
{
	int gaugeWidth = 300;
	int gaugeHeight = 50;
	int gaugeX = 50;
	int gaugeY = 700;

	// Background
	DrawBox(gaugeX, gaugeY, gaugeX + gaugeWidth, gaugeY + gaugeHeight, GetColor(100, 100, 100), TRUE);

	// Foreground
	int currentWidth = (int)((mVacuumGauge / VACUUM_GAUGE_MAX) * gaugeWidth);
	if (currentWidth < 0) currentWidth = 0;
	if (currentWidth > gaugeWidth) currentWidth = gaugeWidth;
	unsigned int gaugeColor = GetColor(0, 255, 255); // Cyan
	if (mVacuumGauge <= 0.0f) gaugeColor = GetColor(255, 0, 0); // Red when empty
	DrawBox(gaugeX, gaugeY, gaugeX + currentWidth, gaugeY + gaugeHeight, gaugeColor, TRUE);

	// Border
	DrawBox(gaugeX, gaugeY, gaugeX + gaugeWidth, gaugeY + gaugeHeight, GetColor(255, 255, 255), FALSE);
	DrawFormatString(gaugeX, gaugeY - 30, GetColor(255, 255, 255), "Vacuum Gauge: %.1f%%", mVacuumGauge);
}




void Player3D::OnEnter(Collider* collider, Collider* check)
{
	if (collider == mpCapsuleCollider && check->mpParentObject->GetTag() == Tag3D_Cow)
	{
		CowMove* cow = dynamic_cast<CowMove*>(check->mpParentObject);

		if (cow->GetCurrentState() != CowMove::STATE_VACUUM)
		{
			cow->IncreaseVacuumTimer();
		
			
			{
				cow->ChangeStateToVacuum();

			}
		}

	}
	if (collider == mpCapsuleCollider && check->mpParentObject->GetTag() == Tag3D_Animal)
	{
		AnimalMove* ani = dynamic_cast<AnimalMove*>(check->mpParentObject);

		if (ani->GetCurrentState() != CowMove::STATE_VACUUM)
		{
			ani->IncreaseVacuumTimer();
			//if (ani->GetVacuumTimer()/2 >= VACUUM_REQUIRE_TIME)
			{
				ani->ChangeStateToVacuum();

			}
		}

	}



}

void Player3D::OnExit(Collider* collider, Collider* check)
{
	if (collider == mpCapsuleCollider && check->mpParentObject->GetTag() == Tag3D_Cow)
	{
		CowMove* cow = dynamic_cast<CowMove*>(check->mpParentObject);

		// �^�C�}�[���Z�b�g
		cow->ResetVacuumTimer();
		cow->SetCurrentState(CowMove::STATE_WALK);
		// Y���W���Z�b�g�i���̈ʒu�ɖ߂��z��j
		VECTOR pos = cow->GetPosition();

		
		cow->SetPosition(pos);
	}
	if (collider == mpCapsuleCollider && check->mpParentObject->GetTag() == Tag3D_Animal)
	{
		AnimalMove* ani = dynamic_cast<AnimalMove*>(check->mpParentObject);
		ani->SetCurrentState(AnimalMove::STATE_WALK);
		ani->SetPosition(ani->GetPosition());
	}
}

void Player3D::OnTrigger(Collider* collider, Collider* check)
{
	

}

//void Player3D::OnExit(Collider* collider, Collider* check)
//{
//	/*if (collider==mpAttachCollider&&check->mpParentObject->GetTag() == Object3D::Tag3D_Enemy3D)
//	{
//		Enemy* pEne = dynamic_cast<Enemy*>(check->mpParentObject);
//
//	}*/
//}


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
