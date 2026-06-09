#pragma once
#include <EffekseerForDXLib.h>
#include "DxLib.h"

// 3D空間上でEffekseer(`.efk`)形式の個別エフェクトファイルを読み込み、ロード・再生・停止・座標追従を管理するクラス
class EffekseerEffect
{
public:
	/*
	 * @brief アセットファイルパス、サイズ、初期座標を受け取り、アセットのロードを行う
	 * [入力] filename: エフェクトファイルのパス, initPos: 初期座標, EffectSize: 描画倍率スケール
	 * [出力] なし
	 * [副作用] エフェクトアセットのロード処理実行
	 */
	EffekseerEffect(const char* filename, VECTOR initPos, float EffectSize);

	/*
	 * @brief 読み込んだエフェクトリソースをDXライブラリ（Effekseer）側から破棄する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] エフェクトアセットリソースの解放
	 */
	~EffekseerEffect();

	/*
	 * @brief エフェクトファイルをロードし、リソースハンドルを取得する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] effectResourcehandleの書き換え
	 */
	void Load();

	/*
	 * @brief 現在再生中のエフェクト位置、回転角度、スケール値を最新値に追従更新する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 再生中エフェクトのワールド行列の更新
	 */
	void Update();

	/*
	 * @brief 描画処理（空メソッド、EffectManagerが全体を一括描画するため不要）
	 * [入力] なし
	 * [出力] なし
	 * [副作用] なし
	 */
	void Draw();

	/*
	 * @brief ロードしたエフェクトリソースの再生を開始し、再生インスタンスハンドルを取得する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 再生ハンドル (playingEffectHandle) の取得、再生位置設定
	 */
	void Play();

	/*
	 * @brief 再生中のエフェクトを強制的に停止する
	 * [入力] なし
	 * [出力] なし
	 * [副作用] 再生中エフェクトのインスタンス消滅
	 */
	void Stop();

	// 位置、回転、スケール、ループ設定のアクセッサ群
	void SetPosition(VECTOR pos) { PlayPos = pos; }
	VECTOR GetPosition() { return PlayPos; }

	void SetRotation(VECTOR rot) { mvRotation = rot; }
	VECTOR GetRotation() { return mvRotation; }

	void SetScale(VECTOR scale) { mvScale = scale; }
	VECTOR GetScale() { return mvScale; }

	void SetLoop(bool mfLoop) { LoopFlag = mfLoop; }

	/*
	 * @brief 現在エフェクトが再生中であるかを判定する
	 * [入力] なし
	 * [出力] 再生中ならtrue
	 * [副作用] なし
	 */
	bool IsPlaying()
	{
		return IsEffekseer3DEffectPlaying(playingEffectHandle) != -1;
	}

private:
	VECTOR PlayPos;             // 再生中の3D位置座標
	VECTOR mvRotation;          // 回転角度（ラジアン）
	VECTOR mvScale;             // XYZそれぞれの描画拡大スケール

	const char* filePath;       // エフェクトファイルアセットのパス
	int effectResourcehandle;   // エフェクトのロード済リソースハンドル
	int playingEffectHandle;    // 現在再生中インスタンスの管理ハンドル

	float effectSize;           // エフェクトロード時の初期サイズ倍率
	const int EffectPlayInterval = 300; // 再生周期パラメータ（未使用）
	int PlayCount;              // 再生フレームカウンタ（未使用）
	bool LoopFlag;              // ループ再生するかどうかのフラグ
};
