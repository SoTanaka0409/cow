#pragma once
#include <vector>

class Scene;
class ObjectManager;
class CowManager;
class AnimalManager;
class GameManager;
class Fever;
class Player3D;

/// @brief 設計ルール：各アクターやマネージャー間の結合度を下げ、シーン遷移時の依存関係の絡まり（循環参照バグ）を回避するための共通窓口
class ServiceLocator
{
public:
	/// @brief 値を取得する
	/// @return 現在アクティブなSceneクラスのポインタ（失敗時はnullptr）
	static Scene* GetCurrentScene();

	/// @brief 値を取得する
	/// @return すべての3D・2Dオブジェクトを一括管理するObjectManagerのポインタ
	static ObjectManager* GetObjectManager();

	/// @brief 値を取得する
	/// @return 1P（メインプレイヤー）として機能するPlayer3Dオブジェクトのポインタ
	static Player3D* GetPlayer();

	/// @return すべてのプレイヤーオブジェクトが格納された動的配列
	/// @brief 内部配列のコピーによる一時オブジェクトのオーバーヘッドが発生するため、毎フレームの過度な呼び出しは避ける
	static std::vector<Player3D*> GetPlayers();

	/// @brief 値を取得する
	/// @return 牛（拉致対象）の生成・再利用・削除タイミングを管理するCowManagerのポインタ
	static CowManager* GetCowManager();

	/// @brief 値を取得する
	/// @return 牛以外の動物（鶏など）の行動AIとオブジェクトプールを管理するAnimalManagerのポインタ
	static AnimalManager* GetAnimalManager();

	/// @brief 値を取得する
	/// @return ゲームの勝敗判定やタイムアップ、フェーズ進行を統括するGameManagerのポインタ
	static GameManager* GetGameManager();

	/// @brief 値を取得する
	/// @return 特定のコンボ条件を満たしたときに発生するフィーバータイムの演出やバフ情報を制御するFeverのポインタ
	static Fever* GetFever();
};
