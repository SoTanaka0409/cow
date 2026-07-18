#include "Animal.h"

/*
 * 入力: filename (モデルパス), initPos (初期座標)
 * 出力: なし
 * 副作用: ベースクラスの初期化、および動物固有のパラメータのデフォルト値設定
 */
Animal::Animal(std::string filename, VECTOR initPos)
	: AnimalMove(filename, initPos)
{
	// スコア計算時の基準仕様として、生成時の初期経験値(XP)を10に固定
	xp_ = 10;
	SetTagAnimal(AnimalMove::kAnimal1);
}

Animal::~Animal()
{
}

/*
 * 入力: なし
 * 出力: なし
 * 副作用: 座標・回転の更新、当たり判定の追従、および死亡判定処理の実行
 */
void Animal::Update()
{
	MoveCharacter();

	// UFO吸引中の強制演出と競合してモデルの軸がブレるバグを防ぐため、自律回転を停止
	if (!(mCurrentState == STATE_VACUUM))
	{
		RotationCharacter();
	}

	ColliderMove();
	CharacterDied();
	model_->Update();
}
