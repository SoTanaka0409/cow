#pragma once
#include "dxlib.h"
#include "Object3D.h"
#include "Texture.h"

// プレイヤーレベルアップ時のスキル報酬選択UIおよび効果の適用を管理するため
class Skill
{
public:
	enum SkillTag
	{
		None,
		kSkill1, // 吸引速度アップ
		kSkill2, // 餌設置
		kSkill3, // 移動速度アップ
	};

	enum StatusTag
	{
		kStatusAttackSpeed,
		kStatusSpeed,
	};

public:
	/*
	 * レベルアップ時の報酬として提示するスキルUIの生成と初期化を行う。
	 * [入力] parent: プレイヤーなど関連付ける親アクター
	 * [出力] なし
	 * [副作用] スキル選択カード用Textureの動的確保
	 */
	Skill(Object3D* parent);
	~Skill();

	/*
	 * スキル選択中におけるカードの登場・退出アニメーションの補間計算を行う。
	 * [入力] なし
	 * [出力] なし
	 * [副作用] カード表示用座標の更新
	 */
	void Update();

	/*
	 * プレイヤーにスキル選択を促すためのカードUIを画面に描画する。
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	void Draw();

	/*
	 * マウス入力からプレイヤーの選択を検知し、該当するスキル効果を適用する。
	 * [入力] なし
	 * [出力] なし
	 * [副作用] ステータス加算、または餌オブジェクトの生成
	 */
	void AddSkill();

	SkillTag GetSkillTag() { return tag_; }

	/*
	 * 外部から強化されたステータス値を参照し、プレイヤーの能力計算に反映させる。
	 * [入力] tag: ステータス種類
	 * [出力] 加算されるボーナス実数値
	 * [副作用] なし
	 */
	float GetStatusDate(StatusTag tag);

	/*
	 * スキル選択画面の開閉状態を切り替え、アニメーションを最初から再生できるようにする。
	 * [入力] flag: UIを表示するかどうかのフラグ
	 * [出力] なし
	 * [副作用] 各種アニメーションフラグとカード座標の初期化
	 */
	void SetSkillFlag(bool flag);

	bool add_skill_flag_;

private:
	Object3D* parent_;
	SkillTag tag_;
	Texture* texture_;
	Texture* texture2_;
	Texture* texture3_;
	float status_date_;
	float status_a_;
	float status_s_;

	int hover_skill_;
	int flash_alpha_;
	bool flash_;

	bool select_anim_;
	int selected_skill_;
	float select_scale_;
	VECTOR select_pos_;

	VECTOR pos1_;
	VECTOR pos2_;
	VECTOR pos3_;

	bool open_anim_;
	float card1_y_;
	float card2_y_;
	float card3_y_;
};