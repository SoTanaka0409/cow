#pragma once
#include "dxlib.h"
#include "Object3D.h"
#include "Texture.h"

/// @brief UFOのパワーアップ体験を提供するため、レベルアップ時に出現する3枚のスキル選択カードと能力上昇効果を制御するクラス
class Skill
{
public:
	enum SkillTag
	{
		None,
		kSkill1, ///< 吸引速度アップ
		kSkill2, ///< 餌設置
		kSkill3, ///< 移動速度アップ
	};

	enum StatusTag
	{
		kStatusAttackSpeed,
		kStatusSpeed,
	};

public:
	/// @param parent プレイヤーなど関連付ける親アクター
	/// @brief スキル選択カード用Textureの動的確保
	Skill(Object3D* parent);
	~Skill();

	/// @brief カード表示用座標の更新
	void Update();

	/// @brief 画面へのカードUI描画
	void Draw();

	/// @brief プレイヤーのステータス加算、または餌オブジェクトの生成トリガーの発行
	void AddSkill();

	SkillTag GetSkillTag() { return tag_; }

	/// @brief 値を取得する
	/// @param tag ステータス種類
	/// @return 加算されるボーナス実数値
	float GetStatusDate(StatusTag tag);

	/// @param flag UIを表示するかどうかのフラグ
	/// @brief 各種アニメーションフラグとカード初期位置の初期化
	void SetSkillFlag(bool flag);

	/// @brief フレーム跨ぎの複数回クリックによる、スキルの二重取得バグを防止するための購入確定フラグ
	bool add_skill_flag_; ///< 状態の有効・無効を管理するフラグ

private:
	void UpdateFlashAnimation();
	void UpdateSelectAnimation();
	void UpdateOpenAnimation();

private:

	Object3D* parent_; ///< 参照対象のオブジェクトを保持するポインタ
	SkillTag tag_;     ///< 現在の状態や種別を管理する値

	/// @brief 仕様制約：UI上にランダム提示される「3枚の異なる選択肢カード」の見た目を個別に制御するための個別テクスチャ
	Texture* texture_;  ///< 画面表示に使用するテクスチャ
	Texture* texture2_; ///< 画面表示に使用するテクスチャ
	Texture* texture3_; ///< 画面表示に使用するテクスチャ

	/// @brief 将来の拡張性（a = AttackSpeed, s = Speed）のバフ加算量を個別に永続保持するためのワーク変数
	float status_a_; ///< 内部状態を管理する値
	float status_s_; ///< 内部状態を管理する値

	int hover_skill_; ///< 内部状態を管理する値
	int flash_alpha_; ///< 内部状態を管理する値
	bool flash_;      ///< 内部状態を管理する値

	/// @brief 仕様制約：カード決定時に即画面を閉じず、選択したカードが拡大フェードする演出（0.5秒）を挟むための状態フラグ
	bool select_anim_;   ///< 内部状態を管理する値
	int selected_skill_; ///< 内部状態を管理する値
	float select_scale_; ///< サイズや範囲の計算に使用する値
	VECTOR select_pos_;  ///< 座標や位置情報を管理する値

	/// @brief 画面解像度やアスペクト比の変更時に、3枚のカードの均等な横並びレイアウトを維持するための基準座標群
	VECTOR pos1_; ///< 座標や位置情報を管理する値
	VECTOR pos2_; ///< 座標や位置情報を管理する値
	VECTOR pos3_; ///< 座標や位置情報を管理する値

	/// @brief 3枚のカードが画面下部から時間差（ディレイ）を持って滑らかに浮上する演出を制御するフラグ
	bool open_anim_; ///< 内部状態を管理する値
	float card1_y_;  ///< 内部状態を管理する値
	float card2_y_;  ///< 内部状態を管理する値
	float card3_y_;  ///< 内部状態を管理する値
};
