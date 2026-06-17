#pragma once
#include <vector>

class Scene;
class ObjectManager;
class CowManager;
class AnimalManager;
class GameManager;
class Fever;
class Player3D;

// グローバルなマネージャーやオブジェクトへのアクセスを提供するロケーター
// 依存性の注入を簡略化するため各インスタンスの取得を仲介する
class ServiceLocator
{
public:
	/*
	 * @brief 現在アクティブなシーンを取得する
	 * [入力] なし
	 * [出力] Sceneポインタ (取得失敗時はnullptr)
	 * [副作用] なし
	 */
	static Scene* GetCurrentScene();

	/*
	 * @brief オブジェクト管理クラスを取得する
	 * [入力] なし
	 * [出力] ObjectManagerポインタ (取得失敗時はnullptr)
	 * [副作用] なし
	 */
	static ObjectManager* GetObjectManager();

	/*
	 * @brief プレイヤーオブジェクト(単体)を取得する
	 * [入力] なし
	 * [出力] Player3Dポインタ (取得失敗時はnullptr)
	 * [副作用] なし
	 */
	static Player3D* GetPlayer();

	/*
	 * @brief 存在する全プレイヤーオブジェクトのリストを取得する
	 * [入力] なし
	 * [出力] Player3Dポインタのベクター
	 * [副作用] ベクター構築のコストあり
	 */
	static std::vector<Player3D*> GetPlayers();

	/*
	 * @brief 牛キャラクター管理クラスを取得する
	 * [入力] なし
	 * [出力] CowManagerポインタ (取得失敗時はnullptr)
	 * [副作用] なし
	 */
	static CowManager* GetCowManager();

	/*
	 * @brief その他の動物管理クラスを取得する
	 * [入力] なし
	 * [出力] AnimalManagerポインタ (取得失敗時はnullptr)
	 * [副作用] なし
	 */
	static AnimalManager* GetAnimalManager();

	/*
	 * @brief ゲーム進行管理クラスを取得する
	 * [入力] なし
	 * [出力] GameManagerポインタ (取得失敗時はnullptr)
	 * [副作用] なし
	 */
	static GameManager* GetGameManager();

	/*
	 * @brief フィーバー状態管理クラスを取得する
	 * [入力] なし
	 * [出力] Feverポインタ (取得失敗時はnullptr)
	 * [副作用] なし
	 */
	static Fever* GetFever();
};
