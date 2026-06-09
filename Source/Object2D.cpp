#include "Object2D.h"
#include "Master.h"
#include "ObjectManager.h"
#include "Scene.h"

Object2D::Object2D(VECTOR initPos)
	: mvPosition(initPos)
	, mvRotation(VGet(0.0f, 0.0f, 0.0f))
	, mbDeleteFlag(false)
	, mnTag(Tag2D::None2D)
	, mbDrawFlag(true)
{
	// 生成したオブジェクトを管理・更新するため、現在のアクティブなシーンのマネージャーへ自動で登録する
	Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->AddObject(this);
}

Object2D::~Object2D()
{
}

void Object2D::Draw()
{
}

void Object2D::Update()
{
}
