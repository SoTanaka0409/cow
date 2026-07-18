#pragma once
#include "Model.h"
#include "Dxlib.h"
#include "Object3D.h"

// 大量配置による描画負荷を考慮し、原則として状態更新を持たない背景用の静的3Dオブジェクト（障害物・装飾）として振る舞う基底クラス
class Object_Stage : public Object3D
{
public:
	// [入力] filename: モデルパス, initPos: 初期座標, scale: 拡大率, angle: 回転角度 [出力] なし [副作用] Modelインスタンスの動的生成
	// 頻繁な生成破棄はメモリ断片化やスパイクを招くため、ステージロード時に一括してインスタンス化すること
	Object_Stage(std::string filename, VECTOR initPos, float scale, VECTOR angle);

	// [入力] なし [出力] なし [副作用] model_のメモリ解放
	// 動的確保したModelリソースの解放漏れ（メモリリーク）を防ぐため、破棄時に確実にdeleteを呼ぶこと
	virtual ~Object_Stage();

	// [入力] なし [出力] なし [副作用] モデルの姿勢計算
	// 静的オブジェクト前提のため基本的に空実装だが、将来的な環境アニメーション（風で揺れる木など）の拡張用に口を残す
	void Update() override;

	// [入力] なし [出力] なし [副作用] バックバッファへの描画命令発行
	// 画面外の無駄な描画コールを防ぐ視錐台カリング等はマネージャー側で行う前提とし、本関数内は描画命令のみに留める
	void Draw() override;

	// [入力] scale: 拡大倍率 [出力] なし [副作用] モデルのスケール上書き
	// 当たり判定を持つ派生クラスの場合、見た目と判定サイズがズレる不具合を防ぐため呼び出し側に同期の責任を持たせること
	void SetScale(float scale);

	// [入力] angle: 各軸の回転角(ラジアン) [出力] なし [副作用] モデルの回転角上書き
	// ジンバルロックを避けるため、極端な多軸回転が必要な場合は適用順序に注意すること
	void SetRotation(VECTOR angle);

	// [入力] r,g,b,a: 0.0〜1.0のカラー値 [出力] なし [副作用] モデルマテリアルの色情報上書き
	// モデルにベイクされた本来のテクスチャ色を破壊するため、ダメージ表現などの一時的なカラーブレンド目的以外での使用は避けること
	void SetColor(float r, float g, float b, float a = 1.0f);

private:
	Model* model_; // 制御対象となる3Dモデルへのポインタ
};
