#include "EffectManager.h"

EffectManager::EffectManager()
	: effectResourcehandle(-1)
	, playingEffecthandle(-1)
{
	Initalize();
}

EffectManager::~EffectManager()
{
}

void EffectManager::Initalize()
{
	// DXライブラリ側でDirect3D11を使用するように設定する（Effekseerの要求仕様）
	SetUseDirect3DVersion(DX_DIRECT3D_11);

	// 表示可能なパーティクルの最大数を指定して、Effekseerシステムを初期化する
	if (Effekseer_Init(EffectParticleLimit) == -1)
	{
		DxLib_End();
	}

	// 画面モード切替（ウィンドウ/フルスクリーン）時、Effekseerのグラフィックスデバイスリセットを防ぐ
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	// DXライブラリでのデバイス喪失・復帰とEffekseerの内部リソース再生成を結びつけるコールバックを設定する
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	// 3Dエフェクト描画時に前後関係（深度値）が正しく考慮されるよう、Zバッファ処理を有効にする
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
}

void EffectManager::Update()
{
	// 毎フレーム呼び出し、現在再生中の全Effekseer3Dエフェクトのアニメーションステップを進める
	UpdateEffekseer3D();
}

void EffectManager::Draw()
{
	// 再生中かつ画面内に収まるEffekseerエフェクトを3D描画パスへ送る
	DrawEffekseer3D();
}

