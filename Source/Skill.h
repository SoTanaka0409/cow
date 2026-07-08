�ｿ#pragma once
#include "dxlib.h"
#include "Object3D.h"
#include "Texture.h"

// プレイヤーレベルアップ時のスキル（ステータス強化・餌設置）のUI選択および効果管理を行うクラス
class Skill
{
public:
	// スキルカードの種類定義
	enum SkillTag
	{
		None,
		Skill_1,    // ステータス強化1 (吸引速度)
		Skill_2,    // 餌（デコイ）設置
		Skill_3,    // ステータス強化2 (プレイヤー速度)
	};

	// 強化可能なプレイヤーパラメータのタグ定義
	enum StatusTag
	{
		Status_AttackSpeed,
		Status_Speed,
	};

public:
	/*
	 * @brief スキル選択画面で表示するUI画像（3枚のスキルカード）をロードする
	 * [入力] parent: プレイヤーなど関連付ける親アクター
	 * [出力] なし
	 * [副作用] スキル選択カード用Textureインスタンスの動的確保
	 */
	Skill(Object3D* parent);
	~Skill();

	/*
	 * @brief カード登場時、選択中の退出アニメーション等の補間計算を行う
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 各カード表示用座標の更新
	 */
	void Update();

	/*
	 * @brief 画面上にスキル選択のカードUIを描画する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	void Draw();

	/*
	 * @brief マウスのホバーおよび左クリック検知を行い、選択されたスキルの効果を発動させる
	 * [入力] なし
	 * [出力] なし
	 * [副作用] スキル効果の適用（XP加算、あるいは餌オブジェクトのnew）
	 */
	void AddSkill();

	SkillTag GetSkillTag() { return tag; }

	/*
	 * @brief 指定された強化ステータスの合計ボーナス値を取得する
	 * [入力] tag: ステータス種類
	 * [出力] 加算されるボーナス実数値
	 * [副作用] なし
	 */
	float GetStatusDate(StatusTag tag);

	/*
	 * @brief スキル選択画面の表示・非表示フラグを設定し、カードのアニメーション変数を初期化する
	 * [入力] flag: 有効にするかどうかの真偽値
	 * [出力] なし
	 * [副作用] カードの初期表示位置、選択アニメーションフラグの初期化
	 */
	void SetSkillFlag(bool flag);

	bool AddSkillFlag;          // スキル選択画面が現在オープンしているかどうかのフラグ

private:
	Object3D* mpParent;         // 親アクター
	SkillTag tag;               // スキルタグ
	Texture* mpTexture;         // カード1（速度アップ）のテクスチャ
	Texture* mpTexture2;        // カード2（餌にんじん）のテクスチャ
	Texture* mpTexture3;        // カード3（吸引力アップ）のテクスチャ
	float StatusDate;           // パラメータ一時変数
	float Status_A;             // 吸引速度の合計強化値
	float Status_S;             // 移動速度の合計強化値

	int mHoverSkill;            // マウスがホバーしているカードの番号 (1?3)
	int mFlashAlpha;            // 選択時のフラッシュ効果用アルファ値
	bool mFlash;                // フラッシュ効果がアクティブかどうかのフラグ

	bool mSelectAnim;           // カードが画面外へフェードアウトするアニメーション中かどうかのフラグ
	int mSelectedSkill;         // 選択されたカード番号
	float mSelectScale;         // 選択アニメーション時のカードスケール
	VECTOR mSelectPos;          // 選択されたカードの表示現在位置

	VECTOR mPos1;               // カード1の標準配置座標
	VECTOR mPos2;               // カード2の標準配置座標
	VECTOR mPos3;               // カード3の標準配置座標

	bool mOpenAnim;             // カードが下からスライドインする登場アニメーション中かどうかのフラグ
	float mCard1Y;              // カード1の現在Y座標
	float mCard2Y;              // カード2の現在Y座標
	float mCard3Y;              // カード3の 現在Y座標
};

