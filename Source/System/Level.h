#pragma once
#include"Dxlib.h"
#include"Object3D.h"
#include"Utility.h"

/// @brief プレイヤーの成長状態（レベル・経験値）および進行度UIを管理するクラス
class Level
{
public:
	Level(Object3D* obj);
	~Level();

	/// @brief 現在の進行度をプレイヤーに伝達するためUIを描画する
	/// @details レベル文字列、経験値バーの画面描画
	void Draw();

	/// @brief 状態変更処理を行う（現在は拡張用にプレースホルダとして残す）
	/// @details なし
	void Update();

	/// @brief 経験値を加算し、一定蓄積時にレベルアップ判定を行う
	/// @param xp 加算する経験値量
	/// @details 経験値の更新、およびレベルアップ時にスキル選択フラグを有効化
	void AddXp(float xp);

	/// @brief レベルアップの進行度を初期化し次レベルの必要経験値を設定する
	/// @details mfMaxXpの更新
	void SetNextLevel();

private:
	/// @brief 経験値の進行割合を視覚的に読み取れるメーターとして描画する
	/// @details 進捗背景、バー進行、全長縁の描画
	void DrawBar();

	Object3D* parent_;      ///< 参照対象のオブジェクトを保持するポインタ
	int gauge_frame_graph_; ///< スキル選択などの備考用に残すための対象オブジェクトのインタ
	float xp_;              ///< レベルアップ判定の基準となる現在の累積経験値量
	float max_xp_;          ///< 難易度調整を形成するための次レベル必要経験値
	int now_level_;         ///< プレイヤーの強さや必要経験値設定の基準となるレベル値

	VECTOR pos  = VGet(Utility::kUiBaseX, Utility::kUiLevelY, 0);  ///< 解像度に合わせたUI配置の基準座標
	VECTOR size = VGet(Utility::kUiLevelW, Utility::kUiLevelH, 0); ///< 視覚幅を確定するためのバーの形状
};
