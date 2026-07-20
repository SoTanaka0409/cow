#include "ServiceLocator.h"
#include "Object3D.h"
#include "Master.h"
#include "ObjectManager.h"
#include "Scene.h"
#include "Stage.h"
#include "CapsuleCollider.h"

/*
 * Object3Dのコンストラクタ。マネージャーへの登録とコライダー生成を行う。
 * [入力] initPos: 初期座標
 * [出力] なし
 * [副作用] マネージャーに自身を登録、コライダーの生成と保持
 */
Object3D::Object3D(VECTOR initPos)
	: position_(initPos)
	, capsule_collider_(nullptr)
	, rotation_(VGet(0.0f, 0.0f, 0.0f))
	, delete_flag_(false)
	, tag_(Tag3D::kNone3d)
	, draw_flag_(true)
	, radius_(0)
{
	// 描画や更新を自動化するため、アクティブなシーンのマネージャーに登録する
	ServiceLocator::GetObjectManager()->AddObject(this);
	capsule_collider_ = new CapsuleCollider(this, initPos, initPos, 0);
}

/*
 * Object3Dのデストラクタ。保持しているコライダーを破棄する。
 * [入力] なし
 * [出力] なし
 * [副作用] コライダーのメモリ解放
 */
Object3D::~Object3D()
{
	if (capsule_collider_ != nullptr)
	{
		delete capsule_collider_;
		capsule_collider_ = nullptr;
	}
}

/*
 * 描画処理。派生クラスでオーバーライドする。
 * [入力] なし
 * [出力] なし
 * [副作用] 画面への描画
 */
void Object3D::Draw()
{
}

void Object3D::DrawShadowCaster()
{
}

void Object3D::DrawDebug()
{
	if (capsule_collider_ != nullptr)
	{
		capsule_collider_->Draw();
	}
}

/*
 * 衝突判定の開始イベントを処理し、ダメージや効果音のトリガーとするため。
 * [入力] なし
 * [出力] なし
 * [副作用] オブジェクト状態の変更
 */
void Object3D::Update()
{
}

/*
 * 衝突開始時の処理。
 * [入力] collider: 自身のコライダー、check: 相手のコライダー
 * [出力] なし
 * [副作用] 派生先の実装に依存
 */
void Object3D::OnEnter(Collider* collider, Collider* check)
{
}

/*
 * 衝突中の処理。
 * [入力] collider: 自身のコライダー、check: 相手のコライダー
 * [出力] なし
 * [副作用] 派生先の実装に依存
 */
void Object3D::OnTrigger(Collider* collider, Collider* check)
{
}

/*
 * 衝突終了時の処理。
 * [入力] collider: 自身のコライダー、check: 相手のコライダー
 * [出力] なし
 * [副作用] 派生先の実装に依存
 */
void Object3D::OnExit(Collider* collider, Collider* check)
{
}
