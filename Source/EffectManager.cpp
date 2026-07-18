#include "EffectManager.h"

/*
 * Effekseerの初期化処理を呼び出す
 * [入力] なし
 * [出力] なし
 * [副作用] メンバ変数の初期化、Effekseer初期化
 */
EffectManager::EffectManager()
	: effect_resource_handle_(-1)
	, playing_effect_handle_(-1)
{
	Initalize();
}

EffectManager::~EffectManager()
{
}

/*
 * 描画デバイスの設定とEffekseerの起動を行う
 * [入力] なし
 * [出力] なし
 * [副作用] Effekseerのシステムが初期化される
 */
void EffectManager::Initalize()
{
	// Effekseerの要求仕様に合わせてDirect3D11を使用する
	SetUseDirect3DVersion(DX_DIRECT3D_11);

	if (Effekseer_Init(kEffectParticleLimit) == -1)
	{
		DxLib_End();
	}

	// 画面モード切替時のデバイスリセットによるリソース喪失を防ぐ
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	// デバイス喪失からの復帰時にEffekseerのリソースを再生成させる
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	// 3Dエフェクト描画時に前後関係が正しく反映されるようZバッファを有効にする
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
}

/*
 * 再生中の全エフェクトの状態を更新する
 * [入力] なし
 * [出力] なし
 * [副作用] エフェクトのアニメーションが進む
 */
void EffectManager::Update()
{
	UpdateEffekseer3D();
}

/*
 * 空間上に存在する全てのエフェクトを描画する
 * [入力] なし
 * [出力] なし
 * [副作用] 画面にエフェクトが描画される
 */
void EffectManager::Draw()
{
	DrawEffekseer3D();
}
