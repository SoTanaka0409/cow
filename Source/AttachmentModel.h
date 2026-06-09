#pragma once
#include "DxLib.h"
#include "Object3D.h"

// 別の3Dモデル（武器や装飾品など）の特定のフレーム（ボーン）に追従して描画されるオブジェクトクラス
class AttachmentModel : public Object3D
{
public:
	/*
	 * @brief アタッチモデルをロードし、親モデルと追従対象のフレームを登録する
	 * [入力] filename: モデルのファイルパス, parentModelHandle: 親モデルハンドル, parentFrameIndex: 追従対象のフレームインデックス
	 * [出力] なし
	 * [副作用] 新しいモデルハンドルがロードされる
	 */
	AttachmentModel(std::string filename, int parentModelHandle, int parentFrameIndex);
	virtual ~AttachmentModel();

	/*
	 * @brief 親モデルの対象フレームのワールド行列を取得し、自身の変換行列に適用する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	void Update() override;

	/*
	 * @brief アタッチモデルを画面に描画する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	void Draw() override;

	int GetHandle() { return mnHandle; }

private:
	int mnHandle;            // アタッチモデルのハンドル
	int mnParentHandle;      // アタッチ先親モデルのハンドル
	int mnParentFrameIndex;  // アタッチ先親フレームのインデックス番号
};
