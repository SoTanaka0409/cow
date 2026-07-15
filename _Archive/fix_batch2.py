import os

cowmove_h = """#pragma once
#include "DxLib.h"
#include "Object3D.h"
#include "Model.h"
#include "ModelUtility.h"
#include "EffekseerEffect.h"
#include "CharacterMove.h"

class SphereCollider;
class CapsuleCollider;

// AIによる自律移動と吸い込み判定を管理する基底クラス
class CowMove : public CharacterMove
{
public:
	enum TagCow
	{
		kNone,
		kCow1,
		kCow2,
		kCow3,
		kCowGold,
		kCowT,
	};

public:
	/*
	 * キャラクターの初期設定を行う
	 * [入力] filename: モデルパス, initPos: 初期配置座標
	 * [出力] なし
	 * [副作用] エフェクトのインスタンス生成
	 */
	CowMove(std::string filename, VECTOR initPos);
	virtual ~CowMove();

	/*
	 * キャラクターの状態を毎フレーム更新する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	void Update() override;

	/*
	 * キャラクターやエフェクトを描画する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	void Draw() override;

	/*
	 * 自律移動や吸引時の座標を更新する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 座標の更新、AI自律移動および吸引時上昇同期
	 */
	virtual void MoveCharacter() override;

	/*
	 * コライダーをモデルの座標に追従させる
	 * [入力] なし
	 * [出力] なし
	 * [副作用] コライダー座標の更新
	 */
	virtual void ColliderMove() override;

	/*
	 * 再利用時のためのパラメータ初期化を行う
	 * [入力] pos: 再配置座標
	 * [出力] なし
	 * [副作用] エフェクトやコライダーのリセット
	 */
	virtual void Reset(VECTOR pos) override;

	/*
	 * 重なりを避けるため他キャラクターと距離を取る
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 座標の微調整
	 */
	void AvoidOtherCows();

	/*
	 * 餌の方向へ向かって移動する
	 * [入力] なし
	 * [出力] 餌に向かっている場合はtrue
	 * [副作用] 座標と移動フラグの更新
	 */
	bool SeekBait();

	/*
	 * AIの徘徊処理を更新する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	virtual void UpdateWanderAI() override;

	/*
	 * 壁との衝突判定と押し出しを行う
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	virtual void CheckWallCollision() override;

	/*
	 * 進行方向へ向くように回転を更新する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	virtual void RotationCharacter() override;

	/*
	 * 回転をモデルに適用する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	virtual void CharacterRotate() override;

	/*
	 * アニメーションを追加する
	 * [入力] state: アニメーション状態, filename: アニメファイル名
	 * [出力] なし
	 * [副作用] なし
	 */
	void AddAnimation(AnimationState state, std::string filename);

	/*
	 * コライダー侵入時の判定を行う
	 * [入力] collider: 自身のコライダー, check: 相手のコライダー
	 * [出力] なし
	 * [副作用] 餌検知フラグや反発処理の実行
	 */
	virtual void OnEnter(Collider* collider, Collider* check) override;

	/*
	 * コライダー接触中の判定を行う
	 * [入力] collider: 自身のコライダー, check: 相手のコライダー
	 * [出力] なし
	 * [副作用] 反発処理の継続
	 */
	virtual void OnTrigger(Collider* collider, Collider* check) override;

	/*
	 * コライダー離脱時の判定を行う
	 * [入力] collider: 自身のコライダー, check: 相手のコライダー
	 * [出力] なし
	 * [副作用] 餌検知フラグの解除
	 */
	virtual void OnExit(Collider* collider, Collider* check) override;

	/*
	 * 限界高度到達時の消滅処理を行う
	 * [入力] なし
	 * [出力] なし
	 * [副作用] エフェクト再生や削除処理の呼び出し
	 */
	virtual void CharacterDied() override;

	/*
	 * 餌によるキル処理を行う
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 削除フラグの有効化と非表示化
	 */
	virtual void KilledByBait();

	/*
	 * 死亡時の共通処理を行う
	 * [入力] reason: 死亡理由
	 * [出力] なし
	 * [副作用] コンボ・スコアの計算、削除フラグの有効化
	 */
	virtual void Die(DeathReason reason) override;

	/*
	 * 種類識別タグを設定する
	 * [入力] tag: 設定するタグ
	 * [出力] なし
	 * [副作用] タグの変更
	 */
	void SetTagCow(TagCow tag) { tag_cow_ = tag; }

	/*
	 * 種類識別タグを取得する
	 * [入力] なし
	 * [出力] 現在のタグ
	 * [副作用] なし
	 */
	TagCow GetTagCow() { return tag_cow_; }

	/*
	 * コライダーの半径を設定する
	 * [入力] radius: 設定する半径
	 * [出力] なし
	 * [副作用] コライダーの半径変更
	 */
	void SetColliderRadius(float radius) { collider_radius_ = radius; }

protected:
	TagCow tag_cow_;
	float collider_radius_ = 50.0f;

	EffekseerEffect* cow_vm_;
	int effect_timer_;
	bool tutorial_vacum_flag_;
};
"""

cow_2_h = """#pragma once
#include "CowMove.h"

// スコア等のパラメータが標準と異なるCow_2を管理するクラス
class Cow_2 : public CowMove
{
public:
	/*
	 * 初期化を行う
	 * [入力] filename: モデルパス, initPos: 初期配置座標
	 * [出力] なし
	 * [副作用] 個別のスコア・コライダー半径の設定
	 */
	Cow_2(std::string filename, VECTOR initPos);
	virtual ~Cow_2();
};
"""

cow_tutorial_h = """#pragma once
#include "CowMove.h"
#include "dxlib.h"

// チュートリアル進行用の特殊なパラメータを持つ牛を管理するクラス
class Cow_Tutorial : public CowMove
{
public:
	/*
	 * チュートリアル用の固有パラメータを設定し初期化する
	 * [入力] filename: モデルファイルパス, initPos: 初期座標
	 * [出力] なし
	 * [副作用] なし
	 */
	Cow_Tutorial(std::string filename, VECTOR initPos);

	/*
	 * デストラクタ
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	virtual ~Cow_Tutorial();
};
"""

import re
with open('Source/CowMove.cpp', 'r', encoding='utf-8') as f:
    cowmove_cpp = f.read()

# Replace mojibake "同?E??E?続キル?E?よ?E?ボEナス?E??E??E?計?E?E" and any similar broken comments with normal ones
cowmove_cpp = re.sub(r'//\s*同.*\n', '// 同じタグの連続キルによるボーナススコア計算\n', cowmove_cpp)

files = {
    'Source/CowMove.h': cowmove_h,
    'Source/Cow_2.h': cow_2_h,
    'Source/Cow_Tutorial.h': cow_tutorial_h,
    'Source/CowMove.cpp': cowmove_cpp
}

for path, content in files.items():
    with open(path, 'w', encoding='utf-8-sig') as f:
        f.write(content)
