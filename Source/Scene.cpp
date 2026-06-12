
#include"Scene.h"
#include"ObjectManager.h"
#include"Master.h"
#include"ColliderManager.h"
#include"GameManager.h"
#include"CowManager.h"
#include"Fever.h"
#include"AnimalManager.h"

/*
 * @brief コンストラクタ
 * @details 各種マネージャークラスの生成とフェード用変数の初期化を行う
 */
	Scene::Scene()
{
	// 各種コンポーネント・マネージャーのインスタンス化
	mpObjectManager = new ObjectManager();
	mpColliderManager = new ColliderManager();
	mpGameManager = new GameManager();
	mpCowManager = new CowManager();
	mpAnimalManager = new AnimalManager();
	mpFever = new Fever();

	// フェード演出用変数の初期化
	mfFadeAlpha = 0.0f;
	mfFadeSpeed = 5.0f;
}

/*
 * @brief デストラクタ
 * @details 生成したマネージャークラスのクリーンアップとメモリ解放を行う（メモリリーク防止）
 */
Scene::~Scene()
{
	// オブジェクトマネージャーの解放
	if (mpObjectManager != nullptr)
	{
		mpObjectManager->DeleteAll3D();
		mpObjectManager->DeleteAll2D(); // 2Dテクスチャオブジェクト群を破棄し、シーン切り替え時のメモリリークを防ぐ
		delete mpObjectManager;
	}

	// コライダーマネージャーの解放
	if (mpColliderManager != nullptr)
	{
		mpColliderManager->DeleteAllCollider();
		delete mpColliderManager;
	}

	// その他マネージャー類の解放
	if (mpGameManager != nullptr)
	{
		delete mpGameManager;
	}
	if (mpCowManager != nullptr)
	{
		delete mpCowManager;
	}
	if (mpFever != nullptr)
	{
		delete mpFever;
	}
	if (mpAnimalManager != nullptr)
	{
		delete mpAnimalManager;
	}
}

/*
 * @brief 描画処理
 * @details 各マネージャーが管理するオブジェクトの描画関数を順次呼び出す
 */
void Scene::Draw()
{
	if (mpObjectManager != nullptr)
	{
		mpObjectManager->Draw();
	}
	if (mpColliderManager != nullptr)
	{
		mpColliderManager->Draw(); // 当たり判定の可視化（デバッグ用など）
	}
	if (mpAnimalManager != nullptr)
	{
		mpAnimalManager->Draw();
	}
}

/*
 * @brief 更新処理
 * @details 各マネージャーのロジック更新（位置計算や状態遷移など）を順次呼び出す
 */
void Scene::Update()
{
	if (mpObjectManager != nullptr)
	{
		mpObjectManager->Update();
	}
	if (mpColliderManager != nullptr)
	{
		mpColliderManager->Update();
	}
	if (mpFever != nullptr)
	{
		mpFever->Update();
	}
	if (mpAnimalManager != nullptr)
	{
		mpAnimalManager->Update();
	}
}

/*
 * @brief シーンフェード演出時の黒スクリーンを描画する
 * [入力] fade: 進行するフェード状態 (In / Out / Load)
 * [出力] なし
 * [副作用] アルファブレンドモードの変更、黒い四角形の描画
 */
void Scene::Fade(SceneFade fade)
{
	// --- フェードイン（画面がだんだん明るくなる） ---
	if (fade == SceneFade::SceneFade_In)
	{
		// 不透明度を減少させる
		mfFadeAlpha -= mfFadeSpeed;
		if (mfFadeAlpha < 0) mfFadeAlpha = 0; // 下限ガード

		// 完全に透明（0）でなければ、黒いスクリーンを描画して被せる
		if (mfFadeAlpha > 0)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)mfFadeAlpha);
			DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE); // 16:9 画面全体を黒で塗りつぶし
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);          // ブレンドモードを通常に戻す
		}
	}
	// --- フェードアウト（画面がだんだん暗くなる） ---
	else if (fade == SceneFade::SceneFade_Out)
	{
		// 不透明度を増加させる
		mfFadeAlpha += mfFadeSpeed;
		if (mfFadeAlpha > 255) mfFadeAlpha = 255; // 上限ガード（DXライブラリのアルファ最大値は255）

		// 黒いスクリーンを描画
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)mfFadeAlpha);
		DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);          // ブレンドモードを通常に戻す
	}
	// --- ロード中（必要に応じて処理を追加可能） ---
	else if (fade == SceneFade::SceneFade_Load)
	{
		// 現在は何も行わない（固定画面の表示やローディングアイコン描画などを拡張可能）
	}
}