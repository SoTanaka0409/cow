#pragma once
#include"Dxlib.h"
#include"Object3D.h"

// プレイヤー（または他のオブジェクト）のレベル・経験値、およびレベルアップUIを処理するクラス
class Level
{
public:
	Level(Object3D* obj);
	~Level();

	/*
	 * @brief 経験値バーと現在のレベルを画面左下に描画する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] レベル文字列、経験値バーの画面描画
	 */
	void Draw();

	/*
	 * @brief 毎フレームの更新処理（現状なし）
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	void Update();

	/*
	 * @brief 経験値を加算し、最大値に達した場合はレベルアップ（およびスキル抽選要求）を行う
	 * [入力] xp: 加算する経験値量
	 * [出力] なし
	 * [副作用] mfxpの加算、レベルアップ(mnNowLevel++)、スキル選択フラグ(SetSkillFlag)の有効化
	 */
	void AddXp(float xp);

	/*
	 * @brief 次のレベルへ昇格するために必要な最大経験値量を再計算する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] mfMaxXpの更新
	 */
	void SetNextLevel();

private:
	/*
	 * @brief 経験値の進捗状況をメーター（バー）として画面に描画する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 進捗枠線（白）、背景（黒）、進捗バー（緑）の描画
	 */
	void DrawBar();

	Object3D* mpParent;        // レベル管理対象のオブジェクトポインタ（通常はPlayer3D）
	float mfxp;                // 現在の獲得経験値量
	float mfMaxXp;             // レベルアップに必要な最大経験値量
	int mnNowLevel;            // 現在のレベル値

	VECTOR pos = VGet(20, 800, 0); // 経験値バーの描画開始画面座標
	VECTOR size = VGet(300, 30, 0); // 経験値バーの幅と高さ
};
