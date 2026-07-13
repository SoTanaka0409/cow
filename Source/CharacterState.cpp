#include "CharacterState.h"
#include "CharacterMove.h"
#include "DxLib.h"
#include "Master.h"
#include <cmath>

/*
 * 入力: character (対象キャラクター)
 * 出力: なし
 * 副作用: 待機時間の初期化
 */
void StateIdle::Enter(CharacterMove* character)
{
	// 複数キャラの待機モーションが完全に同期して不自然に見えるのを防ぐため、タイマーを分散させる
	character->SetActionTimer(60 + GetRand(60));
}

/*
 * 入力: character (対象キャラクター)
 * 出力: なし
 * 副作用: アクションタイマーの更新、接地補正、状態遷移
 */
void StateIdle::Update(CharacterMove* character)
{
	character->DecreaseActionTimer();

	// 地形の起伏によってキャラクターが空中に浮く描画バグを防ぐため、常に下方向への接地を強制する
	VECTOR pos = character->GetPosition();
	pos.y -= 4.0f;
	if (pos.y <= 0)
	{
		pos.y = 0;
	}
	character->SetPosition(pos);

	if (character->GetActionTimer() <= 0)
	{
		// 機械的な反復行動を避け、生物らしい不規則な徘徊を表現するために次アクションを確率で分岐
		if (GetRand(100) < 50)
		{
			character->ChangeState(new StateWalk());
		}
		else
		{
			character->ChangeState(new StateIdle());
		}
	}
}

/*
 * 入力: character (対象キャラクター)
 * 出力: なし
 * 副作用: 歩行時間と進行方向ベクトルの初期化
 */
void StateWalk::Enter(CharacterMove* character)
{
	character->SetActionTimer(60 + GetRand(120));

	// 徘徊仕様に基づき、Y軸(高さ)を排除した平面上のランダムな進行方向ベクトルを生成する
	float angle = GetRand(359) * DX_PI_F / 180.0f;
	VECTOR moveVec;
	moveVec.x = sinf(angle);
	moveVec.y = 0.0f;
	moveVec.z = cosf(angle);
	character->SetMoveVec(moveVec);
}

/*
 * 入力: character (対象キャラクター)
 * 出力: なし
 * 副作用: 座標の更新、接地補正、Idle状態への遷移
 */
void StateWalk::Update(CharacterMove* character)
{
	character->DecreaseActionTimer();

	VECTOR pos = character->GetPosition();
	VECTOR moveVec = character->GetMoveVec();
	float speed = character->GetSpeed();

	pos = VAdd(pos, VScale(moveVec, speed * Master::GetDeltaTimeScaler()));

	// 移動による段差抜けや浮遊バグを防ぐための接地処理（StateIdleと同様）
	pos.y -= 4.0f;
	if (pos.y <= 0)
	{
		pos.y = 0;
	}
	character->SetPosition(pos);

	if (character->GetActionTimer() <= 0)
	{
		character->ChangeState(new StateIdle());
	}
}

/*
 * 入力: character (対象キャラクター)
 * 出力: なし
 * 副作用: なし
 */
void StateVacuum::Enter(CharacterMove* character)
{
	// 暫定対応: コレクション要素を廃止しアクション性を重視する仕様へ変更したため、
	// 単純な回収ではない新しい吸引演出の実装まで空枠とする（期限：次回マイルストーンまで）
}

/*
 * 入力: character (対象キャラクター)
 * 出力: なし
 * 副作用: なし
 */
void StateVacuum::Update(CharacterMove* character)
{
}