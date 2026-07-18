#pragma once
#include "dxlib.h"
#include "Object3D.h"
#include "Texture.h"

// 業務ルール：UFOのパワーアップ体験を提供するため、レベルアップ時に出現する3枚のスキル選択カードと能力上昇効果を制御するクラス
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
	// 入力：parent=プレイヤーなど関連付ける親アクター
	// 副作用：スキル選択カード用Textureの動的確保
	Skill(Object3D* parent);
	~Skill();

	// 副作用：カード表示用座標の更新
	void Update();

	// 副作用：画面へのカードUI描画
	void Draw();

	// 副作用：プレイヤーのステータス加算、または餌オブジェクトの生成トリガーの発行
	void AddSkill();

	SkillTag GetSkillTag() { return tag_; }

	// 入力：tag=ステータス種類
	// 出力：加算されるボーナス実数値
	float GetStatusDate(StatusTag tag);

	// 入力：flag=UIを表示するかどうかのフラグ
	// 副作用：各種アニメーションフラグとカード初期位置の初期化
	void SetSkillFlag(bool flag);

	// バグ回避：フレーム跨ぎの複数回クリックによる、スキルの二重取得バグを防止するための購入確定フラグ
	bool add_skill_flag_;

private:
	void UpdateFlashAnimation();
	void UpdateSelectAnimation();
	void UpdateOpenAnimation();

private:

	Object3D* parent_;
	SkillTag tag_;

	// 仕様制約：UI上にランダム提示される「3枚の異なる選択肢カード」の見た目を個別に制御するための個別テクスチャ
	Texture* texture_;
	Texture* texture2_;
	Texture* texture3_;

	// 将来の拡張性（a = AttackSpeed, s = Speed）のバフ加算量を個別に永続保持するためのワーク変数
	float status_a_;
	float status_s_;

	int hover_skill_;
	int flash_alpha_;
	bool flash_;

	// 仕様制約：カード決定時に即画面を閉じず、選択したカードが拡大フェードする演出（0.5秒）を挟むための状態フラグ
	bool select_anim_;
	int selected_skill_;
	float select_scale_;
	VECTOR select_pos_;

	// 画面解像度やアスペクト比の変更時に、3枚のカードの均等な横並びレイアウトを維持するための基準座標群
	VECTOR pos1_;
	VECTOR pos2_;
	VECTOR pos3_;

	// 3枚のカードが画面下部から時間差（ディレイ）を持って滑らかに浮上する演出を制御するフラグ
	bool open_anim_;
	float card1_y_;
	float card2_y_;
	float card3_y_;
};
