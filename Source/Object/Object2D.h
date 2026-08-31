#pragma once
#include <string>
#include <vector>
#include "Dxlib.h"

/// @brief UIやHUDなど、2D描画される全オブジェクトの共通基盤となるポリモーフィズム用基底クラス
/// @details 動的生成時にObjectManagerへ自動登録されるアーキテクチャのため、ライフサイクル管理に注意すること
class Object2D
{
public:
	/// @brief RTTI(dynamic_cast)の多用によるパフォーマンス低下を避け、キャストを安全に行うための識別タグ
	enum Tag2D
	{
		kNone2d = 0,
		kTag2dDino1 = 1,
		kTag2dDino2 = 100,
	};

public:
	/// @brief 登録後にスコープを抜けるとダングリングポインタとなりクラッシュするため、必ずヒープ領域（new）で生成すること
	/// @param initPos 初期座標 [出力] なし [副作用] 現在のアクティブなObjectManagerへの自身の自動登録
	Object2D(VECTOR initPos);

	/// @brief 派生クラス側で確保した画像ハンドルなどのリソース解放漏れを防ぐため、virtual修飾は絶対に外さないこと
	/// @details なし [出力] なし [副作用] なし
	virtual ~Object2D();

	/// @brief ポーズ中などの時間停止処理を考慮し、マネージャー経由での一括呼び出しを前提とする
	/// @details なし [出力] なし [副作用] 各種パラメータの更新
	virtual void Update();

	/// @brief UIの重なり（Zオーダー）はマネージャー側のリスト登録順に依存するため、生成順序に注意すること
	/// @details なし [出力] なし [副作用] 2Dバックバッファへの描画コール
	virtual void Draw();

public:
	void SetPosition(VECTOR pos) { position_ = pos; };
	VECTOR GetPosition() { return position_; }

	void SetRotation(VECTOR rot) { rotation_ = rot; }
	VECTOR GETRotation() { return rotation_; }

	void SetDeleteFlag(bool flag) { delete_flag_ = flag; }
	bool IsDeleteFlag() { return delete_flag_; }

	void SetDrawFlag(bool flag) { draw_flag_ = flag; }
	bool IsDrawFlag() { return draw_flag_; }

	void SetTag(Tag2D tag) { tag_ = tag; }
	Tag2D GetTag() { return tag_; }

protected:
	VECTOR position_; ///< 画面上の配置座標
	VECTOR rotation_; ///< 回転角度

private:
	bool delete_flag_; ///< 削除判定フラグ
	Tag2D tag_;        ///< オブジェクト識別タグ
	bool draw_flag_;   ///< 描画実行フラグ
};
