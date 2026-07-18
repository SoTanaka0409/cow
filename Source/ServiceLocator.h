#pragma once
#include <vector>

class Scene;
class ObjectManager;
class CowManager;
class AnimalManager;
class GameManager;
class Fever;
class Player3D;

// 設計ルール：各アクターやマネージャー間の結合度を下げ、シーン遷移時の依存関係の絡まり（循環参照バグ）を回避するための共通窓口
class ServiceLocator
{
public:
	// 入力：なし
	// 出力：現在アクティブなSceneクラスのポインタ（失敗時はnullptr）
	static Scene* GetCurrentScene();

	// 入力：なし
	// 出力：すべての3D・2Dオブジェクトを一括管理するObjectManagerのポインタ
	static ObjectManager* GetObjectManager();

	// 入力：なし
	// 出力：1P（メインプレイヤー）として機能するPlayer3Dオブジェクトのポインタ
	static Player3D* GetPlayer();

	// 入力：なし
	// 出力：すべてのプレイヤーオブジェクトが格納された動的配列
	// パフォーマンス理由：内部配列のコピーによる一時オブジェクトのオーバーヘッドが発生するため、毎フレームの過度な呼び出しは避ける
	static std::vector<Player3D*> GetPlayers();

	// 入力：なし
	// 出力：牛（拉致対象）の生成・再利用・削除タイミングを管理するCowManagerのポインタ
	static CowManager* GetCowManager();

	// 入力：なし
	// 出力：牛以外の動物（鶏など）の行動AIとオブジェクトプールを管理するAnimalManagerのポインタ
	static AnimalManager* GetAnimalManager();

	// 入力：なし
	// 出力：ゲームの勝敗判定やタイムアップ、フェーズ進行を統括するGameManagerのポインタ
	static GameManager* GetGameManager();

	// 入力：なし
	// 出力：特定のコンボ条件を満たしたときに発生するフィーバータイムの演出やバフ情報を制御するFeverのポインタ
	static Fever* GetFever();
};
